#include "../include/measure_time.h"
#include "../include/tree_sort.h"
#include "../include/utilS.h"
#include "../include/security.h"
#include <stdio.h>
#include <stdlib.h>

void testar_um_caso(FILE *csv, int tamanho, const char *tipo) {
    char caminho[256];
    int tamanho_lido;
    int *original;
    int *vetor;
    double inicio, fim;

    snprintf(caminho, sizeof(caminho), "dados/dados_%d_%s.txt", tamanho, tipo);

    original = ler_arquivo(caminho, &tamanho_lido);
    vetor = copiar_vetor(original, tamanho_lido);

    inicio = tempo_atual();
    tree_sort(vetor, tamanho_lido);
    fim = tempo_atual();

    printf("[Treesort] tamanho=%-6d tipo=%-12s tempo=%.6f s  %s\n",
           tamanho_lido, tipo, fim - inicio, esta_ordenado(vetor, tamanho_lido) ? "OK" : "ERRO");
    fprintf(csv, "Treesort,%d,%s,%.6f\n", tamanho_lido, tipo, fim - inicio);

    free(original);
    free(vetor);
}

void testar_tipos(FILE *csv, int tamanho, const char **tipos, int t, int quantidade_tipos) {
    if (t < quantidade_tipos) {
        testar_um_caso(csv, tamanho, tipos[t]);
        testar_tipos(csv, tamanho, tipos, t + 1, quantidade_tipos);
    }
}

void testar_tamanhos(FILE *csv, const int *tamanhos, int i, int quantidade_tamanhos,
                      const char **tipos, int quantidade_tipos) {
    if (i < quantidade_tamanhos) {
        testar_tipos(csv, tamanhos[i], tipos, 0, quantidade_tipos);
        testar_tamanhos(csv, tamanhos, i + 1, quantidade_tamanhos, tipos, quantidade_tipos);
    }
}

void medir_tempos_treesort(void) {
    int tamanhos[] = {20000, 60000, 80000};
    const char *tipos[] = {"crescente", "decrescente", "aleatorio"};
    FILE *csv;

    csv = fopen("resultados_treesort.csv", "w");
    verificar_alocacao(csv);
    fprintf(csv, "algoritmo,tamanho,tipo,tempo_segundos\n");

    testar_tamanhos(csv, tamanhos, 0, 3, tipos, 3);

    fclose(csv);
    mensagem_sucesso("Resultados salvos em resultados_treesort.csv");
}
