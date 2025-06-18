#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "comandi.h"
#include "persistenza.h"
#include "vfs.h"

void gestisciComando(char *input) {
	char comando[DIM], arg[DIM], contenuto[DIM];
	sscanf(input, "%s %s %s", comando, arg, contenuto);

    if (strcmp(comando, "mkdir") == 0) {
        creaCartella(arg);
    }
    else if (strcmp(comando, "touch") == 0) {
        creaFile(arg, contenuto);
    }
    else if (strcmp(comando, "cd") == 0) {
        cambiaCartella(arg);
    }
    else if (strcmp(comando, "ls") == 0) {
        mostraFile();
    }
    else if (strcmp(comando, "rm") == 0) {
        rimuoviElemento(arg);
    }
    else if (strcmp(comando, "read") == 0) {
        readFile(arg);
    }
    else if (strcmp(comando, "exit") == 0) {
        printf("Esco.. \n");
        saveFS();
        exit(0);
    }
    else {
        printf("Comando sconosciuto: %s\n", comando);
    }
}