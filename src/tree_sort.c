#include "../include/tree_sort.h"
#include "../include/security.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct No
{
    int valor;
    struct No *esq;
    struct No *dir;
} No;

static No *criar_no(int valor)
{
    No *novo = (No *)malloc(sizeof(No));
    verificar_alocacao(novo);

    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

static void inserir_no(No **raiz, int valor)
{
    No *atual;

    if (*raiz == NULL)
    {
        *raiz = criar_no(valor);
        return;
    }

    atual = *raiz;
    for (;;)
    {
        if (valor < atual->valor)
        {
            if (atual->esq == NULL)
            {
                atual->esq = criar_no(valor);
                return;
            }
            atual = atual->esq;
        }
        else
        {
            if (atual->dir == NULL)
            {
                atual->dir = criar_no(valor);
                return;
            }
            atual = atual->dir;
        }
    }
}

static void inserir_todos(No **raiz, int *vetor, int indice, int tamanho)
{
    int i;

    for (i = indice; i < tamanho; i++)
        inserir_no(raiz, vetor[i]);
}

static void em_ordem(No *raiz, int *vetor, int *indice, No **pilha)
{
    int topo = 0;
    No *atual = raiz;

    while (atual != NULL || topo > 0)
    {
        while (atual != NULL)
        {
            pilha[topo] = atual;
            topo++;
            atual = atual->esq;
        }

        topo--;
        atual = pilha[topo];
        vetor[*indice] = atual->valor;
        *indice = *indice + 1;
        atual = atual->dir;
    }
}

static void em_ordem_imprimir(No *raiz, No **pilha)
{
    int topo = 0;
    No *atual = raiz;

    while (atual != NULL || topo > 0)
    {
        while (atual != NULL)
        {
            pilha[topo] = atual;
            topo++;
            atual = atual->esq;
        }

        topo--;
        atual = pilha[topo];
        printf("%d ", atual->valor);
        atual = atual->dir;
    }
}

static void liberar_arvore(No *raiz, No **pilha)
{
    int topo = 0;
    No *atual = raiz;
    No *ultimo = NULL;
    No *topo_no;

    while (atual != NULL || topo > 0)
    {
        while (atual != NULL)
        {
            pilha[topo] = atual;
            topo++;
            atual = atual->esq;
        }

        topo_no = pilha[topo - 1];

        if (topo_no->dir != NULL && ultimo != topo_no->dir)
        {
            atual = topo_no->dir;
        }
        else
        {
            ultimo = topo_no;
            topo--;
            free(topo_no);
        }
    }
}

static No **criar_pilha(int capacidade)
{
    No **pilha;

    pilha = (No **)malloc(sizeof(No *) * (size_t)capacidade);
    verificar_alocacao(pilha);
    return pilha;
}

void tree_sort(int *vetor, int tamanho)
{
    No *raiz = NULL;
    No **pilha;
    int indice = 0;

    if (vetor == NULL || tamanho <= 1)
        return;

    pilha = criar_pilha(tamanho);
    inserir_todos(&raiz, vetor, 0, tamanho);
    em_ordem(raiz, vetor, &indice, pilha);
    liberar_arvore(raiz, pilha);
    free(pilha);
}

void tree_sort_passo_a_passo(int *vetor, int tamanho)
{
    No *raiz = NULL;
    No **pilha;
    int i, indice = 0;

    if (vetor == NULL || tamanho <= 0)
        return;

    print_azul("-- Tree Sort: passo a passo --\n");
    printf("Vetor original:  ");
    cor_amarela();
    for (i = 0; i < tamanho; i++)
        printf("%d ", vetor[i]);
    cor_normal();
    printf("\n");

    pilha = criar_pilha(tamanho);

    cor_azul();
    printf("\n[Fase 1] Inserindo cada valor na BST (iterativo)\n");
    cor_normal();
    pausa_fase();

    for (i = 0; i < tamanho; i++)
    {
        inserir_no(&raiz, vetor[i]);
        printf("  [");
        cor_azul();
        printf("%d", i + 1);
        cor_normal();
        printf("] insere ");
        cor_verde();
        printf("%-3d", vetor[i]);
        cor_normal();
        printf(" -> em-ordem: ");
        cor_amarela();
        em_ordem_imprimir(raiz, pilha);
        cor_normal();
        printf("\n");
        pausa_passo();
    }

    cor_azul();
    printf("\n[Fase 2] Travessia em-ordem (grava ordenado no vetor)\n");
    cor_normal();
    pausa_fase();

    em_ordem(raiz, vetor, &indice, pilha);

    printf("\nVetor ordenado:  ");
    cor_verde();
    for (i = 0; i < tamanho; i++)
        printf("%d ", vetor[i]);
    cor_normal();
    printf("\n");

    liberar_arvore(raiz, pilha);
    free(pilha);
}
