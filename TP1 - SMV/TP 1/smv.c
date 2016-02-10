#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>

void PesquisaArvore(TipoRegistro *x, TipoApontador Ap, TipoFila * memoria, int numPaginas, int * tamanhoFila, int * misses, int tipo){
    long i = 1;
    ApontadorFila aux;

    while (i < Ap->n && x->Chave > Ap->r[i-1].Chave)
    i++;//CHAVE MAIOR QUE O ANTERIOR
    if (x->Chave == Ap->r[i-1].Chave){
        if(!EstaNaMemoria(*memoria, x->Chave, &aux)){
            (*misses)++;
            ColocaNaMemoria(*Ap, memoria, numPaginas, tamanhoFila, tipo);
        } else {
            if(tipo==1)
                HitLRU(memoria, aux, numPaginas, tamanhoFila);
            else if(tipo==2)
                HitLFU(aux);
        }
        *x = Ap->r[i-1];
        return;
    }
    if (x->Chave < Ap->r[i-1].Chave){
        if(!EstaNaMemoria(*memoria, Ap->r[0].Chave, &aux)){
            (*misses)++;
            ColocaNaMemoria(*Ap, memoria, numPaginas, tamanhoFila, tipo);
        } else {
            if(tipo==1)
                HitLRU(memoria, aux, numPaginas, tamanhoFila);
            else if(tipo==2)
                HitLFU(aux);
        }
        PesquisaArvore(x, Ap->p[i-1], memoria, numPaginas, tamanhoFila, misses, tipo);
    } else {
        if(!EstaNaMemoria(*memoria, Ap->r[0].Chave, &aux)){
            (*misses)++;
            ColocaNaMemoria(*Ap, memoria, numPaginas, tamanhoFila, tipo);
        } else {
      if(tipo==1)
        HitLRU(memoria, aux, numPaginas, tamanhoFila);
      else if(tipo==2)
        HitLFU(aux);
        }
        PesquisaArvore(x, Ap->p[i], memoria, numPaginas, tamanhoFila, misses, tipo);
    }
}

void Caminhamento(TipoRegistro *x, TipoApontador Ap, FILE * fpSaida){
    long i = 1, j;
    while (i < Ap->n && x->Chave > Ap->r[i-1].Chave){ i++;}//CHAVE MAIOR QUE O ANTERIOR
    if (x->Chave == Ap->r[i-1].Chave){
        for(j=0;j < Ap->n; j++)
            fprintf(fpSaida, "%ld ", Ap->r[j].Chave);
        fprintf(fpSaida, "\n");
        return;
    }
    if (x->Chave < Ap->r[i-1].Chave){
        for(j=0;j < Ap->n; j++)
            fprintf(fpSaida, "%ld ", Ap->r[j].Chave);
        Caminhamento(x, Ap->p[i-1], fpSaida);
        } else {
            for(j=0;j < Ap->n; j++)
                fprintf(fpSaida, "%ld ", Ap->r[j].Chave);
        Caminhamento(x, Ap->p[i], fpSaida);
    }
}

int Politicas(TipoApontador HD, int numPaginas, int numConsultas, int * consultas, int tipo){
    TipoFila memoria;
    TipoRegistro x;
    int i, misses=0, tamanhoFila=0;

    FFVazia(&memoria);//CRIA FILA VAZIA PARA MEMORIA

    for(i=0; i<numConsultas; i++){//PERCORRE CONSULTAS FEITAS
        x.Chave=consultas[i];
        PesquisaArvore(&x, HD, &memoria, numPaginas, &tamanhoFila, &misses, tipo);
    }
    return misses;
}

int EstaNaMemoria(TipoFila memoria, int registro, ApontadorFila * retorno){
    int n, j;
    ApontadorFila aux;
    TipoPagina pag;

    aux=memoria.Frente;
    while(aux->Prox!=NULL){//PERCORRE MEMÓRIA
        pag = aux->Prox->pagina;//Página J-1 da memória, para melhorar legibilidade do código
        n = aux->Prox->pagina.n;//Número de registros na página, para melhorar legibilidade do código
        for(j=0;j<n;j++){
            if(registro == pag.r[j].Chave){//HIT
                *retorno=aux;
                return TRUE;
            }
        }
        aux=aux->Prox;
    }
    return FALSE;
}

void ColocaNaMemoria(TipoPagina pag, TipoFila * memoria, int numPaginas, int * tamanhoFila, int tipo){
    if(!Enfileira(pag, memoria, numPaginas, tamanhoFila)){//SE A MEMORIA ESTIVER CHEIA ENTRA IF, SE NÃO INSERE
        if(tipo==2)
            CorrigeLFU(memoria, tamanhoFila, numPaginas);
    Desenfileira(memoria, tamanhoFila);
    Enfileira(pag, memoria, numPaginas, tamanhoFila);
    }
}

void HitLRU(TipoFila * memoria, ApontadorFila aux, int numPaginas, int * tamanhoFila){
    TipoPagina pag;
    pag=aux->Prox->pagina;

    if(aux->Prox->Prox!=NULL){//REGISTRO NA ULTIMA POSICAO DA FILA => LUGAR CORRETO
        if(aux->Prox==memoria->Frente->Prox){//REGISTRO PRIMEIRA POSICAO DA FILA => TIRO DA PRIMEIRA E COLOCO NA ULTIMA
            Desenfileira(memoria, tamanhoFila);//RETIRO DO COMECO DA FILA
            Enfileira(pag, memoria, numPaginas, tamanhoFila);//COLOCO NO FIM DA FILA
        } else {//REGISTRO NO CORPO DA FILA
            if(aux->Prox->Prox != memoria->Tras){//SE NÃO FOR O PENÚLTIMO, É O ULTIMO DA FILA AO PULAR REGISTRO APONTA PARA NULL
                memoria->Tras->Prox=aux->Prox;//COLOCA O REGISTRO NA ULTIMA POS DA FILA
                aux->Prox=aux->Prox->Prox;//PULA UM REGISTRO
                memoria->Tras=memoria->Tras->Prox;//SETA A ULTIMA POSICAO CORRETAMENTE
                memoria->Tras->Prox = NULL;//FAZ ULTIMA POSICAO APONTAR NULL
            } else {//BASTA TROCAR PENÚLTIMO COM ULTIMO
                memoria->Tras->Prox=aux->Prox;//COLOCA REGISTRO ACESSADO NA ULTIMA POSICAO DA FILA
                memoria->Tras=memoria->Tras->Prox;//SETA A ULTIMA POSICAO CORRETAMENTE
                memoria->Tras->Prox = NULL;
            }
        }
    }
}

void HitLFU(ApontadorFila aux){
    aux->Prox->numAcessos++;//AUMENTA O CONTADOR DA CELULA ONDE DEU O HIT
}

void CorrigeLFU(TipoFila * memoria, int * tamanhoFila, int numPaginas){
    ApontadorFila aux, retira, cont;
    int numAcessos;

    cont=memoria->Frente;
    numAcessos=cont->Prox->numAcessos;//RECEBE O ACESSO DO PRIMEIRO DA FILA
    retira=cont;
    while(cont->Prox!=NULL){//PERCORRE MEMÓRIA
        if(cont->Prox->numAcessos<numAcessos){
            retira=cont;//RECEBE O PONTEIRO A SER RETIRADO
            numAcessos=cont->Prox->numAcessos;
        }
    cont=cont->Prox;
    }

    if(retira!=memoria->Frente){//REGISTRO NA PRIMEIRA POSICAO DA FILA => LUGAR CORRETO
        if(retira->Prox->Prox==NULL){//REGISTRO ULTIMA POSICAO DA FILA => TIRO DA ULTIMA E COLOCO NA PRIMEIRA
            retira->Prox->Prox=memoria->Frente->Prox;//ULTIMO APONTA PARA O SEGUNDO
            memoria->Frente->Prox=retira->Prox;
            retira->Prox=NULL;//ULTIMO APONTA PARA NULL
            memoria->Tras=retira;
        } else {//REGISTRO NO CORPO DA FILA
            aux=memoria->Frente->Prox;//RECEBE O PRIMEIRO DA FILA
            memoria->Frente->Prox=retira->Prox;//COLOCA O REGISTRO COMO PRIMEIRO DA FILA
            retira->Prox=retira->Prox->Prox;
            memoria->Frente->Prox->Prox=aux;
        }
    }
}


//###### GERA ARQUIVO ######

void GeraArquivoEntrada(char * entrada){
    int total=30000;
    int marcado[total];
    int ordemArvore=8;
    int numConsultas = 10000;
    int consultas[total];
    int pagsmemoria=125;
    int random;
    int i;
    FILE * fpEntrada;

    fpEntrada=fopen(entrada, "w");

    fprintf(fpEntrada, "1\n");//INSTANCIAS
    fprintf(fpEntrada, "%d %d\n", pagsmemoria*80, ordemArvore);
    fprintf(fpEntrada, "%d\n", numConsultas);

    for(i=0;i<numConsultas;i++){//REGISTROS ARVORE
        random=rand()%total;
        if(marcado[random]==TRUE)
            i--;
        else {
            marcado[random]=TRUE;
            consultas[i]=random;
            fprintf(fpEntrada, "%d ", random);
        }
    }
    fprintf(fpEntrada, "\n");

    fprintf(fpEntrada, "%d\n", numConsultas/20);//EXCLUIDOS
    for(i=0;i<numConsultas/20 ;i++){
        random=rand()%numConsultas;//INDICE DO VETOR
        if(consultas[random]!=-1){//SE O INDICE JA NAO TIVER SIDO EXCLUIDO
            fprintf(fpEntrada,"%d ", consultas[random]);
            consultas[random]=-1;
        } else {
            i--;
        }
    }
    fprintf(fpEntrada, "\n");

    fprintf(fpEntrada, "%d\n", numConsultas/10);//CONSULTAS
    for(i=0;i<numConsultas/10 ;i++){
        random=rand()%numConsultas;
        if(consultas[random]==-1)//ITEM EXCLUIDO
            i--;
        else
            fprintf(fpEntrada, "%d ", consultas[random]);
    }
    fprintf(fpEntrada, "\n");

    fprintf(fpEntrada, "%d\n", numConsultas/20);//CONSULTAS
    for(i=0;i<numConsultas/20 ;i++){
        random=rand()%numConsultas;
        if(consultas[random]==-1)//ITEM EXCLUIDO
            i--;
        else
            fprintf(fpEntrada, "%d ", consultas[random]);
    }

    fclose(fpEntrada);
}
