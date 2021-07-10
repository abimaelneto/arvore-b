#ifndef _arvore_

#define _arvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore-b.h"

#define TAM_PAG_DISCO 77

void inicializaNoArvore(NoArvore* noArvore, int eFolha, CabecalhoArvore* cabecalhoArvore) {
    
    noArvore->folha = eFolha; //1 para folha e 0 para não-folha
    noArvore->nroChavesIndexadas = 0;
    
    noArvore->P1 = -1; 
    noArvore->C1 = -1;
    noArvore->Pr1 = -1;
    noArvore->P2 = -1;
    noArvore->C2 = -1;
    noArvore->Pr2 = -1;
    noArvore->P3 = -1;
    noArvore->C3 = -1;
    noArvore->Pr3 = -1;
    noArvore->P4 = -1;
    noArvore->C4 = -1;
    noArvore->Pr4 = -1;
    noArvore->P5 = -1;
    
}

void insereVeiculoNaArvore(FILE* fb, CabecalhoArvore* cabecalhoArvore, NoArvore* noRaiz, Chave* chaveAInserir) {
    
    //se a descida retornar um split, quer dizer que é preciso inserir a chave promovida no nó atual
    int houveSplitNoFilho = desceNaArvore(fb, noRaiz, chaveAInserir, cabecalhoArvore);
    
    if (houveSplitNoFilho != 0) {
        printf(" Houve split no filho\n");

        //cria nova raiz
        int RRNRaizAntiga = noRaiz->RRNdoNo;
        inicializaNoArvore(noRaiz, '0', cabecalhoArvore);
        noRaiz->P1 = RRNRaizAntiga;
        noRaiz->C1 = chaveAInserir->C;
        noRaiz->Pr1 = chaveAInserir->Pr;
        noRaiz->P2 = chaveAInserir->RRN; //guard o nó criado no split

        noRaiz->RRNdoNo = cabecalhoArvore->RRNproxNo;
        noRaiz->nroChavesIndexadas++;
        cabecalhoArvore->RRNproxNo++;
        cabecalhoArvore->noRaiz = noRaiz->RRNdoNo;
        printf("teste");
        //pulamos para o nó onde houve a inserção e escrevemos no disco
        int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noRaiz->RRNdoNo);
        
        //pulamos para o nó a ser lido
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noRaiz, cabecalhoArvore);
        //insereChave(fb, noRaiz, chaveAInserir, cabecalhoArvore);
        
    }


}

int busca( FILE* fb, int RRNNoBusca, NoArvore* noPai, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore) {
   printf("teste busca");

    //valor a ser retornado. Caso precise promover chave, terá que chamar a inserção novamente
    int ocorreuSplit = 0;
    
     // caso o nó seja folha, tenta inserir
    if( noPai->folha == '1' ){
        
        ocorreuSplit = insereChave(fb, noPai, chaveAInserir, cabecalhoArvore);
        return ocorreuSplit;
    }
    // printf("nao é folha\n");
    //se nó não for folha, tenta encontrar RRN do próximo nó da busca
    
    //aloca nó filho
    NoArvore* noFilho = (NoArvore*) malloc(sizeof(NoArvore));
    
    //busca o nó filho pelo seu RRN
    ocorreuSplit = busca(fb, RRNNoBusca, noFilho, chaveAInserir, cabecalhoArvore);
    if( ocorreuSplit == 1) {
        ocorreuSplit = insereChave(fb, noPai, chaveAInserir, cabecalhoArvore);
    }
    free(noFilho);
    return ocorreuSplit;


}

//retorna 1 se houve split
int desceNaArvore(FILE* fb, NoArvore* noArvore, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore) {
    // int byteOffsetNoArvore = sizeof(CabecalhoArvore) + ( sizeof(NoArvore)*(noArvore->RRNdoNo));
    int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noArvore->RRNdoNo);
    //pulamos para o nó a ser lido
    fseek(fb, byteOffsetNoArvore, SEEK_SET);
    
    //le página completa
    readNoArvore(fb, noArvore);


    //flag que indica se houve split em noArvore
    int houveSplitNoAtual = 0;
    
    printf("Folha: %c\n", noArvore->folha);
    printf("RRN atual: %d\n", noArvore->RRNdoNo);
    
    // se no nó não for folha, faz a busca pelo nó folha
    if(noArvore->folha != '1') {
        //faz a busca pelo nó de RRN número 'RRNNoBusc' e salva em 'noArvore'
        int RRNNoBusca = noArvore->RRNdoNo;
        
        if(chaveAInserir->C < noArvore->C1) {
            RRNNoBusca = noArvore->P1;
        }
        else if (chaveAInserir->C < noArvore->C2) {
            RRNNoBusca = noArvore->P2;
        }
        else if (chaveAInserir->C < noArvore->C3) {
            RRNNoBusca = noArvore->P3;
        }
        else if (chaveAInserir->C < noArvore->C4) {
            RRNNoBusca = noArvore->P4;
        }
        else {
            RRNNoBusca = noArvore->P5;
        }
        
        //verifica possível erro no programa
        if(RRNNoBusca == -1) {
            printf("está tratando como folha");
        }
        
        //cria nó vazio, que lerá o conteúdo do filho
        NoArvore* noFilho = (NoArvore*) malloc(sizeof(NoArvore));
        inicializaNoArvore(noFilho, '0', cabecalhoArvore);
        noFilho->RRNdoNo = RRNNoBusca;
        
        //se a descida retornar um split, quer dizer que é preciso inserir a chave promovida no nó atual
        int houveSplitNoFilho = desceNaArvore(fb, noFilho, chaveAInserir, cabecalhoArvore);
        printf("RRN do filho: %d\n", noArvore->RRNdoNo);
        if (houveSplitNoFilho != 0) {
            //insere a chave a ser promovida no nó atual, não no filho
            houveSplitNoAtual = insereChave(fb, noArvore, chaveAInserir, cabecalhoArvore);
            
        }
    }
 
    //se for folha, insere 
    else{
        //se a inserção retornar um split, quer dizer que é preciso inserir a chave promovida no nó atual
        houveSplitNoAtual = insereChave(fb, noArvore, chaveAInserir, cabecalhoArvore);        
    }
    
 
    return houveSplitNoAtual;
}
int insereChave(FILE* fb, NoArvore* noArvore, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore) {

    //caso haja espaço, ordena e insere
    if( noArvore->nroChavesIndexadas < 4) {
        
        insereChaveEmNoComEspaco(noArvore, chaveAInserir);
        
        //pulamos para o nó onde houve a inserção e escrevemos no disco
        int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ noArvore->RRNdoNo);
        
        //pulamos para o nó a ser lido
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noArvore, cabecalhoArvore);
        
        //não houve split
        return 0;
    }

    //caso não haja espaço faz o split 
    else {
        printf(" Fez spĺit!\n");
        //cria novo nó e atualiza cabecalho
        NoArvore* noSplit = (NoArvore*) malloc(sizeof(NoArvore));
        inicializaNoArvore(noSplit, noArvore->folha, cabecalhoArvore);
        noSplit->RRNdoNo = cabecalhoArvore->RRNproxNo;
        cabecalhoArvore->RRNproxNo++;
        printf("RRN do noSplit: %d", noSplit->RRNdoNo);

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

        free(noSplit);

        return 1;
    }
}


void insereChaveEmNoComEspaco(NoArvore* noArvore, Chave* chaveAInserir) {
    int* ponteiros = malloc(sizeof(int)*6);
    int* chaves = malloc(sizeof(int)*5);
    int* byteOffsets = malloc(sizeof(int)*5);
    
    chaves[0] = chaveAInserir->C;
    chaves[1] = noArvore->C1;
    chaves[2] = noArvore->C2;
    chaves[3] = noArvore->C3;
    chaves[4] = noArvore->C4;

    
    ponteiros[0] = -1; //vai acompanhar prefixoConvertido
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

    //faz a troca até encontrar posição vazia
    for(int pos = 0; chaves[pos+1] != -1; pos++) {
        
        //se for maior que o próximo, troca chave, ponteiro e byteOffset
        if(chaves[pos] > chaves[pos+1]) {

            int tempC = chaves[pos+1];
            chaves[pos+1] = chaves[pos];
            chaves[pos] = tempC;

            int tempP = ponteiros[pos+2];
            ponteiros[pos+2] = ponteiros[pos];
            ponteiros[pos+1] = tempP;

            int tempPr = byteOffsets[pos+1];
            byteOffsets[pos+1] = byteOffsets[pos];
            byteOffsets[pos] = tempPr;
        }
    }

    noArvore->C1 = chaves[0];
    noArvore->C2 = chaves[1];
    noArvore->C3 = chaves[2];
    noArvore->C4 = chaves[3];

    noArvore->P1 = ponteiros[0];
    noArvore->P2 = ponteiros[1];
    noArvore->P3 = ponteiros[2];
    noArvore->P4 = ponteiros[3];
    noArvore->P5 = ponteiros[4];

    noArvore->Pr1 = byteOffsets[0];
    noArvore->Pr2 = byteOffsets[1];
    noArvore->Pr3 = byteOffsets[2];
    noArvore->Pr4 = byteOffsets[3];

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

    //atualiza noArvore e noSplit
    noArvore->C1 = chaves[0];
    noArvore->C2 = chaves[1];
    noArvore->C3 = -1;
    noArvore->C4 = -1;
//atualizar ponteiros e Pr

    noArvore->P1 = ponteiros[0];
    noArvore->P2 = ponteiros[1];
    noArvore->P3 = ponteiros[];
    noArvore->P4 = ponteiros[];
    noArvore->P5 = ponteiros[];

    noArvore->Pr1 = ponteiros[];
    noArvore->Pr2 = ponteiros[];
    noArvore->Pr3 = ponteiros[];
    noArvore->Pr4 = ponteiros[];
    
    //atualiza noSplit
    noSplit->C1 = chaves[3];
    noSplit->C2 = chaves[4];
    noSplit->C3 = -1;
    noSplit->C4 = -1;

    //atualiza chaveAInserir para chave a ser promovida
    chaveAInserir->C = chaves[2];
    chaveAInserir->Pr = byteOffsets[2];
    chaveAInserir->RRN = noSplit->RRNdoNo;
    
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
    //iniciamos com os novos valores no final do vetor e vamos trazendo para a esquerda
    chaves[0] = chaveAInserir->C;
    chaves[1] = noArvore->C1;
    chaves[2] = noArvore->C2;
    chaves[3] = noArvore->C3;
    chaves[4] = noArvore->C4;

    
    ponteiros[0] = -1; //vai acompanhar prefixoConvertido
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
    


    //ordena trazendo para a esquerda
    int pos = 0;
    while(chaves[pos] > chaves[pos+1]) {

        int tempC = chaves[pos+1];
        chaves[pos+1] = chaves[pos];
        chaves[pos] = tempC;

        int tempP = ponteiros[pos+2];
        ponteiros[pos+2] = ponteiros[pos];
        ponteiros[pos+1] = tempP;

        int tempPr = byteOffsets[pos+1];
        byteOffsets[pos+1] = byteOffsets[pos];
        byteOffsets[pos] = tempPr;
                
        pos--;
    }

    //caso o valor novo tenha ficado na primeira posicao, o ponteiro -1 precisa ir para a primeira posicao também
    if(chaves[0] == chaveAInserir->C) {

    }
}

#endif