#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "boys.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS 50				// N_HABILIDADES * 5		50
#define N_BASES 8				// N_HEROIS / 6				8 ou 9
#define N_MISSOES 5256			// T_FIM_DO_MUNDO / 100		5256 

// Retorna um numero aleatorio entre min e max
int aleat (int min, int max){
	return min + rand() % (max-min + 1);
}

// Cria um novo conjunto com elems itens aleatorios cujo
// valor está entre 0 e max
struct set_t *set_aleat (int elems,int max){
	struct set_t *s;
	int n;
	
	s = set_create (max);
   
	// Adiciona aleatorio entre 0 e max no conjunto
	// e continua a adicionar até atingir elems itens
	while(set_card(s) < elems){
		n = aleat(0, max);
		set_add(s, n);
	}
	return s;
} 

// Retorna uma variavel mundo inicializada
// ou NULL em caso de falha
struct mundo_t *cria_mundo(){
	struct mundo_t *W;
	
    // Cria o mundo
	W = malloc(sizeof(struct mundo_t));
	assert(W);

	// Tempo do mundo é dado minutos
	W->tempo = T_INICIO;
	
	// Vetor de heróis 
	W->Vhero = malloc(N_HEROIS * sizeof(struct heroi_t));
	assert(W->Vhero);
	
	// Vetor de bases
	W->Vbases = malloc(N_BASES * sizeof(struct base_t));
	assert(W->Vbases);
	
	// Vetor de missoes
	W->Vmissoes = malloc(N_MISSOES * sizeof(struct missoes_t));
	assert(W->Vmissoes);
	
	// Tentativas de missoes
	W->cumpridas = 0;
	W->falhas = 0;
	
    // Inicializa cada heroi
	for(int i = 0; i < N_HEROIS; i++){
		// Inicia o ID de cada herói
		W->Vhero[i].ID = i;
		// Um herói sem base terá base = -1
		W->Vhero[i].base = -1;
		// Experiencia do herói
		W->Vhero[i].exp = 0 ;
		// Velocidade aleatoria entre 50 e 5000
		W->Vhero[i].velo = aleat(50, 5000) ;
		// Paciencia aleatoria entre 0 e 100
		W->Vhero[i].paciencia = aleat(0, 100);
		// Habilidades aleatórias de 1 a 3,
		// que podem ser um inteiro de 0 a 10
		W->Vhero[i].hab = set_aleat(aleat (1,3), N_HABILIDADES);
	}
	
    // Inicializa cada base
	for(int i = 0; i < N_BASES; i++){
		// Inicia o ID de cada base
		W->Vbases[i].ID = i;
		// Fila onde os heróis esperam para entrar
		W->Vbases[i].espera = lista_cria();
		// Aleatoriza a capacidade da base entre 3 e 10
		W->Vbases[i].lotacao = aleat(3,10);
		// Id's dos heróis presentes no momento na base
		W->Vbases[i].presentes = set_create(N_HEROIS);
		// Cordenadas x,y da base
		W->Vbases[i].local = malloc(sizeof(struct coords_t));
		W->Vbases[i].local->x = aleat(0,N_TAMANHO_MUNDO-1);
		W->Vbases[i].local->y = aleat(0,N_TAMANHO_MUNDO-1);
	}
	
	// Incializa missões	
	for(int i = 0;i < N_MISSOES;i++){
		// Inicia ID de cada missão
		W->Vmissoes[i].ID = i;
		// Habilidades necessarias para concluir a missão
		// que será no minimo 6 e maximo 10
		W->Vmissoes[i].habilidades = set_aleat(aleat(6,10),N_HABILIDADES);
		// Coordenadas x,y da missão
		W->Vmissoes[i].local = malloc(sizeof(struct coords_t));
		W->Vmissoes[i].local->x = aleat(0,N_TAMANHO_MUNDO-1);
		W->Vmissoes[i].local->y	= aleat(0,N_TAMANHO_MUNDO-1);	
	}
	
	return W;
}

void imprime_chega(struct mundo_t *w,int heroi,int base){
	printf("%6d: CHEGA ",w->tempo);
	printf("HEROI %2d BASE %d ",heroi,base);
	printf("(%2d/%2d)",w->Vbases[base].presentes->num,w->Vbases[base].lotacao);
}

void imprime_espera(struct mundo_t *w,int heroi,int base){
	printf("%6d: ESPERA ",w->tempo);
	printf("HEROI %2d BASE %d ",heroi,base);
	// Quantidade de heróis aguardando na fila sem contar o mesmo
	printf("(%2d)",lista_tamanho(w->Vbases[base].espera)-1);
}

void imprime_avisa(struct mundo_t *w,int base){
	char FILA[5];
	
	FILA[0] = 'F';
	FILA[1] = 'I';
	FILA[2] = 'L';
	FILA[3] = 'A';
	FILA[4] = '\0';
	
	printf("%6d: AVISA  PORTEIRO",w->tempo);
	// Na base tal há tal quantidade de heróis
	// que suporta quantidade n de heróis
	printf(" BASE %d (%2d/%2d) ",base,w->Vbases[base].presentes->num,
									w->Vbases[base].lotacao);
	lista_imprime (FILA,w->Vbases[base].espera );	
}

void imprime_admite(struct mundo_t *w,int heroi,int base){
	printf("%6d: AVISA  PORTEIRO",w->tempo);
	printf(" BASE %d ADMITE %2d",base,heroi);
	printf("\n");
}

void imprime_entra(struct mundo_t *w,int heroi,int base,int TPB){
	printf("%6d: ENTRA  ",w->tempo);
	printf("HEROI %2d BASE %d ",heroi,base);
	printf("(%2d/%2d) ",w->Vbases[base].presentes->num, 
						w->Vbases[base].lotacao);
	printf("SAI %d",TPB);
}

void imprime_sai(struct mundo_t *w,int heroi,int base){
	printf("%6d: ",w->tempo);
	printf("SAI    HEROI %2d BASE %d ",heroi,base);
	printf("(%2d/%2d) ",w->Vbases[base].presentes->num, 
						w->Vbases[base].lotacao);
}

void imprime_viaja(struct mundo_t *w,int heroi,int base,long int distancia,long int duracao){
	printf("%6d: VIAJA  ",w->tempo);
	printf("HEROI %2d BASE %d BASE %d ", heroi, w->Vhero[heroi].base,base);
	printf("DIST %ld VEL %d CHEGA %ld ",distancia,
			w->Vhero[heroi].velo,duracao);
}

void imprime_desiste(struct mundo_t *w,int heroi,int base){
	printf("%6d: DESISTE HEROI  %2d BASE %d",w->tempo,heroi,base);
}

void imprime_missao(struct mundo_t *w,int missao){
	printf("%6d: MISSAO %d HAB REQ: ",w->tempo,missao);
	set_print(w->Vmissoes[missao].habilidades);
}

void imprime_base(struct mundo_t *w,int missao,int base,struct set_t *s){
	printf("%6d: MISSAO %d HAB BASE %d: ",w->tempo,missao,base);
	set_print(s);
}

void imprime_cumprida(struct mundo_t *w,int missao,int base){
	printf("%6d: MISSAO %d ",w->tempo,missao);
	printf("CUMPRIDA BASE %d HEROIS: ",base);
	set_print(w->Vbases[base].presentes);

}

void imprime_impossivel(struct mundo_t *w,int missao){
	printf("%6d: MISSAO %d IMPOSSIVEL\n",w->tempo,missao);
}

// Encerra a simulação, gerando um relatório 
// com informações sobre heróis e missões
void FIM(struct mundo_t *w){
	float porcentagem;
	float cumpridas;
	float falhas;
	float media;
	float missoes;
	
	printf("%6d: FIM\n",w->tempo);
	for(int i = 0;i < N_HEROIS;i++){
		printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ",
				w->Vhero[i].ID, w->Vhero[i].paciencia,w->Vhero[i].velo,
				w->Vhero[i].exp);
		set_print (w->Vhero[i].hab);
	}
	cumpridas = w->cumpridas;
	falhas = w->falhas;
	media = falhas / cumpridas;
	missoes = N_MISSOES;
	porcentagem = (cumpridas * 100) / missoes;
	printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), ",w->cumpridas,N_MISSOES,porcentagem);
	printf("MEDIA %.2f TENTATIVAS/MISSAO",media);
	printf("\n");
}

// Distância cartesiana = sqrt( (xa - xb)² + (ya - yb)² )
long calcula_distancia(long x1,long x2,long y1,long y2){
	long X,Y;
	long distancia;
	
	X = (x1 - x2);
	X *= X;
	Y = (y1 - y2);
	Y *= Y;
	distancia = sqrt(X + Y);

	return distancia;
}

// Enquanto não achar uma base
// com heróis que possam cumprir a missão
// imprime as bases em ordem da mais próxima 
// da missão até menos próxima
void MISSAO(struct mundo_t *w,int missao){
	struct set_t *u;
	struct evento_t *ev;
	long *dist;
	int indiceMenor;
	long menorValor;
	int k,m;
	short impossivel;
	
	imprime_missao(w,missao);
	
	// Cria um vetor de distancias de cada base para missao
	dist = malloc(N_BASES * sizeof(long));
	assert(dist);
	
	impossivel = 1; 
	// Calcula a distancia da missão de cada base  
	// e insere no vetor na posição ID da base
	for(int i = 0;i < N_BASES;i++){	
		dist[i] = calcula_distancia(w->Vbases[i].local->x,w->Vmissoes[missao].local->x,
									w->Vbases[i].local->y,w->Vmissoes[missao].local->y);
	}
	
	// Loop para imprimir as bases por ordem de proximidade
	// com a missão e parar se alguma base a puder cumprir	
	for(int i = 0; i < N_BASES; i++){
		// Encontra a base mais próxima não impressa
		indiceMenor = 0;
		// menorValor inicia com um valor máximo que não é possivel
		// estar no vetor (distancia máxima entre 2 pontos ≃ 28284)
		menorValor = N_TAMANHO_MUNDO*N_TAMANHO_MUNDO;
		for(int j = 0; j < N_BASES; j++){ 
			if(dist[j] < menorValor && dist[j] != -1) {
				menorValor = dist[j];
				indiceMenor = j;
			}
		}	
		// Verifica se a base pode cumprir a missão
		k = 0;
		m = 0;
		// Cria o conjunto da união das habilidades
		u = set_create (N_HABILIDADES);
		assert(u);										      
		while(m <= set_card (w->Vbases[indiceMenor].presentes) && k < N_HEROIS ){
			// Faz união das habilidades dos heróis presentes na base
			if(set_in (w->Vbases[indiceMenor].presentes, k) ){
				set_union (u, w->Vhero[k].hab, u);
				m++;	
			}
			k++;
		}
		
		// Verificar se habilidades de missao está contido
		// no conjunto u (conjunto habilidades da base)
		if(set_contains (w->Vmissoes[missao].habilidades,u)){
			impossivel = 0;
		}
						
		// Imprime a base mais próxima
		imprime_base(w,missao,indiceMenor,u);

		set_destroy (u);

		if(!impossivel){
			imprime_cumprida(w,missao,indiceMenor);
			// Incrementa a experiência dos heróis presentes na base BMP
			m = 0;
			k = 0;														
			while(m <= set_card (w->Vbases[indiceMenor].presentes) && k < N_HEROIS){
				if(set_in (w->Vbases[indiceMenor].presentes, k) ){
					w->Vhero[k].exp++;
					m++;
				}
				k++;
			}
			free(dist);
			// Incrementa quantidade de tentativas bem sucedidas
			w->cumpridas++;	
			return;
		}
		// Marca a base como impressa 
		dist[indiceMenor] = -1;      
	}
	
	imprime_impossivel(w,missao);
	free(dist);
	// Cria e insere na LEF o evento MISSAO (T + 24*60, M) para o dia seguinte
	ev = cria_evento (w->tempo + 24*60, -8, missao, -1);
	assert(ev);
	insere_lef (w->lef, ev);
	// Incrementa quantidade de tentativas falhas
	w->falhas++;
}

// O herói H se desloca para uma base D 
void VIAJA(struct mundo_t *w,int heroi,int base){
	struct evento_t *ev;
	long int duracao;
	long int distancia;
	
	// Distância = distância cartesiana entre a base atual de H e a base D 
	distancia = calcula_distancia(w->Vbases[w->Vhero[heroi].base].local->x,w->Vbases[base].local->x,
								w->Vbases[w->Vhero[heroi].base].local->y,w->Vbases[base].local->y);
	// Duração = distância / velocidade de H
	duracao = distancia / w->Vhero[heroi].velo;
	duracao += w->tempo; 
	
	imprime_viaja(w,heroi,base,distancia,duracao);
	printf("\n");
	
	// Cria evento CHEGA (agora + duração, H, D)
	ev = cria_evento (duracao, -1, heroi, base);
	assert(ev);
	insere_lef (w->lef, ev);
}

// O herói H sai da base B. Ao sair, escolhe uma base de destino para viajar
void SAI(struct mundo_t *w,int heroi,int base){
	struct evento_t *ev;
	int D;
	
	// Retira H do conjunto de heróis presentes em B
	set_del (w->Vbases[base].presentes, heroi);
	
	imprime_sai(w,heroi,base);
	
	// Escolhe uma base destino D aleatória	
	D = aleat(0,N_BASES-1);
	
	// Cria evento VIAJA (agora, H, D)
	// evento tipo -6 representa evento viaja
	ev = cria_evento (w->tempo, -6, heroi, D);
	assert(ev);
	insere_lef (w->lef, ev);
	
	// Cria evento AVISA (agora, B)
	// evento tipo -3 representa evento avisa
	ev = cria_evento (w->tempo, -3, heroi, base);
	assert(ev);
	insere_lef (w->lef, ev);		
}

// O herói H entra na base B. Ao entrar, 
// o herói decide quanto tempo vai ficar e agenda sua saída da base
void ENTRA(struct mundo_t *w,int heroi,int base){
	struct evento_t *ev;
	int TPB;	// Tempo de permanência na base
	
	// TPB = 15 + paciência de H * aleatório [1...20]
	TPB = 15 + w->Vhero[heroi].paciencia * aleat(1,20);
	TPB += w->tempo;
	
	// Cria evento SAI (agora + TPB, H, B)
	// tipo -5 representa o evento SAI
	ev = cria_evento (TPB, -5, heroi, base);
	assert(ev);
	insere_lef (w->lef, ev);
	
	imprime_entra(w,heroi,base,TPB);
}

// O porteiro da base B trata a fila de espera
void AVISA(struct mundo_t *w,int heroi,int base){
	struct evento_t *ev;
	
	imprime_avisa(w,base);
	printf("\n");
	
	// Enquanto houver vaga em B e houver heróis esperando na fila
	while(set_card(w->Vbases[base].presentes) < w->Vbases[base].lotacao
		&& !lista_vazia (w->Vbases[base].espera) ){ 
		// Retira primeiro herói (H') da fila de B
		lista_retira(w->Vbases[base].espera, &heroi, 0);
		
		// Adiciona H' ao conjunto de heróis presentes em B
		set_add (w->Vbases[base].presentes, heroi);
		imprime_admite(w,heroi,base);
		
		// Cria evento ENTRA (agora, H', B)
		// evento tipo -4 representa evento entra
		ev = cria_evento (w->tempo,-4, heroi, base);
		assert(ev);
		insere_lef (w->lef, ev);
	}
}

// O herói H desiste de entrar na base B
void DESISTE(struct mundo_t *w,int heroi,int base){
	struct evento_t *ev;
	int D;
	
	// Escolhe uma base destino D aleatória
	D = aleat(0,N_BASES-1);
	
	imprime_desiste(w,heroi,base);
	printf("\n");
	
	// Cria evento VIAJA (agora, H, D)
	// evento tipo -6 representa evento viaja
	ev = cria_evento (w->tempo,-6, heroi, D);
	assert(ev);
	insere_lef (w->lef,ev);
}

// Evento de um herói esperando na fila da base
void ESPERA(struct mundo_t *w,int heroi,int base){
	struct evento_t *ev;
	
	// Adiciona H ao fim da fila de espera de B
	lista_insere(w->Vbases[base].espera,
				heroi,w->Vbases[base].presentes->num);
	
	imprime_espera(w,heroi,base);
	
	// Cria evento AVISA (agora, B)
	// evento tipo -3 representa evento 
	ev = cria_evento (w->tempo,-3,heroi,base);
	assert(ev);
	insere_lef (w->lef,ev);
}

// Representa um herói H chegando em uma base B no instante T
void CHEGA(struct mundo_t *w,int heroi,int base){
	struct evento_t *ev;
	int espera;
	
	// W contem w->tempo portanto,
	// serve como o T (tempo)
	assert(w);
	
	// Atualiza a base de H
	w->Vhero[heroi].base = base;
	
	// Se há vagas em B e a fila de espera está vazia o herói espera	
	if(set_card (w->Vbases[base].presentes) < w->Vbases[base].lotacao 
		&& lista_tamanho(w->Vbases[base].espera) == 0){ 
		espera = 1;
	}else{
		espera = (w->Vhero[heroi].paciencia) > 
		(10 * lista_tamanho (w->Vbases[base].espera));
	}
	
	if(espera){
		// Cria evento ESPERA (agora, H, B)
		// evento tipo -2 representa evento avisa
		ev = cria_evento (w->tempo, -2, heroi, base);
		assert(ev);
		insere_lef (w->lef, ev);
		
		imprime_chega(w,heroi,base);
		printf(" ESPERA");
	}else{
		// Cria evento DESISTE (agora, H, B)
		// evento tipo -7 representa o evento desiste
		ev = cria_evento (w->tempo,-7,heroi,base);
		assert(ev);
		insere_lef (w->lef, ev);
		
		imprime_chega(w,heroi,base);
		printf(" DESISTE");
	}	
}

// Cria os eventos iniciais do mundo
void eventos_iniciais(struct mundo_t *w){
	struct evento_t *ev;
	int tempo;
	int base;
	
	// Cria a lista de eventos que ocorrerão no mundo
	w->lef = cria_lef();
	assert(w->lef);

	// Inicia todos os eventos de chegada	
	for(int i = 0; i < N_HEROIS;i++){
		// Base que o herói irá chegar
		base  = aleat(0,N_BASES-1);
		// Tempo que os eventos chega irão acontecer
		// que é no máximo 3 dias
		tempo = aleat(0,4320);
		
		// Inserir na LEF o evento CHEGA(tempo,herói,base)
		// evento tipo -1 representa o evento CHEGA 
		ev = cria_evento(tempo,-1,w->Vhero[i].ID,base);
		assert(ev);
		insere_lef(w->lef, ev);
	}
	
	// Inicia o tempo que cada missão irá ocorrer
	for(int i = 0; i < N_MISSOES;i++){
		// Tempo lógico que ocorre a missão
		tempo = aleat(0,T_FIM_DO_MUNDO);
		
		// Inserir na LEF o evento MISSÃO (tempo, M)
		// evento do tipo -8 representa o evento MISSÃO 
		ev = cria_evento(tempo,-8,w->Vmissoes[i].ID,-1);
		assert(ev);
		insere_lef(w->lef, ev);
	}
	
	// Inserir na LEF o evento FIM (T) com T = T_FIM_DO_MUNDO
	// evento tipo -10 representa o evento FIM
	tempo = T_FIM_DO_MUNDO;
	
	ev = cria_evento(tempo,-10,-1,-1);
	assert(ev);
	insere_lef(w->lef,ev);
}

void destroi_mundo(struct mundo_t *w){
	// Destroi bases
	for(int i = 0;i < N_BASES;i++){
		set_destroy(w->Vbases[i].presentes);
		lista_destroi(w->Vbases[i].espera);
		free(w->Vbases[i].local);
	}
	
	// Destroi heróis
	for(int i = 0;i < N_HEROIS;i++){
		set_destroy(w->Vhero[i].hab);
	}
	
	// Destroi missões
	for(int i = 0; i < N_MISSOES;i++){
		set_destroy(w->Vmissoes[i].habilidades);
		free(w->Vmissoes[i].local);
	}
	
	// Destroi mundo
	destroi_lef(w->lef);
	free(w->Vbases);
	free(w->Vhero);
	free(w->Vmissoes);
	free(w);

}

