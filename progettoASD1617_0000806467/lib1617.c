#include<stdio.h>
#include "lib1617.h"

NODO* leftRotate(NODO *root, NODO *x) {
	NODO *y = x->right;					/*Imposta y*/
	x->right = y->left;					/*Sposta il sottoalbero sinistro di y nel sottoalbero destro di x*/
	if (y->left != root->p)
		y->left->p = x;
	y->p = x->p;						/*Collega il padre di x a y*/
	if (x->p == root->p)
		root = y;
	else if (x == x->p->left)
		x->p->left = y;
	else
		x->p->right = y;
	y->left = x;                       /*Pone x a sinistra di y*/
	x->p = y;
	return root;
}

NODO* rightRotate(NODO *root, NODO *y) {
	NODO *x = y->left;				  /*Imposta x*/
	y->left = x->right;               /*Sposta il sottoalbero destro di x nel sottoalbero sinistro di y*/
	if (x->right != root->p)
		x->right->p = y;
	x->p = y->p;					  /*Collega il padre di y a x*/
	if (y->p == root->p)
		root = x;
	else if (y == y->p->right)
		y->p->right = x;
	else
		y->p->left = x;
	x->right = y;                    /*Pone y a destra di x*/
	y->p = x;
	return root;
}

int insertWord(NODO** dictionary, char* word) {
	if (*dictionary == NULL) return 1;
	int ret;
	NODO *z;
	z = allocaNodo();
	if (z == NULL) return 1;   //Ritorna 1 se la malloc non è riuscita ad allocare spazio per il nuovo nodo
	ret = AssegnaWordDefNode(&z, word, NULL);
	if (ret == 1) return 1; //La parola non può essere inserita perchè o è troppo corta o troppo lunga o perchè contiene caratteri di punteggiatura
	ret=rbInsert(&(*dictionary),&z);
	if (ret == 1)
		free(z);   //Deaccolo il nodo
	return ret;
}

NODO* allocaNodo() {
	NODO *nuovo = ((NODO*)malloc(sizeof(NODO)));
	if (nuovo == NULL)
		return NULL;                        //Ritorna 1 se la malloc non è riuscita ad allocare spazio per il nuovo nodo
	return nuovo;
}

int rbInsert(NODO** dictionary, NODO **z) {
	NODO *y = (*dictionary)->p;
	NODO *x = (*dictionary);
	int ret;
	while (x != (*dictionary)->p) {
		y = x;
		ret = strcmp((*z)->word, x->word);
		if (ret == 0)
			return 1;				//Nodo già esistente
		if (ret<0)
			x = x->left;
		else
			x = x->right;
	}
	(*z)->p = y;
	if (y == (*dictionary)->p) {
		(*dictionary) = *z;
	}
	else if (strcmp((*z)->word, y->word) < 0) {
		y->left = *z;
	}
	else {
		y->right = *z;
	}
	(*z)->left = (*dictionary)->p;
	(*z)->right = (*dictionary)->p;
	(*z)->color = 'R';
	(*dictionary) = rbInsertFixup((*dictionary), *z);
	return 0;
}

NODO* rbInsertFixup(NODO *root, NODO *z) {
	NODO *y = root->p;
	while (z->p->color == 'R') {
		if (z->p == z->p->p->left) {
			y = z->p->p->right;
			if (y->color == 'R') {               //Caso 1
				z->p->color = 'B';				 //  ""
				y->color = 'B';					 //  ""
				z->p->p->color = 'R';			 //  ""
				z = z->p->p;                     //  ""
			}
			else {
				if (z == z->p->right) {			//Caso 2
					z = z->p;					//  ""
					root = leftRotate(root, z);		//  ""
				}
				z->p->color = 'B';              //Caso 3
				z->p->p->color = 'R';			//  ""
				root = rightRotate(root, z->p->p);	//  ""
			}
		}
		else {
			y = z->p->p->left;
			if (y->color == 'R') {				//Caso 4
				z->p->color == 'B';				//  ""
				y->color = 'B';					//  ""
				z->p->p->color = 'R';			//  ""
				z = z->p->p;					//  ""
			}
			else {
				if (z == z->p->left) {
					z = z->p;					//Caso 5
					root = rightRotate(root, z);		//  ""
				}
				z->p->color = 'B';				//Caso 6
				z->p->p->color = 'R';			//  ""
				root = leftRotate(root, z->p->p);	//  ""
			}
		}
	}
	root->color = 'B';
	return root;
}

void conversionLowecase(char *word) {
	short int i;
	for (i = 0; word[i] != '\0';i++) {
		if ((word[i] != 'à' && word[i] != 'è' && word[i] != 'ì' && word[i] != 'ò' && word[i] != 'ù') && isalpha(word[i])) {
			word[i] = tolower(word[i]);
		}
	}
}

void rbTransplant(NODO **root, NODO *u, NODO *v) {
	if (u->p == (*root)->p)
		(*root) = v;
	else if (u == u->p->left)
		u->p->left = v;
	else
		u->p->right = v;
	v->p = u->p;
}

int cancWord(NODO** dictionary, char* word) {
	NODO *a;
	NODO *z = searchNode((*dictionary), word);
	if (z == NULL)
		return 1;		//Nodo non presente
	NODO *y = z;
	NODO* x;
	char y_original_color = y->color;
	if (z->left == (*dictionary)->p) {
		x = z->right;
		rbTransplant(&(*dictionary), z, z->right);
	}
	else if (z->right == (*dictionary)->p) {
		x = z->left;
		rbTransplant(&(*dictionary), z, z->left);
	}
	else {
		y = treeMinimum(z->right, (*dictionary));
		y_original_color = y->color;
		x = y->right;
		if (y->p == z)
			x->p = y;
		else {
			rbTransplant(&(*dictionary), y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		rbTransplant(&(*dictionary), z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	if (y_original_color == 'B')
		(*dictionary) = rbDeleteFixup((*dictionary), x);
	free(z->word);
	free(z->def);
	free(z);
	return 0;
}

NODO* rbDeleteFixup(NODO* root, NODO* x) {
	NODO* w;
	while (x != root && x->color == 'B') {
		if (x == x->p->left) {
			w = x->p->right;
			if (w->color == 'R') {
				w->color = 'B';
				x->p->color = 'R';
				root = leftRotate(root, x->p);
				w = x->p->right;
			}
			if (w->left->color == 'B' && w->right->color == 'B') {
				w->color = 'R';
				x = x->p;
			}
			else {
				if (w->right->color == 'B') {
					w->left->color = 'B';
					w->color = 'R';
					root = rightRotate(root, w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = 'B';
				w->right->color = 'B';
				root = leftRotate(root, x->p);
				x = root;
			}
		}
		else {
			w = x->p->left;
			if (w->color == 'R') {
				w->color = 'B';
				x->p->color = 'R';
				root = rightRotate(root, x->p);
				w = x->p->left;
			}
			if (w->right->color == 'B' && w->left->color == 'B') {
				w->color = 'R';
				x = x->p;
			}
			else {
				if (w->left->color == 'B') {
					w->right->color = 'B';
					w->color = 'R';
					root = leftRotate(root, w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = 'B';
				w->left->color = 'B';
				root = rightRotate(root, x->p);
				x = root;
			}
		}
	}
	x->color = 'B';
	return root;
}

NODO* treeMinimum(NODO *x, NODO *root) {
	while (x->left != root->p)
		x = x->left;
	return x;
}

int AssegnaWordDefNode(NODO **z,char *stringa, char *def) {
	if (strlen(stringa) > MAX_WORD - 1 || strlen(stringa)<2 || (ispunct(stringa[strlen(stringa) - 1]) && strlen(stringa)==2))
		return 1;                         //Ritorna 1 se la word del nodo è troppo corta o troppo lunga
	int i;
	for (i = 0;i < strlen(stringa)-1;i++) {
		if (stringa[i] != 'à' && stringa[i] != 'è' && stringa[i] != 'ì' && stringa[i] != 'ò' && stringa[i] != 'ù' && stringa[i] != 'é')
			if (ispunct(stringa[i]) || isdigit(stringa[i]))
				return 1;					 //Ritorna 1 se la word del nodo contiene caratteri di punteggiatura o cifre
	}
	char *string,tmp[MAX_WORD];
	if (ispunct(stringa[strlen(stringa) - 1])) {      //Controllo se l'ultimo carattere è un segno di punteggiatura
		strcpy(tmp, stringa);
		tmp[strlen(tmp) - 1] = '\0';
		string = creaStringa(tmp, strlen(tmp));
	}
	else
		string = creaStringa(stringa, strlen(stringa));
	if (string == NULL) return 1;
	(*z)->word = string;

	conversionLowecase((*z)->word);
	if (def == NULL || strcmp(def, "(null)") == 0)
		(*z)->def = NULL;
	else {
		string = creaStringa(def, strlen(def));
		if (string == NULL) return NULL;
		(*z)->def = string;
		conversionLowecase((*z)->def);
	}
	return 0;
}

int init(NODO **root,NODO **T,char *stringa,char *def) {
	int ret;
	(*root) = ((NODO*)malloc(sizeof(NODO)));
	if ((*root) == NULL) return 1;
	ret = AssegnaWordDefNode(&(*root),stringa,def);
	if (ret == 1) {
		free(*root);
		return 1;
	}
	(*T) = ((NODO*)malloc(sizeof(NODO)));
	if ((*T) == NULL)
		return NULL;
	(*T)->word = NULL;
	(*T)->def = NULL;
	(*T)->color = 'R';
	(*T)->left = (*T);
	(*T)->right = (*T);
	(*T)->p = (*T);
	(*root)->p = (*T);
	(*root)->right = (*T);
	(*root)->left = (*T);
	(*root)->color = 'B';
	return 0;
}

NODO* createFromFile(char* nameFile) {
	NODO* T, *dictionary=NULL;
	FILE *ptr;
	char stringa[MAX_WORD];
	ptr = fopen(nameFile, "r");
	if (ptr == NULL) {
		fclose(ptr);
		return (NULL);
	}
	//Inserisco tutte le word(nodi) all'interno dell'albero rosso nero
	while (feof(ptr) == 0) {
		fscanf(ptr, "%s", stringa);
		if (dictionary == NULL)
			init(&dictionary, &T, stringa,NULL);   //Creo la sentinella e la prima word
		else if(dictionary != NULL)
			insertWord(&dictionary, stringa);
	}
	fclose(ptr);
	return dictionary;
}

void printDictionary(NODO*  dictionary) {
	printDictionaryGen(dictionary, stdout);
}

NODO* searchNode(NODO* root, char* word) {
	if (root == NULL || word == NULL) return NULL;
	NODO* x = root;
	int ret;
	char stringa[MAX_WORD];
	strcpy(stringa, word);
	conversionLowecase(stringa);
	while (x != root->p) {
		ret = strcmp(stringa, x->word);
		if (ret == 0)
			return x;
		if (ret<0)
			x = x->left;
		else
			x = x->right;
	}
	return NULL;
}

int countWord(NODO* dictionary) {
	if (dictionary == NULL) return 0;		//Struttura vuota
	if (dictionary == dictionary->right) return 0;	//Se dictionary == dictionary->right significa che sono arrivato alla sentinella T.nil
	return(countWord(dictionary->left) + countWord(dictionary->right) + 1);
}

int insertDef(NODO* dictionary, char* word, char* def) {
	if (strlen(def) > MAX_DEF - 1) return 1;      //Definizione troppo lunga
	NODO* x = searchNode(dictionary, word);
	if (x == NULL) return 1;  //Word non presente all'interno dell'albero rosso nero
	if (x->def != NULL) free(x->def); 
	char *string = creaStringa(def, strlen(def));
	if (string == NULL) return NULL;
	x->def = string;
	conversionLowecase(x->def);
	return 0;
}

char* searchDef(NODO* dictionary, char* word) {
	NODO *x = searchNode(dictionary, word);
	if (x == NULL) return "(null)";
	return x->def;
}

char* creaStringa(char *word,int dim) {
	char *string = (char*)malloc((1 + dim) * sizeof(char));
	if (string == NULL) return NULL;
	strcpy(string, word);
	return string;
}


int saveDictionary(NODO* dictionary, char* fileOutput) {
	if (dictionary == NULL) return NULL;   //Struttura vuota
	FILE *ptr;
	ptr = fopen(fileOutput, "w");
	if (ptr == NULL) {     //Controllo se il puntatore ptr è uguale a NULL
		fclose(ptr);
		return -1;
	}
	printDictionaryGen(dictionary,ptr);
	fclose(ptr);
	return 0;
}

void printDictionaryGen(NODO*  dictionary, FILE *ptr) {
	if (dictionary == NULL) return;					//Struttura vuota
	if (dictionary != dictionary->right) {          //Se dictionary == dictionary->right significa che sono arrivato alla sentinella T.nil
		printDictionaryGen(dictionary->left,ptr);
		fprintf(ptr,"\"%s\" : [%s]\n", dictionary->word, dictionary->def);
		printDictionaryGen(dictionary->right,ptr);
	}
}


char* getWordAt(NODO* dictionary, int index) {
	if (dictionary == NULL)        //Struttura vuota
		return NULL;
	char *b = NULL;
	int n = index;
	getWordAtHelp(dictionary, &b, &n);
	return b;
}

void getWordAtHelp(NODO* dictionary, char** word, int* index) {
	if (dictionary == dictionary->right)   //Caso base
		return *index;
	getWordAtHelp(dictionary->left,&(*word) ,&(*index));
	if (*index >= -1) {
		if (*index > -1)
			*index = *index - 1;
		if (*index == -1 && dictionary != dictionary->right) {
			(*word) = dictionary->word;
			*index = *index - 1;
			return *index;
		}
		getWordAtHelp(dictionary->right,&(*word), &(*index));
	}
	return *index;
}

NODO* importDictionary(char *fileInput) {
	int i,len_word,ret;
	NODO *T, *dictionary = NULL,*z;
	FILE *ptr;
	char stringaWord[MAX_WORD], stringaDef[MAX_DEF];
	//Stringa generica che contiene una riga scritta nel file: la riga comprende 1 word ed una definizione
	char string[MAX_WORD + MAX_DEF + 10];
	ptr = fopen(fileInput, "r");
	if (ptr == NULL) {   
		fclose(ptr);
		return NULL;
	}
	while (feof(ptr) == 0) {
		fgets(string, MAX_WORD + MAX_DEF + 10, ptr);  //Leggo un'intera riga da file : una riga comprende la word e la sua definizione
		//Copio la word su una stringa
		for (i = 1; string[i] != '"';i++){
			stringaWord[i-1] = string[i];
		}
		len_word = i-1;
		stringaWord[len_word] = '\0';   //Aggiungo il terminatore di stringa alla word
		//Copio la definizione su una stringa
		for (i += 5;string[i] != ']';i++){
			stringaDef[i-len_word-6] = string[i];
		}
		//Sostituisco il carattere ']' presente sulla stringa della definzione con il terminatore di stringa
		stringaDef[i-len_word-6] = '\0';
		if (dictionary == NULL) {
			init(&dictionary,&T,stringaWord,stringaDef);
		}
		else if (dictionary != NULL) {
			z = allocaNodo();
			if (z == NULL) return 1;   //Ritorna 1 se la malloc non è riuscita ad allocare spazio per il nuovo nodo
			ret = AssegnaWordDefNode(&z, stringaWord, stringaDef);
			if (ret == 1) return 1; //La parola non può essere inserita perchè o è troppo corta o troppo lunga o perchè contiene caratteri di punteggiatura
			ret = rbInsert(&dictionary, &z);
			if (ret == 1)
				free(z);
		}
 	}
	fclose(ptr);
	return dictionary;
}

int minimum(int a, int b, int c) {
	/* funzione che calcola il minimo di 3 valori */
	int min = a;
	if (b < min) min = b;
	if (c < min) min = c;
	return min;
}

int Levenshtein_distance(char *x, char *y) {
	int m = strlen(x);
	int n = strlen(y);

	int i, j;
	int distance;

	int *prev = malloc((n + 1) * sizeof(int));
	int *curr = malloc((n + 1) * sizeof(int));
	int *tmp = 0;

	for (i = 0; i <= n; i++)
		prev[i] = i;

	for (i = 1; i <= m; i++) {
		curr[0] = i;
		for (j = 1; j <= n; j++) {
			if (x[i - 1] != y[j - 1]) {
				int k = minimum(curr[j - 1], prev[j - 1], prev[j]);
				curr[j] = k + 1;
			}
			else {
				curr[j] = prev[j - 1];
			}
		}
		tmp = prev;
		prev = curr;
		curr = tmp;

		memset((void*)curr, 0, sizeof(int) * (n + 1));
	}

	distance = prev[n];

	free(curr);
	free(prev);

	return distance;
}

int searchAdvance(NODO* dictionary, char* word, char** primoRis, char** secondoRis, char** terzoRis) {
	if (dictionary == NULL) return 0;
	int x, y;
	(*primoRis) = NULL;
	(*secondoRis) = NULL;
	(*terzoRis) = NULL;
	NODO *parola=NULL;
	y = 2000000;
	implementSearchAdvance(dictionary, word,&(*primoRis),&(*secondoRis),&(*terzoRis), &parola, &x, &y);
	if (parola == NULL) return 0;
	(*primoRis) = parola->word;
	parola = NULL;
	y = 2000000;
	implementSearchAdvance(dictionary, word, &(*primoRis), &(*secondoRis), &(*terzoRis), &parola, &x, &y);
	if (parola == NULL) return 0;
	(*secondoRis) = parola->word;
	parola = NULL;
	y = 2000000;
	implementSearchAdvance(dictionary, word, &(*primoRis), &(*secondoRis), &(*terzoRis), &parola, &x, &y);
	if (parola == NULL) return 0;
	(*terzoRis) = parola->word;
	return 1;
}

void implementSearchAdvance(NODO*  dictionary, NODO* word,char** primoRis, char** secondoRis,char** terzoRis ,NODO** parola, int *x, int *y) {
	int z=1;
	if (dictionary == NULL) return;					//Struttura vuota
	if (dictionary != dictionary->right) {          //Se dictionary == dictionary->right significa che sono arrivato alla sentinella T.nil
		implementSearchAdvance(dictionary->left,word, &(*primoRis), &(*secondoRis), &(*terzoRis),&(*parola),&(*x),&(*y));
		if ((*primoRis) != NULL || (*secondoRis) != NULL) {
			z=strcmp((*primoRis), dictionary->word);
			if(z!=0 && (*secondoRis) != NULL)
				z = strcmp((*secondoRis), dictionary->word);
		}
		if (z != 0) {									//Se la word non appartiene già a primoRis o secondoRis
			(*x) = Levenshtein_distance(word, dictionary->word);
			if (*x < *y) {
				(*parola) = dictionary;
				(*y) = (*x);
			}
		}
		implementSearchAdvance(dictionary->right, word, &(*primoRis), &(*secondoRis), &(*terzoRis), &(*parola), &(*x), &(*y));
	}
}

int Parent(int i) {
	if(i%2==0)
		return (i/2)-1;
	return i / 2;
}

//Funzione che restituisce il figlio sinistro di un nodo in un albero Heap 
int LeftHeap(int i) {
	return (2 * (i+1))-1;
}

//Funzione che restituisce il figlio destro di un nodo in un albero Heap 
int RightHeap(int i) {
	return (2 * (i+1));
}

//Funzione per conservare la proprietà di un minHeap
void MinHeapify(HEAPMIN *A,int  i) {
	int minimum;
	TABELLA *tmp;
	int l = LeftHeap(i);
	int r = RightHeap(i);
	if (l <= A->heapSize && A->vett[l]->frequenza < A->vett[i]->frequenza) {
		minimum = l;
	}
	else minimum = i;
	if (r <= A->heapSize && A->vett[r]->frequenza < A->vett[minimum]->frequenza)
		minimum = r;
	if (minimum != i) {
		tmp = A->vett[i];
		A->vett[i] = A->vett[minimum];
		A->vett[minimum] = tmp;
		MinHeapify(A, minimum);
	}
}

//Funzione per costruire un MinHeap a partire da un array di input non ordinato
void BuildMinHeap(HEAPMIN *A) {
	int length = A->heapSize+1,i;
	for (i = (length/2)-1; i>=0 ; i--)
		MinHeapify(A, i);
}



int HeapExtractMin(HEAPMIN *A, TABELLA **x) {
	if (A->heapSize == -1) //Vettore dell'Heap vuoto
		return -1;
	*x = A->vett[0];
	A->vett[0] = A->vett[A->heapSize];
	A->heapSize = A->heapSize - 1;
	MinHeapify(A, 0);
	return 0;
}

void HeapDecreaseKey(HEAPMIN *A,int i,TABELLA *z) {
	int ret;
	TABELLA *tmp,*nuovo;
	A->vett[i] = z;
	while (i > 0 && A->vett[Parent(i)]->frequenza > A->vett[i]->frequenza) {
		tmp = A->vett[i];
		A->vett[i] = A->vett[Parent(i)];
		A->vett[Parent(i)] = tmp;
		i = Parent(i);
	}
}

int MinHeapInsert(HEAPMIN *A, TABELLA *z) {
	if (A->heapSize == A->length)   //Vettore dell'Heap pieno
		return -1;
	A->heapSize = A->heapSize + 1;
	HeapDecreaseKey(A, A->heapSize,z);
	return 0;
}

char compressHuffmanHelp(NODO* dictionary,TABELLA *z,char carattere,int *contatoreBit,FILE *ptr) {
	if (dictionary == NULL) return 0;					//Struttura vuota
	if (dictionary == dictionary->right) return carattere;  //Se dictionary == dictionary->right significa che sono arrivato alla sentinella T.nil
	else{          
		carattere = compressHuffmanHelp(dictionary->left, z,carattere,contatoreBit,ptr);
		carattere = scriviCodificaCarattere(z, dictionary->word, carattere, contatoreBit, ptr);   //Codifico la word
		carattere = scriviCodificaCarattere(z, "{", carattere, contatoreBit, ptr);     //Codifico il carattere per separare le word dalle definizioni
		carattere = scriviCodificaCarattere(z, dictionary->def, carattere, contatoreBit, ptr);   //Codifico la def
		carattere = scriviCodificaCarattere(z, "{", carattere, contatoreBit, ptr);    //Codifico il carattere per separare le definizioni dalle word
		carattere = compressHuffmanHelp(dictionary->right, z, carattere, contatoreBit, ptr);
	}
}

char scriviCodificaCarattere(TABELLA *z, char *stringa, char carattere, int *contatoreBit, FILE *ptr) {
	int i = 0, k, bit[20],flag = 0;
	if (stringa == 0 || stringa[0] == '{') {
		if(stringa == 0)
			k = codificaCarattere(z, bit, 0) - 1;
		else
			k = codificaCarattere(z, bit, 123) - 1;
		//scrivo i bit sul carattere
		while (k >= 0) {
			if (*contatoreBit == 8) {
				//Ho raggiunto il massimo numero di bit per 1 carattere...scrivo il carattere sul file
				fprintf(ptr, "%c", carattere);
				*contatoreBit = 0;
				carattere = 0;
			}
			carattere = carattere | bit[k]; //Applico la maschera per settare il bit meno significato
			if (*contatoreBit < 7)
				carattere = carattere << 1;
			*contatoreBit = *contatoreBit + 1;
			k--;
		}
	}
	else {
		while (stringa[i] != '\0')
		{
			k = codificaCarattere(z, bit, stringa[i]) - 1;
			//scrivo i bit sul carattere
			while (k >= 0) {
				if (*contatoreBit == 8) {
					//Ho raggiunto il massimo numero di bit per 1 carattere...scrivo il carattere sul file
					fprintf(ptr, "%c", carattere);
					*contatoreBit = 0;
					carattere = 0;
				}
				carattere = carattere | bit[k]; //Applico la maschera per settare il bit meno significato
				if (*contatoreBit < 7)
					carattere = carattere << 1;
				*contatoreBit = *contatoreBit + 1;
				k--;
			}
			i++;
		}
	}
	return carattere;
}

TABELLA* createTreeHuffman(HEAPMIN *A) {
	int i = 0, n = A->heapSize, ret;
	TABELLA *z, *x, *y;
	//Creazione albero di Huffman
	for (i = 0; i < n;i++) {
		ret = HeapExtractMin(A, &x);
		if (ret == -1) return NULL;   //Il vettore era vuoto e quindi non posso estrarre niente
		ret = HeapExtractMin(A, &y);
		if (ret == -1) return NULL;
		x->bit = 0;
		y->bit = 1;
		ret = allocaTabella(&z, -2, x->frequenza + y->frequenza, x, y);
		if (ret == -1) return NULL;
		ret = MinHeapInsert(A, z);
		if (ret == -1) return NULL;
	}
	ret = HeapExtractMin(A, &z);  //Radice dell'albero;
	if (ret == -1) return NULL;
	z->bit = -2;
	return z;
}

int compressHuffman(NODO* dictionary, char* fileOutput) {
	int ret, i, bit[15],contatoreBit = 0;
	char carattere = 0;
	HEAPMIN A;
	TABELLA *z;
	FILE *ptr;
	NODO *l;
	ptr = fopen(fileOutput, "w");
	if (ptr == NULL) {
		fclose(ptr);
		return -1;
	}
	A.heapSize = -1;
	A.length = 255;
	ret = CreateFrequencyTableHeapHuffman(dictionary, &A);
	if (ret == -1) return -1;       //Errore allocazione malloc per il nuovo nodo
	//Costruisco la coda di priorità : MinHeap
	BuildMinHeap(&A);
	//Inserisco l'albero di Huffman all'interno del file
	for (i = 0; i <= A.heapSize;i++)
		fprintf(ptr, "%d %d\n", A.vett[i]->carattere,A.vett[i]->frequenza);
	fprintf(ptr, "%d %d ", -2, -2);   //Sequenza per indicare la fine del vettore
	z = createTreeHuffman(&A);  //Richiamo una funzione che mi crea l'albero di huffman
	if (z == NULL) return -1;
	carattere = compressHuffmanHelp(dictionary, z, carattere, &contatoreBit, ptr);  //Codifico le word e le definizioni chiamando una funzione ricorsiva che attraversa l'albero
	//Codifico l'ultimo carattere
	if (contatoreBit != 8)
		carattere = carattere << (8-contatoreBit-1);
	fprintf(ptr, "%c", carattere);
	fclose(ptr);
	//Applico la free dell'intero albero per liberare la memoria precedentemente occupata dalla funzione malloc
	freeAlberoHuffman(z);
	return 0;
}

//Restituisce il numero di bit necessari per la codifica del carattere
int codificaCarattere(TABELLA *z,int *bit,int carattere) {
	int i = 0;
	if(z == NULL) return 0;
	i = codificaCarattere(z->left,bit,carattere) + i;
	if ((z->bit != -2) && (z->carattere == carattere || i >= 1)) {
		bit[i] = z->bit;
		i = i + 1;
		return i;
	}
	i = codificaCarattere(z->right,bit,carattere) + i;
	if ((z->bit != -2) && (z->carattere == carattere || i >= 1)) {
		bit[i] = z->bit;
		i = i + 1;
		return i;
	}
	return i;
}

void freeAlberoHuffman(TABELLA *z) {
	if (z == NULL) return;
	freeAlberoHuffman(z->left);
	freeAlberoHuffman(z->right);
	free(z);
	return;
}

int decompressHuffman(char *fileInput, NODO** dictionary){
	int i, j,contatoreBit=8,codifica=0,separatore = 0, ret = 0;
	char carattere;
	NODO *T,*u;
	FILE *ptr;
	char stringaWord[MAX_WORD], stringaDef[MAX_DEF];
	ptr = fopen(fileInput, "r");
	if (ptr == NULL) {
		fclose(ptr);
		return NULL;
	}
	HEAPMIN A;
	TABELLA *z, *x, *y;
	A.heapSize = -1;
	A.length = 255;
	fscanf(ptr, "%d %d", &i, &j);
	//Rileggo l'Heap dal file
	while (i != -2) {
		ret = allocaTabella(&x, i, j, NULL, NULL);
		if (ret == -1) return -1;
		A.heapSize = A.heapSize + 1;
		A.vett[A.heapSize] = x;
		fscanf(ptr, "%d %d",&i,&j);
	}
	fscanf(ptr, "%c",&carattere);
	codifica = A.heapSize;
	z = createTreeHuffman(&A);  //Richiamo una funzione che mi crea l'albero di huffman
	if (z == NULL) return -1;
	x = z;   //Mi faccio una copia del puntatore della radice dell'albero per non perdere quello originale
	//Decodifico i bit
	j = 0;
	while (feof(ptr) == 0) {
		//Rileggo nuovo carattere
		if (contatoreBit == 8) {
			fscanf(ptr, "%c", &carattere);
			contatoreBit = 0;
		}
		codifica = carattere & 128;
		if (codifica == 128)
			codifica = 1;
		carattere = carattere << 1;
		contatoreBit++;
		//}
		//Scendo nell'albero
		if (codifica == 0)
			x = x->left;
		else if (codifica == 1)
			x = x->right;
		if (x->carattere != -2) {
			//Carattere separatore ... se separatore è uguale a 0 allora separo la word dalla definzione altrimenti separo la definizione dalla word
			if (x->carattere == 123 && separatore == 0) {
				stringaWord[j] = '\0';
				j = 0;
				separatore++;
			}
			else if (x->carattere == 123 && separatore == 1) {
					if (strcmp(stringaDef, "(null)") != 0)
						stringaDef[j] = '\0';
					if (dictionary == NULL)   
						init(&dictionary,&T,stringaWord,stringaDef);   //Creo la sentinella e la prima word
					else if (dictionary != NULL) {
						u = allocaNodo();
						if (u == NULL) return -1;   //Ritorna 1 se la malloc non è riuscita ad allocare spazio per il nuovo nodo
						ret = AssegnaWordDefNode(&u, stringaWord, stringaDef);
						if (ret == 1) return -1; //La parola non può essere inserita perchè o è troppo corta o troppo lunga o perchè contiene caratteri di punteggiatura
						ret = rbInsert(&(*dictionary), &u);
						if (ret == 1)
							free(u);
					}
					j = 0;
					separatore = 0;
			}
			//Aggiungo caratteri alla word
			else if (separatore == 0) {
				stringaWord[j] = x->carattere;
				j++;
			}
			//Aggiungo caratteri alla definizione
			else if (separatore == 1) {
				if (x->carattere == 0)
					strcpy(stringaDef, ("(null)"));
				else {
					stringaDef[j] = x->carattere;
					j++;
				}
			}
			x = z;
		}
	}
	fclose(ptr);
	freeAlberoHuffman(z);
	return 0;
}


int allocaTabella(TABELLA **nuovo,int carattere,int frequenza, TABELLA *left, TABELLA *right) {
	(*nuovo) = ((TABELLA*)malloc(sizeof(TABELLA)));
	if ((*nuovo) == NULL)
		return -1;
	(*nuovo)->carattere = carattere;
	(*nuovo)->frequenza = frequenza;
	(*nuovo)->left = left;
	(*nuovo)->right = right;
	return 0;
}

int CreateFrequencyTableHeapHuffman(NODO *dictionary,HEAPMIN *A) {
	int i,ret; 
	TABELLA *nuovo, *frequenze[256];   //In frequency applico una tabella hash a indirizzamento indiretto...
	for (i = 0;i < 256;i++)
		frequenze[i] = NULL;
	ret = allocaTabella(&nuovo, 123, 1,NULL,NULL);    //Carattere speciale per dividere le word dalle definizioni e per dividere le definizioni dalle word
	if (ret == -1) return -1;
	frequenze[123] = nuovo;
	ret = allocaTabella(&nuovo, 0, 1, NULL, NULL);   //Carattere NULL
	if (ret == -1) return -1;
	frequenze[0] = nuovo;
	ret = FrequencyTable(dictionary, &frequenze);
	if (ret == -1) return -1;
	//Aggiungo le lettere presenti nella tavola delle frequenze all'interno del vettore Heap
	for (i = 0;i < 256;i++) {
		if (frequenze[i] != NULL) {
			A->vett[A->heapSize+1] = frequenze[i];
			A->heapSize = A->heapSize + 1;
		}
	}
	return 0;
}

int FrequencyTable(NODO* dictionary,TABELLA **frequenze) {
	int i = 0,ret;
	TABELLA *nuovo;
	if (dictionary == NULL) return 0;					//Struttura vuota
	if (dictionary == dictionary->right) return 0;      //Se dictionary == dictionary->right significa che sono arrivato alla sentinella T.nil
	else{         
		ret = FrequencyTable(dictionary->left,frequenze);
		while (dictionary->word[i] != '\0') {      //Word 
			if (frequenze[dictionary->word[i]] != NULL)  //Se la lettera non è presente all'interno della tabella delle frequenze l'aggiungo
				frequenze[dictionary->word[i]]->frequenza = frequenze[dictionary->word[i]]->frequenza + 1;
			else { //Se la lettera è presente all'interno della tabella delle frequenze aumento di 1 la sua frequenza
				ret = allocaTabella(&nuovo, dictionary->word[i], 1, NULL, NULL);    
				if (ret == -1) return -1;
			frequenze[dictionary->word[i]] = nuovo;
			}
			i++;
		}
		frequenze[123]->frequenza = frequenze[123]->frequenza + 1;  //Aumento la frequenza del carattere speciale per dividere le word dalle definizioni
		if(dictionary->def == NULL)    //Aumento la frequenza del carattere NULL ovvero 0
			frequenze[0]->frequenza = frequenze[0]->frequenza + 1;
		else {
			i = 0;
			while (dictionary->def[i] != '\0') {   //Definizioni
				if (frequenze[dictionary->def[i]] != NULL)
					frequenze[dictionary->def[i]]->frequenza = frequenze[dictionary->def[i]]->frequenza + 1;
				else {
					ret = allocaTabella(&nuovo, dictionary->def[i], 1, NULL, NULL);
					if (ret == -1) return -1;
					frequenze[dictionary->def[i]] = nuovo;
				}
				i++;
			}
		}
		frequenze[123]->frequenza = frequenze[123]->frequenza + 1;    //Aumento la frequenza del carattere speciale per dividere le definizioni dalle word
		ret = FrequencyTable(dictionary->right,frequenze);
	}
}