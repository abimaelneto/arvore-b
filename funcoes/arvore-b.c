#ifndef _arvore_

#define _arvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore-b.h"

void writeHeaderTreeVeiculo(FILE* fb, CabecalhoArvore* cabecalhoArvore) {
    

    //escreve conforme especificado no trabalho
    fwrite(cabecalhoArvore->status, sizeof(char), 1, fb);
    fwrite(cabecalhoArvore->noRaiz, sizeof(int), 1, fb);
    fwrite(cabecalhoArvore->RRNRRNNoBusca, sizeof(int), 1, fb);
    fwrite(cabecalhoArvore->lixo, sizeof(char), 68, fb);
}

void inicializaNoArvore(NoArvore* noArvore, int eFolha, CabecalhoArvore* cabecalhoArvore) {
    
    noArvore->folha = eFolha; //1 para folha e 0 para não-folha
    noArvore->nroChavesIndexadas = 0;
    noArvore->RRNdoNo = cabecalhoArvore->RRNRRNNoBusca;
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

    cabecalhoArvore->RRNNoBusca++;
}

void insereVeiculoNaArvore(FILE* fb, CabecalhoArvore* cabecalhoArvore, int prefixoConvertido, int byteOffset) {
    
    Chave* chaveAInserir;
    chaveAInserir->C = prefixoConvertido;
    chaveAInserir->Pr = byteOffset;
    
    int RRNNoBusca = cabecalhoArvore->noRaiz; //proximo nó a ser buscado
    
    //aloca nó pai
    NoArvore* noPai = (NoArvore*) malloc(sizeof(NoArvore));
    
    int criarNo = 0;
    //faz a busca pelo nó de RRN número 'RRNNoBusc' e salva em 'noArvore'
    criarNo = busca(fb, RRNNoBusca, noPai, chaveAInserir);

    if( criarNo == 1)  {
        criaNo(fb, noPai, chaveAInserir);
    }

    free(noPai);

}

int busca( FILE* fb, int RRNNoBusca, NoArvore* noPai, Chave* chaveAInserir) {
   
    //conhecemos o tamanho da "página de disco". 
    //pulamos para o nó a ser lido
    fseek(fb, ( sizeof(CabecalhoArvore)+(sizeof(NoArvore)*(RRNNoBusca))), SEEK_SET);
    
    //le página completa
    readNoArvore(fb, noPai);

    //valor a ser retornado. Caso precise promover chave, terá que chamar a inserção novamente
    int ocorreuSplit = 0;
    
     // caso o nó seja folha, tenta inserir
    if( noPai->folha == 1 ){
        
        ocorreuSplit = insere(fb, noPai, chaveAInserir);
        return ocorreuSplit;
    }
    
    //se nó não for folha, tenta encontrar RRN do próximo nó da busca
    if(chaveAInserir < noPai->C1) {
        RRNNoBusca = noPai->P1;
    }
    else if (chaveAInserir < noPai->C2) {
        RRNNoBusca = noPai->P2;
    }
    else if (chaveAInserir < noPai->C3) {
        RRNNoBusca = noPai->P3;
    }
    else if (chaveAInserir < noPai->C4) {
        RRNNoBusca = noPai->P4;
    }
    else {
        RRNNoBusca = noPai->P5;
    }
    //aloca nó filho
    NoArvore* noFilho = (NoArvore*) malloc(sizeof(NoArvore));
    
    //busca o nó filho pelo seu RRN
    ocorreuSplit = busca(fb, RRNNoBusca, noFilho, chaveAInserir);
    if( ocorreuSplit == 1) {
        ocorreuSplit = insere(fb, noPai, chaveAInserir);
    }
    return ocorreuSplit;

    
}

//retorna 1 se houve split
int insere(FILE* fb, NoArvore* noArvore, Chave* chaveAInserir) {
        
    //caso haja espaço, ordena e insere
    if( noArvore->nroChavesIndexadas < 4) {
        
        //ordena chaves
        ordenaChavesEInsere(noArvore, chaveAInserir);
        return 0;
    }

    //caso não haja espaço faz o split 
    else {
        
        //cria novo nó
        NoArvore* noSplit = (NoArvore*) malloc(sizeof(NoArvore));
        inicializaNoArvore(noSplit, noArvore->folha, cabecalhoArvore);
        
        //organiza as chaves entre o nó antigo e o novo
        //atualiza chaveAInserir para a chave que está sendo promovida
        split(fb, noArvore, noSplit, chaveAInserir);
        
        //escreve o nó criado à direita
        writeNoArvore(fb, noSplit);

        //escreve o nó criado à esquerda
        writeNoArvore(fb, noArvore);

        return 1;
    }
}

void readNoArvore(FILE* fb, NoArvore* noArvore) {
    fread(&(noArvore->folha), sizeof(char), 1, fb); 
    fread(&(noArvore->nroChavesIndexadas), sizeof(int), 1, fb);
    fread(&(noArvore->RRNdoNo), sizeof(int), 1, fb);
    fread(&(noArvore->P1), sizeof(int), 1, fb);
    fread(&(noArvore->C1), sizeof(int), 1, fb);
    fread(&(noArvore->Pr1), sizeof(int), 1, fb);
    fread(&(noArvore->P2), sizeof(int), 1, fb);
    fread(&(noArvore->C2), sizeof(int), 1, fb);
    fread(&(noArvore->Pr2), sizeof(int), 1, fb);
    fread(&(noArvore->P3), sizeof(int), 1, fb);
    fread(&(noArvore->C3), sizeof(int), 1, fb);
    fread(&(noArvore->Pr3), sizeof(int), 1, fb);
    fread(&(noArvore->P4), sizeof(int), 1, fb);
    fread(&(noArvore->C4), sizeof(int), 1, fb);
    fread(&(noArvore->Pr4), sizeof(int), 1, fb);
    fread(&(noArvore->P5), sizeof(int), 1, fb);
}

void writeNoArvore(FILE* fb, NoArvore* noArvore) {
    fwrite(noArvore->folha, sizeof(char), 1, fb); 
    fwrite(&(noArvore->nroChavesIndexadas), sizeof(int), 1, fb);
    fwrite(&(noArvore->RRNdoNo), sizeof(int), 1, fb);
    fwrite(&(noArvore->P1), sizeof(int), 1, fb);
    fwrite(&(noArvore->C1), sizeof(int), 1, fb);
    fwrite(&(noArvore->Pr1), sizeof(int), 1, fb);
    fwrite(&(noArvore->P2), sizeof(int), 1, fb);
    fwrite(&(noArvore->C2), sizeof(int), 1, fb);
    fwrite(&(noArvore->Pr2), sizeof(int), 1, fb);
    fwrite(&(noArvore->P3), sizeof(int), 1, fb);
    fwrite(&(noArvore->C3), sizeof(int), 1, fb);
    fwrite(&(noArvore->Pr3), sizeof(int), 1, fb);
    fwrite(&(noArvore->P4), sizeof(int), 1, fb);
    fwrite(&(noArvore->C4), sizeof(int), 1, fb);
    fwrite(&(noArvore->Pr4), sizeof(int), 1, fb);
    fwrite(&(noArvore->P5), sizeof(int), 1, fb);
}

void ordenaChavesEInsere(NoArvore* noArvore, Chave* chaveAInserir) {
    int P[5];
    int C[4];
    int Pr[4];

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

    ordenaChaves(
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

    //atualiza noSplit
    noArvore->C1 = chaves[3];
    noArvore->C2 = chaves[4];
    noArvore->C3 = -1;
    noArvore->C4 = -1;

    //atualiza chaveAInserir para chave a ser promovida
    chaveAInserir->C = chaves[2];
    chaveAInserir->Pr = byteOffsets[2];
    chaveAInserir->RRN = noSplit->RRNdoNo;
    
    // como foi feito split, os dois nós possuem 2 chaves
    noArvore->nroChavesIndexadas=2;
    noSplit->nroChavesIndexadas=2;   
    
}   

void ordenaChaves(
    NoArvore* noArvore,
    Chave* chaveAInserir,
    int* chaves,
    int* byteOffsets,
    int* ponteiros
) {
    //iniciamos com os novos valores no final do vetor e vamos trazendo para a esquerda
    int chaves[5] = {
        noArvore->C1,
        noArvore->C2,
        noArvore->C3,
        noArvore->C4,
        chaveAInserir->C
    };
    int ponteiros[6] = {
        noArvore->P1,
        noArvore->P2,
        noArvore->P3,
        noArvore->P4,
        noArvore->P5,
        -1 //vai acompanhar prefixoConvertido
    };

    int byteOffSets[5] = {
        noArvore->Pr1,
        noArvore->Pr2,
        noArvore->Pr3,
        noArvore->Pr4,
        chaveAInserir->Pr
    };

    
    //ordena trazendo para a esquerda
    int pos = 4;
    while(chaves[pos] > chaves[pos+1]) {

        int tempC = chaves[pos+1];
        chaves[pos+1] = chaves[pos];
        chaves[pos] = tempC;

        int tempP = ponteiros[pos+2];
        ponteiros[pos+2] = ponteiros[pos];
        ponteiros[pos+1] = tempP;

        int tempPr = byteOffSets[pos+1];
        byteOffSets[pos+1] = byteOffSets[pos];
        byteOffSets[pos] = tempPr;
                
        pos--;
    }

    //caso o valor novo tenha ficado na primeira posicao, o ponteiro -1 precisa ir para a primeira posicao também
    if(chaves[0] == chaveAInserir->C) {

    }
}

#endif