#ifndef FILA
#define FILA

#include "eventos.h"
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"


// -------- #defines --------

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
#define CHEGA     7
#define FIM       8
#define MORRE     9

#define EXPERIENCIA 1


// -------- Structs de Entidades --------

struct heroi {
	int id;
	struct cjto_t *habilidades;
	int paciencia;
	int velocidade;
	int experiencia;
	int base;
	int status;  // Vivo 1, morto 0
};

struct base {
	int id;
	int lotacao;
	struct cjto_t *presentes;	
	struct lista_t *espera;
	struct coordenadas *local;
	int max;
	int n_missoes;
};

struct missao {
	int id;
	struct cjto_t *habilidades;
	int perigo;
	struct coordenadas *local;
	int status;   // Cumprida 1, 0 c.c.
	int tent_missao;
};

struct mundo {
	struct heroi *heroi[N_HEROIS];
	struct base *base[N_BASES];
	struct missao *missao[N_MISSOES];
	struct coordenadas *Tamanho_Mundo;
	int T;
	int n_eventos;	
};



// -------- Funcoes de Estado Inicial e Final --------

// Todas as funcoes de inicializacao retornarao -1
// ou NULL em caso de falha.


// Aloca e atribui estatisticas do mundo.
struct mundo *cria_mundo ();

// Atribui estatisticas aos herois.
int inicia_heroi (struct heroi *H, int id);
	
// Atribui estatisticas a base.
int inicia_base (struct base *B, int id);

// Atribui estatisticas a missao.
int inicia_missao (struct missao *MI, int id);

// Desaloca todas as entidades.
void destroi_mundo (struct mundo *M, struct fprio_t *LEF);

// Cria e insere no mundo as bases.
int cria_bases (struct mundo *M);

// Cria e insere os herois no mundo.
// Cria e insere o evento CHEGA na LEF.
int cria_herois (struct mundo *M, struct fprio_t *LEF);

// Cria e insere as missoes no mundo.
// Cria e insere na LEF os eventos MISSAO.
int cria_missoes (struct mundo *M, struct fprio_t *LEF);

// Agenda o fim do mundo.
int cria_fim_mundo (struct mundo *M, struct fprio_t *LEF);



#endif
