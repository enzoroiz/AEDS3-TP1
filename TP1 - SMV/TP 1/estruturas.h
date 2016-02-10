#include <stdlib.h>
#include <stdio.h>
#define FALSE 0
#define TRUE  1

//########### TAD ÁRVORE #############

typedef long TipoChave;
typedef struct TipoRegistro {
  TipoChave Chave;
} TipoRegistro;
typedef struct TipoPagina* TipoApontador;
typedef struct TipoPagina {
  int n;
  TipoRegistro *r;//r[MM];
  TipoApontador *p;//p[MM+1];
} TipoPagina;

//########### TAD FILA #############

typedef struct Celula_strFila *ApontadorFila;
typedef struct Celula_strFila {
int numAcessos;
TipoPagina pagina;
ApontadorFila Prox;
} CelulaFila;
typedef struct TipoFila {
ApontadorFila Frente, Tras;
} TipoFila;

//########### FUNÇÕES ÁRVORE #############

void Inicializa(TipoApontador *);
void InsereNaPagina(TipoApontador, TipoRegistro, TipoApontador);
void Ins(TipoRegistro, TipoApontador, short *, TipoRegistro *,  TipoApontador *, int);
void Insere(TipoRegistro, TipoApontador *, int);
void Reconstitui(TipoApontador, TipoApontador, int, short *, int);
void Antecessor(TipoApontador, int, TipoApontador, short *, int);
void Ret(TipoChave, TipoApontador *, short *, int);
void Retira(TipoChave, TipoApontador *, int);

//########### FUNÇÕES SMV #############

int EstaNaMemoria(TipoFila, int, ApontadorFila *);
void ColocaNaMemoria(TipoPagina, TipoFila *, int, int *, int);
void HitLRU(TipoFila *, ApontadorFila, int, int *);
void HitLFU(ApontadorFila);
void CorrigeLFU(TipoFila *, int *, int);
int Politicas(TipoApontador, int, int, int *, int);
void Caminhamento(TipoRegistro *, TipoApontador, FILE *);

//########### FUNÇÕES FILA #############

void FFVazia(TipoFila *);
int Enfileira(TipoPagina, TipoFila *, int, int *);
void Desenfileira(TipoFila *, int *);
int Vazia(TipoFila);
