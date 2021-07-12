#ifndef _funcionalidades_

#define _funcionalidades_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "./converte-prefixo/convertePrefixo.h"


#include "./funcionalidadesTrabalho.h"
#include "./funcoesAux.h"
#include "./funcoes/linhas.h"
#include "./funcoes/veiculos.h"
#include "./funcoes/arvore-b/arvore-b.h"
#include "./funcoes/arvore-b/insercao.h"

//armazena registros contidos em csvFile no arquivo binaryFile
int leVeiculos() {
    char csvFile[100];
    char binaryFile[100];

    scanf("%[^ ]", csvFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^\n]", binaryFile); // le nome do arquivo até espaço
    
    //tenta abrir csv
    FILE* fp= fopen(csvFile, "r");
    if(fp == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //tenta abrir binario
    FILE* fw= fopen(binaryFile, "w+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //le cabecalho do csv
    CabecalhoVeiculo* cabecalhoVeiculo = (CabecalhoVeiculo* ) malloc(sizeof(CabecalhoVeiculo));
    cabecalhoVeiculo->status = '0';
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    fseek(fw, 175, SEEK_SET); // jump header veiculo
    readCsvHeaderVeiculo(fp, cabecalhoVeiculo);
    
    //le os registros do csv e conta numero de registros
    int nVeiculos = 0;
    Veiculo** veiculos = readCsvFileVeiculo(fp, &nVeiculos, cabecalhoVeiculo, fw);
    
    malloc(sizeof(veiculos));
    
    //indica erro se o arquivo estiver vazio
    if(veiculos == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }    
    

    //le a posicao final do arquivo e guarda no cabecalho
    fseek(fw, 0, SEEK_END);
    size_t fl;
    fl = ftell(fw);
    cabecalhoVeiculo->byteProxReg = fl;
    
    //escreve cabecalho atualizado no binario
    fseek(fw, 0, SEEK_SET);
    cabecalhoVeiculo->status = '1';
    cabecalhoVeiculo->nroRegistros = nVeiculos - cabecalhoVeiculo->nroRegRemovidos - 1;
    writeBinaryHeaderVeiculo(fw, cabecalhoVeiculo);

    fclose(fp);
    fclose(fw);
    
    FreeVeiculos(veiculos, nVeiculos);
    free(cabecalhoVeiculo);

    binarioNaTela(binaryFile);

    return EXIT_SUCCESS;
};

//armazena registros contidos em csvFile no arquivo binaryFile
int leLinhas() {
    char csvFile[100];
    char binaryFile[100];

    scanf("%[^ ]", csvFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^\n]", binaryFile); // le nome do arquivo até espaço
    
    
    //tenta abrir csv
    FILE* fp= fopen(csvFile, "r");
    if(fp == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //tenta abrir binario
    FILE* fw= fopen(binaryFile, "w+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //le cabecalho do csv
    CabecalhoLinha* cabecalhoLinha = (CabecalhoLinha* ) malloc(sizeof(CabecalhoLinha));
    cabecalhoLinha->status = '0';
    fwrite(cabecalhoLinha, sizeof(char), 1, fw); //status byte
    fseek(fw, 82, SEEK_SET); // jump header linha
    readCsvHeaderLinha(fp, cabecalhoLinha);
    
    //le os registros do csv e conta numero de registros
    int nLinhas = 0;
    Linha** linhas = readCsvFileLinha(fp, &nLinhas, cabecalhoLinha, fw);
    
    malloc(sizeof(linhas));
    
    //indica erro se o arquivo estiver vazio
    if(linhas == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //le a posicao final do arquivo e guarda no cabecalho
    fseek(fw, 0, SEEK_END);
    size_t fl;
    fl = ftell(fw);
    cabecalhoLinha->byteProxReg = fl;

    //escreve cabecalho atualizado no binario
    fseek(fw, 0, SEEK_SET);
    cabecalhoLinha->status = '1';
    cabecalhoLinha->nroRegistros = nLinhas - cabecalhoLinha->nroRegRemovidos - 1;
    writeBinaryHeaderLinha(fw, cabecalhoLinha);
    
    fclose(fp);
    fclose(fw);
    
    FreeLinhas(linhas, nLinhas);
    free(cabecalhoLinha);

    binarioNaTela(binaryFile);

    return EXIT_SUCCESS;
};

//lista todos os registros não removidos no arquivo binaryFile
int listaVeiculos() {
    char binaryFile[100];
    scanf("%[^\n]", binaryFile); // le nome do arquivo até espaço
    
    //tenta abrir o binario
    FILE* fw= fopen(binaryFile, "r+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //verifica se o arquivo está consistente
    CabecalhoVeiculo* cabecalhoVeiculo = (CabecalhoVeiculo* ) malloc(sizeof(CabecalhoVeiculo));
    fscanf(fw, "%c", &cabecalhoVeiculo->status);
    if(cabecalhoVeiculo->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //escreve status 0
    fseek(fw, 0, SEEK_SET);
    cabecalhoVeiculo->status = '0';
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    
    //le o cabecalho
    readBinaryHeaderVeiculo(fw, cabecalhoVeiculo); 
    
    //se o arquivo nao estiver vazio
    if(cabecalhoVeiculo->nroRegistros > 0) {
        
        //loop no numero total de registros
        int totalRegistros = cabecalhoVeiculo->nroRegistros + cabecalhoVeiculo->nroRegRemovidos;
        for(int cont = 0; cont < totalRegistros; cont++) {
            
            //le o registro atual
            Veiculo* veiculo =(Veiculo* ) malloc(sizeof(Veiculo));
            readBinaryDataRegisterVeiculo(fw, veiculo);
            
            //printa se nao for removido
            if(veiculo->removido == '1'){
                printBinaryDataRegisterVeiculo(cabecalhoVeiculo, veiculo);
            }
            free(veiculo);
        }
    }
    else {
        printf("Registro inexistente.");
    }
    //escreve cabecalho atualizado
    cabecalhoVeiculo->status = '1';
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    free(cabecalhoVeiculo);
    fclose(fw);

    return EXIT_SUCCESS;
}

//lista todos os registros não removidos no arquivo binaryFile
int listalinhas(){
    char binaryFile[100];
    scanf("%[^\n]", binaryFile); // le nome do arquivo até espaço

    //tenta abrir o binario
    FILE* fw= fopen(binaryFile, "r+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //verifica se o arquivo está consistente
    CabecalhoLinha* cabecalhoLinha = (CabecalhoLinha* ) malloc(sizeof(CabecalhoLinha));
    
    fscanf(fw, "%c", &cabecalhoLinha->status);
    if(cabecalhoLinha->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    //escreve status 0
    fseek(fw, 0, SEEK_SET);
    cabecalhoLinha->status = '0';
    fwrite(cabecalhoLinha, sizeof(char), 1, fw); //status byte
    
    //le o cabecalho
    readBinaryHeaderLinha(fw, cabecalhoLinha); 

    //se o arquivo nao estiver vazio
    if(cabecalhoLinha->nroRegistros > 0) {
        //loop no numero total de registros
        int totalRegistros = cabecalhoLinha->nroRegistros + cabecalhoLinha->nroRegRemovidos;
        for(int cont = 0; cont < totalRegistros; cont++) {
            
            //le o registro atual
            Linha* linha =(Linha* ) malloc(sizeof(Linha));
            readBinaryDataRegisterLinha(fw, linha);
            
            //printa se nao for removido
            if(linha->removido == '1'){
                printBinaryDataRegisterLinha(cabecalhoLinha, linha);
            }

            free(linha);
        }
    }
    else {
        printf("Registro inexistente.");
    }
    //escreve cabecalho atualizado
    cabecalhoLinha->status = '1';
    fwrite(cabecalhoLinha, sizeof(char), 1, fw); //status byte
    free(cabecalhoLinha);
    fclose(fw);
    return EXIT_SUCCESS;
}

// busca por campos do tipo nomeDoCampo com conteudo 'valor' no arquivo binaryFile
int buscaVeiculo() {
    char binaryFile[100];
    char nomeDoCampo [20];
    char valor [30];
    int valorNum = 0;
    
    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^ ]", nomeDoCampo); //le nome do campo
    scanf("%*c"); // le espaco
    
    if ( 
        strcmp(nomeDoCampo, "quantidadeLugares") == 0 ||
        strcmp(nomeDoCampo, "codLinha") == 0
    ){
            
        scanf("%d", &valorNum); //le valor de acordo com tipo
    }
    else if (
        strcmp(nomeDoCampo, "prefixo") == 0 ||
        strcmp(nomeDoCampo, "data") == 0 ||
        strcmp(nomeDoCampo, "modelo") == 0 ||
        strcmp(nomeDoCampo, "categoria") == 0
    ) {
        scan_quote_string(valor); //le valor de acordo com tipo
    }
    
    //tenta abrir o binario
    FILE* fw= fopen(binaryFile, "r+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //verifica se o arquivo está consistente
    CabecalhoVeiculo* cabecalhoVeiculo = (CabecalhoVeiculo* ) malloc(sizeof(CabecalhoVeiculo));
    fscanf(fw, "%c", &cabecalhoVeiculo->status);
    if(cabecalhoVeiculo->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //escreve status 0
    fseek(fw, 0, SEEK_SET);
    cabecalhoVeiculo->status = '0';
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    
    //le o cabecalho
    readBinaryHeaderVeiculo(fw, cabecalhoVeiculo);

    //checa se o arquivo contém registros
    if(cabecalhoVeiculo->nroRegistros > 0) {
        
        //loop dos registros
        int totalRegistros = cabecalhoVeiculo->nroRegistros + cabecalhoVeiculo->nroRegRemovidos;
        for(int cont = 0; cont < totalRegistros; cont++) {
            
            //le registro
            Veiculo* veiculo =(Veiculo* ) malloc(sizeof(Veiculo));
            readBinaryDataRegisterVeiculo(fw, veiculo);
            
            //checa se é removido
            if(veiculo->removido == '1' ){
                //considera inicialmente igual e para qualquer diferença que ocorra, considera diferente
                int equals = 1;
                
                //verifica apenas o campo de interesse e compara-se o valor desejado com o valor do registro
                if(strcmp(nomeDoCampo, "prefixo") == 0) {
                    for(int cont = 0; cont < 5; cont++) {
                        if(valor[cont] != veiculo->prefixo[cont]){
                            equals = 0;
                        }
                    }
                }
                else if(strcmp(nomeDoCampo, "data") == 0) {
                    for(int cont = 0; cont < 10; cont++) {
                        if(valor[cont] != veiculo->data[cont]){
                            equals = 0;
                        }
                    }
                }
                else if(strcmp(nomeDoCampo, "quantidadeLugares") == 0) {
                    int qtde = valorNum; 
                    if(qtde != veiculo->quantidadeLugares){
                        equals = 0;
                    }
                }
                else if(strcmp(nomeDoCampo, "modelo") == 0) {
                    if(veiculo->tamanhoModelo > 0){
                        for(int cont = 0; cont < veiculo->tamanhoModelo; cont++) {
                            if(valor[cont] != veiculo->modelo[cont]){
                                equals = 0;
                            }
                        }
                    }
                }
                else if(strcmp(nomeDoCampo, "categoria") == 0) {
                    if(veiculo->tamanhoCategoria > 0) {
                        for(int cont = 0; cont < veiculo->tamanhoCategoria; cont++) {
                            if(valor[cont] != veiculo->categoria[cont]){
                                equals = 0;
                            }
                        }
                    }
                }
                //printa se for igual realmente
                if(equals == 1) {
                    printBinaryDataRegisterVeiculo(cabecalhoVeiculo, veiculo);
                }
            }
            free(veiculo);
        }
        
    }
    else {
        printf("Registro inexistente.");
    }
    //escreve cabecalho atualizado
    cabecalhoVeiculo->status = '1';
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    free(cabecalhoVeiculo);
    fclose(fw);

    return EXIT_SUCCESS;
}

// busca por campos do tipo nomeDoCampo com conteudo 'valor' no arquivo binaryFile
int buscaLinha() {
    char binaryFile[100];
    char nomeDoCampo [20];
    char valor [30];
    int valorNum = 0;
    
    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^ ]", nomeDoCampo); //le nome do campo
    scanf("%*c"); // le espaco
    
    //le inteiro ou valor de string, dependendo do campo
    if ( strcmp(nomeDoCampo, "codLinha") == 0 ){
        scanf("%d", &valorNum); //le valor de acordo com tipo
    }
    else if (
        strcmp(nomeDoCampo, "aceitaCartao") == 0 ||
        strcmp(nomeDoCampo, "nomeLinha") == 0 ||
        strcmp(nomeDoCampo, "corLinha") == 0
    ) {
        scan_quote_string(valor); //le valor de acordo com tipo
    }

    //tenta abrir o binario
    FILE* fw= fopen(binaryFile, "r+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
     
    //verifica se o arquivo está consistente
    CabecalhoLinha* cabecalhoLinha = (CabecalhoLinha* ) malloc(sizeof(CabecalhoLinha));
    
    fscanf(fw, "%c", &cabecalhoLinha->status);
    if(cabecalhoLinha->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    //escreve status 0
    fseek(fw, 0, SEEK_SET);
    cabecalhoLinha->status = '0';
    fwrite(cabecalhoLinha, sizeof(char), 1, fw); //status byte
    
    //le o cabecalho
    readBinaryHeaderLinha(fw, cabecalhoLinha); 

    //checa se o arquivo contém registros
    if(cabecalhoLinha->nroRegistros > 0) {
        
        //loop dos registros
        int totalRegistros = cabecalhoLinha->nroRegistros + cabecalhoLinha->nroRegRemovidos;
        for(int cont = 0; cont < totalRegistros; cont++) {
            //le registro
            Linha* linha =(Linha* ) malloc(sizeof(Linha));
            readBinaryDataRegisterLinha(fw, linha);

            //checa se é removido
            if(linha->removido == '1' ){
                //considera inicialmente igual e para qualquer diferença que ocorra, considera diferente
                int equals = 1;

                //verifica apenas o campo de interesse e compara-se o valor desejado com o valor do registro
                if(strcmp(nomeDoCampo, "codLinha") == 0) {
                    int qtde = valorNum; 
                    if(qtde != linha->codLinha){
                        equals = 0;
                    }
                }
                else if(strcmp(nomeDoCampo, "aceitaCartao") == 0) {
                    if(valor[0] != linha->aceitaCartao){
                        equals = 0;
                    }
                }
                else if(strcmp(nomeDoCampo, "nomeLinha") == 0) {
                    if(linha->tamanhoNome > 0){
                        for(int cont = 0; cont < linha->tamanhoNome; cont++) {
                            if(valor[cont] != linha->nomeLinha[cont]){
                                equals = 0;
                            }
                        }
                    }
                }
                else if(strcmp(nomeDoCampo, "corLinha") == 0) {
                    if(linha->tamanhoCor > 0) {
                        for(int cont = 0; cont < linha->tamanhoCor; cont++) {
                            if(valor[cont] != linha->corLinha[cont]){
                                equals = 0;
                            }
                        }
                    }
                }
                //printa se for igual realmente
                if(equals == 1) {
                    printBinaryDataRegisterLinha(cabecalhoLinha, linha);
                }
            }
            free(linha);
        }
    }
    else {
        printf("Registro inexistente.");
    }
    //escreve cabecalho atualizado
    cabecalhoLinha->status = '1';
    fwrite(cabecalhoLinha, sizeof(char), 1, fw); //status byte
    free(cabecalhoLinha);
    fclose(fw);

    return EXIT_SUCCESS;
}

// insere o numero de registros nVeiculos no arquivo binaryFile com as entradas do usuario
int insereVeiculos() {
    char binaryFile[100];
    int nEntradas = 0;
    
    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%d", &nEntradas); //le nome do campo
    
    //tenta abrir o binario
    FILE* fw= fopen(binaryFile, "r+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //le o cabecalho
    CabecalhoVeiculo* cabecalhoVeiculo = (CabecalhoVeiculo* ) malloc(sizeof(CabecalhoVeiculo));
    fscanf(fw, "%c", &cabecalhoVeiculo->status);
    if(cabecalhoVeiculo->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    fseek(fw, 0, SEEK_SET);
    cabecalhoVeiculo->status = '0';
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    readBinaryHeaderVeiculo(fw, cabecalhoVeiculo);
    
    //insere o numero desejado de entradas na proxima posicao
    fseek(fw, cabecalhoVeiculo->byteProxReg, SEEK_SET);

    for(int cont = 0; cont < nEntradas; cont++) {
        Veiculo* veiculo =(Veiculo* ) malloc(sizeof(Veiculo));

        //entrada do usuario
        scan_quote_string(veiculo->prefixo);
        scanf("%*c"); // le espaco  
        scan_quote_string(veiculo->data);
        scanf("%*c"); // le espaco  
        scanf("%d", &veiculo->quantidadeLugares); //le nome do campo
        scanf("%*c"); // le espaco  
        scanf("%d", &veiculo->codLinha); //le nome do campo
        scanf("%*c"); // le espaco  
        scan_quote_string(veiculo->modelo);
        scanf("%*c"); // le espaco  
        scan_quote_string(veiculo->categoria);


        //escreve lixo se o campo for nulo
        if(veiculo->data[0] == '\0') {
            memset(veiculo->data, '@', 10);
            veiculo->data[0] = '\0';
        }
        
        //guarda o tamanho dos campos variaveis
        veiculo->tamanhoModelo = strlen(veiculo->modelo);
        veiculo->tamanhoCategoria = strlen(veiculo->categoria); 
        
        //calcula tamanho do registro
        veiculo->tamanhoRegistro = 5 + 10 + 4*4 + veiculo->tamanhoModelo + veiculo->tamanhoCategoria;        
        veiculo->removido = '1';
        
        //escreve no arquivo
        writeBinaryDataRegisterVeiculo(fw, veiculo);
        
        free(veiculo);
    }
    
    //atualiza o cabecalho
    cabecalhoVeiculo->byteProxReg = ftell(fw);
    cabecalhoVeiculo->nroRegistros = cabecalhoVeiculo->nroRegistros + nEntradas;
    cabecalhoVeiculo->status = '1';
    fseek(fw, 0, SEEK_SET);
    
    //escreve cabecalho atualizado
    writeBinaryHeaderVeiculo(fw, cabecalhoVeiculo);
    
    free(cabecalhoVeiculo);
    fclose(fw);

    binarioNaTela(binaryFile);
    
    return EXIT_SUCCESS;
}

// insere o numero de registros nLinhas no arquivo binaryFile com as entradas do usuario
int insereLinhas() {

    char binaryFile[100];
    int nEntradas = 0;
    
    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%d", &nEntradas); //le nome do campo

    //tenta abrir o binario
    FILE* fw= fopen(binaryFile, "r+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    //le o cabecalho
    CabecalhoLinha* cabecalhoLinha = (CabecalhoLinha* ) malloc(sizeof(CabecalhoLinha));
    
    //verifica se o arquivo está consistente (status 1)
    fscanf(fw, "%c", &cabecalhoLinha->status);
    if(cabecalhoLinha->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //escreve status 0
    fseek(fw, 0, SEEK_SET);
    cabecalhoLinha->status = '0';
    fwrite(cabecalhoLinha, sizeof(char), 1, fw); //status byte
    readBinaryHeaderLinha(fw, cabecalhoLinha);
    
    //insere o numero desejado de entradas na proxima posicao
    fseek(fw, cabecalhoLinha->byteProxReg, SEEK_SET);

    for(int cont = 0; cont < nEntradas; cont++) {

        Linha* linha =(Linha* ) malloc(sizeof(Linha));
        
        //entrada do usuario
        scanf("%d", &linha->codLinha); //le nome do campo
        scanf("%*c"); // le espaco  
        scan_quote_string(&linha->aceitaCartao);
        scanf("%*c"); // le espaco  
        scan_quote_string(linha->nomeLinha);
        scanf("%*c"); // le espaco  
        scan_quote_string(linha->corLinha);

        //guarda o tamanho dos campos variaveis
        linha->tamanhoNome = strlen(linha->nomeLinha); 
        linha->tamanhoCor = strlen(linha->corLinha); 

        linha->tamanhoRegistro = 1 + 3*4 + linha->tamanhoNome + linha->tamanhoCor;        
        linha->removido = '1';
        
        //escreve no arquivo
        writeBinaryDataRegisterLinha(fw, linha);

        free(linha);
    }
    
    //atualiza o cabecalho
    cabecalhoLinha->byteProxReg = ftell(fw);
    cabecalhoLinha->nroRegistros = cabecalhoLinha->nroRegistros + nEntradas;
    cabecalhoLinha->status = '1';
    fseek(fw, 0, SEEK_SET);
    
    //escreve cabecalho atualizado
    writeBinaryHeaderLinha(fw, cabecalhoLinha);
    
    free(cabecalhoLinha);
    fclose(fw);

    binarioNaTela(binaryFile);
    
    return EXIT_SUCCESS;
}


int criaArvoreVeiculos() {

    /*
    //le arquivos da entrada do usuário
    char binaryFile[100];
    char treeFile[100];
    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^\n]", treeFile); // le nome do arquivo até fim da linha
    */

    char binaryFile[100] = "veiculo1.bin";
    char treeFile[100] = "indicePrefixo1.bin";
    printf("funcionalidade\n");
    //tenta abrir o binario
    FILE* fw= fopen(binaryFile, "r+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    

    //verifica se o arquivo de veículos está consistente
    CabecalhoVeiculo* cabecalhoVeiculo = (CabecalhoVeiculo* ) malloc(sizeof(CabecalhoVeiculo));
    

    //escreve status 0
    // fseek(fw, 0, SEEK_SET);
    fseek(fw, 1, SEEK_SET);
    //cabecalhoVeiculo->status = '0';
    // cabecalhoVeiculo->status = '1';
    // fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    
    
    //cria o arquivo da árvore b
    FILE* fb= fopen(treeFile, "w+");    
    if(fb == NULL){
        printf("Falha no processamento do arquivoB.\n");
        return EXIT_FAILURE;
    }
    //inicializa cabecalho do arquivo da árvore-b
    CabecalhoArvore* cabecalhoArvore = (CabecalhoArvore*) malloc(sizeof(CabecalhoArvore));
    
    //valores iniciais
    cabecalhoArvore->status ='0';
    cabecalhoArvore->noRaiz = -1;
    cabecalhoArvore->RRNproxNo = 0;
    memset(cabecalhoArvore->lixo, '@', 68); //preenche com lixo
    writeHeaderTree(fb, cabecalhoArvore);

    //le o cabecalho inteiro do arquivo de dados
    readBinaryHeaderVeiculo(fw, cabecalhoVeiculo); 
    printf("leu cabeçalho\n");
    
    

    //se o arquivo nao estiver vazio
    if(cabecalhoVeiculo->nroRegistros > 0) {
        cabecalhoArvore->noRaiz = 0;

        //cria o nó raiz zerado, como folha
        NoArvore* noRaiz = (NoArvore*) malloc(sizeof(NoArvore));
        inicializaNoArvore(noRaiz, '1', cabecalhoArvore);
        noRaiz->RRNdoNo = cabecalhoArvore->noRaiz;
        cabecalhoArvore->RRNproxNo = 1;
        //escreve o nó raiz na árvore
        writeNoArvore(fb, noRaiz, cabecalhoArvore);

        //loop no numero total de registros
        int totalRegistros = cabecalhoVeiculo->nroRegistros;
        for(int cont = 0; cont < totalRegistros; cont++) {
         
        // for(int cont = 0; cont < 6; cont++) {
            printf("\n--------");
            printf("\nIteração: %d\n", cont);
            printf("Nó Raiz: %d\n", noRaiz->RRNdoNo);
            //le o registro atual
            Veiculo* veiculo = (Veiculo* ) malloc(sizeof(Veiculo));
            readBinaryDataRegisterVeiculo(fw, veiculo);
            
            // se não foi removido, insere na árvore
            if(veiculo->removido == '1'){
                
                //converte o prefixo para inteiro e lê byteoffset
                int prefixoConvertido = convertePrefixo(veiculo->prefixo);
                int byteOffsetVeiculo = ftell(fw);
                
                //cria chave com prefixo e byteOffset. inicialmente vai tentar inserir no nó raiz
                Chave* chaveAInserir = malloc(sizeof(Chave));
                
                chaveAInserir->P = -1;
                chaveAInserir->C = prefixoConvertido;
                chaveAInserir->Pr = byteOffsetVeiculo;
                
                //printf("%d\n", chaveAInserir->C);
                
                //insere na arvore e atualiza cabecalhoArvore
                insereVeiculoEmArvoreNova(
                    fb,
                    cabecalhoArvore,
                    noRaiz,
                    chaveAInserir);

                free(chaveAInserir);
            }

            free(veiculo);
        }
        
        //escreve cabecalho da arvore no arquivo
        fseek(fb, 0, SEEK_SET);
        cabecalhoArvore->status='1';
        writeHeaderTree(fb, cabecalhoArvore);

        free(noRaiz);
        free(cabecalhoArvore);
        printf("liberou cabeçalho");
        
        binarioNaTela(treeFile);
        fclose(fb);
        printf("liberou cabeçalho2");
    }
    else {
        printf("não é maior que zero\n");
        printf("Falha no processamento do arquivo.\n");
    }

    /*
    //atualiza status 1
    fseek(fw, 0, SEEK_SET);
    cabecalhoVeiculo->status = '1';

    //escreve cabecalho atualizado
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    */
    printf("%d %d %d", cabecalhoVeiculo->nroRegistros, cabecalhoVeiculo->status, cabecalhoVeiculo->nroRegRemovidos);
    free(cabecalhoVeiculo);

    fclose(fw);
    printf("fechou");

    return EXIT_SUCCESS;
}


int criaArvoreLinhas() {
    return 0;
}
int buscaArvoreVeiculos() {
    return 0;
}
int buscaArvoreLinhas() {
    return 0;
}
int insereVeiculosArvore() {

    char binaryFile[100];
    char treeFile[100];
    int nEntradas = 0;
    
    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^ ]", treeFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%d", &nEntradas); //le numero de entradas a serem inseridas
    
    //tenta abrir arquivo de dados
    FILE* fw= fopen(binaryFile, "rb+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo1.\n");
        return EXIT_FAILURE;
    }

    //abre o arquivo da árvore b
    FILE* fb= fopen(treeFile, "rb+");    
    if(fb == NULL){
        printf("Falha no processamento do arquivo2.\n");
        return EXIT_FAILURE;
    }

    //lê cabecalho do arquivo da árvore-b
    CabecalhoArvore* cabecalhoArvore = (CabecalhoArvore*) malloc(sizeof(CabecalhoArvore));
    readHeaderTree(fb, cabecalhoArvore);

    //le o cabecalho inteiro do arquivo de dados
    CabecalhoVeiculo* cabecalhoVeiculo = (CabecalhoVeiculo* ) malloc(sizeof(CabecalhoVeiculo));
    readBinaryHeaderVeiculo(fw, cabecalhoVeiculo); 
    //escreve status 0
    // fseek(fw, 0, SEEK_SET);
    fseek(fw, 1, SEEK_SET);
    //cabecalhoVeiculo->status = '0';
    // cabecalhoVeiculo->status = '1';
    // fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte

    
    // for(int cont = 0; cont < nEntradas; cont++) {
    for(int cont = 0; cont < 1; cont++) {
        
        Veiculo* veiculo =(Veiculo* ) malloc(sizeof(Veiculo));
        
        //processa entrada do usuario
        leRegistroASerInserido(veiculo);
        
        //cria chave com prefixo e byteOffset. inicialmente vai tentar inserir no nó raiz
        Chave* chaveAInserir = malloc(sizeof(Chave));
        
        //converte o prefixo para inteiro e lê byteoffset
        chaveAInserir->C = convertePrefixo(veiculo->prefixo);
        chaveAInserir->Pr = cabecalhoVeiculo->byteProxReg;
        
        chaveAInserir->P = -1;

        //insere na árvore B
        insereVeiculoEmArvoreExistente(fb, cabecalhoArvore, chaveAInserir);

        //posiciona cursor na proxima posicao disponível e escreve
        fseek(fw, chaveAInserir->Pr, SEEK_SET);
        printf("não está escrevendo no arquivo de dados\n");
        // writeBinaryDataRegisterVeiculo(fw, veiculo);
         
        printf("\n--------");
        printf("\nIteração: %d\n", cont);

        free(chaveAInserir);
        free(veiculo);

    }
    
        
    //escreve cabecalho da arvore no arquivo
    fseek(fb, 0, SEEK_SET);
    cabecalhoArvore->status='1';
    printf("não atualiza cabeçalho árvore");
    //writeHeaderTree(fb, cabecalhoArvore);
    
    free(cabecalhoArvore);
    printf("liberou cabeçalho");
    
    binarioNaTela(treeFile);
    fclose(fb);
    printf("liberou cabeçalho2");

    /*
    //atualiza status 1
    fseek(fw, 0, SEEK_SET);
    cabecalhoVeiculo->status = '1';

    //escreve cabecalho atualizado
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    */
    free(cabecalhoVeiculo);

    fclose(fw);

    return 0;
    
}


int insereLinhasArvore() {
    return 0;
}

#endif