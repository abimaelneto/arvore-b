#ifndef _funcoesInsercaoArvore_

#define _funcoesInsercaoArvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./insercao.h"

int insereChave(
    FILE* fb,
    NoArvore* noArvore,
    Chave* chave,
    CabecalhoArvore* cabecalhoArvore,
    int RRNNoBusca) {

    //caso haja espaço, ordena e insere
    if( noArvore->nroChavesIndexadas < 4) {
        
        insereChaveEmNoComEspaco(noArvore, chave, RRNNoBusca);
        
        //pulamos para o nó onde houve a inserção e escrevemos no disco
        int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noArvore->RRNdoNo);
        
        //pulamos para o nó a ser escrito
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        // printf("não está salvando no arquivo de árvore");
        writeNoArvore(fb, noArvore, cabecalhoArvore);

        //não houve split
        return 0;
    }

    //caso não haja espaço faz o split 
    else {
        
        //cria novo nó e atualiza cabecalho
        NoArvore* noSplit = (NoArvore*) malloc(sizeof(NoArvore));
        inicializaNoArvore(noSplit, noArvore->folha, cabecalhoArvore);
        noSplit->RRNdoNo = cabecalhoArvore->RRNproxNo;
        cabecalhoArvore->RRNproxNo++;

        //organiza as chaves entre o nó antigo e o novo
        //atualiza chave para a chave que está sendo promovida
        split(fb, noArvore, noSplit, chave);
        
        //pulamos para o nó que sofreu split e escreve no disco
        int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noArvore->RRNdoNo);
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noArvore, cabecalhoArvore);

        //pulamos para o nó criado no split e escreve no disco
        byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noSplit->RRNdoNo);
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noSplit, cabecalhoArvore);        

        free(noSplit);

        return 1;
    }
}


void insereChaveEmNoComEspaco(NoArvore* noArvore, Chave* chave, int RRNNoBusca) {
    
    int* ponteiros = malloc(sizeof(int)*4);
    int* chaves = malloc(sizeof(int)*4);
    int* byteOffsets = malloc(sizeof(int)*4);
    
    chaves[0] = chave->C;
    chaves[1] = noArvore->C1;
    chaves[2] = noArvore->C2;
    chaves[3] = noArvore->C3;

    //P1 nunca é considerado.
    ponteiros[0] = chave->P;
    ponteiros[1] = noArvore->P2;
    ponteiros[2] = noArvore->P3;
    ponteiros[3] = noArvore->P4;
    
    byteOffsets[0] = chave->Pr;
    byteOffsets[1] = noArvore->Pr1;
    byteOffsets[2] = noArvore->Pr2;
    byteOffsets[3] = noArvore->Pr3;

    //faz a troca até encontrar posição vazia
    
    for(int pos = 0; (pos+1)<4; pos++) {
        if(chaves[pos+1] == -1) {
            break;
        }
        //se for maior que o próximo, troca chave, ponteiro e byteOffset
        if(chaves[pos] > chaves[pos+1]) {

            int tempC = chaves[pos+1];
            chaves[pos+1] = chaves[pos];
            chaves[pos] = tempC;

            int tempP = ponteiros[pos+1];
            ponteiros[pos+1] = ponteiros[pos];
            ponteiros[pos] = tempP;

            int tempPr = byteOffsets[pos+1];
            byteOffsets[pos+1] = byteOffsets[pos];
            byteOffsets[pos] = tempPr;
        }
    }
    

    //atualiza chaves, ponteiros e byteOffsets do nó
    noArvore->C1 = chaves[0];
    noArvore->C2 = chaves[1];
    noArvore->C3 = chaves[2];
    noArvore->C4 = chaves[3];

    noArvore->P2 = ponteiros[0];
    noArvore->P3 = ponteiros[1];
    noArvore->P4 = ponteiros[2];
    noArvore->P5 = ponteiros[3];

    noArvore->Pr1 = byteOffsets[0];
    noArvore->Pr2 = byteOffsets[1];
    noArvore->Pr3 = byteOffsets[2];
    noArvore->Pr4 = byteOffsets[3];
    
    //nova chave foi indexada
    noArvore->nroChavesIndexadas++;

    free(ponteiros);
    free(chaves);
    free(byteOffsets);

}

void split(
    FILE* fb, 
    NoArvore* noArvore,
    NoArvore* noSplit,
    Chave* chave)
{
    
    int* ponteiros = malloc(sizeof(int)*5);
    int* chaves = malloc(sizeof(int)*5);
    int* byteOffsets = malloc(sizeof(int)*5);

    ordenaChavesParaSplit(
        noArvore,
        chave,
        chaves,
        byteOffsets,
        ponteiros);

    //atualiza chaves de noArvore
    noArvore->C1 = chaves[0];
    noArvore->C2 = chaves[1];
    noArvore->C3 = -1;
    noArvore->C4 = -1;
    
    //atualizar ponteiros e P
    noArvore->P2 = ponteiros[0];
    noArvore->P3 = ponteiros[1];
    noArvore->P4 = -1;
    noArvore->P5 = -1;

    noArvore->Pr1 = byteOffsets[0];
    noArvore->Pr2 = byteOffsets[1];
    noArvore->Pr3 = -1;
    noArvore->Pr4 = -1;
    
    //atualiza noSplit
    noSplit->C1 = chaves[3];
    noSplit->C2 = chaves[4];
    noSplit->C3 = -1;
    noSplit->C4 = -1;

    noSplit->P1 = ponteiros[2];
    noSplit->P2 = ponteiros[3];
    noSplit->P3 = ponteiros[4];
    noSplit->P4 = -1;
    noSplit->P5 = -1;

    noSplit->Pr1 = byteOffsets[3];
    noSplit->Pr2 = byteOffsets[4];
    noSplit->Pr3 = -1;
    noSplit->Pr4 = -1;

    //atualiza chave para chave a ser promovida
    chave->C = chaves[2];
    chave->Pr = byteOffsets[2];
    chave->P = noSplit->RRNdoNo;
    
    // como foi feito split, os dois nós possuem 2 chaves
    noArvore->nroChavesIndexadas=2;
    noSplit->nroChavesIndexadas=2;   
    
}   

void ordenaChavesParaSplit(
    NoArvore* noArvore,
    Chave* chave,
    int* chaves,
    int* byteOffsets,
    int* ponteiros
) {
    
    chaves[0] = chave->C;
    chaves[1] = noArvore->C1;
    chaves[2] = noArvore->C2;
    chaves[3] = noArvore->C3;
    chaves[4] = noArvore->C4;

    ponteiros[0] = chave->P;
    ponteiros[1] = noArvore->P2;
    ponteiros[2] = noArvore->P3;
    ponteiros[3] = noArvore->P4;
    ponteiros[4] = noArvore->P5;

    byteOffsets[0] = chave->Pr;
    byteOffsets[1] = noArvore->Pr1;
    byteOffsets[2] = noArvore->Pr2;
    byteOffsets[3] = noArvore->Pr3;
    byteOffsets[4] = noArvore->Pr4;
    
    
    for( int pos = 0; pos< 4; pos++) {
        if(chaves[pos] > chaves[pos+1]) {

            int tempC = chaves[pos+1];
            chaves[pos+1] = chaves[pos];
            chaves[pos] = tempC;

            int tempP = ponteiros[pos+1];
            ponteiros[pos+1] = ponteiros[pos];
            ponteiros[pos] = tempP;

            int tempPr = byteOffsets[pos+1];
            byteOffsets[pos+1] = byteOffsets[pos];
            byteOffsets[pos] = tempPr;
                    
        }
    }    

}

#endif