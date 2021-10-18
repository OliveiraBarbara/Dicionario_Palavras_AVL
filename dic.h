#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

/*Struct da lista para armazenar os sinonimos*/
typedef struct lista_sinonimo{
	char sinonimo[MAX];
	struct lista_sinonimo *prox_sin; 
}lista_sinonimo;

/*Struct da arvore para armazenar as palavras*/
typedef struct arvore{
	char palavra[MAX];
	char idioma[3];
	int fb;
	struct lista_sinonimo *primeiro_conceito;
	struct arvore *esq; 
	struct arvore *dir;
}arvore;

/*Escopo de todas as funções utilizadas na implementação*/
lista_sinonimo *aloca_sinonimo(char *s);
arvore *aloca(char *str, char *id, char *s);

void rot_direita(arvore **tree);
void rot_esquerda(arvore **tree);
void rot_dupla_direita(arvore **tree);
void rot_dupla_esquerda(arvore **tree);

void insereArvore(arvore **tree, char *str, char *id, char *sin, int *mudou);
void insere(arvore **tree, char *str, char *id, char *sin);

void busca(arvore *tree, char *str);

void remover(arvore **tree, char *str);
void removeArvore(arvore **tree, char *str, int *mudou);
void remove_sinonimo(arvore **tree, char *str1, char *str2);

void lista_idioma(arvore *tree, char *id);
void lista_idioma_emIntervalo(arvore *tree, char *id, char *i, char *f);

void libera(arvore *tree);
