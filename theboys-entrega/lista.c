#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL se falhar.
lista_t* lista_cria (){
	lista_t *l;
	
	l = malloc(sizeof(lnode_t));
	if(!l) return NULL;
	
	l->size = 0;
	l->head = NULL;
	l->tail = NULL;
	
	return l;
}

// Remove todos os elementos da lista, libera espaço.
// Retorno: NULL.
lista_t* lista_destroi (lista_t* lst){
	lnode_t *temp;
	lnode_t *aux;
	
	if(!lst) return NULL;
	
	aux = lst->head;	
	while(aux){
		temp = aux;
		aux = aux->next;
		free(temp);
	}
	free(lst);
	return NULL;  
}	
	
// Nas operações insere/retira/consulta, a posição 0 indica o início da lista;
// se a posição for -1 ou for além do fim, considerar o fim da lista.

// Insere o elemento na lista na posição indicada.
// Retorno: número de elementos na lista após a operação ou -1 se falhar.
int lista_insere (lista_t* lst, int elem, int pos){
	lnode_t *aux;
	lnode_t *nodo;
	int cont;
	
	if(!lst) return -1;

	nodo = malloc(sizeof(lnode_t));
	if(!nodo) return -1;

	nodo->val = elem;
	if(!lst->head){					//inserir sem elementos
		nodo->prev = NULL;
		nodo->next = NULL;
		lst->head = nodo;
		lst->tail = nodo;
	}else if(pos == 0){				//inserir no inicio
		nodo->prev = NULL;
		nodo->next = lst->head;
		lst->head->prev = nodo;
		lst->head = nodo;
	}else if(pos >= lst->size || pos == -1){	//inserir no final
		nodo->prev = lst->tail;
		nodo->next = NULL;
		lst->tail->next = nodo;
		lst->tail = nodo;
	}else{
		cont = 0;
		aux = lst->head;
		while(cont < pos){			//inserir no meio
			aux = aux->next;
			cont++;	
		}
		nodo->next = aux;
		nodo->prev = aux->prev;			
		aux->prev = nodo;
		aux = nodo->prev;
		aux->next = nodo;
	}
	lst->size++;

	return lst->size;
}

// Retira o elemento da lista, na posição indicada.
// Retorno: número de elementos na lista após a operação ou -1 se falhar.
int lista_retira (lista_t* lst, int* elem, int pos){
	lnode_t *aux;
	int cont;
	
	if(!lst) return -1;
	//caso a posição seja menor que o minimo de posições
	if(pos < -1) return -1;
	//caso a lista esteja vazia
	if(!lst->head){ 
		return lst->size; 
	}	
	
	cont = 0;
	aux = lst->head;						
	if(lst->head == lst->tail){				//caso haja apenas um elemento
		lst->head = NULL;
		lst->tail = NULL;
	}else if(pos == -1 || pos >= lst->size){			//retira do final
		lst->tail = lst->tail->prev;
		aux = lst->tail->next;
		lst->tail->next = NULL;
	}else if(pos == 0){							//retira do inicio
		lst->head = lst->head->next;
		aux = lst->head->prev;
		lst->head->prev = NULL;
	}else{
		while(cont < pos){						//retira do meio
			aux = aux->next;					//aponta pra o elemento que se deseja remover
			cont++;
		}
		aux->next->prev = aux->prev;		
		aux->prev->next = aux->next;
	}
	*elem = aux->val;
	free(aux);
	lst->size--;
	
	return lst->size;
}

// Informa o valor do elemento na posição indicada, sem retirá-lo.
// Retorno: número de elementos na lista ou -1 se falhar.
int lista_consulta (lista_t* lst, int* elem, int pos){
	lnode_t *aux;
	int cont;
	
	if(!lst) return -1;
	if(pos < -1 || pos >= lst->size) return -1;

	aux = lst->head;
	if(pos == -1){
		while(aux->next){
			aux = aux->next;
		}
	}else{
		cont = 0;
		while(cont < pos){
			aux = aux->next;
			cont++;
		}
	}	
	*elem = aux->val;
	
	return lst->size;
}

// Informa a posição da 1ª ocorrência do elemento indicado.
// Retorno: posição do elemento ou -1 se não encontrar ou falhar.
int lista_procura (lista_t* lst, int elem){
	lnode_t *aux;
	int cont;
	
	if(!lst) return -1;

	aux = lst->head;
	cont = 0;
	while(aux->val != elem && cont < lst->size-1){
		aux = aux->next;
		cont++;
	}
	if(aux->val != elem){
		return -1;
	}
	return cont;
}

// Informa se a lista está vazia.
// Retorno: 1 se a lista está vazia ou 0 caso contrário.
int lista_vazia (lista_t* lst){
	if(!lst) return 0;
	
	if(!lst->head){
		return 1;
	}
	return 0;
}

// Informa o tamanho da lista (o número de elementos presentes nela).
// Retorno: número de elementos na lista ou -1 se falhar.
int lista_tamanho (lista_t* lst){
	if(!lst) return -1;

	return lst->size;
}	

// Imprime o conteúdo da lista, do inicio ao fim, neste formato:
// [ valor valor valor valor ... ]
// Esta função não faz parte do TAD, mas é útil para testes.
void lista_imprime (char *nome, lista_t* lst){
	lnode_t *aux;
	int cont;
	
	if(!lst){
		printf("ERRO\n");
		return;
	}
	
	printf("%s [", nome);
	
	aux = lst->head;
	cont = 0;
	while(cont < lst->size){
		printf(" %d",aux->val);
		aux = aux->next;
		cont++;
	}
	printf(" ]");
}
