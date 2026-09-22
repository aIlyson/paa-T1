#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

void ler_numeros(FILE *arquivo, int *vetor, int indice, int tamanho)
{
    int i;

    for (i = indice; i < tamanho; i++)
    {
        if (fscanf(arquivo, "%d", &vetor[i]) != 1)
            vetor[i] = 0;
    }
}

int *ler_arquivo(const char *caminho, int *tamanho)
{
    FILE *arquivo;
    int *vetor;
    int n;

    arquivo = fopen(caminho, "r");
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'.\n", caminho);
        fprintf(stderr, "Rode primeiro o gerador (tools/gerador.c).\n");
        exit(1);
    }

    if (fscanf(arquivo, "%d", &n) != 1 || n < 0)
    {
        fprintf(stderr, "Erro: formato invalido em '%s'.\n", caminho);
        fclose(arquivo);
        exit(1);
    }

    vetor = (int *) malloc(sizeof(int) * (size_t) n);
    if (vetor == NULL && n > 0)
    {
        fprintf(stderr, "Erro: falha ao alocar %d elementos.\n", n);
        fclose(arquivo);
        exit(1);
    }

    ler_numeros(arquivo, vetor, 0, n);

    fclose(arquivo);
    *tamanho = n;
    return vetor;
}

void copiar_posicao(int *destino, const int *origem, int indice, int tamanho)
{
    int i;

    for (i = indice; i < tamanho; i++)
        destino[i] = origem[i];
}

int *copiar_vetor(const int *origem, int tamanho)
{
    int *vetor;

    vetor = (int *) malloc(sizeof(int) * (size_t) tamanho);
    if (vetor == NULL && tamanho > 0)
    {
        fprintf(stderr, "Erro: falha ao copiar vetor de %d elementos.\n", tamanho);
        exit(1);
    }

    copiar_posicao(vetor, origem, 0, tamanho);

    return vetor;
}

double tempo_atual(void)
{
#ifdef _WIN32
    static int inicializado = 0;
    static LARGE_INTEGER frequencia;
    LARGE_INTEGER contador;

    if (!inicializado)
    {
        QueryPerformanceFrequency(&frequencia);
        inicializado = 1;
    }

    if (frequencia.QuadPart == 0)
        return (double) clock() / CLOCKS_PER_SEC;

    QueryPerformanceCounter(&contador);
    return (double) contador.QuadPart / (double) frequencia.QuadPart;
#else
    return (double) clock() / CLOCKS_PER_SEC;
#endif
}

int verificar_ordem(const int *vetor, int indice, int tamanho, int ordenado)
{
    int i;

    if (!ordenado)
        return 0;

    for (i = indice; i < tamanho; i++)
    {
        if (vetor[i - 1] > vetor[i])
            return 0;
    }

    return 1;
}

int esta_ordenado(const int *vetor, int tamanho)
{
    return verificar_ordem(vetor, 1, tamanho, 1);
}

int verificar_integridade(const int *orig, const int *ord, int n)
{
    unsigned long long soma_orig = 0;
    unsigned long long soma_ord = 0;
    unsigned long long xor_orig = 0;
    unsigned long long xor_ord = 0;
    int i;

    if (n < 0)
        return 0;

    for (i = 0; i < n; i++)
    {
        soma_orig += (unsigned long long) (unsigned int) orig[i];
        soma_ord += (unsigned long long) (unsigned int) ord[i];
        xor_orig ^= (unsigned long long) (unsigned int) orig[i];
        xor_ord ^= (unsigned long long) (unsigned int) ord[i];
    }

    return (soma_orig == soma_ord) && (xor_orig == xor_ord);
}
