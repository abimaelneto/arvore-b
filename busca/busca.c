
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "convertePrefixo.h"




int buscaVeiculoArvore() {


  // Dados de entrada
  char nomeEntrada[100];
  char nomeArvore[100];
  char prefixo[100];
  char valor[100];


  // Lê os dados de entrada
  scanf("%[^ ]", nomeEntrada);
  scanf("%*c");
  scanf("%[^ ]", nomeArvore);
  scanf("%*c");
  scanf("%[^ ]", prefixo);
  scanf("%*c");
  scanf("%[^ ]", valor);
  scanf("%*c");


  // Converte o prefixo para inteiro
  int prefixo_int = convertePrefixo(valor);


  // Tenta abrir o arquivo dos veículos
  FILE* arqVeiculos = fopen(nomeEntrada, "rb+");
  if(arqVeiculos == NULL){
      printf("Falha no processamento do arquivo.\n");
      return EXIT_FAILURE;
  }


  // Tenta abrir o arquivo da árvore
  FILE* arqArvore = fopen(nomeArvore, "rb+");
  if(arqArvore == NULL){
      printf("Falha no processamento do arquivo.\n");
      return EXIT_FAILURE;
  }


  // Inicia os arquivos como inconsistentes
  char status = '0';
  fwrite(&status, sizeof(char), 1, arqVeiculos);
  fwrite(&status, sizeof(char), 1, arqArvore);


  // Identifica o nó raiz
  int noRaiz;
  fread(&noRaiz, sizeof(int), 1, arqArvore);


  // Caso da árvore vazia
  if (noRaiz == -1) {
    printf("Registro inexistente.");
    return 0;
  }


  // Vai até o nó raíz
  fseek(arqArvore, noRaiz * 77, SEEK_SET);
  

  // Termina os arquivos como consistentes
  status = '1';
  fseek(arqVeiculos, 0, SEEK_SET);
  fseek(arqArvore, 0, SEEK_SET);
  fwrite(&status, sizeof(char), 1, arqVeiculos);
  fwrite(&status, sizeof(char), 1, arqArvore);


  // Fecha os arquivos
  fclose(arqArvore);
  fclose(arqVeiculos);

}



int main() {

  int opcao;
  scanf("%d", &opcao);
  scanf("%*c");

  buscaVeiculoArvore();

  return EXIT_SUCCESS;

}
