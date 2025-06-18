#include <stdio.h>
#include "comandi.h"
#include "vfs.h"
#include "persistenza.h"
#define DIM 150

int main(void) {

	initFS();
	printf("----------------------------------------------------\n");
	printf("File System + shell di Claudio\n");
	printf("cd -> navigazione cartelle\n");
	printf("mkdir -arg -> crea cartella\n");
	printf("touch -arg -contenuto -> crea file, contenuto non può contenere spazi\n");
	printf("ls -> mostra file nella directory\n");
	printf("rm -arg -> mostra file nella directory\n");
	printf("read -arg -> leggi il file di testo nella directory\n");
	printf("exit -> esci\n");
	printf("----------------------------------------------------\n");
	char comando[DIM];

	while (1) {
		printf(">>>>> ");
		if (fgets(comando, sizeof(comando), stdin) != NULL) {
			gestisciComando(comando);
		}
	}

	return 0;
}