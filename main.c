/* Abimael Alves de Oliveira Neto
    NUSP.: 10747822   
    Fernando César  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "./funcoes/linhas.h" //funcoes que manipulam linhas
#include "./funcoes/veiculos.h" //funcoes que manipulam veiculos
#include "./funcoesAux.h" //auxiliares - strings e binarioNaTela
#include "./funcionalidadesTrabalho.h" //funcionalidades de 1 a 8
#include "./funcoes/arvore-b/arvore-b.h" //funcionalidades de 1 a 8

int main(void) {

    //le o numero da funcao e seleciona
    int selecionaFuncao = 0; //seleciona de 1 a 8
    scanf("%d", &selecionaFuncao); //seleciona funcao
    scanf("%*c"); //le espaco
    // selecionaFuncao = 13;
    //chama a funcionalidade do trabalho
    switch(selecionaFuncao) {
        case 1:
            leVeiculos();
            break;
        case 2:
            leLinhas();
            break;
        case 3:
            listaVeiculos();
            break;
        case 4:
            listalinhas();
            break;
        case 5:
            buscaVeiculo();
            break;
        case 6:
            buscaLinha();
            break;
        case 7:
            insereVeiculos();
            break;
        case 8:
            insereLinhas();
            break;
        case 9:
            criaArvoreVeiculos();
            break;
        case 10:
            criaArvoreLinhas();
            break;
        case 11:
            buscaArvoreVeiculos();
            break;
        case 12:
            buscaArvoreLinhas();
            break;
        case 13:
            insereVeiculosArvore();
            break;
        case 14:
            insereLinhasArvore();
            break;
    }
    
    return EXIT_SUCCESS;
}