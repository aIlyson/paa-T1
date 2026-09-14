#include "../include/tree_sort.h"
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *esq;
    struct No *dir;
} No;

No *criar_no(int valor) {
    No *novo = (No *) malloc(sizeof(No));
    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

void inserir_no(No **raiz, int valor) {
    if (*raiz == NULL) {
        *raiz = criar_no(valor);
    }

    else if (valor < (*raiz)->valor) {
        inserir_no(&((*raiz)->esq), valor);
    }

    else {
        inserir_no(&((*raiz)->dir), valor);
    }
}

void inserir_todos(No **raiz, int *vetor, int indice, int tamanho) {
    if (indice < tamanho) {
        inserir_no(raiz, vetor[indice]);
        inserir_todos(raiz, vetor, indice + 1, tamanho);
    }
}

void em_ordem(No *raiz, int *vetor, int *indice) {
    if (raiz != NULL) {
        em_ordem(raiz->esq, vetor, indice);

        vetor[*indice] = raiz->valor;
        *indice = *indice + 1;

        em_ordem(raiz->dir, vetor, indice);
    }
}

void liberar_arvore(No *raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esq);
        liberar_arvore(raiz->dir);
        free(raiz);
    }
}

void tree_sort(int *vetor, int tamanho) {
    No *raiz = NULL;
    int indice = 0;

    inserir_todos(&raiz, vetor, 0, tamanho);
    em_ordem(raiz, vetor, &indice);
    liberar_arvore(raiz);
}
