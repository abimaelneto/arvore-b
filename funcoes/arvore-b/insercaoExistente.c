#ifndef _insereVeiculoArvore_

#define _insereVeiculoArvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./insercao.h"
#include "../veiculos.h"
#include "../../funcoesAux.h"

void insereChaveEmArvoreExistente(FILE* fb, CabecalhoArvore* cabecalhoArvore, Chave* chaveAInserir) {
    
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
        
        //move o cursor para o nó a ser escrito
        fseek(fb, byteOffsetNoArvore, SEEK_SET);
        writeNoArvore(fb, noRaiz, cabecalhoArvore);  

        printf("\nNova raiz!\n");
        printNoArvore(noRaiz);
    }

    free(noRaiz);

}

void leVeiculoASerInserido(Veiculo* veiculo) {

    //entrada do usuario
    scan_quote_string(veiculo->prefixo);
    scanf("%*c"); // le espaco  
    scan_quote_string(veiculo->data);
    scanf("%*c"); // le espaco  
    scanf("%d", &veiculo->quantidadeLugares); //le nome do campo
    scanf("%*c"); // le espaco  
    scanf("%d", &veiculo->codLinha); //le nome do campo
    scanf("%*c"); // le espaco  
    scan_quote_string(veiculo->modelo);
    scanf("%*c"); // le espaco  
    scan_quote_string(veiculo->categoria);


    //escreve lixo se o campo for nulo
    if(veiculo->data[0] == '\0') {
        memset(veiculo->data, '@', 10);
        veiculo->data[0] = '\0';
    }
    
    //guarda o tamanho dos campos variaveis
    veiculo->tamanhoModelo = strlen(veiculo->modelo);
    veiculo->tamanhoCategoria = strlen(veiculo->categoria); 
    
    //calcula tamanho do registro
    veiculo->tamanhoRegistro = 5 + 10 + 4*4 + veiculo->tamanhoModelo + veiculo->tamanhoCategoria;        
    veiculo->removido = '1';
        
}

void leLinhaASerInserida(Linha* linha) {
    //entrada do usuario
    scanf("%d", &linha->codLinha); //le nome do campo
    scanf("%*c"); // le espaco  
    scan_quote_string(&linha->aceitaCartao);
    printf("ACEITA CARTÃO %c", linha->aceitaCartao);
    scanf("%*c"); // le espaco  
    scan_quote_string(linha->nomeLinha);
    scanf("%*c"); // le espaco  
    scan_quote_string(linha->corLinha);

    //guarda o tamanho dos campos variaveis
    linha->tamanhoNome = strlen(linha->nomeLinha); 
    linha->tamanhoCor = strlen(linha->corLinha); 

    linha->tamanhoRegistro = 1 + 3*4 + linha->tamanhoNome + linha->tamanhoCor;        
    linha->removido = '1';
}

#endif