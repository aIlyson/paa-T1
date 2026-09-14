#include <stdio.h>
#include <stdlib.h>

#define SEED_ALEATORIOS 42u

void preencher_crescente(int *v, int indice, int n) {
    if (indice < n) {
        v[indice] = indice;
        preencher_crescente(v, indice + 1, n);
    }
}

void preencher_decrescente(int *v, int indice, int n) {
    if (indice < n) {
        v[indice] = n - indice;
        preencher_decrescente(v, indice + 1, n);
    }
}

void preencher_aleatorio(int *v, int indice, int n) {
    if (indice < n) {
        v[indice] = rand() % 1000001;
        preencher_aleatorio(v, indice + 1, n);
    }
}

void gravar_numeros(FILE *f, int *v, int indice, int n) {
    if (indice < n) {
        fprintf(f, "%d\n", v[indice]);
        gravar_numeros(f, v, indice + 1, n);
    }
}

void salvar_arquivo(const char *caminho, int *v, int n) {
    FILE *f = fopen(caminho, "w");
    if (f == NULL) {
        fprintf(stderr, "Erro ao criar arquivo: %s\n", caminho);
        exit(1);
    }
    fprintf(f, "%d\n", n);
    gravar_numeros(f, v, 0, n);
    fclose(f);
    printf("Gerado: %s (%d numeros)\n", caminho, n);
}

void gerar_um_tamanho(int n) {
    int *v = (int *) malloc(sizeof(int) * (size_t) n);
    char caminho[256];

    preencher_crescente(v, 0, n);
    snprintf(caminho, sizeof(caminho), "../dados/dados_%d_crescente.txt", n);
    salvar_arquivo(caminho, v, n);

    preencher_decrescente(v, 0, n);
    snprintf(caminho, sizeof(caminho), "../dados/dados_%d_decrescente.txt", n);
    salvar_arquivo(caminho, v, n);

    srand(SEED_ALEATORIOS + (unsigned) n);
    preencher_aleatorio(v, 0, n);
    snprintf(caminho, sizeof(caminho), "../dados/dados_%d_aleatorio.txt", n);
    salvar_arquivo(caminho, v, n);

    free(v);
}

void gerar_todos(const int *tamanhos, int indice, int quantidade) {
    if (indice < quantidade) {
        gerar_um_tamanho(tamanhos[indice]);
        gerar_todos(tamanhos, indice + 1, quantidade);
    }
}

int main(void) {
    int tamanhos[] = {20000, 60000, 80000};

    gerar_todos(tamanhos, 0, 3);

    printf("\nTodos os arquivos de teste foram gerados em ../dados/\n");
    printf("Os DOIS algoritmos (Treesort e Block Sort) devem usar esses MESMOS arquivos.\n");
    return 0;
}
