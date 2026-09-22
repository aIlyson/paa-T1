#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

int *ler_arquivo(const char *caminho, int *tamanho);
void ler_numeros(FILE *arquivo, int *vetor, int indice, int tamanho);

int *copiar_vetor(const int *origem, int tamanho);
void copiar_posicao(int *destino, const int *origem, int indice, int tamanho);

double tempo_atual(void);

int esta_ordenado(const int *vetor, int tamanho);
int verificar_ordem(const int *vetor, int indice, int tamanho, int ordenado);

int verificar_integridade(const int *orig, const int *ord, int n);

#endif
