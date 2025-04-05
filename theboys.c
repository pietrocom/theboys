// programa principal do projeto "The Boys - 2024/2"
// Autor: Pietro Comin, GRR 20241955

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "entidades.h"
#include "eventos.h"
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "auxiliares.h"


// ------- -Programa Principal --------

// Caso haja algum erro, retorna -1

int main () {

	//srand(time(NULL));
	srand(0);
	
// EVENTOS INICIAIS
	
	// * LEF
	struct fprio_t *LEF = fprio_cria();
	if (!LEF)
		return -1;

	// * Mundo
	struct mundo *M = cria_mundo();

	// * Bases
	if (cria_bases(M) == -1)
		return -1;	
	
	// * Herois
	if (cria_herois(M, LEF) == -1)
		return -1;

	// * Missoes
	if (cria_missoes(M, LEF) == -1)
		return -1;
	

	// * Fim
	if (cria_fim_mundo(M, LEF) == -1)
		return -1;


// LOOP PRINCIPAL

	int controle = 1;
	
	while (controle) {
		int *tipo = malloc(sizeof(int));
		int *prio = malloc(sizeof(int));	

		void *evento_geral = fprio_retira(LEF, tipo, prio);
	
		int status = 0;

		switch (*tipo) {
			case CHEGA:
				status = chega(evento_geral, LEF);
				break;
			case ESPERA:
				status = espera(evento_geral, LEF);
				break;
			case DESISTE:
				status = desiste(evento_geral, LEF);
				break;
			case AVISA:
				status = avisa(evento_geral, LEF);
				break;
			case ENTRA:
				status = entra(evento_geral, LEF);
				break;
			case SAI:
				status = sai(evento_geral, LEF);
				break;
			case VIAJA:
				status = viaja(evento_geral, LEF);
				break;
			case MORRE:
				status = morre(evento_geral, LEF);
				break;
			case MISSAO:
				status = missao(evento_geral, LEF);
				break;
			case FIM:
				fim(evento_geral, LEF);
				controle = 0; // Encerrar o loop principal
			default:
				break;
		}

		free(tipo);
		free(prio);
	
		if (status != 0) 
			return -1;

	}
	
	destroi_mundo(M, LEF);

	return (0) ;
}

