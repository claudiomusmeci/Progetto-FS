#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "persistenza.h"
#include "vfs.h"

extern t_inode inodes[MAX];

void saveFS() {
	FILE* fp;
	

	fp = fopen("savestate", "wb");
	if (!fp) {
		// Se il file non esiste lo creo
		printf("Errore nell'apertura del file nel salvataggio!\n");
	}
	fwrite(inodes, sizeof(t_inode), MAX, fp);
	fclose(fp);
	printf("File system salvato.\n");
}

void loadFS() {
    FILE* fp = fopen("savestate", "rb");
	if (!fp) {
		// Se il file non esiste lo creo
		printf("Errore nell'apertura del file nel caricamento!\n");
	}
    fread(inodes, sizeof(t_inode), MAX, fp);
    fclose(fp);
    printf("File system caricato.\n");
}