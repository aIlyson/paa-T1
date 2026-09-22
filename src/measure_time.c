#include "../include/measure_time.h"
#include "../include/tree_sort.h"
#include "../include/block_sort.h"
#include "../include/utils.h"
#include "../include/security.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

typedef void (*FuncaoOrdenacao)(int *, int);

static void testar_algoritmo(FILE *csv, const char *nome, FuncaoOrdenacao funcao,
                             const int *original, int tamanho_lido,
                             int tamanho, const char *tipo)
{
    int *vetor;
    double inicio, fim, duracao;
    int ordenado, integro;
    const char *status;

    vetor = copiar_vetor(original, tamanho_lido);

    inicio = tempo_atual();
    funcao(vetor, tamanho_lido);
    fim = tempo_atual();

    duracao = fim - inicio;
    ordenado = esta_ordenado(vetor, tamanho_lido);
    integro = verificar_integridade(original, vetor, tamanho_lido);
    status = (ordenado && integro) ? "OK" : "ERRO";

    printf("%-10s | %d | %-10s | %s | %.6f s\n",
           nome, tamanho, tipo, status, duracao);

    if (!ordenado)
        mensagem_erro("Vetor nao ordenado!");
    if (!integro)
        mensagem_erro("Integridade violada (elemento perdido ou duplicado)!");

    fprintf(csv, "%s,%d,%s,%.6f\n", nome, tamanho, tipo, duracao);

    free(vetor);
}

static void testar_um_caso(FILE *csv, int tamanho, const char *tipo)
{
    char caminho[256];
    int tamanho_lido;
    int *original;

    snprintf(caminho, sizeof(caminho), "experimentos/dados/dados_%d_%s.txt", tamanho, tipo);
    original = ler_arquivo(caminho, &tamanho_lido);

    testar_algoritmo(csv, "Tree Sort", tree_sort,
                     original, tamanho_lido, tamanho, tipo);
    testar_algoritmo(csv, "Block Sort", block_sort,
                     original, tamanho_lido, tamanho, tipo);

    free(original);
}

void medir_tempos_comparativos(void)
{
    int tamanhos[] = {20000, 60000, 80000};
    const char *tipos[] = {"crescente", "decrescente", "aleatorio"};
    FILE *csv;
    int i, j;

#ifdef _WIN32
    (void) _mkdir("experimentos");
    (void) _mkdir("experimentos/resultados");
#else
    (void) mkdir("experimentos", 0755);
    (void) mkdir("experimentos/resultados", 0755);
#endif

    csv = fopen("experimentos/resultados/resultados_comparativos.csv", "w");
    verificar_alocacao(csv);
    fprintf(csv, "algoritmo,tamanho,tipo,tempo_segundos\n");

    printf("\n%-10s | %-6s | %-10s | %-4s | %s\n",
           "Algoritmo", "Tamanho", "Tipo", "OK?", "Tempo");

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            testar_um_caso(csv, tamanhos[i], tipos[j]);

    fclose(csv);
    mensagem_sucesso("Resultados salvos em experimentos/resultados/resultados_comparativos.csv (18 registros)");
}
