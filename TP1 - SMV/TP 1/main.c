#include <stdlib.h>
#include <stdio.h>
#include "estruturas.h"

int main(int argc, char *argv[]){
    char * entrada = argv[1];
    char * saida = argv[2];

    FILE *fpEntrada=NULL, *fpSaida=NULL;
    int numInstancias, numBytes, ordemArvore, numValores, numExcluidos, numConsultas, numConsultasExibidas, FIFO, LRU, LFU, i, j;
    TipoRegistro x;

    TipoApontador HD;
    Inicializa(&HD);

    fpEntrada=fopen(entrada, "r");
    fpSaida=fopen(saida, "w");
    fscanf(fpEntrada, "%d", &numInstancias);

    for(i=0;i<numInstancias;i++){
        fscanf(fpEntrada, "%d", &numBytes);
        fscanf(fpEntrada, "%d", &ordemArvore);

        int numPaginas = numBytes/80;//SEGUNDO ESPECIFICAÇÃO UMA PÁGINA OCUPA 80 BYTES PARA ARQUITETURA 64 BITS

        fscanf(fpEntrada, "%d", &numValores);
        int valores[numValores];
        for(j=0;j<numValores;j++){
            fscanf(fpEntrada, "%d", &valores[j]);
            x.Chave=valores[j];
            Insere(x, &HD, ordemArvore);
        }

        fscanf(fpEntrada, "%d", &numExcluidos);
        int excluidos[numExcluidos];
        for(j=0;j<numExcluidos;j++){
            fscanf(fpEntrada, "%d", &excluidos[j]);
            x.Chave=excluidos[j];
            Retira(x.Chave, &HD, ordemArvore);
        }

        fscanf(fpEntrada, "%d", &numConsultas);
        int consultas[numConsultas];
        for(j=0;j<numConsultas;j++){
              fscanf(fpEntrada, "%d", &consultas[j]);
        }

        FIFO=0;
        LRU=1;
        LFU=2;

        fprintf(fpSaida, "%d %d %d\n", Politicas(HD, numPaginas, numConsultas, &consultas, FIFO), Politicas(HD, numPaginas, numConsultas, &consultas, LRU), Politicas(HD, numPaginas, numConsultas, &consultas, LFU));      FIFO=Politicas(HD, numPaginas, numConsultas, &consultas, FIFO);

        fscanf(fpEntrada, "%d", &numConsultasExibidas);
        int consultasExibidas[numConsultasExibidas];
        for(j=0;j<numConsultasExibidas;j++){
              fscanf(fpEntrada, "%d", &consultasExibidas[j]);
              x.Chave=consultasExibidas[j];
              Caminhamento(&x, HD, fpSaida);
        }
    }
    fclose(fpEntrada);
    fclose(fpSaida);
    return 0;
}
