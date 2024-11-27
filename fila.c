#include "fila.h"
#include <stdio.h>
#include <stdlib.h>

struct fila_t *fila_cria() {
	struct fila_t *fila = malloc(sizeof(struct fila_t));
	if (!fila) 
		return NULL;

	fila->prim = NULL;
	fila->ult = NULL;
	fila->tamanho = 0;
	return fila;
}

struct fila_t *fila_destroi(struct fila_t *fila) {
	while (fila->prim != NULL) {
		struct item_t *aux = fila->prim;
		fila->prim = fila->prim->prox;
		free(aux);
	}
	free(fila);
	return NULL;
}

int fila_insere(struct fila_t *fila, int item) {
	if (fila == NULL)
		return -1;

	struct item_t *novo = malloc(sizeof(struct item_t));
	if (!novo) 
		return -1;

	novo->valor = item;

	// Caso a fila esteja vazia
	if (fila->prim == NULL) {
		fila->prim = novo;
		fila->ult = novo;
		novo->prox = NULL;
		fila->tamanho++;
		return fila->tamanho;
	}
 
	fila->ult->prox = novo;
	novo->prox = NULL;
	fila->ult = novo;
	fila->tamanho++;
	return fila->tamanho;
}

int fila_retira(struct fila_t *fila, int *item) {
	if (fila == NULL)
		return -1;

	struct item_t *aux;
	// Retira do inicio
	aux = fila->prim;
	*item = aux->valor;
	fila->prim = aux->prox;
	if (!fila->prim) 
		fila->ult = NULL;

	free(aux);
	fila->tamanho--;
	return fila->tamanho;
}

int fila_consulta(struct fila_t *fila, int *item) {
	// Verifica se fila eh nula ou pos invalida
	if (fila == NULL)
		return -1;

	struct item_t *aux = fila->prim;	

	*item = aux->valor;
	return fila->tamanho;
}

int fila_procura(struct fila_t *fila, int valor) {
	if (fila == NULL) return -1;

	struct item_t *aux = fila->prim;
	int i = 0;
	while (aux != NULL) {
		if (aux->valor == valor) return i;
		aux = aux->prox;
		i++;
	}
	// Se nao encontrar retorna erro
	return -1;  
}

int fila_tamanho(struct fila_t *fila) {
	if (fila == NULL) 
		return -1;
	return fila->tamanho;
}

void fila_imprime(struct fila_t *fila) {
	if (fila == NULL || fila->prim == NULL) {
		return;
	}

	struct item_t *aux = fila->prim;

	if (aux != NULL) {
		printf("%d", aux->valor);
		aux = aux->prox;
	}

	while (aux != NULL) {
		printf(" %d", aux->valor);
		aux = aux->prox;
	}
}

