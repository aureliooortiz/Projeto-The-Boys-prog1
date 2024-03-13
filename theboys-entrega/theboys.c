// programa principal do projeto "The Boys - 2023"
// Autor: Aurélio Gabriel Ortiz Vieira, GRR: 20231953

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "boys.h"

// minimize o uso de variáveis globais
#define EV_CHEGA -1
#define EV_ESPERA -2
#define EV_AVISA -3
#define EV_ENTRA -4
#define EV_SAI -5
#define EV_VIAJA -6
#define EV_DESISTE -7
#define EV_MISSAO -8
#define EV_FIM -10

#define T_FIM_DO_MUNDO 525600
	
// programa principal
int main (){
	struct mundo_t *w;
	struct evento_t *e;
	
	srand(0);
	// iniciar o mundo
	w = cria_mundo();
	eventos_iniciais(w);
	
	// executar o laço de simulação
	while(w->tempo != T_FIM_DO_MUNDO){
		e = retira_lef(w->lef);
		w->tempo = e->tempo;
		switch(e->tipo){
			case EV_CHEGA:											// 1º
				// Dado 1 é o herói
				// Dado 2 é a base
				CHEGA(w,e->dado1,e->dado2);
				printf("\n");			
			break;
			case EV_ESPERA: 										// 2º	
				ESPERA(w,e->dado1,e->dado2);
				printf("\n");
			break;		
			case EV_AVISA:											// 3º
				AVISA(w,e->dado1,e->dado2);
			break;
			case EV_ENTRA:											// 4º	
				ENTRA(w,e->dado1,e->dado2);
				printf("\n");
			break;
			case EV_SAI:											// 5º
				SAI(w,e->dado1,e->dado2);
				printf("\n");			
			break;
			case EV_VIAJA:											// 6º
				VIAJA(w,e->dado1,e->dado2);
			break;
			case EV_DESISTE:										// 7º
				DESISTE(w,e->dado1,e->dado2);
			break;
			case EV_MISSAO:											// 8º
				// dado 1 representa o ID da missao
				MISSAO(w,e->dado1);
			break;
			case EV_FIM:											// Ultimo
				FIM(w);
			break;
		}
		destroi_evento(e);
	}

	// destruir o mundo
	destroi_mundo(w);
	
	printf("\n");
	return (0) ;
}

