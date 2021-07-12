#ifndef _cabecalhoInsercaoArvore_

#define _cabecalhoInsercaoArvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./arvore-b.h"

//algoritmo de mais alto nível
int insercao(FILE* fb, CabecalhoArvore* cabecalhoArvore, int* RRNNoBusca, Chave* chave);

//funções intermediárias
int insereChave( FILE* fb, NoArvore* noArvore, Chave* chave, CabecalhoArvore* cabecalhoArvore, int* RRNNoBusca);
void insereChaveEmNoComEspaco(NoArvore* noArvore, Chave* chave, int* RRNNoBusca);
void split( FILE* fb, NoArvore* noArvore, NoArvore* noSplit, Chave* chave);
void ordenaChavesParaSplit( NoArvore* noArvore, Chave* chave, int* chaves, int* byteOffsets, int* ponteiros);

//funções de inserção em árvore existente
void leVeiculoASerInserido(Veiculo* veiculo);
void leLinhaASerInserida(Linha* linha);

void criaNovaRaiz( FILE* fb, CabecalhoArvore* cabecalhoArvore, Chave* chave);

#endif