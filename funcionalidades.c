#ifndef _funcionalidades_

#define _funcionalidades_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "./funcionalidades.h"
#include "./funcoesAux.h"
#include "./dados/linhas.h"
#include "./dados/veiculos.h"
#include "./arvore-b/arvore-b.h"
#include "./arvore-b/insercao.h"
#include "./arvore-b/busca.h"

int criaArvoreVeiculos() {
    
    //le arquivos da entrada do usuário
    char binaryFile[100];
    char treeFile[100];

    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^\n]", treeFile); // le nome do arquivo até fim da linha
        
    //tenta abrir arquivo de dados
    FILE* fw= fopen(binaryFile, "rb+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //cria o arquivo da árvore b
    FILE* fb= fopen(treeFile, "w+");    
    if(fb == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    
    //verifica consistência do arquivo de dados
    CabecalhoVeiculo* cabecalhoVeiculo = (CabecalhoVeiculo* ) malloc(sizeof(CabecalhoVeiculo));
    fread(cabecalhoVeiculo, sizeof(char), 1, fw);
    if( cabecalhoVeiculo->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    // atualiza status e lê cabecalho do arquivo de dados
    cabecalhoVeiculo->status = '0';
    fseek(fw, 0, SEEK_SET);
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw);
    readBinaryHeaderVeiculo(fw, cabecalhoVeiculo); 

    //atualiza status e inicializa cabecalho do arquivo da árvore-b
    CabecalhoArvore* cabecalhoArvore = (CabecalhoArvore*) malloc(sizeof(CabecalhoArvore));
    cabecalhoArvore->status = '0';
    fwrite(cabecalhoArvore, sizeof(char), 1, fb);
    cabecalhoArvore->noRaiz = -1;
    cabecalhoArvore->RRNproxNo = 0;
    memset(cabecalhoArvore->lixo, '@', 68); //preenche com lixo
    fseek(fb, 0, SEEK_SET);
    writeHeaderTree(fb, cabecalhoArvore);

    //cria o nó raiz zerado, como folha
    NoArvore* noRaiz = (NoArvore*) malloc(sizeof(NoArvore));
    inicializaNoArvore(noRaiz, '1', cabecalhoArvore);
    cabecalhoArvore->RRNproxNo = 1;
    cabecalhoArvore->noRaiz = 0;
    noRaiz->RRNdoNo = cabecalhoArvore->noRaiz;
    //escreve o nó raiz na árvore
    writeNoArvore(fb, noRaiz, cabecalhoArvore);

    free(noRaiz);

    //se o arquivo nao estiver vazio, constrói árvore
    if(cabecalhoVeiculo->nroRegistros > 0) {
       
        //loop no numero total de registros
        int totalRegistros = cabecalhoVeiculo->nroRegistros + cabecalhoVeiculo->nroRegRemovidos;
        
        for(int cont = 0; cont < totalRegistros; cont++) {
         
            //le posicao do cursor antes da leitura do registro
            int byteOffset = ftell(fw);

            //le o registro atual
            Veiculo* veiculo = (Veiculo* ) malloc(sizeof(Veiculo));
            readBinaryDataRegisterVeiculo(fw, veiculo);
            
            // se não foi removido, insere na árvore
            if(veiculo->removido == '1'){
                
                //cria chave com prefixo e byteOffset. inicialmente vai tentar inserir no nó raiz
                Chave* chave = malloc(sizeof(Chave));
                
                //converte o prefixo para inteiro e lê byteoffset
                chave->C = convertePrefixo(veiculo->prefixo);
                chave->Pr = byteOffset;
            
                chave->P = -1;
                
                //começa busca pelo nó raiz
                int* RRNNoBusca = (int*) malloc(sizeof(int));
                *RRNNoBusca = cabecalhoArvore->noRaiz;
                
                //insere na arvore e atualiza cabecalhoArvore
                int novaRaiz = insercao(
                    fb,
                    cabecalhoArvore,
                    RRNNoBusca,
                    chave
                );

                //se for necessário, cria uma nova raiz
                if (novaRaiz == 1) {
                    criaNovaRaiz(fb, cabecalhoArvore, chave);
                }
                
                //escreve cabeçalho atualizado no arquivo
                fseek(fb, 0, SEEK_SET);
                writeHeaderTree(fb, cabecalhoArvore);
                
                free(RRNNoBusca);
                free(chave);
            }

            free(veiculo);
        }
        
        //escreve cabecalho da arvore no arquivo
        fseek(fb, 0, SEEK_SET);
        cabecalhoArvore->status='1';
        writeHeaderTree(fb, cabecalhoArvore);

        free(cabecalhoArvore);
        
        fclose(fb);
        binarioNaTela(treeFile);
        
    }
    else {
        printf("Falha no processamento do arquivo.\n");

    }

    free(cabecalhoVeiculo);

    fclose(fw);    

    return EXIT_SUCCESS;
}
int criaArvoreLinhas() {
    //le arquivos da entrada do usuário
    char binaryFile[100];
    char treeFile[100];

    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^\n]", treeFile); // le nome do arquivo até fim da linha
        
    //tenta abrir arquivo de dados
    FILE* fw = fopen(binaryFile, "rb+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //cria o arquivo da árvore b
    FILE* fb = fopen(treeFile, "w+");    
    if(fb == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //verifica consistência do arquivo de dados
    CabecalhoLinha* cabecalhoLinha = (CabecalhoLinha* ) malloc(sizeof(CabecalhoLinha));
    fread(cabecalhoLinha, sizeof(char), 1, fw);
    if( cabecalhoLinha->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    

    // atualiza status e lê cabecalho do arquivo de dados
    cabecalhoLinha->status = '0';
    fseek(fw, 0, SEEK_SET);
    fwrite(cabecalhoLinha, sizeof(char), 1, fw);
    readBinaryHeaderLinha(fw, cabecalhoLinha); 

    //atualiza status e inicializa cabecalho do arquivo da árvore-b
    CabecalhoArvore* cabecalhoArvore = (CabecalhoArvore*) malloc(sizeof(CabecalhoArvore));
    cabecalhoArvore->status = '0';
    fwrite(cabecalhoArvore, sizeof(char), 1, fb);
    cabecalhoArvore->noRaiz = -1;
    cabecalhoArvore->RRNproxNo = 0;
    memset(cabecalhoArvore->lixo, '@', 68); //preenche com lixo
    fseek(fb, 0, SEEK_SET);
    writeHeaderTree(fb, cabecalhoArvore);

    //cria o nó raiz zerado, como folha
    NoArvore* noRaiz = (NoArvore*) malloc(sizeof(NoArvore));
    inicializaNoArvore(noRaiz, '1', cabecalhoArvore);
    cabecalhoArvore->RRNproxNo = 1;
    cabecalhoArvore->noRaiz = 0;
    noRaiz->RRNdoNo = cabecalhoArvore->noRaiz;
    //escreve o nó raiz na árvore
    writeNoArvore(fb, noRaiz, cabecalhoArvore);

    free(noRaiz);

    //se o arquivo nao estiver vazio, constrói árvore
    if(cabecalhoLinha->nroRegistros > 0) {
       
        //loop no numero total de registros
        int totalRegistros = cabecalhoLinha->nroRegistros + cabecalhoLinha->nroRegRemovidos;
        
        for(int cont = 0; cont < totalRegistros; cont++) {

            //le posicao do cursor antes da leitura do registro
            int byteOffset = ftell(fw);

            //le o registro atual
            Linha* linha = (Linha* ) malloc(sizeof(Linha));
            readBinaryDataRegisterLinha(fw, linha);
            
            // se não foi removido, insere na árvore
            if(linha->removido == '1'){
                
                //cria chave com prefixo e byteOffset. inicialmente vai tentar inserir no nó raiz
                Chave* chave = malloc(sizeof(Chave));
                
                //converte o prefixo para inteiro e lê byteoffset
                chave->C = linha->codLinha;
                chave->Pr = byteOffset;
            
                chave->P = -1;
                //começa busca pelo nó raiz
                int* RRNNoBusca = (int*) malloc(sizeof(int));
                *RRNNoBusca = cabecalhoArvore->noRaiz;
                
                //insere na arvore e atualiza cabecalhoArvore
                int novaRaiz = insercao(
                    fb,
                    cabecalhoArvore,
                    RRNNoBusca,
                    chave
                );

                //se for necessário, cria uma nova raiz
                if (novaRaiz == 1) {
                    criaNovaRaiz(fb, cabecalhoArvore, chave);
                }
                
                //escreve cabeçalho atualizado no arquivo
                fseek(fb, 0, SEEK_SET);
                writeHeaderTree(fb, cabecalhoArvore);

                free(RRNNoBusca);
                free(chave);
            }

            free(linha);
        }
        
        //escreve cabecalho da arvore no arquivo
        fseek(fb, 0, SEEK_SET);
        cabecalhoArvore->status='1';
        writeHeaderTree(fb, cabecalhoArvore);

        free(cabecalhoArvore);
        
        fclose(fb);
        binarioNaTela(treeFile);
        
    }
    else {
        printf("Falha no processamento do arquivo.\n");
    }

    free(cabecalhoLinha);

    fclose(fw);    

    return EXIT_SUCCESS;
}
int buscaArvoreVeiculos() {
    
    char binaryFile[100];
    char treeFile[100];
    int nEntradas = 0;
    char prefixoDeBusca [30];
    
    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^ ]", treeFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%*[^ ]"); // le palavra "prefixo"
    scanf("%*c"); // le espaco
    scan_quote_string(prefixoDeBusca); //le valor de acordo com tipo
    
    //tenta abrir arquivo de dados
    FILE* fw= fopen(binaryFile, "rb+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //abre o arquivo da árvore b
    FILE* fb= fopen(treeFile, "rb+");    
    if(fb == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //atualiza status e lê cabecalho do arquivo da árvore-b
    CabecalhoArvore* cabecalhoArvore = (CabecalhoArvore*) malloc(sizeof(CabecalhoArvore));
    cabecalhoArvore->status = '0';
    fwrite(cabecalhoArvore, sizeof(char), 1, fb);
    fseek(fb, 0, SEEK_SET);
    readHeaderTree(fb, cabecalhoArvore);

    //atualiza status e lê cabecalho do arquivo de dados
    CabecalhoVeiculo* cabecalhoVeiculo = (CabecalhoVeiculo* ) malloc(sizeof(CabecalhoVeiculo));
    cabecalhoVeiculo->status = '0';
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw);
    readBinaryHeaderVeiculo(fw, cabecalhoVeiculo); 
    
    //cria chave com prefixo e byteOffset. inicialmente vai tentar inserir no nó raiz
    Chave* chave = malloc(sizeof(Chave));
    
    //converte o prefixo para inteiro
    chave->C = convertePrefixo(prefixoDeBusca);
    chave->P = -1;

    //começa a busca pelo nó raiz
    int* RRNNoBusca = (int*) malloc(sizeof(int));
    *RRNNoBusca = cabecalhoArvore->noRaiz;

    //busca chave na árvore, atualizando chave a inserir com o byteoffset retornado
    int encontrouChave = busca(fb, cabecalhoArvore, RRNNoBusca, chave);
    
    //se encontrou o registro, imprime na tela
    if( encontrouChave == 1) {
        
        //posiciona cursor na posicao do registro encontrado
        fseek(fw, chave->Pr, SEEK_SET);
        
        //le registro de dados e imprime na tela
        Veiculo* veiculo =(Veiculo* ) malloc(sizeof(Veiculo));
        readBinaryDataRegisterVeiculo(fw, veiculo);
        printBinaryDataRegisterVeiculo(cabecalhoVeiculo, veiculo);    

        free(veiculo);
    }
    //caso contrário, imprime mensagem
    else {
        printf("Registro inexistente.\n");
    }
        
    //atualiza status dos arquivos para 1
    fseek(fb, 0, SEEK_SET);
    cabecalhoArvore->status = '1';
    fwrite(cabecalhoArvore, sizeof(char), 1, fb);
    fseek(fw, 0, SEEK_SET);
    cabecalhoVeiculo->status = '1';
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fb);    

    free(RRNNoBusca);
    free(chave);
    free(cabecalhoArvore);
    free(cabecalhoVeiculo);
    
    fclose(fb);
    fclose(fw);
    
    return 0;
}
int buscaArvoreLinhas() {
    
    char binaryFile[100];
    char treeFile[100];
    int nEntradas = 0;
    char codigoDeBusca [30];
    
    scanf("%[^ ]", binaryFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%[^ ]", treeFile); // le nome do arquivo até espaço
    scanf("%*c"); // le espaco
    scanf("%*[^ ]"); // le palavra "codLinha"
    scanf("%*c"); // le espaco
    scan_quote_string(codigoDeBusca); //le valor de acordo com tipo
    
    //tenta abrir arquivo de dados
    FILE* fw= fopen(binaryFile, "rb+");    
    if(fw == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //abre o arquivo da árvore b
    FILE* fb= fopen(treeFile, "rb+");    
    if(fb == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //atualiza status e lê cabecalho do arquivo da árvore-b
    CabecalhoArvore* cabecalhoArvore = (CabecalhoArvore*) malloc(sizeof(CabecalhoArvore));
    cabecalhoArvore->status = '0';
    fwrite(cabecalhoArvore, sizeof(char), 1, fb);
    fseek(fb, 0, SEEK_SET);
    readHeaderTree(fb, cabecalhoArvore);

    //atualiza status e lê cabecalho do arquivo de dados
    CabecalhoLinha* cabecalhoLinha = (CabecalhoLinha* ) malloc(sizeof(CabecalhoLinha));
    cabecalhoLinha->status = '0';
    fwrite(cabecalhoLinha, sizeof(char), 1, fw);
    readBinaryHeaderLinha(fw, cabecalhoLinha); 
    
    //cria chave com prefixo e byteOffset. inicialmente vai tentar inserir no nó raiz
    Chave* chave = malloc(sizeof(Chave));
    
    //converte o prefixo para inteiro
    chave->C = atoi(codigoDeBusca);
    chave->P = -1;

    //começa a busca pelo nó raiz
    int* RRNNoBusca = (int*) malloc(sizeof(int));
    *RRNNoBusca = cabecalhoArvore->noRaiz;

    //busca chave na árvore, atualizando chave a inserir com o byteoffset retornado
    int encontrouChave = busca(fb, cabecalhoArvore, RRNNoBusca, chave);
    
    //se encontrou o registro, imprime na tela
    if( encontrouChave == 1) {
        
        //posiciona cursor na posicao do registro encontrado
        fseek(fw, chave->Pr, SEEK_SET);
        
        //le registro de dados e imprime na tela
        Linha* linha =(Linha* ) malloc(sizeof(Linha));
        readBinaryDataRegisterLinha(fw, linha);
        printBinaryDataRegisterLinha(cabecalhoLinha, linha);    

        free(linha);
    }
    //caso contrário, imprime mensagem
    else {
        printf("Registro inexistente.\n");
    }
        
    //atualiza status dos arquivos para 1
    fseek(fb, 0, SEEK_SET);
    cabecalhoArvore->status = '1';
    fwrite(cabecalhoArvore, sizeof(char), 1, fb);
    fseek(fw, 0, SEEK_SET);
    cabecalhoLinha->status = '1';
    fwrite(cabecalhoLinha, sizeof(char), 1, fb);    

    free(RRNNoBusca);
    free(chave);
    free(cabecalhoArvore);
    free(cabecalhoLinha);
    
    fclose(fb);
    fclose(fw);

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
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //abre o arquivo da árvore b
    FILE* fb= fopen(treeFile, "rb+");    
    if(fb == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //verifica consistência do arquivo de dados
    CabecalhoVeiculo* cabecalhoVeiculo = (CabecalhoVeiculo* ) malloc(sizeof(CabecalhoVeiculo));
    fread(cabecalhoVeiculo, sizeof(char), 1, fw);
    if( cabecalhoVeiculo->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    //atualiza status e lê cabecalho do arquivo de dados
    cabecalhoVeiculo->status = '0';
    fseek(fw, 0, SEEK_SET);
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw);
    readBinaryHeaderVeiculo(fw, cabecalhoVeiculo); 
    
    //verifica consistência do arquivo de árvore-b
    CabecalhoArvore* cabecalhoArvore = (CabecalhoArvore*) malloc(sizeof(CabecalhoArvore));
    fread(cabecalhoArvore, sizeof(char), 1, fb);
    if( cabecalhoArvore->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    //atualiza status e lê cabecalho do arquivo da árvore-b
    fseek(fb, 0, SEEK_SET);
    cabecalhoArvore->status = '0';
    fwrite(cabecalhoArvore, sizeof(char), 1, fb);
    fseek(fb, 0, SEEK_SET);
    readHeaderTree(fb, cabecalhoArvore);

    for(int cont = 0; cont < nEntradas; cont++) {
        
        Veiculo* veiculo =(Veiculo* ) malloc(sizeof(Veiculo));
        
        //processa entrada do usuario
        leVeiculoASerInserido(veiculo);
        
        //cria chave com prefixo e byteOffset. inicialmente vai tentar inserir no nó raiz
        Chave* chave = malloc(sizeof(Chave));
        
        //converte o prefixo para inteiro e lê byteoffset
        chave->C = convertePrefixo(veiculo->prefixo);
        chave->Pr = cabecalhoVeiculo->byteProxReg;
        chave->P = -1;
        
        //começa busca pelo nó raiz
        int* RRNNoBusca = (int*) malloc(sizeof(int));
        *RRNNoBusca = cabecalhoArvore->noRaiz;
        
        //insere na arvore e atualiza cabecalhoArvore
        insercao(
            fb,
            cabecalhoArvore,
            RRNNoBusca,
            chave
        );

        //posiciona cursor na proxima posicao disponível e escreve
        fseek(fw, chave->Pr, SEEK_SET);
        writeBinaryDataRegisterVeiculo(fw, veiculo);

        //atualiza cabecalho do arquivo de dados com byte do próximo registro e número de registros
        cabecalhoVeiculo->byteProxReg = ftell(fw);
        cabecalhoVeiculo->nroRegistros++;
        fseek(fw, 1, SEEK_SET);
        writeBinaryHeaderVeiculo(fw, cabecalhoVeiculo);

        free(chave);
        free(veiculo);
        free(RRNNoBusca);

    }
    
        
    //atualiza cabecalho dos arquivos e escreve
    fseek(fb, 0, SEEK_SET);
    cabecalhoArvore->status='1';
    writeHeaderTree(fb, cabecalhoArvore);
    
    fseek(fw, 0, SEEK_SET);
    cabecalhoVeiculo->status = '1';    
    fwrite(cabecalhoVeiculo, sizeof(char), 1, fw); //status byte
    
    free(cabecalhoArvore);
    free(cabecalhoVeiculo);
    
    fclose(fw);
    fclose(fb);
    binarioNaTela(treeFile);

    return 0;
    
}
int insereLinhasArvore() {
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
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    //abre o arquivo da árvore b
    FILE* fb= fopen(treeFile, "rb+");    
    if(fb == NULL){
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    //verifica consistência do arquivo de dados
    CabecalhoLinha* cabecalhoLinha = (CabecalhoLinha* ) malloc(sizeof(CabecalhoLinha));
    fread(cabecalhoLinha, sizeof(char), 1, fw);
    if( cabecalhoLinha->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    
    //atualiza status e lê cabecalho do arquivo de dados
    cabecalhoLinha->status = '0';
    fseek(fw, 0, SEEK_SET);
    fwrite(cabecalhoLinha, sizeof(char), 1, fw);
    readBinaryHeaderLinha(fw, cabecalhoLinha); 

    //verifica consistência do arquivo de árvore-b
    CabecalhoArvore* cabecalhoArvore = (CabecalhoArvore*) malloc(sizeof(CabecalhoArvore));
    fread(cabecalhoArvore, sizeof(char), 1, fb);
    if( cabecalhoArvore->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }
    //atualiza status e lê cabecalho do arquivo da árvore-b
    fseek(fb, 0, SEEK_SET);
    cabecalhoArvore->status = '0';
    fwrite(cabecalhoArvore, sizeof(char), 1, fb);
    fseek(fb, 0, SEEK_SET);
    readHeaderTree(fb, cabecalhoArvore);
    
    
    for(int cont = 0; cont < nEntradas; cont++) {
        
        Linha* linha =(Linha* ) malloc(sizeof(Linha));
        
        //processa entrada do usuario
        leLinhaASerInserida(linha);
        
        //cria chave com prefixo e byteOffset. inicialmente vai tentar inserir no nó raiz
        Chave* chave = malloc(sizeof(Chave));
        
        //converte o prefixo para inteiro e lê byteoffset
        chave->C = linha->codLinha;
        chave->Pr = cabecalhoLinha->byteProxReg;
        chave->P = -1;

        //posiciona cursor na proxima posicao disponível e escreve
        fseek(fw, chave->Pr, SEEK_SET);

        //começa busca pelo nó raiz
        int* RRNNoBusca = (int*) malloc(sizeof(int));
        *RRNNoBusca = cabecalhoArvore->noRaiz;
        
        //insere na arvore e atualiza cabecalhoArvore
        insercao(
            fb,
            cabecalhoArvore,
            RRNNoBusca,
            chave
        );
        
        //escreve em disco
        writeBinaryDataRegisterLinha(fw, linha);

        //atualiza cabecalho do arquivo de dados com byte do próximo registro e número de registros
        cabecalhoLinha->byteProxReg = ftell(fw);
        cabecalhoLinha->nroRegistros++;
        fseek(fw, 0, SEEK_SET);
        writeBinaryHeaderLinha(fw, cabecalhoLinha);

        free(chave);
        free(linha);
        free(RRNNoBusca);

    }
    
        
    //escreve cabecalho da arvore no arquivo
    fseek(fb, 0, SEEK_SET);
    cabecalhoArvore->status='1';
    writeHeaderTree(fb, cabecalhoArvore);
    
    free(cabecalhoArvore);
    
    fclose(fb);
    binarioNaTela(treeFile);

    //atualiza status 1
    fseek(fw, 0, SEEK_SET);
    cabecalhoLinha->status = '1';
    fwrite(cabecalhoLinha, sizeof(char), 1, fw); //status byte
    
    free(cabecalhoLinha);

    fclose(fw);


    return 0;
}

#endif