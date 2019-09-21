#define MAX_DEF 50
#define MAX_WORD 20
#define MIN_WORD 2

typedef struct NODO {
	char *word;
	char *def;
	char color;
	struct NODO *left;
	struct NODO *right;
	struct NODO *p;
}NODO;

typedef struct TABELLA {
	int carattere;
	int frequenza;
	int bit;
	struct TABELLA *left;
	struct TABELLA *right;
}TABELLA;

typedef struct HEAPMIN {
	TABELLA *vett[256];
	int heapSize;
	int length;
}HEAPMIN;

/*
Input:
-T: sentinella dell'albero rosso nero
-x: nodo dell'albero rosso-nero
Output:
-Restituisce il puntatore alla radice
*/
NODO* leftRotate(NODO *T, NODO *x);


/*
Input:
-T: sentinella dell'albero rosso nero
-y: nodo dell'albero rosso-nero
Output:
-Restituisce il puntatore alla radice dell'albero rosso nero
*/
NODO* rightRotate(NODO *T, NODO *y);

/*
Input:
-dictionary: la struttura dati in cui avete memorizzato il dizionario
-word: la parola da inserire nel dizionario , senza la definizione
Output:
-0 in caso di assenza di errori
-1 in caso di presenza di errori
*/
int rbInsert(NODO** dictionary, NODO **z);

/*
Input:
-T: sentinella dell'albero rosso nero
-z: nodo da inserire  nell'albero rosso-nero
Output:
-Restituisce il puntatore radice dell'albero rosso nero
*/
NODO* rbInsertFixup(NODO *T, NODO *z);


int init(NODO **root, NODO **T, char *stringa, char *def);


/*
Input:
-word: parola da convertire in minuscolo
*/
void conversionLowecase(char *word);


/*
Input:
-T: sentinella dell'albero rosso nero
-z : nodo dell'albero rosso nero da eliminare
Output:
-puntatore alla radice dell'albero rosso nero
*/
NODO* rbDeleteFixup(NODO* T, NODO* x);


/*
Input:
-T: sentinella dell'albero rosso nero
-u : nodo dell'albero rosso nero
-v : nodo dell'albero rosso nero
Output:
-puntatore alla radice dell'albero rosso nero
*/
void rbTransplant(NODO **root, NODO *u, NODO *v);


/*
Input:
-T: sentinella dell'albero rosso nero
-x : nodo dell'albero rosso nero
Output:
-puntatore al nodo con valore minimo
*/
NODO* treeMinimum(NODO *x, NODO *T);


/*
Input:
-T: sentinella dell'albero rosso nero
Output:
-puntatore al nodo con voce = 'word'
*/
NODO* searchNode(NODO* T, char* word);


/*
Input:
-word: puntatore alla word da creare
-dim: dimensione lunghezza word
Output:
-puntatore alla nuova stringa creata
*/
char* creaStringa(char *word, int dim);


void getWordAtHelp(NODO* dictionary, char** word, int* index);

int minimum(int a, int b, int c);

int Levenshtein_distance(char *x, char *y);

void implementSearchAdvance(NODO*  dictionary, NODO* word, char** primoRis, char** secondoRis, char** terzoRis, NODO** parola, int *x, int *y);

int Parent(int i);

int LeftHeap(int i);

int RightHeap(int i);

void MinHeapify(HEAPMIN *A, int  i);

void BuildMinHeap(HEAPMIN *A);

void HeapDecreaseKey(HEAPMIN *A, int i, TABELLA *z);

int MinHeapInsert(HEAPMIN *A, TABELLA *z);

int HeapExtractMin(HEAPMIN *A, TABELLA **x);

int allocaTabella(TABELLA **nuovo, int carattere, int frequenza, TABELLA *left, TABELLA *right);

int CreateFrequencyTableHeapHuffman(NODO *dictionary, HEAPMIN *A);

int codificaCarattere(TABELLA *z, int *bit, int carattere);

void freeAlberoHuffman(TABELLA *z);

void printDictionaryGen(NODO*  dictionary, FILE *ptr);
int FrequencyTable(NODO* dictionary, TABELLA **frequenze);

char compressHuffmanHelp(NODO* dictionary, TABELLA *z, char carattere, int *contatoreBit, FILE *ptr);

char scriviCodificaCarattere(TABELLA *z, char *stringa, char carattere, int *contatoreBit, FILE *ptr);

TABELLA* createTreeHuffman(HEAPMIN *A);

int AssegnaWordDefNode(NODO **z, char *stringa, char *def);

NODO* allocaNodo();