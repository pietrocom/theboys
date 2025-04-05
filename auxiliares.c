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


// -------- Funcoes Auxiliares --------

int herois_contem_missao(struct mundo *M, struct base *B, struct missao *MI, struct cjto_t *uniao) {
	if (!M || !B || !MI || !uniao)
		return 0;

	// Limpa o conjunto recebido antes de usar
	for (int i = 0; i < uniao->cap; i++) {
		uniao->flag[i] = false;
	}

	// Cria conjunto auxiliar
	struct cjto_t *temp_uniao = cjto_cria(uniao->cap);
	if (!temp_uniao)
		return 0;

	// Itera sobre os herois presentes na base
	for (int id = 0; id < N_HEROIS; id++) {
		if (cjto_pertence(B->presentes, id)) {
			struct heroi *H = M->heroi[id];
			struct cjto_t *nova_uniao = cjto_uniao(temp_uniao, H->habilidades);
			if (!nova_uniao) {
				cjto_destroi(temp_uniao);
				return 0;
			}

			// Atualiza o conjunto auxiliar
			cjto_destroi(temp_uniao);
			temp_uniao = nova_uniao;
		}
	}

	// Atualiza o conjunto recebido com os valores finais
	for (int i = 0; i < uniao->cap; i++) {
		uniao->flag[i] = temp_uniao->flag[i];
	}

	int contem = cjto_contem(uniao, MI->habilidades);

	// Libera o conjunto auxiliar
	cjto_destroi(temp_uniao);

	return contem;
}

// Retorna um num entre min e max, inclusive
int aleat (int min, int max) {
	return rand() % (max - min + 1) + min;
}

int quadrado (int x) {
	return x * x;
}

int distancia_cartesiana (struct coordenadas c1, struct coordenadas c2) {
	float d;
	d = sqrt( quadrado(c1.x - c2.x) + quadrado(c1.y - c2.y) );
	// Converte o tipo e arredonda
	return (int)round(d);
}

