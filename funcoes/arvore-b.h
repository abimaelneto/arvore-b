#ifndef _cabecalhoArvore_

#define _cabecalhoArvore_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int Pr1; 
    int P2; 
    int C2; 
    int Pr2;
    int P3; 
    int C3; 
    int Pr3;
    int P4; 
    int C4; 
    int Pr4;
    int P5; 
} NoArvore;


typedef struct {
    int C; 
    int Pr;
    int RRN; //RRN do nó onde a chave se encontra
} Chave;


//genericas
void writeHeaderTreeVeiculo(FILE* fb, CabecalhoArvore* cabecalhoArvore);
void readNoArvore(FILE* fb, NoArvore* noArvore);
void writeNoArvore(FILE* fb, NoArvore* noArvore, CabecalhoArvore* cabecalhoArvore);

//criacao da arvore
void insereVeiculoNaArvore(FILE* fb, CabecalhoArvore* cabecalhoArvore, NoArvore* noRaiz, Chave* chaveAInserir);
void inicializaNoArvore(NoArvore* noArvore, int eFolha, CabecalhoArvore* cabecalhoArvore);
int busca( FILE* fb, int RRNNoBusca, NoArvore* noPai, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore);
int desceNaArvore(FILE* fb, NoArvore* noArvore, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore);
int insereChave(FILE* fb, NoArvore* noArvore, Chave* chaveAInserir, CabecalhoArvore* cabecalhoArvore);
void insereChaveEmNoComEspaco(NoArvore* noArvore, Chave* chaveAInserir);
void split( FILE* fb, NoArvore* noArvore, NoArvore* noSplit, Chave* chaveAInserir);
void ordenaChavesParaSplit( NoArvore* noArvore, Chave* chaveAInserir, int* chaves, int* byteOffsets, int* ponteiros);
void criaNo(FILE* fb, NoArvore* noASerCriado, Chave* chaveAInserir);

#endif