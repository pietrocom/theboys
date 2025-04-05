#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "entidades.h"
#include "eventos.h"
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "entidades.h"
#include "auxiliares.h"


// -------- Funcoes de Estado Inicial e Final --------

struct mundo *cria_mundo () {
	struct mundo *M;
	M = malloc(sizeof(struct mundo));
	if (!M)
		return NULL;

	M->n_eventos = 0;	
	return M;
}

int inicia_heroi (struct heroi *H, int id) {
	H->id = id;

	H->experiencia = 0;

	H->paciencia = aleat(0, 100);

	H->velocidade = aleat(50, 5000);

	H->habilidades = cjto_aleat(aleat(1, 3), N_HABILIDADES);
	if (H->habilidades == NULL)
		return -1;

	H->status = 1;
	
	return 0;
}

int inicia_base(struct base *B, int id) {
	B->id = id;

	B->local = malloc(sizeof(struct coordenadas));
	if (!B->local)
		return -1;

	B->local->x = aleat(0, N_TAMANHO_MUNDO - 1);
	B->local->y = aleat(0, N_TAMANHO_MUNDO - 1);

	B->lotacao = aleat(3, 10);

	B->presentes = cjto_cria(N_HEROIS);
	if (!B->presentes) {
		free(B->local); 
		return -1;
	}

	B->espera = lista_cria();
	if (!B->espera) {
		cjto_destroi(B->presentes);  
		free(B->local);
		return -1;
	}

	B->max = 0;
	B->n_missoes = 0;
	return 0;
}


int inicia_missao(struct missao *MI, int id) {
	MI->id = id;

	MI->local = malloc(sizeof(struct coordenadas));
	if (!MI->local)
		return -1;

	MI->local->x = aleat(0, N_TAMANHO_MUNDO - 1);
	MI->local->y = aleat(0, N_TAMANHO_MUNDO - 1);

	MI->habilidades = cjto_aleat(aleat(6, 10), N_HABILIDADES);
	if (!MI->habilidades) {
		free(MI->local);  
		return -1;
	}

	MI->perigo = aleat(0, 100);
	MI->status = 0;
	MI->tent_missao = 0;
	return 0;
}


void destroi_mundo (struct mundo *M, struct fprio_t *LEF) {
	for (int i = 0; i < N_HEROIS; i++) {
		cjto_destroi(M->heroi[i]->habilidades);
		free(M->heroi[i]);
	}

	for (int i = 0; i < N_BASES; i++) {
		cjto_destroi(M->base[i]->presentes);
		lista_destroi(M->base[i]->espera);
		free(M->base[i]->local);
		free(M->base[i]);
	}

	for (int i = 0; i < N_MISSOES; i++) {
		cjto_destroi(M->missao[i]->habilidades);
		free(M->missao[i]->local);
		free(M->missao[i]);
	}

	free(M);
	fprio_destroi(LEF);
}

int cria_bases(struct mundo *M) {
	for (int i = 0; i < N_BASES; i++) {
		struct base *B = malloc(sizeof(struct base));
		if (!B)
			return -1;

		if (inicia_base(B, i) == -1) {
			free(B);  
			return -1;
		}

		M->base[i] = B;
	}

	return 0;
}


int cria_herois(struct mundo *M, struct fprio_t *LEF) {
	for (int i = 0; i < N_HEROIS; i++) {
		struct heroi *H = malloc(sizeof(struct heroi));
		if (!H)
			return -1;

		if (inicia_heroi(H, i) == -1) {
			free(H);  
			return -1;
		}

		M->heroi[i] = H;

		struct evento_1 *evento = malloc(sizeof(struct evento_1));
		if (!evento) {
			free(H);  
			return -1;
		}

		evento->B = M->base[aleat(0, N_BASES - 1)];
		evento->T = aleat(0, 4320);
		evento->H = H;
		evento->M = M;

		if (fprio_insere(LEF, evento, CHEGA, evento->T) == -1) {
			free(evento);  
			free(H);       
			return -1;
		}
	}

	return 0;
}


int cria_missoes(struct mundo *M, struct fprio_t *LEF) {
	for (int i = 0; i < N_MISSOES; i++) {
		struct missao *MI = malloc(sizeof(struct missao));
		if (!MI) 
			return -1;

		if (inicia_missao(MI, i) == -1) {
			free(MI); 
			return -1;
		}

		M->missao[i] = MI;

		struct evento_4 *evento = malloc(sizeof(struct evento_4));
		if (!evento) {
			cjto_destroi(MI->habilidades); 
			free(MI->local);             
			free(MI);                    
			return -1;
		}

		evento->M = M;
		evento->MI = MI;
		evento->T = aleat(0, T_FIM_DO_MUNDO - 1);

		if (fprio_insere(LEF, evento, MISSAO, evento->T) == -1) {
			free(evento);    
			cjto_destroi(MI->habilidades); 
			free(MI->local);             
			free(MI);                     
			return -1;
		}
	}

	return 0;
}



int cria_fim_mundo (struct mundo *M, struct fprio_t *LEF) {
	int tempo = T_FIM_DO_MUNDO;

	struct evento_5 *evento;
	evento = malloc(sizeof(struct evento_5));
	if (!evento)
		return -1;

	evento->M = M;
	evento->T = tempo;	

	if (fprio_insere(LEF, evento, FIM, tempo) == -1)
		return -1;

	return 0;
}

