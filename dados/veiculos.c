#ifndef _veiculodados_

#define _veiculodados_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "veiculos.h"

/*** dados CSV ***/
CabecalhoVeiculo* readCsvHeaderVeiculo(FILE* fp, CabecalhoVeiculo* cabecalhoVeiculo){
    
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
            strcpy(cabecalhoVeiculo->descrevePrefixo, registerData);
        }
        else if (currField == 1) {
            strcpy(cabecalhoVeiculo->descreveData, registerData);
        }
        else if (currField == 2) {                
            strcpy(cabecalhoVeiculo->descreveLugares, registerData);
        }
        else if (currField == 3) {                
            strcpy(cabecalhoVeiculo->descreveLinha, registerData);
        }
        else if (currField == 4) {                
            strcpy(cabecalhoVeiculo->descreveModelo, registerData);
        }
        else if (currField == 5) {                
            strcpy(cabecalhoVeiculo->descreveCategoria, registerData);
        }
        
        currField++;
    }

    return cabecalhoVeiculo;
}

Veiculo* readCsvDataRegisterVeiculo(FILE* fp, FILE* fw, CabecalhoVeiculo* cabecalhoVeiculo) {
    Veiculo* veiculo =(Veiculo* ) malloc(sizeof(Veiculo));
    veiculo->removido = '1';
    char c = 0;
    int currField = 0;

    //le ate o final da linha ou do arquivo
    while (c != '\n' && c != EOF){
        char registerData[50]; //armazena o conteudo do campo
        int currPos = 0; //posicao do cursor
        int ret = fscanf(fp, "%c", &c); //cursor de leitura

        // lê até encontrar vírgula, final de linha, final do arquivo, valor nulo
        while (
            c != ',' && 
            c != '\n' &&
            ret != EOF &&
            c != 0 ) {    
            registerData[currPos] = c;
            if(registerData[currPos] == '*') {
                cabecalhoVeiculo->nroRegRemovidos = cabecalhoVeiculo->nroRegRemovidos + 1;
                veiculo->removido = '0';
                currPos--;
            }
            
            currPos ++; 
            ret = fscanf(fp, "%c", &c); //cursor de leitura
        }
        if(ret == EOF){
            free(veiculo);
            return NULL;
        }
        
        registerData[currPos] = '\0'; //marca fim do registro
        
        //trata cada um dos campos, tratando valores nulos, string e int
        if (currField == 0) {
            strcpy(veiculo->prefixo, registerData);
        }
        else if (currField == 1) {
            if(strcmp("NULO", registerData) == 0) {    
                for(int temp = 0; temp < 10; temp++) {
                    if(temp==0){
                        veiculo->data[temp] = '\0';    
                        continue;
                    }
                    veiculo->data[temp] = '@';
                }
            }
            else {
                strcpy(veiculo->data, registerData);
            }
            
        }
        else if (currField == 2) {
            if(strcmp("NULO", registerData) == 0) {    
                veiculo->quantidadeLugares = -1;
            }
            else{
                int quantidadeLugares = atoi(registerData);
                veiculo->quantidadeLugares = quantidadeLugares;
            }
            
        }
        else if (currField == 3) {
            if(strcmp("NULO", registerData) == 0) {    
                veiculo->codLinha = -1;
            }
            else {
                int codLinha = atoi(registerData);
                veiculo->codLinha = codLinha;
            }
        }
        else if (currField == 4) {
            if(strcmp("NULO", registerData) == 0) {    
                veiculo->tamanhoModelo = 0;    
            }
            else {                
                strcpy(veiculo->modelo, registerData);
                veiculo->tamanhoModelo = strlen(veiculo->modelo); // desconsidera '\0' ?
            }
        }
        else if (currField == 5) {
            if(strcmp("NULO", registerData) == 0) {
                veiculo->tamanhoCategoria = 0;    
            }
            else {
                strcpy(veiculo->categoria, registerData);
                veiculo->tamanhoCategoria = strlen(veiculo->categoria); // desconsidera '\0'
            }
            
        }
        
        //passa para o proximo campo
        currField++; 
    }
    
    //soma os tamanhos fixos, os variaveis e o numero de campos inteiros multiplicado por 4
    veiculo->tamanhoRegistro = 5 + 10 + 4*4 + veiculo->tamanhoModelo + veiculo->tamanhoCategoria;
    writeBinaryDataRegisterVeiculo(fw, veiculo);
    
    return veiculo;
}

Veiculo** readCsvFileVeiculo(FILE* fp, int* nVeiculos, CabecalhoVeiculo* cabecalhoVeiculo, FILE* fw){

    Veiculo** veiculos = NULL; //vai armazenar varias variaveis do tipo Veiculo*
    Veiculo* veiculo; //struct que armazena o registro
    *nVeiculos = 0; //contador
    
    char c = 0;
    
    do{
        //chama a funcao para ler um registro do csv por vez, salvando entao na struct Veiculo e incrementando a contagem
        veiculo = readCsvDataRegisterVeiculo(fp, fw, cabecalhoVeiculo);
        veiculos = (Veiculo** ) realloc(veiculos, sizeof(Veiculo* )*(*nVeiculos + 1));
        veiculos[*nVeiculos] = veiculo;        
        (*nVeiculos)++; 
        
    } while(veiculo != NULL); 

    return veiculos;    
    
}

/*** dados BINARIO ***/
Veiculo* readBinaryHeaderVeiculo(FILE* fp, CabecalhoVeiculo* cabecalhoVeiculo) {
    fread(&(cabecalhoVeiculo->byteProxReg), sizeof(long int), 1, fp);
    fread(&(cabecalhoVeiculo->nroRegistros), sizeof(int), 1, fp);
    //printf("%d\n", cabecalhoVeiculo->nroRegistros);
    fread(&(cabecalhoVeiculo->nroRegRemovidos), sizeof(int), 1, fp);
    fread(cabecalhoVeiculo->descrevePrefixo, sizeof(char), 18, fp);
    fread(cabecalhoVeiculo->descreveData, sizeof(char), 35, fp);
    fread(cabecalhoVeiculo->descreveLugares, sizeof(char), 42, fp);
    fread(cabecalhoVeiculo->descreveLinha, sizeof(char), 26, fp);
    fread(cabecalhoVeiculo->descreveModelo, sizeof(char), 17, fp);
    fread(cabecalhoVeiculo->descreveCategoria, sizeof(char), 20, fp);
}

void readBinaryDataRegisterVeiculo(FILE* fw, Veiculo* veiculo) {
    fread(&(veiculo->removido), sizeof(char), 1, fw); 
    fread(&(veiculo->tamanhoRegistro), sizeof(int), 1, fw);
    fread(veiculo->prefixo, sizeof(char), 5, fw); //writes name and the '\0' char
    fread(veiculo->data, sizeof(char), 10, fw); //writes name and the '\0' char
    fread(&(veiculo->quantidadeLugares), sizeof(int), 1, fw);
    fread(&(veiculo->codLinha), sizeof(int), 1, fw);

    //confere se o modelo tem tamanho maior que zero
    fread(&(veiculo->tamanhoModelo), sizeof(int), 1, fw);
    if(veiculo->tamanhoModelo > 0) {
        fread(veiculo->modelo, sizeof(char), veiculo->tamanhoModelo, fw); 
    }
    
    //confere se a categoria tem tamanho maior que zero
    fread(&(veiculo->tamanhoCategoria), sizeof(int), 1, fw);
    if(veiculo->tamanhoCategoria > 0) {
        fread(veiculo->categoria, sizeof(char), veiculo->tamanhoCategoria, fw);     
    }   

}

CabecalhoVeiculo* writeBinaryHeaderVeiculo(FILE* fw, CabecalhoVeiculo* cabecalhoVeiculo){    

    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw);
    fwrite(&(cabecalhoVeiculo->byteProxReg), sizeof(char), 8, fw);
    fwrite(&(cabecalhoVeiculo->nroRegistros), sizeof(int), 1, fw);
    fwrite(&(cabecalhoVeiculo->nroRegRemovidos), sizeof(int), 1, fw);
    fwrite(cabecalhoVeiculo->descrevePrefixo, sizeof(char), 18, fw);
    fwrite(cabecalhoVeiculo->descreveData, sizeof(char), 35, fw);
    fwrite(cabecalhoVeiculo->descreveLugares, sizeof(char), 42, fw);
    fwrite(cabecalhoVeiculo->descreveLinha, sizeof(char), 26, fw);
    fwrite(cabecalhoVeiculo->descreveModelo, sizeof(char), 17, fw);
    fwrite(cabecalhoVeiculo->descreveCategoria, sizeof(char), 20, fw);
}

Veiculo* writeBinaryDataRegisterVeiculo(FILE* fw, Veiculo* ponteiroVeiculo){    
    Veiculo* veiculo =(Veiculo* ) malloc(sizeof(Veiculo));
    veiculo = ponteiroVeiculo;
    fwrite(&(veiculo->removido), sizeof(char), 1, fw); 
    fwrite(&(veiculo->tamanhoRegistro), sizeof(int), 1, fw);
    fwrite(veiculo->prefixo, sizeof(char), 5, fw); 
    fwrite(veiculo->data, sizeof(char), 10, fw); 
    fwrite(&(veiculo->quantidadeLugares), sizeof(int), 1, fw);
    fwrite(&(veiculo->codLinha), sizeof(int), 1, fw);
    fwrite(&(veiculo->tamanhoModelo), sizeof(int), 1, fw);
    if(veiculo->tamanhoModelo > 0) {
        fwrite(veiculo->modelo, sizeof(char), veiculo->tamanhoModelo, fw); 
    }
    fwrite(&(veiculo->tamanhoCategoria), sizeof(int), 1, fw);
    if(veiculo->tamanhoCategoria > 0) {
        fwrite(veiculo->categoria, sizeof(char), veiculo->tamanhoCategoria, fw);     
    }
    

    return veiculo;
}

/*** dados de print/auxiliares ***/
void printBinaryDataRegisterVeiculo(CabecalhoVeiculo* cabecalhoVeiculo, Veiculo* veiculo) {
    
    //nessa funcao, optou-se por printar sempre o tamanho conhecido dos campos,
    //para garantir a formatação correta e evitar problemas por endereços indesejados

    printf("%.18s: %.5s\n", cabecalhoVeiculo->descrevePrefixo, veiculo->prefixo);
    
    if (veiculo->tamanhoModelo > 0) {
        printf("%.17s: %.*s\n",
            cabecalhoVeiculo->descreveModelo,
            veiculo->tamanhoModelo,
            veiculo->modelo
        );
    }
    else {
        printf("%.17s: campo com valor nulo\n", cabecalhoVeiculo->descreveModelo);
    }
    if (veiculo->tamanhoCategoria > 0) {
        printf("%.20s: %.*s\n",
            cabecalhoVeiculo->descreveCategoria,
            veiculo->tamanhoCategoria,
            veiculo->categoria
        );
    }
    else {
        printf("%.20s: campo com valor nulo\n", cabecalhoVeiculo->descreveCategoria);
    }
    char data[10] = "";
    strcpy(data, veiculo->data); 
    
    if( data[0] != '\0' ){
        printData(data, cabecalhoVeiculo->descreveData);
    }
    else {
        printf("%.35s: campo com valor nulo\n", cabecalhoVeiculo->descreveData);
    }
    
    printf("%.42s: %d\n", cabecalhoVeiculo->descreveLugares, veiculo->quantidadeLugares);

    printf("\n"); //pula linha conforme especificado
}

void printData(char *data, char* descreveData) {

    char limiter[2] = "-";
    
    char* ano = (char*) malloc(sizeof(char) * 4);
    char* mes = (char*) malloc(sizeof(char) * 2);
    char* dia = (char*) malloc(sizeof(char) * 2);

    ano = strtok(data, limiter);
    mes = strtok(NULL, limiter);
    dia = strtok(NULL, limiter);
    
    char* nomeDoMes = (char*) malloc(sizeof(char) * 12);
    converteNumeroEmNomeDoMes(mes, nomeDoMes);    
    
    printf("%.35s: %s de %s de %s\n",
            descreveData,
            dia,
            nomeDoMes,
            ano
    );
    
    free(nomeDoMes);
    
}

void converteNumeroEmNomeDoMes(char* mes, char* nomeDoMes) {
    int numeroMes = atoi(mes);
    switch(numeroMes) {
        case 1:
            strcpy(nomeDoMes,"janeiro");
            break;
        case 2:
            strcpy(nomeDoMes,"fevereiro");
            break;
        case 3:
            strcpy(nomeDoMes,"março");
            break;
        case 4:
            strcpy(nomeDoMes,"abril");
            break;
        case 5:
            strcpy(nomeDoMes,"maio");
            break;
        case 6:
            strcpy(nomeDoMes,"junho");
            break;
        case 7:
            strcpy(nomeDoMes,"julho");
            break;
        case 8:
            strcpy(nomeDoMes,"agosto");
            break;
        case 9:
            strcpy(nomeDoMes,"setembro");
            break;
        case 10:
            strcpy(nomeDoMes,"outubro");
            break;
        case 11:
            strcpy(nomeDoMes,"novembro");
            break;
        case 12:
            strcpy(nomeDoMes,"dezembro");
            break;
        
        default:
            printf("invalid Month number. \nPlease try again ....\n");
            break;
        }    
}

//libera memoria
void FreeVeiculos(Veiculo** Veiculo, int nVeiculos){
    for(int i = 0; i< nVeiculos; i++){
        free(Veiculo[i]);
    }
    free(Veiculo);
}

#endif