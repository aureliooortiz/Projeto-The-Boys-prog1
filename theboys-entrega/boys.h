// TAD de simulação theboys

#include "set.h"
#include "lef.h"
#include "lista.h"

#ifndef _THEBOYS_H
#define _THEBOYS_H

struct coords_t {
	int x;
	int y;
};

struct base_t {
	int ID;						// Identificação da base     
	int lotacao;				// Capacidade máxima da base
	struct set_t *presentes;	// Conjunto dos ID's que estão naquela base
	lista_t *espera;			// Fila onde os heróis esperam para entrar na base
	struct coords_t *local;		// Localização da base (x,y) 
};

struct heroi_t {   
	int ID;					// Numero que identifica o herói
	struct set_t *hab;		// Poderes do herói entre 0 e N_HABILIDADES
	int velo;				// Velocidade de deslocamento do herói entre 0 e 
	int base;				// ID da base onde se encontra o herói atualmente
	int exp;				// Indica o número de missões que o herói participou
	int paciencia;			// Vai indicar se um herói irá ou não esperar na fila 
							// da base que deseja entrar entre 0 e 	
};

struct missoes_t{
	int ID;						// Número inteiro ≥ 0 que identifica a missão
	struct set_t *habilidades;	// Conjunto de habilidades necessárias para cumprir a missão
	struct coords_t *local;		// Localização da missão (par de coordenadas inteiras X, Y ≥ 0)
};

struct mundo_t {
	int tempo;					// Tempo loǵico dos eventos
	int cumpridas;				// Tentativas de realizar missão bem sucedidas	
	int falhas;					// Tentativas de realizar missão falha	
	struct heroi_t *Vhero;		// Vetor de heróis
	struct base_t *Vbases;		// Vetor de bases
	struct missoes_t *Vmissoes;	// Vetor de missoes;
	struct lef_t *lef;			// Todos os eventos que ocorrerão no mundo
};

// Retorna uma variavel mundo inicializada
// ou NULL em caso de falha
struct mundo_t *cria_mundo();

// Encerra a simulação, gerando um relatório com informações sobre heróis e missões
void FIM(struct mundo_t *w);

// Enquanto não achar uma base
// com heróis que possam cumprir a missão
// imprime as bases em ordem da mais próxima 
// da missão até menos próxima
void MISSAO(struct mundo_t *w,int missao);

// O herói H se desloca para uma base D 
void VIAJA(struct mundo_t *w,int heroi,int base);

// O herói H sai da base B. Ao sair, escolhe uma base de destino para viajar
void SAI(struct mundo_t *w,int heroi,int base);

// O herói H entra na base B. Ao entrar, 
// o herói decide quanto tempo vai ficar e agenda sua saída da base
void ENTRA(struct mundo_t *w,int heroi,int base);

// O porteiro da base B trata a fila de espera
void AVISA(struct mundo_t *w,int heroi,int base);

// O herói H desiste de entrar na base B
void DESISTE(struct mundo_t *w,int heroi,int base);

// Evento de um herói esperando na fila da base
void ESPERA(struct mundo_t *w,int heroi,int base);

// Representa um herói H chegando em uma base B no instante T
void CHEGA(struct mundo_t *w,int heroi,int base);

// Cria os eventos iniciais do mundo
void eventos_iniciais(struct mundo_t *w);

// Libera memória de tudo no mundo
void destroi_mundo(struct mundo_t *w);














#endif
