#include "../include/block_sort.h"
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
        printf("      rotate: blocos [%d..%d] + [%d..%d] em [%d..%d]\n",
               left, middle, middle + 1, right, left, right);
        printf("        antes : ");
        imprimir_intervalo(array, left, right);
        printf("\n");
    }

    reverse(array, left, middle);
    reverse(array, middle + 1, right);
    reverse(array, left, right);

    if (rastrear)
    {
        printf("        depois: ");
        imprimir_intervalo(array, left, right);
        printf("\n");
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
        printf("      lower_bound(%d) em [%d..%d) -> %d\n", value, inicio, fim, left);

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
        printf("      upper_bound(%d) em [%d..%d) -> %d\n", value, inicio, fim, left);

    return left;
}

static void insertion_sort(int array[], int left, int right)
{
    int i, j;

    if (rastrear)
    {
        printf("    insertion_sort [%d..%d] antes : ", left, right);
        imprimir_intervalo(array, left, right);
        printf("\n");
    }

    for (i = left + 1; i <= right; i++)
        for (j = i; j > left && array[j - 1] > array[j]; j--)
            swap(&array[j - 1], &array[j]);

    if (rastrear)
    {
        printf("                     depois: ");
        imprimir_intervalo(array, left, right);
        printf("\n");
    }
}

static void merge_in_place(int array[], int left, int mid, int right)
{
    int corte, valor, fronteira, tamanho_dir;

    if (left > mid || mid + 1 > right)
        return;

    if (rastrear)
        printf("    merge [%d..%d] com lados [%d..%d] e [%d..%d]\n",
               left, right, left, mid, mid + 1, right);

    if (array[mid] <= array[mid + 1])
    {
        if (rastrear)
            printf("      lados ja intercalados, nada a fazer\n");
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
        printf("\n[Fase 1] Blocos de %d elemento(s) ordenados com insertion sort:\n",
               tamanho_bloco);

    for (inicio = 0; inicio < tamanho; inicio += tamanho_bloco)
    {
        fim = inicio + tamanho_bloco - 1;
        if (fim > tamanho - 1)
            fim = tamanho - 1;
        insertion_sort(vetor, inicio, fim);
    }

    if (rastrear)
        printf("\n[Fase 2] Merges progressivos (bottom-up) in-place:\n");

    for (largura = tamanho_bloco; largura < tamanho; )
    {
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

    printf("\n-- Block Sort: demonstracao passo a passo --\n");
    printf("Vetor original:  ");
    imprimir_intervalo(vetor, 0, tamanho - 1);
    printf("\n");

    rastrear = 1;
    block_sort_com_bloco(vetor, tamanho, TAMANHO_BLOCO_DEMO);
    rastrear = 0;

    printf("\nVetor ordenado:  ");
    imprimir_intervalo(vetor, 0, tamanho - 1);
    printf("\n");
}
