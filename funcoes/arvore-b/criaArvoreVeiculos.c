#ifndef _criaArvoreVeiculos_

#define _criaArvoreVeiculos_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./arvore-b.h"
#include "./insercao.h"
#include "../veiculos.h"


void insereVeiculoEmArvoreNova(FILE* fb, CabecalhoArvore* cabecalhoArvore, Chave* chaveAInserir) {
    
    NoArvore* noRaiz = (NoArvore*) malloc(sizeof(NoArvore));
    inicializaNoArvore(noRaiz, '0', cabecalhoArvore);
    noRaiz->RRNdoNo = cabecalhoArvore->noRaiz;
    printf("\nNó raiz:%x\n", noRaiz->RRNdoNo);
    //se o nó raiz sofreu split, é preciso criar nova raiz
    int houveSplitNoAtual = desceNaArvore(fb, noRaiz, chaveAInserir, cabecalhoArvore);
    
    if (houveSplitNoAtual != 0) {
    
        //cria nova raiz
        int RRNRaizAntiga = noRaiz->RRNdoNo;
        inicializaNoArvore(noRaiz, '0', cabecalhoArvore);
        noRaiz->P1 = RRNRaizAntiga;
        noRaiz->C1 = chaveAInserir->C;
        noRaiz->Pr1 = chaveAInserir->Pr;
        noRaiz->P2 = chaveAInserir->P; //guarda o nó criado no split

        noRaiz->RRNdoNo = cabecalhoArvore->RRNproxNo;
        cabecalhoArvore->RRNproxNo++;
        cabecalhoArvore->noRaiz = noRaiz->RRNdoNo;
        noRaiz->nroChavesIndexadas++;
        
        //pulamos para o nó onde houve a inserção e escrevemos no disco
        int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noRaiz->RRNdoNo);
        
        //pulamos para o nó a ser escrito
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noRaiz, cabecalhoArvore);  

        printf("\nNova raiz!\n");
        printNoArvore(noRaiz);
    }
    free(noRaiz);
}

int desceNaArvore(
    FILE* fb,
    NoArvore* noArvore,
    Chave* chaveAInserir,
    CabecalhoArvore* cabecalhoArvore)
{
    
    //pulamos para o nó a ser lido e lemos para a RAM
    int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noArvore->RRNdoNo);
    fseek(fb, byteOffsetNoArvore, SEEK_SET);
    printf("RRN atual: %d\n", noArvore->RRNdoNo);
    readNoArvore(fb, noArvore);
    
    printf("Antes da inserção\n");
    printNoArvore(noArvore);
    
    //flag que indicará se houve split no nó atual
    int houveSplitNoAtual = 0;

    //logs
    printf("Chave a inserir: %x\n", chaveAInserir->C);
    printf("Ponteiro da chave: %x\n", chaveAInserir->P);
    printf("RRN atual: %d\n", noArvore->RRNdoNo);
    
    int RRNNoBusca = -3;
        
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
    //apenas para debug. Significa que não conseguiu relacionar qual chave era maior de forma correta
    if (RRNNoBusca == -3) {
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
    printf("\nDepois da inserção\n");
    printNoArvore(noArvore);

    return houveSplitNoAtual;
}




#endif