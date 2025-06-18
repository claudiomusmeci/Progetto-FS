#pragma once
#define MAX 32
#define MAXB 128
/**
 * @brief Inizializza il file system (in RAM).
 */
void initFS();

/**
 * @brief Comando per creare una cartella.
 */
void creaCartella(char* arg);

/**
 * @brief Comando per creare un file.
 */
void creaFile(char* arg, char* contenuto);
/**
 * @brief Comando per cambiare directory.
 */
void cambiaCartella(char* arg);

/**
 * @brief Comando per eliminare un elemento.
 */
void rimuoviElemento(char* arg);


/**
 * @brief Mostra i file in una cartella.
 */
void mostraFile();

/**
 * @brief Legge un file di testo.
 */
void readFile(char* arg);

typedef struct inode{
	char nome[32];
	int isDirectory;
	int size;
	int first;
	int parentNode;
	int permessi;
}t_inode;

typedef struct fat {
	int utilizzato;
	int next;
}t_fat;