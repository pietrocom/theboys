#ifndef AUXILIARES
#define AUXILIARES

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


// -------- Structs Auxiliares --------

struct coordenadas {
	int x;
	int y;
};



// -------- Funcoes Auxiliares --------

// Se a uniao das habilidades dos herois contiver 
// as habilidades da missao retorna 1; 0 c.c.
int herois_contem_missao(struct mundo *M, struct base *B, struct missao *MI, struct cjto_t *uniao);

// Retorna um num entre min e max, inclusive
int aleat (int min, int max);

// Retorna o numero ao quadrado
int quadrado (int x);

// Retorna a distancia arredondada
int distancia_cartesiana (struct coordenadas c1, struct coordenadas c2);


#endif
