#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>

//########## OPERAÇÕES FILA ##########
void FFVazia(TipoFila *Fila){
    Fila->Frente = (ApontadorFila) malloc(sizeof(CelulaFila));
    Fila->Tras = Fila->Frente;
    Fila->Frente->Prox = NULL;
} /* FFVazia */

int Vazia(TipoFila Fila){
    return (Fila.Frente == Fila.Tras);
} /* Vazia */

int Enfileira(TipoPagina pagina, TipoFila *Fila, int numPaginas, int * tamanhoFila){
    if(*tamanhoFila==numPaginas)
        return 0;
    else {
        Fila->Tras->Prox = (ApontadorFila) malloc(sizeof(CelulaFila));
        Fila->Tras = Fila->Tras->Prox;
        Fila->Tras->pagina = pagina;
        Fila->Tras->numAcessos = 1;
        Fila->Tras->Prox = NULL;
        (*tamanhoFila)++;
    }
    return 1;
} /* Enfileira */

void Desenfileira(TipoFila *Fila, int * tamanhoFila){
    ApontadorFila q;
    if (Vazia(*Fila)){
        printf(" Erro fila esta vazia\n");
        return;
    }
    q = Fila->Frente;
    Fila->Frente = Fila->Frente->Prox;
    (*tamanhoFila)--;
    free(q);
} /* Desenfileira */
