#ifndef _linhaFuncoes_

#define _linhaFuncoes_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linhas.h"

/*** FUNCOES CSV ***/

CabecalhoLinha* readCsvHeaderLinha(FILE* fp, CabecalhoLinha* cabecalhoLinha){
    
    char c = 0;
    int currField = 0;

    while (c != '\n' && c != EOF){
        char registerData[50];
        int currPos = 0;

        int ret = fscanf(fp, "%c", &c); //return

        // lê até encontrar vírgula, final de linha, final do arquivo, valor nulo
        while (
            c != ',' && 
            c != '\n' &&
            ret != EOF &&
            c != 0 ) {    
            registerData[currPos] = c;
            currPos ++;
            ret = fscanf(fp, "%c", &c);
        }
        if(ret == EOF){
            return NULL;
        }

        registerData[currPos] = '\0'; //marca fim do registro
        
        //usa strcpy para strings e atoi para inteiros
        if (currField == 0) {
            strcpy(cabecalhoLinha->descreveCodigo, registerData);
        }
        else if (currField == 1) {
            strcpy(cabecalhoLinha->descreveCartao, registerData);
        }
        else if (currField == 2) {                
            strcpy(cabecalhoLinha->descreveNome, registerData);
        }
        else if (currField == 3) {                
            strcpy(cabecalhoLinha->descreveCor, registerData);
        }
        
        currField++;
    }

    return cabecalhoLinha;
}

Linha* readCsvDataRegisterLinha(FILE* fp, FILE* fw, CabecalhoLinha* cabecalhoLinha) {
    Linha* linha =(Linha* ) malloc(sizeof(Linha));
    linha->removido = '1';
    char c = 0;
    int currField = 0;
    
    //le ate o final da linha ou do arquivo
    while (c != '\n' && c != EOF){
        char registerData[50];
        int currPos = 0;

        int ret = fscanf(fp, "%c", &c); //return

        // lê até encontrar vírgula, final de linha, final do arquivo, valor nulo
        while (
            c != ',' && 
            c != '\n' &&
            ret != EOF &&
            c != 0 ) {    
            registerData[currPos] = c;
            if(registerData[currPos] == '*') {
                cabecalhoLinha->nroRegRemovidos = cabecalhoLinha->nroRegRemovidos + 1;
                linha->removido = '0';
                currPos--;
            }
            
            currPos ++;
            ret = fscanf(fp, "%c", &c);
        }
        if(ret == EOF){
            free(linha);
            return NULL;
        }
        
        registerData[currPos] = '\0'; //marca fim do registro
        
        //trata cada um dos campos, tratando valores nulos, string e int
        if (currField == 0) {
            int codLinha = atoi(registerData);
            linha->codLinha = codLinha;
        }
        else if (currField == 1) {
            linha->aceitaCartao = *registerData;
        }
        else if (currField == 2) {
            if(strcmp("NULO", registerData) == 0) {    
                linha->tamanhoNome = 0;    
            }
            else {                
                strcpy(linha->nomeLinha, registerData);
                linha->tamanhoNome = strlen(linha->nomeLinha); // desconsidera '\0' ?
            }
        }
        else if (currField == 3) {
            if(strcmp("NULO", registerData) == 0) {
                linha->tamanhoCor = 0;    
            }
            else {
                strcpy(linha->corLinha, registerData);
                linha->tamanhoCor = strlen(linha->corLinha); // desconsidera '\0'
            }
        }
        
        //passa para o proximo campo
        currField++;
    }
    
    //soma os tamanhos fixos, os variaveis e o numero de campos inteiros multiplicado por 4
    linha->tamanhoRegistro = 1 + 3*4 + linha->tamanhoNome + linha->tamanhoCor;
    writeBinaryDataRegisterLinha(fw, linha);
    
    return linha;
}

Linha** readCsvFileLinha(FILE* fp, int* nLinhas, CabecalhoLinha* cabecalhoLinha, FILE* fw){
    
    Linha** linhas = NULL; //armazena linhas
    Linha* linha; //armazena campos do registro
    *nLinhas = 0; //contador

    char c = 0;
    
    do{
        //chama a funcao para ler um registro do csv por vez, salvando entao na struct Veiculo e incrementando a contagem
        linha = readCsvDataRegisterLinha(fp, fw, cabecalhoLinha);
        linhas = (Linha** ) realloc(linhas, sizeof(Linha* )*(*nLinhas + 1));
        linhas[*nLinhas] = linha;
        (*nLinhas)++;
        
    } while(linha != NULL); 
    
    cabecalhoLinha->nroRegistros = *nLinhas;
    
    return linhas;    
    
}

/*** FUNCOES BINARIO ***/
Linha* readBinaryHeaderLinha(FILE* fp, CabecalhoLinha* cabecalhoLinha) {
    fread(&(cabecalhoLinha->byteProxReg), sizeof(long int), 1, fp);
    fread(&(cabecalhoLinha->nroRegistros), sizeof(int), 1, fp);
    fread(&(cabecalhoLinha->nroRegRemovidos), sizeof(int), 1, fp);
    fread(cabecalhoLinha->descreveCodigo, sizeof(char), 15, fp);
    
    fread(cabecalhoLinha->descreveCartao, sizeof(char), 13, fp);
    fread(cabecalhoLinha->descreveNome, sizeof(char), 13, fp);
    fread(cabecalhoLinha->descreveCor, sizeof(char), 24, fp);
}

Linha* readBinaryDataRegisterLinha(FILE* fw, Linha* linha) {
    fread(&(linha->removido), sizeof(char), 1, fw); 
    fread(&(linha->tamanhoRegistro), sizeof(int), 1, fw);
    fread(&(linha->codLinha), sizeof(int), 1, fw); 
    fread(&(linha->aceitaCartao), sizeof(char), 1, fw);
    
    fread(&(linha->tamanhoNome), sizeof(int), 1, fw);
    if(linha->tamanhoNome > 0) {
        fread(linha->nomeLinha, sizeof(char), linha->tamanhoNome, fw); 
    }
    
    fread(&(linha->tamanhoCor), sizeof(int), 1, fw);
    if(linha->tamanhoCor > 0) {
        fread(linha->corLinha, sizeof(char), linha->tamanhoCor, fw);     
    }   
}

Linha* writeBinaryDataRegisterLinha(FILE* fw, Linha* linha){    
    
    fwrite(&(linha->removido), sizeof(char), 1, fw); 
    fwrite(&(linha->tamanhoRegistro), sizeof(int), 1, fw);
    fwrite(&(linha->codLinha), sizeof(int), 1, fw);
    fwrite(&(linha->aceitaCartao), sizeof(char), 1, fw);
    
    fwrite(&(linha->tamanhoNome), sizeof(int), 1, fw); 
    if(linha->tamanhoNome > 0) {
        fwrite(&(linha->nomeLinha), sizeof(char), linha->tamanhoNome, fw);     
    }
    
    fwrite(&(linha->tamanhoCor), sizeof(int), 1, fw);
    if(linha->tamanhoCor > 0) {
        fwrite(linha->corLinha, sizeof(char), linha->tamanhoCor, fw);     
    }
    
}

CabecalhoLinha* writeBinaryHeaderLinha(FILE* fw, CabecalhoLinha* cabecalhoLinha){   

    fwrite(cabecalhoLinha, sizeof(char), 1, fw);
    fwrite(&(cabecalhoLinha->byteProxReg), sizeof(int), 2, fw);
    fwrite(&(cabecalhoLinha->nroRegistros), sizeof(int), 1, fw);
    fwrite(&(cabecalhoLinha->nroRegRemovidos), sizeof(int), 1, fw);
    fwrite(cabecalhoLinha->descreveCodigo, sizeof(char), 15, fw);
    fwrite(cabecalhoLinha->descreveCartao, sizeof(char), 13 , fw);
    fwrite(cabecalhoLinha->descreveNome, sizeof(char), 13, fw);
    fwrite(cabecalhoLinha->descreveCor, sizeof(char), 24, fw);
}

void printBinaryDataRegisterLinha(CabecalhoLinha* cabecalhoLinha, Linha* linha) {
    
    //nessa funcao, optou-se por printar sempre o tamanho conhecido dos campos,
    //para garantir a formatação correta e evitar problemas por endereços indesejados

    printf("%.15s: %d\n", cabecalhoLinha->descreveCodigo, linha->codLinha);
    if (linha->tamanhoNome > 0) {
        printf("%.13s: %.*s\n",
            cabecalhoLinha->descreveNome,
            linha->tamanhoNome,
            linha->nomeLinha
        );
    }
    else {
        printf("%.13s: campo com valor nulo\n", cabecalhoLinha->descreveNome);
    }
    if (linha->tamanhoCor > 0) {
        printf("%.24s: %.*s\n",
            cabecalhoLinha->descreveCor,
            linha->tamanhoCor,
            linha->corLinha
        );
    }
    else {
        printf("%.24s: campo com valor nulo\n", cabecalhoLinha->descreveCor);
    }
    char aceitaCartao[2];
    strcpy(aceitaCartao, &(linha->aceitaCartao));
    printf("%.13s: ", cabecalhoLinha->descreveCartao);
    switch(aceitaCartao[0]) {
        case 'S':
            printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
            break;
        case 'N':
            printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
            break;
        case 'F':
            printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
            break;
        default:
            printf("campo com valor nulo\n");
            break;
    }
    
    printf("\n");
}

void FreeLinhas(Linha** Linha, int nLinhas){
    for(int i = 0; i< nLinhas; i++){
        free(Linha[i]);
    }
    free(Linha);
}

#endif