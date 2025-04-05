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
#include "auxiliares.h"


// -------- Funcoes de Eventos --------

// Para todas as funcoes, em caso de sucesso o retorno sera 0.
// Caso contrario, retorno sera -1.

int chega (struct evento_1 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	evento->M->n_eventos++;

	int espera;
	int tamanho_fila = lista_tamanho(evento->B->espera);
	if (tamanho_fila == -1)
		return -1;

	// Atualiza posicao do heroi
	evento->H->base = evento->B->id;
	
	// Se houver espaco e nao houver fila
	if ( (evento->B->lotacao > cjto_card(evento->B->presentes)) && (tamanho_fila == 0) )
		espera = 1;
	else 
		espera = evento->H->paciencia > (10 * tamanho_fila);


	// Chaves colocadas para evitar ambiguidade
	if (espera) {
		if (fprio_insere(LEF, evento, ESPERA, evento->T) == -1)
			return -1;
		printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", evento->T, 
			evento->H->id, evento->B->id, lista_tamanho(evento->B->espera), 
			evento->B->lotacao);
	}
	else {
		if (fprio_insere(LEF, evento, DESISTE, evento->T) == -1)
			return -1;
		printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", evento->T, 
			evento->H->id, evento->B->id, lista_tamanho(evento->B->espera), 
			evento->B->lotacao);
	}

	return 0;
}

int espera (struct evento_1 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	evento->M->n_eventos++;
	
	if (lista_insere(evento->B->espera, evento->H->id, -1) == -1)  
		return -1;

	// Atualiza tamanho maximo da fila	
	int tam = lista_tamanho(evento->B->espera);
	if (tam == -1)
		return -1;
	if (evento->B->max < tam)
		evento->B->max = tam;
	
	printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", evento->T, evento->H->id, 
		evento->B->id, lista_tamanho(evento->B->espera) - 1);

	struct evento_2 *evento_2;
	evento_2 = malloc(sizeof(struct evento_2));
	if (!evento_2)
		return -1;
	evento_2->B = evento->B;
	evento_2->T = evento->T;
	evento_2->M = evento->M;	

	if (fprio_insere(LEF, evento_2, AVISA, evento->T) == -1) {
		free(evento);
		free(evento_2);
		return -1;
	}

	free(evento);

	return 0;
}

int desiste (struct evento_1 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	evento->M->n_eventos++;

	int indice_base = aleat(0, N_BASES - 1);

	printf("%6d: DESIST HEROI %2d BASE %d\n", evento->T, evento->H->id, evento->B->id);

	struct evento_3 *evento_3;
	evento_3 = malloc(sizeof(struct evento_3));
	if (!evento_3)
		return -1;
	evento_3->T = evento->T;
	evento_3->B = evento->B;
	evento_3->H = evento->H;
	evento_3->M = evento->M;
	evento_3->D = evento->M->base[indice_base];

	if (fprio_insere(LEF, evento_3, VIAJA, evento->T) == -1)
		return -1;

	free(evento);

	return 0;
}


int avisa (struct evento_2 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	evento->M->n_eventos++;

	struct base *B = evento->B;

	printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [ ", evento->T, B->id, 
		cjto_card(B->presentes), B->lotacao);
	lista_imprime(B->espera);
	printf(" ]\n");

	// Se houver espaco e se nao houver fila
	while ( (cjto_card(B->presentes) < B->lotacao) && (lista_tamanho(B->espera) > 0) ) {
		int id;
		lista_retira(B->espera, &id, 0);

		if (cjto_insere(B->presentes, id) == -1)
			return -1;

		printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", evento->T, B->id, id);

		struct evento_1 *evento_entrada = malloc(sizeof(struct evento_1));
		if (!evento_entrada)
			return -1;

		evento_entrada->H = evento->M->heroi[id];
		evento_entrada->B = B;
		evento_entrada->M = evento->M;
		evento_entrada->T = evento->T;

		if (fprio_insere(LEF, evento_entrada, ENTRA, evento_entrada->T) == -1)
			return -1;
	}

	free(evento);
	return 0;
}


int entra (struct evento_1 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	evento->M->n_eventos++;

	int tpb = 15 + (evento->H->paciencia * aleat(1, 20));
	evento->T += tpb;	

	if (fprio_insere(LEF, evento, SAI, evento->T) == -1)
		return -1;

	printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", 
		evento->T - tpb, evento->H->id, evento->B->id, 
		cjto_card(evento->B->presentes), evento->B->lotacao, evento->T);
	
	return 0;
}


int sai (struct evento_1 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	evento->M->n_eventos++;

	if (cjto_retira(evento->B->presentes, evento->H->id) == -1)
		return -1;

	printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", evento->T, 
		evento->H->id, evento->B->id, cjto_card(evento->B->presentes), 
		evento->B->lotacao);

	int indice_base = aleat(0, N_BASES - 1);

	struct evento_3 *evento_3;
	evento_3 = malloc(sizeof(struct evento_3));
	if (!evento_3)
		return -1;
	evento_3->D = evento->M->base[indice_base];
	evento_3->T = evento->T;
	evento_3->B = evento->B;
	evento_3->H = evento->H;
	evento_3->M = evento->M;
	if (fprio_insere(LEF, evento_3, VIAJA, evento->T) == -1)
		return -1;

	struct evento_2 *evento_2;
	evento_2 = malloc(sizeof(struct evento_2));
	if (!evento_2)
		return -1;
	evento_2->T = evento->T;
	evento_2->B = evento->B;
	evento_2->M = evento->M;

	if (fprio_insere(LEF, evento_2, AVISA, evento_2->T) == -1)
		return -1;

	free(evento);

	return 0;
}


int viaja (struct evento_3 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	evento->M->n_eventos++;

	if (evento->H->velocidade <= 0)
		return -1;

	struct coordenadas *c1 = evento->B->local;
	struct coordenadas *c2 = evento->D->local;
	int d = distancia_cartesiana(*c1, *c2);
	int duracao = d / evento->H->velocidade;

	printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", 
	       evento->T, evento->H->id, evento->B->id, evento->D->id, d, 
	       evento->H->velocidade, evento->T + duracao);

	struct evento_1 *evento_1 = malloc(sizeof(struct evento_1));
	if (!evento_1) 
		return -1;

	evento_1->T = evento->T + duracao;
	evento_1->B = evento->D; // Base de destino
	evento_1->H = evento->H;
	evento_1->M = evento->M;

	if (fprio_insere(LEF, evento_1, CHEGA, evento_1->T) == -1) {
		free(evento_1);
		free(evento);
		return -1;
	}


	if (evento) {
		free(evento);
	}

	return 0;
}


int morre (struct evento_6 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	evento->M->n_eventos++;
	
	if (cjto_retira(evento->B->presentes, evento->H->id) == -1) {
		free(evento);
		return -1;
	}
	
	evento->H->status = 0;  // Morto	

	printf("%6d: MORRE  HEROI %2d MISSAO %d\n", evento->T, evento->H->id, 
		evento->MI->id);

	struct evento_2 *evento_2;
	evento_2 = malloc(sizeof(struct evento_2));
	if (!evento_2)
		return -1;
	evento_2->T = evento->T;
	evento_2->B = evento->B;
	evento_2->M = evento->M;

	if (fprio_insere(LEF, evento_2, AVISA, evento_2->T) == -1) {
		free(evento_2);
		free(evento);
		return -1;
	}

	free(evento);

	return 0;
}


int missao (struct evento_4 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	struct mundo *M = evento->M;
	struct missao *MI = evento->MI;

	MI->tent_missao++;

	printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", evento->T, MI->id, MI->tent_missao);
	cjto_imprime(MI->habilidades);
	printf(" ]\n");

	struct cjto_t *uniao_habilidades = cjto_cria(N_HABILIDADES);
	if (!uniao_habilidades)
		return -1;

	struct base *base_selecionada = NULL;
	int menor_distancia = INT_MAX;

	for (int i = 0; i < N_BASES; i++) {
		struct base *B = M->base[i];
		int distancia = distancia_cartesiana(*(B->local), *(MI->local));

		// Cria um conjunto auxiliar para calcular a uniao
		struct cjto_t *temp_uniao = cjto_cria(N_HABILIDADES);
		if (!temp_uniao) {
			cjto_destroi(uniao_habilidades);
			return -1;
		}

		// Verificar se os herois da base contem as habilidades da missao
		int contem = herois_contem_missao(M, B, MI, temp_uniao);	

		if (contem && distancia < menor_distancia) {
			base_selecionada = B;
			menor_distancia = distancia;

			// Atualizar o conjunto de habilidades de forma segura
			cjto_destroi(uniao_habilidades);
			uniao_habilidades = cjto_copia(temp_uniao);
			if (!uniao_habilidades) {
				cjto_destroi(temp_uniao);
				return -1;
			}
		}

		cjto_destroi(temp_uniao);
	}	

	// MI cumprida
	if (base_selecionada) {
		printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", evento->T, MI->id, 
			base_selecionada->id);
		cjto_imprime(uniao_habilidades);
		printf(" ]\n");

		base_selecionada->n_missoes++;

		// Atualizar status da missao
		MI->status = 1;

		// Para todos os herois da base
		for (int id = 0; id < N_HEROIS; id++) {
			if (cjto_pertence(base_selecionada->presentes, id)) {
				struct heroi *H = M->heroi[id];
				int risco = MI->perigo / (H->paciencia + H->experiencia + 1);

				// Caso de morte
				if (risco > aleat(0, 30)) {
					struct evento_6 *evento_morte = malloc(sizeof(struct evento_6));
					if (!evento_morte) {
						cjto_destroi(uniao_habilidades);
						return -1;
					}
					evento_morte->H = H;
					evento_morte->B = base_selecionada;
					evento_morte->MI = MI;
					evento_morte->M = M;
					evento_morte->T = evento->T;
					if (fprio_insere(LEF, evento_morte, MORRE, evento_morte->T) == -1) {
						cjto_destroi(uniao_habilidades);
						return -1;
					}
				} 
				// Se safou da morte
				else {
					H->experiencia += EXPERIENCIA;
				}
			}
		}
		free(evento);
	} 
	// MI reagendada
	else {
		printf("%6d: MISSAO %d IMPOSSIVEL\n", evento->T, MI->id);
		evento->T += 1440; 
		if (fprio_insere(LEF, evento, MISSAO, evento->T) == -1) {
			cjto_destroi(uniao_habilidades);
			uniao_habilidades = NULL;
			return -1;
		}
	}

	// Libera o conjunto principal
	cjto_destroi(uniao_habilidades);

	return 0;
}


int fim (struct evento_5 *evento, struct fprio_t *LEF) {
	if (!evento || !LEF)
		return -1;

	printf("525600: FIM\n");

	struct mundo *M = evento->M;

	M->n_eventos++;

	// Imprime informacoes sobre os herois
	for (int i = 0; i < N_HEROIS; i++) {
		struct heroi *H = M->heroi[i];
		if (H->status)
			printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [ ", i, 
				H->paciencia, H->velocidade, H->experiencia);
		else
			printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ ", i, 
				H->paciencia, H->velocidade, H->experiencia);
		cjto_imprime(H->habilidades);
		printf(" ]\n");
	}

	// Imprime informacoes sobre as bases
	for (int i = 0; i < N_BASES; i++) {
		struct base *B = M->base[i];
		printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %4d\n", i, 
			B->lotacao, B->max, B->n_missoes);	
	}

	printf("EVENTOS TRATADOS: %d\n", M->n_eventos);

	// Calcula e imprime a porcentagem de missoes cumpridas
	int missoes_cumpridas = 0;
	for (int i = 0; i < N_MISSOES; i++) {
		if (M->missao[i]->status) {
			missoes_cumpridas++;
		}
	}
	float n_missoes = N_MISSOES;
	float porcentagem_missoes = 100 * missoes_cumpridas / n_missoes;
	printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", missoes_cumpridas, 
		N_MISSOES, porcentagem_missoes);

	// Calcula e imprime tentativas por missao
	int tent_missao_max = 0;
	int tent_missao_min = INT_MAX;
	int soma_tentativas = 0;
	for (int i = 0; i < N_MISSOES; i++) {
		int tentativas = M->missao[i]->tent_missao;
		if (tentativas < tent_missao_min) {
			tent_missao_min = tentativas;
		}
		if (tentativas > tent_missao_max) {
			tent_missao_max = tentativas;
		}
		soma_tentativas += tentativas;
	}
	float media_tentativas = soma_tentativas / n_missoes;
	printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", tent_missao_min, 
		tent_missao_max, media_tentativas);

	// Calcula e imprime a taxa de mortalidade
	int mortos = 0;
	for (int i = 0; i < N_HEROIS; i++) {
		if (M->heroi[i]->status == 0) { // Status == 0 -> herói morto
			mortos++;
		}
	}
	float n_herois = N_HEROIS;
	float taxa_mortalidade = 100 * mortos / n_herois;
	printf("MORTOS: %d\n", mortos);
	printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mortalidade);

	free(evento);

	return 0;
}


