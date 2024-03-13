#include <stdio.h>
#include <stdlib.h>
#include "lef.h"

/* 
 * Cria um evento contendo os parâmetros informados.
 * Retorna um ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2){
	struct evento_t *ev;
	
	ev = malloc(sizeof(struct evento_t));
	if(!ev) return NULL;
	
	ev->tempo = tempo;
	ev->tipo = tipo;
	ev->dado1 = dado1;
	ev->dado2 = dado2;
	
	return ev;
}

/* 
 * Destroi um evento e retorna NULL.
 */ 
struct evento_t *destroi_evento (struct evento_t *e){
	if(!e) return NULL;
	
	free(e);
	
	return NULL;
}

/* 
 * Cria uma LEF vazia 
 * Retorna um ponteiro para a LEF ou NULL se falhar.
 */
struct lef_t *cria_lef (){
	struct lef_t *lef;
	
	lef = malloc(sizeof(struct lef_t));
	if(!lef) return NULL;

	lef->primeiro = NULL;	
		
	return lef;
}

/* 
 * Destroi a LEF, liberando a memória ocupada por ela, inclusive os eventos.
 * Retorna NULL.
 */
struct lef_t *destroi_lef (struct lef_t *l){
	struct evento_t *aux;
	
	if(!l) return NULL;
	
	// Retira Nodo da lef e verifica se há evento
	// se houver destroi evento
	while(!vazia_lef(l)){
		aux = retira_lef (l);
		if(aux){
			destroi_evento(aux);
		}	
	}
	free(l);
	return NULL;
}


/*
 * Insere o evento apontado na LEF na ordem de tempos crescentes.
 * Eventos com o mesmo tempo devem respeitar a politica FIFO.
 * Retorna 1 em caso de sucesso ou 0 caso contrario.
*/
int insere_lef (struct lef_t *l, struct evento_t *e){
	struct nodo_lef_t *nodo;
	struct nodo_lef_t *aux;
	struct nodo_lef_t *aux1;
	
	if(!l || !e) return 0;
	nodo = malloc(sizeof(struct nodo_lef_t));
	if(!nodo) return 0;
	
	aux = l->primeiro;
	//aux1 irá apontar para o nodo anterior a aux
	aux1 = aux;
	nodo->evento = e;	
	nodo->prox = NULL;
	//inserir caso não haja elementos ou caso o tempo 
	//do evento seja menor que o tempo do primeiro evento
	if(!l->primeiro || e->tempo < aux->evento->tempo){	
        nodo->prox = l->primeiro;
        l->primeiro = nodo;
	}else{	
		//enquanto houver nodo e o tempo do evento a ser inserido
		//for maior que o tempo 
		while(aux->prox && e->tempo >= aux->evento->tempo){
			//tratar eventos com tempo igual com FIFO
			if(e->tempo == aux->evento->tempo){
         	  	nodo->prox = aux->prox;
         	   	aux->prox = nodo;				
				return 1;
			}
			aux1 = aux;
			aux = aux->prox;
		}
		//caso seja o ultimo elemento
		if(e->tempo == aux->evento->tempo){
			nodo->prox = aux->prox;
			aux->prox = nodo;				
		}else if(e->tempo > aux->evento->tempo && !aux->prox){
			aux->prox = nodo;
			nodo->prox = NULL;
		}else{
			aux1->prox = nodo;
			aux1 = aux1->prox;
			aux1->prox = aux; 
		}	
	}
	
	return 1;
} 

/* 
 * Retira o primeiro evento da LEF.
 * Retorna ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *retira_lef (struct lef_t *l){
	struct evento_t *ptr;
	struct nodo_lef_t *aux;
	
	if(!l) return NULL;
	if(!l->primeiro) return NULL;
	
	ptr = l->primeiro->evento;
	aux = l->primeiro;
	//primeiro agora aponta para o segundo elemento
	l->primeiro = l->primeiro->prox;
	free(aux);
	
	return ptr;
}

/* 
 * Informa se a LEF está vazia.
 * Retorna 1 se estiver vazia, 0 se não estiver ou -1 se falhar.
 */
int vazia_lef (struct lef_t *l){
	if(!l) return -1;
	
	if(!l->primeiro){
		return 1;
	}
	return 0;
}

/* 
 * Imprime a LEF. Não faz parte do TAD, mas serve para depuração.
 * Formato de saída:
 * tempo tipo dado1 dado2
 * tempo tipo dado1 dado2
 * ...
 * total XX eventos
 */
void imprime_lef (struct lef_t *l){
	struct nodo_lef_t *aux;
	struct evento_t *ev;
	int i;
	
	if(!l) return;
	
	printf("LEF:\n");
	if(vazia_lef (l)){
		printf ("  Total 0 eventos\n");
		return;
	}
	
	aux = l->primeiro;
	ev = aux->evento;
	printf ("  tempo %d tipo %d d1 %d d2 %d\n", ev->tempo, ev->tipo, ev->dado1, ev->dado2);
	i = 1;
	while (aux->prox){
		aux = aux->prox;		
		ev = aux->evento;
		printf ("  tempo %d tipo %d d1 %d d2 %d\n", ev->tempo, ev->tipo, ev->dado1, ev->dado2);
		i++;
	}
	
	printf ("  Total %d eventos", i);
	printf("\n");
}	

