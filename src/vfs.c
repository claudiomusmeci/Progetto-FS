#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "vfs.h"
#include "storage.h"
#include "persistenza.h"

t_inode inodes[MAX];
t_fat FAT[MAXB];
char data[MAXB][MAXB];
int cartellaCorrente = 0;

int trovaBloccoLibero() {
	for (int i = 0; i < MAXB; i++) {
		if (FAT[i].utilizzato == 0) {
			return i;
		}
	}
	return -1;
}


void initFS() {

	FILE* fp = fopen("savestate", "rb");
	if (fp) {
		fclose(fp);
		loadFS(); // File esiste → lo carico
		return;
	}
	else {
		for (int i = 0; i < MAX; i++) {
			// Reset degli inode
			inodes[i].nome[0] = '\0'; // Carattere che indica lo slot libero
			inodes[i].isDirectory = 0;
			inodes[i].size = 0;
			inodes[i].first = -1;
			inodes[i].parentNode = -1;
			inodes[i].permessi = 0;

			// Reset di FAT
			FAT[i].utilizzato = 0;
			FAT[i].next = -1;
		}
		// Inizializzo il primo inode come root
		strcpy(inodes[0].nome, "/");
		inodes[0].isDirectory = 1;
		inodes[0].size = 0;
		inodes[0].first = -1;
		inodes[0].parentNode = 0;
		inodes[0].permessi = 0755;

		cartellaCorrente = 0;

		return;
	}
	

}

void creaCartella(char* arg) {
	for (int i = 1; i < MAX; i++) {
		if (strcmp(inodes[i].nome, "\0") == 0) {
			// Sono in un blocco vuoto
			strcpy(inodes[i].nome, arg);
			inodes[i].isDirectory = 1;
			inodes[i].size = 0;
			inodes[i].first = -1;
			inodes[i].parentNode = cartellaCorrente;
			inodes[i].permessi = 0755;
			printf("Cartella %s creata correttamente! \n", arg);
			return;
		}
	}
	printf("inode esaurito! \n");
}

void creaFile(char* arg, char* contenuto) {
	int bloccoIniziale;
	bloccoIniziale = trovaBloccoLibero();
	if (bloccoIniziale < 0) {
		printf("Nessun blocco disponibile!\n");
		return;
	}
	for (int i = 1; i < MAX; i++) {
		if (strcmp(inodes[i].nome, "\0") == 0) {
			// Sono in un blocco vuoto
			strcpy(inodes[i].nome, arg);
			inodes[i].isDirectory = 0;
			inodes[i].size = 0;
			inodes[i].first = bloccoIniziale;
			inodes[i].parentNode = cartellaCorrente;
			inodes[i].permessi = 0644;

			int len = strlen(contenuto);
			int scritto = 0;
			int successivo = -1;
			int bloccoCorrente = bloccoIniziale;
			while (scritto < len) {
				int daScrivere = (len - scritto > MAXB) ? MAXB : len - scritto;
				memcpy(data[bloccoCorrente], contenuto + scritto, daScrivere);
				

				scritto = scritto + daScrivere;

				if (daScrivere < MAXB) {
					data[bloccoCorrente][daScrivere] = '\0';
				}
				FAT[bloccoCorrente].utilizzato = 1;

				if (scritto < len) {
					int prossimoBlocco = trovaBloccoLibero();
					if (prossimoBlocco < 0) {
						printf("Memoria esaurita\n");
						return;
					}
					FAT[bloccoCorrente].next = prossimoBlocco;
					bloccoCorrente = prossimoBlocco;
				}
				else {
					FAT[bloccoCorrente].next = -1;
				}
			}

			printf("File creato nella cartella corrente! \n");
			return;
		}
	}
	printf("inode esaurito! \n");
	return;
}

void cambiaCartella(char* arg) {
	if (strcmp(arg, "..") == 0) {
		if (cartellaCorrente != 0) {
			cartellaCorrente = inodes[cartellaCorrente].parentNode;
		}
		else {
			cartellaCorrente = 0; // Imposto root
		}
		return;
	}

	for (int i = 1; i < MAX; i++) {
		if (inodes[i].nome != '\0' && inodes[i].isDirectory && inodes[i].parentNode == cartellaCorrente && strcmp(inodes[i].nome, arg) == 0) {
			cartellaCorrente = i;
			return;	
		}
	}

	printf("Cartella %s non trovata! \n", arg);
	return;
}

void mostraFile() {
	for (int i = 1; i < MAX; i++) {
		if (inodes[i].nome != '\0' && inodes[i].parentNode == cartellaCorrente) {
			printf("%s, tipo: %s \n", inodes[i].nome, inodes[i].isDirectory ? "DIR" : "FILE");
		}
		
	}
	return;
}

void rimuoviElemento(char* arg) {
	for (int i = 1; i < MAX; i++) {
		if ((strcmp(inodes[i].nome, arg) == 0) && inodes[i].parentNode == cartellaCorrente) {

			// Svuoto la FAT
			int blocco = inodes[i].first;
			while (blocco != -1) {
				int prossimo = FAT[blocco].next;
				FAT[blocco].utilizzato = 0;
				FAT[blocco].next = -1;
				blocco = prossimo;
			}
		

			// Sono in un blocco vuoto
			inodes[i].nome[0] = '\0';
			inodes[i].isDirectory = 0;
			inodes[i].size = 0;
			inodes[i].first = -1;
			inodes[i].parentNode = -1;
			inodes[i].permessi = 0644;
			printf("Elemento eliminato! \n");
			return;
		}
	}
	printf("inode esaurito! \n");
	return;
}

void readFile(char* arg) {
	int bloccoCorrente = -1;
	int indiceFile = -1;
	for (int i = 0; i < MAX; i++) {
		if (strcmp(inodes[i].nome, arg) == 0 && inodes[i].parentNode == cartellaCorrente && inodes[i].isDirectory == 0) {
			bloccoCorrente = inodes[i].first;
			indiceFile = i;
			break;
		}
	}

	if (indiceFile < 0) {
		printf("File %s non trovato!\n", arg);
		return;
	}
	else {
		printf("Contenuto di '%s':\n", arg);
		while (bloccoCorrente != -1) {
			printf("%.*s", MAXB, data[bloccoCorrente]);
			bloccoCorrente = FAT[bloccoCorrente].next;
		}
		printf("\n");
		return;
	}
}
