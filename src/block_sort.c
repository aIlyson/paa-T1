#include "../include/block_sort.h"
#include "../include/security.h"
#include <stdio.h>

#define TAMANHO_BLOCO_BASE 16
#define TAMANHO_BLOCO_DEMO 2

static int rastrear = 0;

static void imprimir_intervalo(const int *array, int left, int right)
{
    int i;
    for (i = left; i <= right; i++)
        printf("%d ", array[i]);
}

static void imprimir_blocos(const int *array, int tamanho, int tamanho_bloco)
{
    int i, j, fim;

    printf("  blocos: ");
    for (i = 0; i < tamanho; i += tamanho_bloco)
    {
        fim = i + tamanho_bloco - 1;
        if (fim > tamanho - 1)
            fim = tamanho - 1;
        print_amarelo("[");
        for (j = i; j <= fim; j++)
        {
            printf("%d", array[j]);
            if (j < fim)
                printf(" ");
        }
        print_amarelo("]");
        printf(" ");
    }
    printf("\n");
}

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void reverse(int array[], int left, int right)
{
    while (left < right)
    {
        swap(&array[left], &array[right]);
        left++;
        right--;
    }
}

static void rotate(int array[], int left, int middle, int right)
{
    if (middle < left || middle >= right)
        return;

    if (rastrear)
    {
        int i;
        printf("      rotate [%d..%d]: ", left, right);
        cor_vermelha();
        for (i = left; i <= right; i++)
        {
            if (i == middle + 1)
                printf("| ");
            printf("%d ", array[i]);
        }
        cor_normal();
        printf(" -> ");
    }

    reverse(array, left, middle);
    reverse(array, middle + 1, right);
    reverse(array, left, right);

    if (rastrear)
    {
        int i;
        int corte = left + (right - middle) - 1;
        cor_verde();
        for (i = left; i <= right; i++)
        {
            if (i == corte + 1)
                printf("| ");
            printf("%d ", array[i]);
        }
        cor_normal();
        printf("\n");
        pausa_passo();
    }
}

static int lower_bound(const int array[], int left, int right, int value)
{
    int inicio = left;
    int fim = right;
    int middle;

    while (left < right)
    {
        middle = left + (right - left) / 2;
        if (array[middle] < value)
            left = middle + 1;
        else
            right = middle;
    }

    if (rastrear)
    {
        cor_amarela();
        printf("      lower_bound(%d) em [%d..%d) -> %d\n", value, inicio, fim, left);
        cor_normal();
        pausa_passo();
    }

    return left;
}

static int upper_bound(const int array[], int left, int right, int value)
{
    int inicio = left;
    int fim = right;
    int middle;

    while (left < right)
    {
        middle = left + (right - left) / 2;
        if (array[middle] <= value)
            left = middle + 1;
        else
            right = middle;
    }

    if (rastrear)
    {
        cor_amarela();
        printf("      upper_bound(%d) em [%d..%d) -> %d\n", value, inicio, fim, left);
        cor_normal();
        pausa_passo();
    }

    return left;
}

static void insertion_sort(int array[], int left, int right)
{
    int i, j;

    if (rastrear)
    {
        printf("    bloco [%d..%d]: ", left, right);
        cor_vermelha();
        imprimir_intervalo(array, left, right);
        cor_normal();
        printf(" -> ");
    }

    for (i = left + 1; i <= right; i++)
        for (j = i; j > left && array[j - 1] > array[j]; j--)
            swap(&array[j - 1], &array[j]);

    if (rastrear)
    {
        cor_verde();
        imprimir_intervalo(array, left, right);
        cor_normal();
        printf("\n");
        pausa_passo();
    }
}

static void merge_in_place(int array[], int left, int mid, int right)
{
    int corte, valor, fronteira, tamanho_dir;

    if (left > mid || mid + 1 > right)
        return;

    if (rastrear)
    {
        cor_azul();
        printf("    merge [%d..%d]: lados [%d..%d] e [%d..%d]\n",
               left, right, left, mid, mid + 1, right);
        cor_normal();
        pausa_passo();
    }

    if (array[mid] <= array[mid + 1])
    {
        if (rastrear)
        {
            cor_azul();
            printf("      lados ja intercalados, nada a fazer\n");
            cor_normal();
            pausa_passo();
        }
        return;
    }

    if (left == mid)
    {
        valor = array[left];
        fronteira = lower_bound(array, mid + 1, right + 1, valor);
        if (fronteira - 1 > mid)
            rotate(array, left, mid, fronteira - 1);
        return;
    }

    if (mid + 1 == right)
    {
        valor = array[right];
        fronteira = upper_bound(array, left, mid + 1, valor);
        if (fronteira <= mid)
            rotate(array, fronteira, mid, right);
        return;
    }

    corte = left + (mid - left) / 2;
    valor = array[corte + 1];
    fronteira = lower_bound(array, mid + 1, right + 1, valor);
    tamanho_dir = fronteira - 1 - mid;

    rotate(array, corte + 1, mid, fronteira - 1);
    merge_in_place(array, left, corte, corte + tamanho_dir);
    merge_in_place(array, corte + tamanho_dir + 1, fronteira - 1, right);
}

static void block_sort_com_bloco(int *vetor, int tamanho, int tamanho_bloco)
{
    int inicio, fim, largura, esq, meio, dir;

    if (vetor == NULL || tamanho <= 1)
        return;
    if (tamanho_bloco < 1)
        tamanho_bloco = 1;

    if (rastrear)
    {
        cor_azul();
        printf("\n[Fase 1] Ordenando blocos de %d elemento(s) com insertion sort\n",
               tamanho_bloco);
        cor_normal();
        pausa_fase();
    }

    for (inicio = 0; inicio < tamanho; inicio += tamanho_bloco)
    {
        fim = inicio + tamanho_bloco - 1;
        if (fim > tamanho - 1)
            fim = tamanho - 1;
        insertion_sort(vetor, inicio, fim);
    }

    if (rastrear)
    {
        imprimir_blocos(vetor, tamanho, tamanho_bloco);
        pausa_fase();
        cor_azul();
        printf("\n[Fase 2] Merges progressivos (bottom-up) in-place\n");
        cor_normal();
        pausa_fase();
    }

    for (largura = tamanho_bloco; largura < tamanho; )
    {
        if (rastrear)
        {
            cor_azul();
            printf("\n  largura %d:\n", largura);
            cor_normal();
        }
        for (esq = 0; esq < tamanho; esq += 2 * largura)
        {
            meio = esq + largura - 1;
            if (meio > tamanho - 1)
                break;
            dir = esq + 2 * largura - 1;
            if (dir > tamanho - 1)
                dir = tamanho - 1;
            merge_in_place(vetor, esq, meio, dir);
        }
        if (rastrear)
        {
            printf("  vetor: ");
            cor_amarela();
            imprimir_intervalo(vetor, 0, tamanho - 1);
            cor_normal();
            printf("\n");
            pausa_fase();
        }
        if (largura > tamanho / 2)
            break;
        largura *= 2;
    }
}

void block_sort(int *vetor, int tamanho)
{
    block_sort_com_bloco(vetor, tamanho, TAMANHO_BLOCO_BASE);
}

void block_sort_passo_a_passo(int *vetor, int tamanho)
{
    if (vetor == NULL || tamanho <= 0)
        return;

    print_azul("-- Block Sort: passo a passo --\n");
    printf("Vetor original:  ");
    cor_amarela();
    imprimir_intervalo(vetor, 0, tamanho - 1);
    cor_normal();
    printf("\n");

    rastrear = 1;
    block_sort_com_bloco(vetor, tamanho, TAMANHO_BLOCO_DEMO);
    rastrear = 0;

    printf("\nVetor ordenado:  ");
    cor_verde();
    imprimir_intervalo(vetor, 0, tamanho - 1);
    cor_normal();
    printf("\n");
}
