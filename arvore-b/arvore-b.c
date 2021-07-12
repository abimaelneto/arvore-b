#ifndef _arvoreGenericas_

#define _arvoreGenericas_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./arvore-b.h"



void writeHeaderTree(FILE* fb, CabecalhoArvore* cabecalhoArvore) {

    //escreve conforme especificado no trabalho
    fwrite(&(cabecalhoArvore->status), sizeof(char), 1, fb);
    fwrite(&(cabecalhoArvore->noRaiz), sizeof(int), 1, fb);
    fwrite(&(cabecalhoArvore->RRNproxNo), sizeof(int), 1, fb);
    fwrite(&(cabecalhoArvore->lixo), sizeof(char), 68, fb);
}

void readHeaderTree(FILE* fb, CabecalhoArvore* cabecalhoArvore) {

    //escreve conforme especificado no trabalho
    fread(&(cabecalhoArvore->status), sizeof(char), 1, fb);
    fread(&(cabecalhoArvore->noRaiz), sizeof(int), 1, fb);
    fread(&(cabecalhoArvore->RRNproxNo), sizeof(int), 1, fb);
    fread(&(cabecalhoArvore->lixo), sizeof(char), 68, fb);
}

void readNoArvore(FILE* fb, NoArvore* noArvore) {
    fread(&(noArvore->folha), sizeof(char), 1, fb); 
    fread(&(noArvore->nroChavesIndexadas), sizeof(int), 1, fb);
    fread(&(noArvore->RRNdoNo), sizeof(int), 1, fb);
    fread(&(noArvore->P1), sizeof(int), 1, fb);
    fread(&(noArvore->C1), sizeof(int), 1, fb);
    fread(&(noArvore->Pr1), sizeof(long int), 1, fb);
    fread(&(noArvore->P2), sizeof(int), 1, fb);
    fread(&(noArvore->C2), sizeof(int), 1, fb);
    fread(&(noArvore->Pr2), sizeof(long int), 1, fb);
    fread(&(noArvore->P3), sizeof(int), 1, fb);
    fread(&(noArvore->C3), sizeof(int), 1, fb);
    fread(&(noArvore->Pr3), sizeof(long int), 1, fb);
    fread(&(noArvore->P4), sizeof(int), 1, fb);
    fread(&(noArvore->C4), sizeof(int), 1, fb);
    fread(&(noArvore->Pr4), sizeof(long int), 1, fb);
    fread(&(noArvore->P5), sizeof(int), 1, fb);
}

void writeNoArvore(FILE* fb, NoArvore* noArvore, CabecalhoArvore* cabecalhoArvore) {
    
    fwrite(&(noArvore->folha), sizeof(char), 1, fb); 
    fwrite(&(noArvore->nroChavesIndexadas), sizeof(int), 1, fb);
    fwrite(&(noArvore->RRNdoNo), sizeof(int), 1, fb);
    fwrite(&(noArvore->P1), sizeof(int), 1, fb);
    fwrite(&(noArvore->C1), sizeof(int), 1, fb);
    fwrite(&(noArvore->Pr1), sizeof(long int), 1, fb);
    fwrite(&(noArvore->P2), sizeof(int), 1, fb);
    fwrite(&(noArvore->C2), sizeof(int), 1, fb);
    fwrite(&(noArvore->Pr2), sizeof(long int), 1, fb);
    fwrite(&(noArvore->P3), sizeof(int), 1, fb);
    fwrite(&(noArvore->C3), sizeof(int), 1, fb);
    fwrite(&(noArvore->Pr3), sizeof(long int), 1, fb);
    fwrite(&(noArvore->P4), sizeof(int), 1, fb);
    fwrite(&(noArvore->C4), sizeof(int), 1, fb);
    fwrite(&(noArvore->Pr4), sizeof(long int), 1, fb);
    fwrite(&(noArvore->P5), sizeof(int), 1, fb);
    
}

void inicializaNoArvore(NoArvore* noArvore, int eFolha, CabecalhoArvore* cabecalhoArvore) {
    
    noArvore->folha = eFolha; //'1' para folha e '0' para não-folha
    noArvore->nroChavesIndexadas = 0;
    
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
    
}
void printNoArvore(NoArvore* noArvore) {
    
    printf("Folha: %c\n ", noArvore->folha); 
    printf("Nro. Chaves: %d\n ", noArvore->nroChavesIndexadas);
    printf("RRN: %d %x\n", noArvore->RRNdoNo, noArvore->RRNdoNo);
    printf("P1: %d %x\n", noArvore->P1, noArvore->P1);
    printf("C1: %x\n", noArvore->C1);
    printf("P2: %d %x\n", noArvore->P2, noArvore->P2);
    printf("C2: %x\n", noArvore->C2);
    printf("P3: %d %x\n", noArvore->P3, noArvore->P3);
    printf("C3: %x \n", noArvore->C3);
    printf("P4: %d %x\n", noArvore->P4, noArvore->P4);
    printf("C4: %x\n", noArvore->C4);
    printf("P5: %d %x\n", noArvore->P5, noArvore->P5);
}

#endif