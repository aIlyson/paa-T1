#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void ler_numeros(FILE *arquivo, int *vetor, int indice, int tamanho) {
    if (indice < tamanho) {
        fscanf(arquivo, "%d", &vetor[indice]);
        ler_numeros(arquivo, vetor, indice + 1, tamanho);
    }
}

int *ler_arquivo(const char *caminho, int *tamanho) {
    FILE *arquivo;
    int *vetor;
    int n;

    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'.\n", caminho);
        fprintf(stderr, "Rode primeiro o gerador (pasta gerador/).\n");
        exit(1);
    }

    fscanf(arquivo, "%d", &n);
    vetor = (int *) malloc(sizeof(int) * (size_t) n);
    ler_numeros(arquivo, vetor, 0, n);

    fclose(arquivo);
    *tamanho = n;
    return vetor;
}

void copiar_posicao(int *destino, const int *origem, int indice, int tamanho) {
    if (indice < tamanho) {
        destino[indice] = origem[indice];
        copiar_posicao(destino, origem, indice + 1, tamanho);
    }
}

int *copiar_vetor(const int *origem, int tamanho) {
    int *vetor;

    vetor = (int *) malloc(sizeof(int) * (size_t) tamanho);
    copiar_posicao(vetor, origem, 0, tamanho);

    return vetor;
}

double tempo_atual(void) {
    return (double) clock() / CLOCKS_PER_SEC;
}

int verificar_ordem(const int *vetor, int indice, int tamanho, int ordenado) {
    int resultado;

    if (indice >= tamanho) {
        resultado = ordenado;
    }

    else if (vetor[indice - 1] > vetor[indice]) {
        resultado = verificar_ordem(vetor, indice + 1, tamanho, 0);
    }

    else {
        resultado = verificar_ordem(vetor, indice + 1, tamanho, ordenado);
    }

    return resultado;
}

int esta_ordenado(const int *vetor, int tamanho) {
    return verificar_ordem(vetor, 1, tamanho, 1);
}
