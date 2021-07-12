#ifndef _cabecalhoInsercaoArvore_

#define _cabecalhoInsercaoArvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./arvore-b.h"

int insereChave( FILE* fb, NoArvore* noArvore, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore, int RRNNoBusca);

void insereChaveEmNoComEspaco(NoArvore* noArvore, Chave* chaveAInserir, int RRNNoBusca);
void split( FILE* fb, NoArvore* noArvore, NoArvore* noSplit, Chave* chaveAInserir);
void ordenaChavesParaSplit( NoArvore* noArvore, Chave* chaveAInserir, int* chaves, int* byteOffsets, int* ponteiros);

void insereChaveEmArvoreExistente(FILE* fb, CabecalhoArvore* cabecalhoArvore, Chave* chaveAInserir);
void leVeiculoASerInserido(Veiculo* veiculo);
void leLinhaASerInserida(Linha* linha);

void insereChaveEmArvoreNova(FILE* fb, CabecalhoArvore* cabecalhoArvore, Chave* chaveAInserir);
int desceNaArvore( FILE* fb, NoArvore* noArvore, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore);

#endif