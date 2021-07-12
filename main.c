/* Abimael Alves de Oliveira Neto
    NUSP.: 10747822    
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "./dados/linhas.h" //dados que manipulam linhas
#include "./dados/veiculos.h" //dados que manipulam veiculos
#include "./funcoesAux.h" //auxiliares 
#include "./funcionalidades.h" //funcionalidades de 1 a 8
#include "./arvore-b/arvore-b.h" //funcionalidades de 1 a 8

int main(void) {

    //le o numero da funcao e seleciona
    int selecionaFuncao = 0; //seleciona de 1 a 8
    scanf("%d", &selecionaFuncao); //seleciona funcao
    scanf("%*c"); //le espaco
    
    //chama a funcionalidade do trabalho
    switch(selecionaFuncao) {
        // case 1:
        //     leVeiculos();
        //     break;
        // case 2:
        //     leLinhas();
        //     break;
        // case 3:
        //     listaVeiculos();
        //     break;
        // case 4:
        //     listalinhas();
        //     break;
        // case 5:
        //     buscaVeiculo();
        //     break;
        // case 6:
        //     buscaLinha();
        //     break;
        // case 7:
        //     insereVeiculos();
        //     break;
        // case 8:
        //     insereLinhas();
        //     break;
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