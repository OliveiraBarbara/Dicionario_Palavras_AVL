/***************************************************
Nome: Bárbara Beatriz Bueno de Oliveira
RGA: 2019.0743.014-8
Implementacao 3
Disciplina: Estruturas de Dados e Programacao I
Professor: Ronaldo Fiorilo
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dic.h"

lista_sinonimo *aloca_sinonimo(char *s){
	lista_sinonimo *nova;
	
	nova = (lista_sinonimo *) malloc (sizeof (lista_sinonimo)); /*aloco um espaco na memoria*/
	
	if(nova != NULL){
		strcpy(nova->sinonimo, s); /*na posição da lista na variavel do sinonimo eu atribuo o sinonimo correto*/
		nova->prox_sin = NULL;
	}else{ /*se a alocacao nao deu certo, informo na tela*/
		printf("Erro ao alocar memória\n");
	}
	return nova;
}

arvore *aloca(char *str, char *id, char *s){
	arvore *nova;
	
	nova = (arvore *) malloc (sizeof (arvore)); /*aloco um espaço na memória*/
	
	if(nova != NULL){ /*se consegui alocar o espaço faço o novo nó receber o conteudo (palavra, sinonimo e idioma)*/
		strcpy(nova->palavra, str);
		strcpy(nova->idioma, id);
		nova->fb = 0;
		nova->primeiro_conceito = aloca_sinonimo(s);
		nova->esq = NULL;
		nova->dir = NULL;
	}else{ /*se a alocacao nao deu certo, informo na tela*/
		printf("Erro ao alocar memória\n");
	}
	return nova;
}

void rot_direita(arvore **tree){
	arvore *u, *p;
	p = (*tree);
	
	u = p->esq;
	p->esq = u->dir;
	u->dir = p;
	p->fb = 0;
	u->fb = 0;
	*tree = u;
}

void rot_esquerda(arvore **tree){
	arvore *u, *p;
	p = (*tree);
	
	u = p->dir;
	p->dir = u->esq;
	u->esq = p;
	p->fb = 0;
	u->fb = 0;
	*tree = u;
}

void rot_dupla_direita(arvore **tree){
	arvore *p, *u, *v;
	p = *tree;
	u = p->esq;
	v = u->dir;
	
	u->dir = v->esq;
	p->esq = v->dir;
	v->esq = u;
	v->dir = p;
	if(v->fb == 1){
		u->fb = (-1);
		p->fb = 0;
	}else if(v->fb == (-1)){
		p->fb = 1;
		u->fb = 0;
	}else if(v->fb == 0){
		u->fb = 0;
		p->fb = 0;
	}
	v->fb = 0;
	*tree = v;
}

void rot_dupla_esquerda(arvore **tree){
	arvore *p, *z, *y;
	p = *tree;
	z = p->dir;
	y = z->esq;
	
	z->esq = y->dir;
	p->dir = y->esq;
	y->esq = p;
	y->dir = z;
	if(y->fb == 1){
		z->fb = 0;
		p->fb = (-1);
	}else if(y->fb == (-1)){
		z->fb = 1;
		p->fb = 0;
	}else if(y->fb == 0){
		z->fb = 0;
		p->fb = 0;
	}
	y->fb = 0;
	*tree = y;
}

void insere(arvore **tree, char *str, char *id, char *sin){
	int mudou;
	insereArvore(tree, str, id, sin, &mudou);
}

void insereArvore(arvore **tree, char *str, char *id, char *sin, int *mudou){
	lista_sinonimo *nova, *aux;

	if(*tree == NULL){
		*tree = aloca(str, id, sin);
		(*mudou) = 1;
	}else if(strcmp(str, (*tree)->palavra) < 0){
		insereArvore(&(*tree)->esq, str, id, sin, mudou);
		if(*mudou == 1){
			(*tree)->fb = (*tree)->fb - 1;
			if((*tree)->fb == -2){
				if((*tree)->esq->fb == -1)
					rot_direita(tree);
				else
					rot_dupla_direita(tree);
				*mudou = 0;
			}else if((*tree)->fb == 0){
				*mudou = 0;
			}else if((*tree)->fb == -1){
				*mudou = 1;
			}
		}
	}else if(strcmp(str, (*tree)->palavra) > 0){
		insereArvore(&(*tree)->dir, str, id, sin, mudou);	
		if(*mudou == 1){
			(*tree)->fb = (*tree)->fb + 1;
			if((*tree)->fb == 2){
				if((*tree)->dir->fb == 1)
					rot_esquerda(tree);
				else
					rot_dupla_esquerda(tree);
				*mudou = 0;
			}else if((*tree)->fb == 0){
				*mudou = 0;
			}else if((*tree)->fb == 1){
				*mudou = 1;
			}
		}
	}else if(strcmp(str, (*tree)->palavra) == 0){ /*se a palavra já existir, aloco somento o seu novo sinonimo*/
		if(strcmp(sin, (*tree)->primeiro_conceito->sinonimo) < 0){
			nova = aloca_sinonimo(sin);
			nova->prox_sin = (*tree)->primeiro_conceito;
			(*tree)->primeiro_conceito = nova;
		}else{
			aux = (*tree)->primeiro_conceito;
			
			while(aux->prox_sin != NULL && (strcmp(sin, (*tree)->primeiro_conceito->sinonimo) > 0))
				aux = aux->prox_sin;
			
			nova = aloca_sinonimo(sin);
			nova->prox_sin = aux->prox_sin;
			aux->prox_sin = nova;
		}
	}
}

void busca(arvore *tree, char *str){
	lista_sinonimo *q;

	if(tree == NULL) /*se a palavra não existir na arvore ou a arvore estiver vazia eu retorno a mensagem*/
		printf("hein?\n");
	else{
		if(strcmp(str, tree->palavra) == 0){ /*se a palavra existir, imprimo seus respectivos sinonimos*/
			for(q = tree->primeiro_conceito; q != NULL; q = q->prox_sin)
				printf("%s\n", q->sinonimo);
		}else if(strcmp(str, tree->palavra) < 0) /*se a palavra for menor lexicograficamento eu busco na subarvore esquerda*/
			busca(tree->esq, str);
		else if(strcmp(str, tree->palavra) > 0)/*se a palavra for maior lexicograficamento eu busco na subarvore direita*/
			busca(tree->dir, str);
	}
}

void remover(arvore **tree, char *str){
	int mudou;
	removeArvore(tree, str, &mudou);
}

void removeArvore(arvore **tree, char *str, int *mudou){
	arvore *q;
	
	if((*tree) != NULL){/*se tiver elementos na arvore ou ainda nao cheguei ainda não cheguei ao fim da arvore*/
		if(strcmp(str, (*tree)->palavra) == 0){ /*verifico se a palavra que quero remover eh a palavra que está na arvore*/
			q = (*tree); /*faco q receber o elemento que quero remover*/
			if(((*tree)->esq == NULL) && ((*tree)->dir == NULL)){ /*remocao para no folha*/
				free(*tree); /*se for uma folha, removo direto o elemento*/
		          (*tree) = NULL; 
		          *mudou = 1;
			}else if((*tree)->esq == NULL){ /*remocao para no com um filho a direita*/
				(*tree) = (*tree)->dir; /*faço o no receber seu filho*/
				q->dir = NULL; /*faco o filho do q ser nulo*/
				free(q); /*removo o q*/
				*mudou = 1;
			}else if((*tree)->dir == NULL){ /*remocao para no com um filho a esquerda*/
				(*tree) = (*tree)->esq; /*faço o no receber seu filho*/
				q->esq = NULL; /*faco o filho do q ser nulo*/
				free(q);
				*mudou = 1;
			}else{ /*remocao para no com 2 filhos*/
				q = (*tree)->esq; /*o q recebe o filho esquerdo do no que vou remover*/
				while(q->dir != NULL) /*procuro o maior elemento mais a esquerda do no*/
					q = q->dir;
				strcpy((*tree)->palavra, q->palavra); /*passo as informacoes do no que eu encontrei para o no que vou remover*/
				strcpy((*tree)->idioma, q->idioma);
				(*tree)->primeiro_conceito = q->primeiro_conceito;
				removeArvore((&(*tree)->esq), (*tree)->palavra, mudou); /*chamo a funcao pra remover o no que troquei as informacoes*/
			}	
		}else if(strcmp(str, (*tree)->palavra) < 0){ /*se a palavra for menor lexicograficamento eu busco na subarvore esquerda o elemento que vou remover*/
			removeArvore((&(*tree)->esq), str, mudou);
			if(*mudou == 1){
				(*tree)->fb = (*tree)->fb + 1;
				if((*tree)->fb == 2){
					if((*tree)->dir->fb >= 0){
						rot_esquerda(tree);
						if((*tree)->fb == -1)
							*mudou = 0;
						else
							*mudou = 1;
					}else{
						rot_dupla_esquerda(tree);
						if((*tree)->fb == -1)
							*mudou = 0;
						else
							*mudou = 1;
					}
				}else if((*tree)->fb == 0)
					*mudou = 1;
				else if((*tree)->fb == 1)
					*mudou = 1;
			}
		}else if(strcmp(str, (*tree)->palavra) > 0){/*se a palavra for maior lexicograficamento eu busco na subarvore direita o elemento que vou remover*/
			removeArvore((&(*tree)->dir), str, mudou);
			if(*mudou == 1){
				(*tree)->fb = (*tree)->fb - 1;
				if((*tree)->fb == -2){
					if((*tree)->esq->fb <= 0){
						rot_direita(tree);
						if((*tree)->fb == -1)
							*mudou = 0;
						else
							*mudou = 1;
					}else{
						rot_dupla_direita(tree);
						if((*tree)->fb == -1)
							*mudou = 0;
						else
							*mudou = 1;
					}
				}else if((*tree)->fb == 0)
					*mudou = 1;
				else if((*tree)->fb == -1)
					*mudou = 0;
			}
		}
	}
}

void remove_sinonimo(arvore **tree, char *str1, char *str2){
	arvore *q;
	lista_sinonimo *p, *s;
	s = NULL;
	
	if(*tree != NULL){ /*se tiver elementos na arvore ou ainda nao cheguei ainda não cheguei ao fim da arvore*/
		if(strcmp(str1, (*tree)->palavra) == 0){ /*verifico se a palavra que quero remover eh a palavra que está na arvore*/
			p = (*tree)->primeiro_conceito;
			q = (*tree);
			while (p != NULL && (strcmp(p->sinonimo, str2) != 0)){ /*procuro o sinonimo na lista de sinonimo*/
				s = p;
				p = p->prox_sin;
			}
			if (p != NULL){ /*se ele estiver na lista eu verifico se ele é o primeiro sinonimo, se for o primeiro eu faco a primeira posicao da lista de sinonimo receber o sinonimo seguinte do que eu vou remover, e se ele nao for o primeiro, faco o sinonimo anterior receber o sinonimo seguinte do que vou remover*/
				if(s != NULL){
					s->prox_sin = p->prox_sin;
				}else{
					(*tree)->primeiro_conceito = p->prox_sin;
				}
				free(p); /*removo o sinonimo*/
			}
			if(q != NULL && q->primeiro_conceito == NULL) /*verifico se a palavra ficou sem sinonimo, se ficou eu chamo a funcao para remover ela*/
				remover((&(*tree)), str1);
		}else if(strcmp(str1, (*tree)->palavra) < 0) /*se a palavra for menor lexicograficamento eu busco na subarvore esquerda o elemento que vou remover*/
			remove_sinonimo((&(*tree)->esq), str1, str2);
		else if(strcmp(str1, (*tree)->palavra) > 0) /*se a palavra for maior lexicograficamento eu busco na subarvore direita o elemento que vou remover*/
			remove_sinonimo((&(*tree)->dir), str1, str2);
	}
}


void lista_idioma(arvore *tree, char *id){ 
	lista_sinonimo *q;
	
	if (tree != NULL){ /*verifico se a arvore nao eh nula*/
		lista_idioma(tree->esq, id); /*vou ate a folha da subarvore esquerda*/
		if(strcmp(id, tree->idioma) == 0){ /*verifico se o idioma eh o que quero*/
			printf("%s : ", tree->palavra); /*imprimo a palavra com seus sinonimos*/
			printf("%s", tree->primeiro_conceito->sinonimo);
			for(q = tree->primeiro_conceito->prox_sin; q != NULL; q = q->prox_sin)
				printf(", %s", q->sinonimo);
			printf("\n");
		}
		lista_idioma(tree->dir, id); /*vou ate a folha da subarvore direita*/
	} 
}

void lista_idioma_emIntervalo(arvore *tree, char *id, char *i, char *f){
	lista_sinonimo *q;
	
	if (tree != NULL){ /*verifico se a arvore nao eh nula*/
		lista_idioma_emIntervalo(tree->esq, id, i, f); /*vou ate a folha da subarvore esquerda*/
		if((strcmp(id, tree->idioma) == 0)){ /*verifico se o idioma eh o que quero*/
			if((tree->palavra[0] >= i[0]) && (tree->palavra[0] <= f[0])){ /*verifico se a palavra esta entre as iniciais informadas*/
				printf("%s : ", tree->palavra); /*imprimo a palavra com seus sinonimos*/
				printf("%s", tree->primeiro_conceito->sinonimo);
				for(q = tree->primeiro_conceito->prox_sin; q != NULL; q = q->prox_sin)
					printf(", %s", q->sinonimo);
				printf("\n");
			}
		}
		lista_idioma_emIntervalo(tree->dir, id, i, f); /*vou ate a folha da subarvore direita*/
	} 
}

/*Funcao libera: Libera o espaco que foi armazenado em uma arvore*/
void libera(arvore *tree){/*realizo a liberacao do espaço armazenado da arvore em pos-ordem para começar dos nos folhas e por ultimo a raiz*/
	
	if(tree != NULL){ 
		libera(tree->esq);
		libera(tree->dir);
		free(tree);
	}
}
