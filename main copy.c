/* Abimael Alves de Oliveira Neto
    NUSP.: 10747822     
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

int main(void) {

    //le o numero da funcao e seleciona
    int selecionaFuncao = 0; //seleciona de 1 a 8
    scanf("%d", &selecionaFuncao); //seleciona funcao
    scanf("%*c"); //le espaco

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
    }
    
    return EXIT_SUCCESS;
}