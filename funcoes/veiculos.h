#ifndef _veiculos_

#define _veiculos_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct conforme determinado no trabalho
typedef struct{
    char removido;
    int tamanhoRegistro;
    char prefixo[5];
    char data[10]; //fixo AAAA-MM-DD
    int quantidadeLugares;
    int codLinha;
    int tamanhoModelo;
    char modelo[200]; //variavel
    int tamanhoCategoria;
    char categoria[200]; //variavel
} Veiculo;

//struct conforme determinado no trabalho
typedef struct {
    char status;
    long int byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[18];
    char descreveData[35];
    char descreveLugares[42];
    char descreveLinha[26];
    char descreveModelo[17];
    char descreveCategoria[20];
} CabecalhoVeiculo;

/*** FUNCOES CSV ***/
//le todos os registros de um arquivo csv
Veiculo** readCsvFileVeiculo(FILE* fp, int* nVeiculos, CabecalhoVeiculo* cabecalhoVeiculo, FILE* fw);

//escreve o cabecalho do arquivo csv, conforme especificado na struct
CabecalhoVeiculo* readCsvHeaderVeiculo(FILE* fp, CabecalhoVeiculo* cabecalhoVeiculo);

//le um registro do csv, retornando uma struct veiculo
Veiculo* readCsvDataRegisterVeiculo(FILE* fp, FILE* fw, CabecalhoVeiculo* cabecalhoVeiculo);

/*** FUNCOES BINARIO ***/
//le o cabecalho do binario, conforme especificado na struct
Veiculo* readBinaryHeaderVeiculo(FILE* fp, CabecalhoVeiculo* cabecalhoVeiculo);

//le um registro do binario, conforme especificado na struct
void readBinaryDataRegisterVeiculo(FILE* fp, Veiculo* veiculo);

//escreve o cabecalho no arquivo binario, conforme especificado na struct
CabecalhoVeiculo* writeBinaryHeaderVeiculo(FILE* fw, CabecalhoVeiculo* cabecalhoVeiculo);

//escreve um registro no binario, conforme especificado na struct
Veiculo* writeBinaryDataRegisterVeiculo(FILE* fw, Veiculo* teste);

/*** Funcoes de print/auxiliares ***/
//printa um registro inteiro na stdout, conforme especificado, tratando de valores nulos
void printBinaryDataRegisterVeiculo(CabecalhoVeiculo* cabecalhoVeiculo, Veiculo* veiculo);

//apenas transforma o numero do mes em nome (Ex.: 1-> "janeiro")
void converteNumeroEmNomeDoMes(char* mes, char* nomeDoMes);

//funcao auxiliar para printar a data no formato especificado
void printData(char *data, char* descreveData);

void FreeVeiculos(Veiculo** Veiculo, int nVeiculos);
#endif