// programa principal do projeto "The Boys - 2024/2"
// Autor: Pietro Comin, GRR 20241955

#include <stdlib.h>
#include <stdio.h>
#include "fprio.h"
#include "lista.h"
#include "fila.h"
#include "conjunto.h"

// seus #defines vão aqui

#define T_INICIO         0
#define T_FIM_DO_MUNDO   525600
#define N_TAMANHO_MUNDO  20000
#define N_HABILIDADES    10
#define N_HEROIS         N_HABILIDADES * 5
#define N_BASES          N_HEROIS / 5
#define N_MISSOES        T_FIM_DO_MUNDO / 100

#define ESPERA    0
#define DESISTE   1
#define AVISA     2
#define VIAJA     3
#define ENTRA     4
#define SAI       5
#define MISSAO    6


// Structs de Entidades

struct coordenadas {
	int x;
	int y;
}

struct heroi {
	int id;
	struct cjto_t habilidades;
	int paciencia;
	int velocidade;
	int experiencia;
	int base;
}

struct base {
	int id;
	int lotacao;
	struct cjto_t *presentes;
	struct fila_t *espera;
	struct coordenadas local;
}

struct missao {
	int id;
	struct cjto_t habilidades;
	int perigo;
	struct coordenadas local;
}

struct mundo {
	int NHerois;
	struct heroi[];
	int NBases;
	struct base[];
	int NMissoes;
	struct missao[];
	int NHabilidades;
	struct coordenadas Tamanho_Mundo;
	int T;
}


// Structs de Eventos

// T, H, B
struct evento_1 {
	struct heroi H;
	struct base *B;
	int T;
}




// Funcoes

void chega (struct evento_1 *evento, struct fprio_t *LEF) {
	float espera;
	int tamanho_fila = fprio_tamanho(B->espera);

	evento->H->base = evento->B->id;

	if ( (evento->B->lotacao < cjto_card(evento->B->presentes)) && (tamanho_fila == 0) )
		espera = 1;
	else 
		espera = evento->H->paciencia > (10 * tamanho_fila);

	if (espera)
		fprio_insere(LEF, evento, ESPERA, evento->T);
	else
		fprio_insere(LEF, evento, DESISTE, evento->T);
}

void espera (struct evento_1 *evento, struct fprio_t *LEF) {
	int erro = fila_insere(evento->B->espera, evento->H);
	
	fprio_insere(LEF, evento)
}



// Programa Principal
int main () {
	

	return (0) ;
}

