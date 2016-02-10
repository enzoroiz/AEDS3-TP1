#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>

void Inicializa(TipoApontador *Dicionario){
    *Dicionario = NULL;
}

void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir){
    short NaoAchouPosicao;
    int k;
    k = Ap->n;  NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao){
      if (Reg.Chave >= Ap->r[k-1].Chave){
          NaoAchouPosicao = FALSE;
          break;
      }
      Ap->r[k] = Ap->r[k-1];
      Ap->p[k+1] = Ap->p[k];
      k--;
      if (k < 1)
          NaoAchouPosicao = FALSE;
      }
      Ap->r[k] = Reg;
      Ap->p[k+1] = ApDir;
      Ap->n++;
}

void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno,  TipoApontador *ApRetorno, int ordemArvore){
    long i = 1; long j;
    TipoApontador ApTemp;
    if (Ap == NULL){
        *Cresceu = TRUE; (*RegRetorno) = Reg; (*ApRetorno) = NULL;
        return;
    }
    while (i < Ap->n && Reg.Chave > Ap->r[i-1].Chave)  i++;
    if (Reg.Chave == Ap->r[i-1].Chave){
        printf(" Erro: Registro ja esta presente\n"); *Cresceu = FALSE;
        return;
    }
    if (Reg.Chave < Ap->r[i-1].Chave)
        i--;
        Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, ordemArvore);
        if (!*Cresceu)
            return;
    if (Ap->n < 2*ordemArvore){//PAGINA TEM ESPAÇO
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = FALSE;
        return;
    }
    /* Overflow: Pagina tem que ser dividida */
    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->p = (TipoApontador *) malloc (2*ordemArvore*sizeof(TipoApontador));
    ApTemp->r = (TipoRegistro *) malloc (2*ordemArvore*sizeof(TipoRegistro) + sizeof(TipoRegistro));
    ApTemp->n = 0;  ApTemp->p[0] = NULL;
    if (i < ordemArvore + 1){
        InsereNaPagina(ApTemp, Ap->r[(2*ordemArvore)-1], Ap->p[2*ordemArvore]);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    }
    else InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    for (j = ordemArvore + 2; j <= 2*ordemArvore; j++)
    InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j]);
    Ap->n = ordemArvore;  ApTemp->p[0] = Ap->p[ordemArvore+1];
    *RegRetorno = Ap->r[ordemArvore];  *ApRetorno = ApTemp;
}

void Insere(TipoRegistro Reg, TipoApontador *Ap, int ordemArvore){
    short Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, ordemArvore);
    if (Cresceu)  /* Arvore cresce na altura pela raiz */{
        ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->p = (TipoApontador *) malloc (2*ordemArvore*sizeof(TipoApontador));
        ApTemp->r = (TipoRegistro *) malloc (2*ordemArvore*sizeof(TipoRegistro) + sizeof(TipoRegistro));
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;  *Ap = ApTemp;
    }
}

void Reconstitui(TipoApontador ApPag, TipoApontador ApPai, int PosPai, short *Diminuiu, int ordemArvore){
    TipoPagina *Aux;  long DispAux, j;
    if (PosPai < ApPai->n){ /* Aux = TipoPagina a direita de ApPag */
        Aux = ApPai->p[PosPai+1];  DispAux = (Aux->n - ordemArvore + 1) / 2;
        ApPag->r[ApPag->n] = ApPai->r[PosPai];
        ApPag->p[ApPag->n + 1] = Aux->p[0];  ApPag->n++;
        if (DispAux > 0){ /* Existe folga: transfere de Aux para ApPag */
            for (j = 1; j < DispAux; j++)
                InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);
            ApPai->r[PosPai] = Aux->r[DispAux-1];  Aux->n -= DispAux;
            for (j = 0; j < Aux->n; j++) Aux->r[j] = Aux->r[j + DispAux];
            for (j = 0; j <= Aux->n; j++) Aux->p[j] = Aux->p[j + DispAux];
            *Diminuiu = FALSE;
        }
        else { /* Fusao: intercala Aux em ApPag e libera Aux */
            for (j = 1; j <= ordemArvore; j++)
                InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);
                free(Aux);
            for (j = PosPai + 1; j < ApPai->n; j++){
                ApPai->r[j-1] = ApPai->r[j];
                ApPai->p[j] = ApPai->p[j+1];
            }
            ApPai->n--;
            if (ApPai->n >= ordemArvore)
                *Diminuiu = FALSE;
        }
    } else { /* Aux = TipoPagina a esquerda de ApPag */
        Aux = ApPai->p[PosPai-1]; DispAux = (Aux->n - ordemArvore + 1) / 2;
        for (j = ApPag->n; j >= 1; j--) ApPag->r[j] = ApPag->r[j-1];
            ApPag->r[0] = ApPai->r[PosPai-1];
        for (j = ApPag->n; j >= 0; j--) ApPag->p[j+1] = ApPag->p[j];
            ApPag->n++;
        if (DispAux > 0){
        for (j = 1; j < DispAux; j++)
            InsereNaPagina(ApPag, Aux->r[Aux->n - j],
            Aux->p[Aux->n - j + 1]);
            ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
            ApPai->r[PosPai-1] = Aux->r[Aux->n - DispAux];
            Aux->n -= DispAux;  *Diminuiu = FALSE;
        } else { /* Fusao: intercala ApPag em Aux e libera ApPag */
            for (j = 1; j <= ordemArvore; j++)
            InsereNaPagina(Aux, ApPag->r[j-1], ApPag->p[j]);
            free(ApPag);  ApPai->n--;
            if (ApPai->n >= ordemArvore)
                *Diminuiu = FALSE;
        }
    }
}

void Antecessor(TipoApontador Ap, int Ind, TipoApontador ApPai, short *Diminuiu, int ordemArvore){
    if (ApPai->p[ApPai->n] != NULL){
        Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu, ordemArvore);
    if (*Diminuiu)
        Reconstitui(ApPai->p[ApPai->n], ApPai, (long)ApPai->n, Diminuiu, ordemArvore);
        return;
    }
    Ap->r[Ind-1] = ApPai->r[ApPai->n - 1];
    ApPai->n--;  *Diminuiu = (ApPai->n < ordemArvore);
}

void Ret(TipoChave Ch, TipoApontador *Ap, short *Diminuiu, int ordemArvore){
    long j, Ind = 1;
    TipoApontador Pag;
    if (*Ap == NULL){
        printf("Erro: registro nao esta na arvore\n"); *Diminuiu = FALSE;
        return;
    }
    Pag = *Ap;
    while (Ind < Pag->n && Ch > Pag->r[Ind-1].Chave)
        Ind++;
    if (Ch == Pag->r[Ind-1].Chave){
        if (Pag->p[Ind-1] == NULL){ /* TipoPagina folha */
            Pag->n--;
            *Diminuiu = (Pag->n < ordemArvore);
        for (j = Ind; j <= Pag->n; j++){
            Pag->r[j-1] = Pag->r[j];
            Pag->p[j] = Pag->p[j+1];
        }
        return;
        }
    /* TipoPagina nao e folha: trocar com antecessor */
    Antecessor(*Ap, Ind, Pag->p[Ind-1], Diminuiu, ordemArvore);
    if (*Diminuiu)
        Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu, ordemArvore);
        return;
    }
    if (Ch > Pag->r[Ind-1].Chave)
        Ind++;
    Ret(Ch, &Pag->p[Ind-1], Diminuiu, ordemArvore);
    if (*Diminuiu)
        Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu, ordemArvore);
}

void Retira(TipoChave Ch, TipoApontador *Ap, int ordemArvore){
    short Diminuiu;
    TipoApontador Aux;
    Ret(Ch, Ap, &Diminuiu, ordemArvore);
    if (Diminuiu && (*Ap)->n == 0)  {
        Aux = *Ap;   *Ap = Aux->p[0];
        free(Aux);
    }
}
