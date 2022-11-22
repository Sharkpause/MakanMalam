#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define WHITE "\033[0;37m"
#define RED "\033[0;31m"

void cpyerr(char* message, int type, int choice) {
	if(type == 0) {
		strcpy(message, RED);
		strcat(message, "[ERROR] Input bukan bilangan!\n\n");
	} else {
		snprintf(message, 46, "%s[ERROR] %d itu bukan operasi asli!\n\n", RED, choice);
	}

	int c;
	while(c = getchar() != '\n' && c != EOF);
}

void changeAmount(int* amount, char* message, int choice) {
	int newAmount;
	
	while(1) {
		printf("%s", message);
		message[0] = '\0';
		
		printf("\e[1;1H\e[2J");
		printf("%sJumlah: %d\n%sJumlah baru: %s", GREEN, *amount, BLUE, WHITE);
		if(scanf("%d", &newAmount) != 1) {
			cpyerr(message, 0, choice);
			continue;
		}
		*amount = newAmount;
		break;
	}
}

void modify(char list[][100], int *size, char* message) {
	int choice;
	char item[100];

	while(1) {
		printf("\e[1;1H\e[2J");

		printf("%s", message);
		message[0] = '\0';

		printf("%sDaftar makanan:\n%s", GREEN, WHITE);
		for(int i = 0; i < *size; ++i) {
			printf("- %s\n", list[i]);
		}
		printf("\n%sDaftar operasi:%s\n1 = KELUAR - Kembali ke menu utama\n2 = TAMBAH - Tambah makanan baru ke daftarnya\n3 = HAPUS - Hapus makanan dari daftarnya\n%sMasukkan nomor: %s", BLUE, WHITE, BLUE, WHITE);

		if(scanf("%d", &choice) != 1) {
			cpyerr(message, 0, choice);
			continue;
		}
		printf("\n");

		int c;
		while(c = getchar() != '\n' && c != EOF);

		switch(choice) {
			default:
				cpyerr(message, 1, choice);
				break;
			case 1:
				return;
			case 2:
				printf("%sMasukkan nama makanan barunya: %s", BLUE, WHITE);
				fgets(item, 100, stdin);
				item[strcspn(item, "\n")] = 0;

				strcpy(list[*size], item);
				++*size;

				break;
			case 3:
				printf("%sMasukkan nama makanan untuk dihapus: %s", BLUE, WHITE);
				fgets(item, 100, stdin);
				item[strcspn(item, "\n")] = 0;

				int i;

				for(i = 0; i < *size; ++i) {
					if(strcmp(list[i], item) == 0) break;
				}

				for(int j = i; j < *size-1; ++j) {
					strcpy(list[j], list[j+1]);
				}

				--*size;

				break;
		}
	}
}

void loadData(FILE* file, char list[][100], int* listSize, int* amount) {
	char ch;
	file = fopen("daftar.txt", "r");

	if(file == NULL) {
		file = fopen("daftar.txt", "w");

		*listSize = 0;
		*amount = 0;

		fclose(file);
	} else {
		char savedAmount[4], savedSize[4];

		int i = 0;
		
		while(ch != EOF && ch != '\n') {
			ch = fgetc(file);
			if(ch != ' ') {
				savedAmount[i] = ch;
				++i;
			} else {
				break;
			}
		}
		i = 0;
		while(ch != EOF) {
			ch = fgetc(file);
			if(ch != '\n') {
				savedSize[i] = ch;
				++i;
			} else {
				break;
			}
		}

		for(int i = 0; i < 100; ++i ) {
			int j = 0;
			while(ch != EOF) {
				ch = fgetc(file);
				if(ch != ',' && ch != '\n') {
					list[i][j] = ch;
					++j;
				} else {
					break;
				}
			}
		}

		*amount = atoi(savedAmount);
		*listSize = atoi(savedSize);

		fclose(file);
	}
}

void saveData(FILE* file, char list[][100], int listSize, int amount) {
	file = fopen("daftar.txt", "w");

	char numbers[10];
	snprintf(numbers, 10, "%d %d\n", amount, listSize);
	fputs(numbers, file);
	for(int i = 0; i < listSize; ++i) {
		fputs(list[i], file);
		fputs(",", file);
	}

	fclose(file);
}

int main() {
	FILE* file;

	int amount, choice, listSize, pickedSize = 0;
	char list[100][100], message[41], picked[100][100];

	loadData(file, list, &listSize, &amount);
	
	system("tput smcup");
	printf("\e[1;1H\e[2J");

	srand(time(NULL));

	while(1) {
		printf("\e[1;1H\e[2J");

		printf("%s", message);
		message[0] = '\0';

		if(pickedSize > 0) {
			for(int i = 0; i < amount; ++i) {
				printf("%sTerpilih: %s%s\n", GREEN, WHITE, picked[i]);
			}
			printf("\n");
		}

		memset(picked, 0, sizeof(picked));
		pickedSize = 0;

		printf("%sJumlah terpilih: %s%d\n\n%sDaftar operasi:%s\n1 = KELUAR - Simpan perubahan dan keluar dari program\n2 = JUMLAH - Ubah jumlah terpilih yang dihasilkan secara acak\n3 = UBAH - Tambah dan hapus makanan dari daftar makanan\n4 = PILIH - Memulai memilih makan secara acak\n%sMasukkan nomor: %s", GREEN, WHITE, amount, BLUE, WHITE, BLUE, WHITE);

		if(scanf("%d", &choice) != 1) {
			cpyerr(message, 0, choice);
			continue;
		}
		printf("\n");

		switch(choice) {
			default:
				cpyerr(message, 1, choice);
				break;
			case 1: // KELUAR
				saveData(file, list, listSize, amount);

				system("tput rmcup");
				return 0;
			case 2: // JUMLAH
				changeAmount(&amount, message, choice);

				break;
			case 3:
				modify(list, &listSize, message);

				break;
			case 4:
				if(listSize > 0) {
					for(int i = 0; i < amount; ++i) {
						strcpy(picked[i], list[rand() % listSize]);
						++pickedSize;
					}
				}
		}

	}

	return 0;
}
	
