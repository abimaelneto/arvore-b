#ifndef _cabecalhoBuscaArvore_

#define _cabecalhoBuscaArvore_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./arvore-b.h"

int busca( FILE* fb, CabecalhoArvore* cabecalhoArvore, int* RRNNoBusca, Chave* chave);
int verificaSeAChaveEstaNoNo(Chave* chave, NoArvore* noArvore, int* RRNNoBusca);

#endif