#ifndef _cabecalhoArvore_

#define _cabecalhoArvore_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../veiculos.h"
#include "../linhas.h"

#define TAM_PAG_DISCO 77

//struct conforme determinado no trabalho
typedef struct{
    char status;
    int noRaiz;
    int RRNproxNo;
    char lixo[68]; 
} CabecalhoArvore;

typedef struct{
    char folha; //1 para folha e 0 para não-folha
    int nroChavesIndexadas;
    int RRNdoNo;
    int P1; 
    int C1; 
    long int Pr1; 
    int P2; 
    int C2; 
    long int Pr2;
    int P3; 
    int C3; 
    long int Pr3;
    int P4; 
    int C4; 
    long int Pr4;
    int P5; 
} NoArvore;


typedef struct {
    int P; 
    int C; 
    int Pr;
} Chave;


//genericas
void writeHeaderTree(FILE* fb, CabecalhoArvore* cabecalhoArvore);
void readHeaderTree(FILE* fb, CabecalhoArvore* cabecalhoArvore);

void readNoArvore(FILE* fb, NoArvore* noArvore);
void writeNoArvore(FILE* fb, NoArvore* noArvore, CabecalhoArvore* cabecalhoArvore);

void printNoArvore(NoArvore* noArvore);


//criacao da arvore
void inicializaNoArvore(NoArvore* noArvore, int eFolha, CabecalhoArvore* cabecalhoArvore);
int desceNaArvore(FILE* fb, NoArvore* noArvore, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore);

//insere veiculo na arvore
int desceNaArvore(FILE* fb, NoArvore* noArvore, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore);


#endif