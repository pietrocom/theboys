#ifndef EVENTOS
#define EVENTOS

#include "entidades.h"
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"

// -------- Structs de Eventos --------

// T, H, B, M
struct evento_1 {
	struct heroi *H;
	struct base *B;
	struct mundo *M;
	int T;
};

// T, B, M 
struct evento_2 {
	struct base *B;
	struct mundo *M;
	int T;
};

// T, H, B, D, M
struct evento_3 {
	struct heroi *H;
	struct base *B;
	struct base *D;
	struct mundo *M;
	int T;
};

// T, M, MI
struct evento_4 {
	struct mundo *M;
	struct missao *MI;
	int T;	
};

// T, M
struct evento_5 {
	struct mundo *M;
	int T;
};

// T, H, B, M, MI
struct evento_6 {
	struct heroi *H;
	struct base *B;
	struct mundo *M;
	struct missao *MI;
	int T;
};


// -------- Funcoes de Evento --------

// Para todas as funcoes, em caso de sucesso o retorno sera 0.
// Caso contrario, retorno sera -1.


// Determina se ira esperar ou desistir.
// Atualiza a LEF com o proximo evento de desistencia ou espera.
int chega (struct evento_1 *evento, struct fprio_t *LEF);
	
// Adiciona o heroi a fila de espera da base.
// Atualiza o maximo de tamanho dessa fila. 
int espera (struct evento_1 *evento, struct fprio_t *LEF);
	
// Remove o heroi da base.
// Agenda um evento de viagem para outra base.
int desiste (struct evento_1 *evento, struct fprio_t *LEF);
	
// Informa o porteiro da base sobre os herois na fila de espera.
// Admite herois na base se houver espaço.
// Agenda um evento de entrada para cada heroi admitido.
int avisa (struct evento_2 *evento, struct fprio_t *LEF);

// Processa a entrada de um heroi.
// Atualiza o tempo para proxima saida do heroi.
int entra (struct evento_1 *evento, struct fprio_t *LEF);

// Remove o heroi da lista de presentes.
// Agenda um evento de viagem.
int sai (struct evento_1 *evento, struct fprio_t *LEF);
	
// Calcula distancia de duracao para chegada em outra base.
int viaja (struct evento_3 *evento, struct fprio_t *LEF);

// Remove heroi da base.
// Avisa outras bases.
int morre (struct evento_6 *evento, struct fprio_t *LEF);

// Processa se foi cumprida, risco, morte ou ganho de experiencia.
int missao (struct evento_4 *evento, struct fprio_t *LEF);
	
// Finaliza a simulacao mostrando estatisticas.
int fim (struct evento_5 *evento, struct fprio_t *LEF);



#endif
