#ifndef _cabecalhoInsercaoArvore_

#define _cabecalhoInsercaoArvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./arvore-b.h"

int insereChave( FILE* fb, NoArvore* noArvore, Chave* chave, CabecalhoArvore* cabecalhoArvore, int RRNNoBusca);

void insereChaveEmNoComEspaco(NoArvore* noArvore, Chave* chave, int RRNNoBusca);
void split( FILE* fb, NoArvore* noArvore, NoArvore* noSplit, Chave* chave);
void ordenaChavesParaSplit( NoArvore* noArvore, Chave* chave, int* chaves, int* byteOffsets, int* ponteiros);

void insereChaveEmArvoreExistente(FILE* fb, CabecalhoArvore* cabecalhoArvore, Chave* chave);
void leVeiculoASerInserido(Veiculo* veiculo);
void leLinhaASerInserida(Linha* linha);

void insereChaveEmArvoreNova(FILE* fb, CabecalhoArvore* cabecalhoArvore, Chave* chave);
int desceNaArvore( FILE* fb, NoArvore* noArvore, Chave* chave, CabecalhoArvore* cabecalhoArvore);

#endif