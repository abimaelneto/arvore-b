#ifndef _linhas_

#define _linhas_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct conforme determinado no trabalho
typedef struct{
    char removido;
    int tamanhoRegistro;
    int codLinha;
    char aceitaCartao;
    int tamanhoNome;
    char nomeLinha[200]; //variavel
    int tamanhoCor;
    char corLinha[200]; //variavel
} Linha;

//struct conforme determinado no trabalho
typedef struct {
    char status;
    long int byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[15];
    char descreveCartao[13];
    char descreveNome[13];
    char descreveCor[24];
} CabecalhoLinha;

/*** FUNCOES CSV ***/
//le todos os registros de um arquivo csv
Linha** readCsvFileLinha(FILE* fp, int* nLinhas, CabecalhoLinha* cabecalhoLinha, FILE* fw);

//escreve o cabecalho do arquivo csv, conforme especificado na struct
CabecalhoLinha* readCsvHeaderLinha(FILE* fp, CabecalhoLinha* cabecalhoLinha);

//le um registro do csv, retornando uma struct linha
Linha* readCsvDataRegisterLinha(FILE* fp, FILE* fw, CabecalhoLinha* cabecalhoLinha);

/*** FUNCOES BINARIO ***/
//le o cabecalho do binario, conforme especificado na struct
Linha* readBinaryHeaderLinha(FILE* fp, CabecalhoLinha* cabecalhoLinha);

//le um registro do binario, conforme especificado na struct
Linha* readBinaryDataRegisterLinha(FILE* fp, Linha* linha);

//escreve o cabecalho no arquivo binario, conforme especificado na struct
CabecalhoLinha* writeBinaryHeaderLinha(FILE* fw, CabecalhoLinha* cabecalhoLinha);

//escreve um registro no binario, conforme especificado na struct
Linha* writeBinaryDataRegisterLinha(FILE* fw, Linha* teste);

/*** Funcoes de print/auxiliares ***/
//printa um registro inteiro na stdout, conforme especificado, tratando de valores nulos
void printBinaryDataRegisterLinha(CabecalhoLinha* cabecalhoLinha, Linha* linha);
void FreeLinhas(Linha** Linha, int nLinhas);

#endif