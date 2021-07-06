#ifndef _funcoes_

#define _funcoes_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct conforme determinado no trabalho
typedef struct{
    char status;
    int noRaiz;
    int RRNproxNo;
    char lixo[68]; 
} ArvoreCabecalho;

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

#endif