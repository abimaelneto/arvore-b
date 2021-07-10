#ifndef _arvore_

#define _arvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore-b.h"

#define TAM_PAG_DISCO 77


void insereVeiculoNaArvore(FILE* fb, CabecalhoArvore* cabecalhoArvore, NoArvore* noRaiz, Chave* chaveAInserir) {
    
    //se o nó raiz sofreu split, é preciso criar nova raiz
    int houveSplitNoAtual = desceNaArvore(fb, noRaiz, chaveAInserir, cabecalhoArvore);
    
    if (houveSplitNoAtual != 0) {
        printf("Cria nova raiz\n");
    
        //cria nova raiz
        int RRNRaizAntiga = noRaiz->RRNdoNo;
        inicializaNoArvore(noRaiz, '0', cabecalhoArvore);
        noRaiz->P1 = RRNRaizAntiga;
        noRaiz->C1 = chaveAInserir->C;
        noRaiz->Pr1 = chaveAInserir->Pr;
        noRaiz->P2 = chaveAInserir->P; //guarda o nó criado no split

        noRaiz->RRNdoNo = cabecalhoArvore->RRNproxNo;
        noRaiz->nroChavesIndexadas++;
        cabecalhoArvore->RRNproxNo++;
        cabecalhoArvore->noRaiz = noRaiz->RRNdoNo;
        
        //pulamos para o nó onde houve a inserção e escrevemos no disco
        int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noRaiz->RRNdoNo);
        
        //pulamos para o nó a ser lido
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noRaiz, cabecalhoArvore);
        
    }


}


//retorna 1 se houve split
int desceNaArvore(FILE* fb, NoArvore* noArvore, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore) {
    
    //pulamos para o nó a ser lido e lemos para a RAM
    int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noArvore->RRNdoNo);
    fseek(fb, byteOffsetNoArvore, SEEK_SET);
    readNoArvore(fb, noArvore);

    //flag que indicará se houve split no nó atual
    int houveSplitNoAtual = 0;

    //logs
    printf("Chave a inserir: %x\n", chaveAInserir->C);
    printf("Folha: %c\n", noArvore->folha);
    printf("RRN atual: %d\n", noArvore->RRNdoNo);
    
    int RRNNoBusca = 0;
        
    if(chaveAInserir->C < noArvore->C1) {
        RRNNoBusca = noArvore->P1;
    }
    else if (chaveAInserir->C < noArvore->C2 || noArvore-> C2 == -1) {
        RRNNoBusca = noArvore->P2;
        
    }
    else if (chaveAInserir->C < noArvore->C3 || noArvore-> C3 == -1) {
        RRNNoBusca = noArvore->P3;
    }
    else if (chaveAInserir->C < noArvore->C4 || noArvore-> C4 == -1) {
        RRNNoBusca = noArvore->P4;
    }
    else {
        RRNNoBusca = noArvore->P5;
    }
    
    if (RRNNoBusca == 0) {
        printf("ERRO DE COMPARAÇÃO DE CHAVES!\n");
    }

    // se o nó não for folha, faz a busca pelo nó folha
    if(noArvore->folha != '1') {
        
        //verifica possível erro no programa
        if(RRNNoBusca == -1) {
            printf("ERRO: está tratando como folha");
        }
        
        //cria nó vazio, que lerá o conteúdo do filho
        NoArvore* noFilho = (NoArvore*) malloc(sizeof(NoArvore));
        inicializaNoArvore(noFilho, '0', cabecalhoArvore);
        noFilho->RRNdoNo = RRNNoBusca;

        printf("RRN do filho: %d\n", noFilho->RRNdoNo);
        
        //flag que indica se uma chave foi promovida
        int houveSplitNoFilho = desceNaArvore(fb, noFilho, chaveAInserir, cabecalhoArvore);
        
        if (houveSplitNoFilho != 0) {
            //insere a chave a ser promovida no nó atual, não no filho
            houveSplitNoAtual = insereChave(fb, noArvore, chaveAInserir, cabecalhoArvore, RRNNoBusca);
            
        }
    }
 
    //se for folha, insere 
    else{
        //se a inserção retornar um split, quer dizer que é preciso inserir a chave promovida no nó atual
        houveSplitNoAtual = insereChave(fb, noArvore, chaveAInserir, cabecalhoArvore, RRNNoBusca);        
    }
    
 
    return houveSplitNoAtual;
}

int insereChave(
    FILE* fb,
    NoArvore* noArvore,
    Chave* chaveAInserir,
    CabecalhoArvore* cabecalhoArvore,
    int RRNNoBusca) {

    //caso haja espaço, ordena e insere
    if( noArvore->nroChavesIndexadas < 4) {
        
        insereChaveEmNoComEspaco(noArvore, chaveAInserir, RRNNoBusca);
        
        //pulamos para o nó onde houve a inserção e escrevemos no disco
        int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noArvore->RRNdoNo);
        
        //pulamos para o nó a ser escrito
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noArvore, cabecalhoArvore);
        
        //não houve split
        return 0;
    }

    //caso não haja espaço faz o split 
    else {
        printf("Fez spĺit!\n");
        //cria novo nó e atualiza cabecalho
        NoArvore* noSplit = (NoArvore*) malloc(sizeof(NoArvore));
        inicializaNoArvore(noSplit, noArvore->folha, cabecalhoArvore);
        noSplit->RRNdoNo = cabecalhoArvore->RRNproxNo;
        cabecalhoArvore->RRNproxNo++;
        printf("RRN do noSplit: %d\n", noSplit->RRNdoNo);

        //organiza as chaves entre o nó antigo e o novo
        //atualiza chaveAInserir para a chave que está sendo promovida
        split(fb, noArvore, noSplit, chaveAInserir);
        
        //pulamos para o nó que sofreu split e escreve no disco
        int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noArvore->RRNdoNo);
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noArvore, cabecalhoArvore);
        
        //pulamos para o nó criado no split e escreve no disco
        byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noSplit->RRNdoNo);
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noSplit, cabecalhoArvore);
        
        if(noArvore->nroChavesIndexadas != 2 || noSplit->nroChavesIndexadas != 2) {
            printf("sdfjkhdjkflahsjkdfhjaskldhfjklahsdkfhaljksdfhklajsdhfjkasdhfjkhas");
        }
        free(noSplit);

        return 1;
    }
}

void insereChaveEmNoComEspaco(NoArvore* noArvore, Chave* chaveAInserir, int RRNNoBusca) {
    
    int* ponteiros = malloc(sizeof(int)*4);
    int* chaves = malloc(sizeof(int)*4);
    int* byteOffsets = malloc(sizeof(int)*4);
    
    chaves[0] = chaveAInserir->C;
    chaves[1] = noArvore->C1;
    chaves[2] = noArvore->C2;
    chaves[3] = noArvore->C3;

    //P1 nunca é considerado.
    ponteiros[0] = chaveAInserir->P;
    ponteiros[1] = noArvore->P2;
    ponteiros[2] = noArvore->P3;
    ponteiros[3] = noArvore->P4;
    
    byteOffsets[0] = chaveAInserir->Pr;
    byteOffsets[1] = noArvore->Pr1;
    byteOffsets[2] = noArvore->Pr2;
    byteOffsets[3] = noArvore->Pr3;

    //faz a troca até encontrar posição vazia
    
    for(int pos = 0; (pos+1)<4; pos++) {
        if(chaves[pos+1] == -1) {
            break;
        }
        printf("TESTE %d\n", pos);
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
    Chave* chaveAInserir)
{
    
    int* ponteiros = malloc(sizeof(int)*6);
    int* chaves = malloc(sizeof(int)*5);
    int* byteOffsets = malloc(sizeof(int)*5);

    ordenaChavesParaSplit(
        noArvore,
        chaveAInserir,
        chaves,
        byteOffsets,
        ponteiros);

    //atualiza chaves de noArvore
    noArvore->C1 = chaves[0];
    noArvore->C2 = chaves[1];
    noArvore->C3 = -1;
    noArvore->C4 = -1;
    
    //atualizar ponteiros e P
    noArvore->P1 = ponteiros[0];
    noArvore->P2 = ponteiros[1];
    noArvore->P3 = ponteiros[2];
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

    noSplit->P1 = ponteiros[3];
    noSplit->P2 = ponteiros[4];
    noSplit->P3 = ponteiros[5];
    noSplit->P4 = -1;
    noSplit->P5 = -1;

    noSplit->Pr1 = byteOffsets[3];
    noSplit->Pr2 = byteOffsets[4];
    noSplit->Pr3 = -1;
    noSplit->Pr4 = -1;

    //atualiza chaveAInserir para chave a ser promovida
    chaveAInserir->C = chaves[2];
    chaveAInserir->Pr = byteOffsets[2];
    chaveAInserir->P = noSplit->RRNdoNo;
    
    // como foi feito split, os dois nós possuem 2 chaves
    noArvore->nroChavesIndexadas=2;
    noSplit->nroChavesIndexadas=2;   
    
}   

void ordenaChavesParaSplit(
    NoArvore* noArvore,
    Chave* chaveAInserir,
    int* chaves,
    int* byteOffsets,
    int* ponteiros
) {
    
    chaves[0] = chaveAInserir->C;
    chaves[1] = noArvore->C1;
    chaves[2] = noArvore->C2;
    chaves[3] = noArvore->C3;
    chaves[4] = noArvore->C4;

    ponteiros[0] = chaveAInserir->P;
    ponteiros[1] = noArvore->P1;
    ponteiros[2] = noArvore->P2;
    ponteiros[3] = noArvore->P3;
    ponteiros[4] = noArvore->P4;
    ponteiros[5] = noArvore->P5;

    byteOffsets[0] = chaveAInserir->Pr;
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
    //se a chave nova está na última posição, troca também o ponteiro para a última posição
    if(chaves[4] == chaveAInserir->C) {
        int tempP = ponteiros[5];
        ponteiros[5] = ponteiros[4];
        ponteiros[4] = tempP;
    }

}

#endif