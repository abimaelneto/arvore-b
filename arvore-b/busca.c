#ifndef _dadosBuscaArvore_

#define _dadosBuscaArvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./busca.h"
#include "./arvore-b.h"


int busca(
    FILE* fb,
    CabecalhoArvore* cabecalhoArvore,
    int* RRNNoBusca,
    Chave* chave)
{
    
    //pulamos para o nó a ser lido e lemos para a RAM
    int byteOffsetNoArvore = TAM_PAG_DISCO*(1+ *RRNNoBusca);
    fseek(fb, byteOffsetNoArvore, SEEK_SET);
    
    NoArvore* noArvore = (NoArvore*) malloc(sizeof(NoArvore));
    inicializaNoArvore(noArvore, '0', cabecalhoArvore);
    readNoArvore(fb, noArvore);
    
    //flag que indicará se registro foi encontrado
    int registroEncontrado = 0;
    
    //retorna 1 caso seja igual, ou 0 se não for.
    // O RRNNoBusca é o próximo RRN a ser verificado
    int encontrouChave = verificaSeAChaveEstaNoNo(chave, noArvore, RRNNoBusca);
    
    if( encontrouChave == 1 ) {
        free(noArvore);
        return 1;
    }
    
    else {
        //caso não tenha encontrado e o nó é folha, o registro não existe
        if(noArvore->folha == '1') {
            free(noArvore);
            return 0;
        }

        encontrouChave = busca(fb, cabecalhoArvore, RRNNoBusca, chave);
        free(noArvore);
        return encontrouChave;
    }
 
   
}

int verificaSeAChaveEstaNoNo(Chave* chave, NoArvore* noArvore, int* RRNNoBusca) {
    
    //se for igual a alguma chave, retorna o valor do byteOffset correspondente e salva o RRN do nó onde foi encontrado
    if( chave->C == noArvore->C1 ) {
        chave->Pr = noArvore->Pr1;
        return 1;
    }
    else if( chave->C == noArvore->C2 ) {
        chave->Pr = noArvore->Pr2;
        return 1;
    }
    else if( chave->C == noArvore->C3 ) {
        chave->Pr = noArvore->Pr3;
        return 1;
    }
    else if( chave->C == noArvore->C4 ) {
        chave->Pr = noArvore->Pr4;
        return 1;
    }
    //caso não seja igual a nenhuma, compara para ver qual o próximo RRN a ser buscado
    else if(chave->C < noArvore->C1) {
        *RRNNoBusca = noArvore->P1;
    }
    else if (chave->C < noArvore->C2 || noArvore-> C2 == -1) {
        *RRNNoBusca = noArvore->P2;
    }
    else if (chave->C < noArvore->C3 || noArvore-> C3 == -1) {
        *RRNNoBusca = noArvore->P3;
    }
    else if (chave->C < noArvore->C4 || noArvore-> C4 == -1) {
        *RRNNoBusca = noArvore->P4;
    }
    else {
        *RRNNoBusca = noArvore->P5;
    }

    return 0;
}

#endif