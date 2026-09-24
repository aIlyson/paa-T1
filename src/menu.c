#include "../include/menu.h"
#include "../include/security.h"
#include "../include/tree_sort.h"
#include "../include/block_sort.h"
#include "../include/measure_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#define TAMANHO_DEMO 7

static int vetor_demo[TAMANHO_DEMO];

static void gerar_vetor_demo(void)
{
    int i, j, repetido;

    for (i = 0; i < TAMANHO_DEMO; i++)
    {
        do
        {
            vetor_demo[i] = 1 + rand() % 99;
            repetido = 0;
            for (j = 0; j < i; j++)
                if (vetor_demo[j] == vetor_demo[i])
                    repetido = 1;
        } while (repetido);
    }
}

static void print_menu(void)
{
    printf("\n");
    printf("==========================================\n");
    printf("   PAA - TRABALHO 02: ORDENACAO (UFPI)    \n");
    printf("   Dupla 08: Alysson & Maria Julia        \n");
    printf("==========================================\n");
    printf("1 - Demonstracao do Tree Sort (passo a passo)\n");
    printf("2 - Demonstracao do Block Sort (passo a passo)\n");
    printf("3 - Executar medicao de desempenho (Tree x Block)\n");
    printf("4 - Informacoes teoricas e de Ambiente\n");
    printf("0 - Sair\n");
    printf("==========================================\n");
}

static void print_titulo_menu(void)
{
    print_amarelo("Digite sua opcao: ");
}

static void demonstracao_tree_sort(void)
{
    int vetor[TAMANHO_DEMO];
    int i;

    for (i = 0; i < TAMANHO_DEMO; i++)
        vetor[i] = vetor_demo[i];

    tree_sort_passo_a_passo(vetor, TAMANHO_DEMO);
}

static void demonstracao_block_sort(void)
{
    int vetor[TAMANHO_DEMO];
    int i;

    for (i = 0; i < TAMANHO_DEMO; i++)
        vetor[i] = vetor_demo[i];

    block_sort_passo_a_passo(vetor, TAMANHO_DEMO);
}

static void exibir_ambiente(void)
{
    const char *os = getenv("OS");
    const char *cpu = getenv("PROCESSOR_IDENTIFIER");
    const char *nucleos = getenv("NUMBER_OF_PROCESSORS");

#ifdef _WIN32
    printf("Sistema operacional: %s (Windows)\n",
           (os != NULL) ? os : "Windows");
#else
    printf("Sistema operacional: %s\n", (os != NULL) ? os : "Nao disponivel");
#endif

    printf("Processador (CPU): %s\n",
           (cpu != NULL) ? cpu : "Nao disponivel");
    if (nucleos != NULL)
        printf("Nucleos logicos: %s\n", nucleos);

#ifdef _WIN32
    {
        MEMORYSTATUSEX mem;
        mem.dwLength = sizeof(mem);
        if (GlobalMemoryStatusEx(&mem))
            printf("Memoria RAM total: %.1f GB\n",
                   (double) mem.ullTotalPhys / (1024.0 * 1024.0 * 1024.0));
    }
#endif

    printf("Compilador: ");
#if defined(__GNUC__)
    printf("GCC %d.%d.%d (%s)\n", __GNUC__, __GNUC_MINOR__,
           __GNUC_PATCHLEVEL__, __VERSION__);
#else
    printf("Nao disponivel\n");
#endif
    printf("Flags de compilacao: -Wall -Wextra -std=c99 -Iinclude -O2\n");
    printf("Data da compilacao: %s %s\n", __DATE__, __TIME__);
}

static void exibir_informacoes(void)
{
    print_titulo("INFORMACOES TEORICAS E AMBIENTE DE EXECUCAO");

    printf("\n[1] Tree Sort (Grupo A)\n");
    printf("    Paradigma: Arvore Binaria de Busca (BST) + percurso em-ordem\n");
    printf("    Melhor caso : O(n log n)\n");
    printf("    Caso medio  : O(n log n)\n");
    printf("    Pior caso   : O(n^2) (dados ordenados/decrescentes = BST degenerada)\n");
    printf("    Memoria     : O(n) auxiliar | Estavel: sim (iguais sao inseridos a direita)\n");
    printf("    Implementacao: insercao, percurso e liberacao 100%% iterativos (sem recursao)\n");

    printf("\n[2] Block Sort / Block Merge Sort (Grupo B)\n");
    printf("    Paradigma: subdivisao em blocos + intercalacao in-place estavel\n");
    printf("               (busca binaria lower/upper_bound + rotacao por triplo reverse)\n");
    printf("    Melhor caso : O(n log n) (otimizacao de caminho feliz nos merges)\n");
    printf("    Caso medio  : O(n log^2 n) (merge in-place por rotacoes)\n");
    printf("    Pior caso   : O(n log^2 n)\n");
    printf("    Nota: variantes avancadas com intercambio de blocos (ex.: WikiSort)\n");
    printf("          atingem O(n log n); a variante com rotacoes custa O(n log^2 n)\n");
    printf("    Memoria     : O(1) em heap + O(log n) de pilha de recursao | Estavel: sim\n");

    printf("\n");
    exibir_ambiente();
}

void menu_principal(void)
{
    int opcao = -1;

    srand((unsigned) time(NULL));
    gerar_vetor_demo();

    do
    {
        print_menu();
        print_titulo_menu();

        int res = scanf("%d", &opcao);
        if (res == EOF)
        {
            opcao = 0;
            break;
        }
        if (res != 1)
        {
            limpar_buffer();
            mensagem_erro("Entrada invalida, tente novamente.");
            pausar_tela();
            continue;
        }
        limpar_buffer();

        if (opcao >= 1 && opcao <= 4)
            limpar_tela();

        switch (opcao)
        {
        case 1:
            print_titulo("DEMONSTRACAO - TREE SORT");
            demonstracao_tree_sort();
            mensagem_sucesso("Algoritmo ordenado!");
            pausar_tela();
            break;

        case 2:
            print_titulo("DEMONSTRACAO - BLOCK SORT");
            demonstracao_block_sort();
            mensagem_sucesso("Algoritmo ordenado!");
            pausar_tela();
            break;

        case 3:
            print_titulo("MEDICAO DE DESEMPENHO");
            mensagem_info("Rodando 9 cenarios x 2 algoritmos (18 execucoes)...");
            medir_tempos_comparativos();
            pausar_tela();
            break;

        case 4:
            exibir_informacoes();
            pausar_tela();
            break;

        case 0:
            mensagem_sucesso("Saindo do sistema...");
            break;

        default:
            mensagem_info("Opcao invalida, tente novamente.");
            pausar_tela();
            break;
        }
    } while (opcao != 0);
}
