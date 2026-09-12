#include "../include/menu.h"
#include "../include/security.h"
#include "../include/tree_sort.h"
#include "../include/block_sort.h"
#include <stdio.h>
#include <stdlib.h>

void print_menu()
{
    printf("\n");
    printf("==========================================\n");
    printf("   PAA - TRABALHO 02: ORDENACAO (UFPI)    \n");
    printf("   Dupla 08: Alysson & Maria Julia        \n");
    printf("==========================================\n");
    printf("1 - Testar Ordenacao com Vetor Pequeno\n");
    printf("2 - Medir Tempos de Execucao (Desempenho)\n");
    printf("3 - Informacoes dos Algoritmos\n");
    printf("0 - Sair\n");
    printf("==========================================\n");
}

void print_titulo_menu()
{
    printf("Digite sua opcao: ");
}

void menu_principal()
{
    int opcao;

    do
    {
        print_menu();
        print_titulo_menu();

        if (scanf("%d", &opcao) != 1)
        {
            limpar_buffer();
            mensagem_erro("Entrada invalida, tente novamente.");
            pausar_tela();
            continue;
        }
        limpar_buffer();

        switch (opcao)
        {
        case 1:
            print_titulo("DEMONSTRACAO COM VETOR PEQUENO");
            tree_sort(NULL, 0);
            block_sort(NULL, 0);
            mensagem_sucesso("Demonstracao finalizada!");
            pausar_tela();
            break;

        case 2:
            print_titulo("TESTES DE DESEMPENHO");
            mensagem_info("Modulo de medicao pronto para execucao.");
            pausar_tela();
            break;

        case 3:
            print_titulo("INFORMACOES DOS ALGORITMOS");
            printf("1. Treesort (Grupo A)\n");
            printf("   - Paradigma: Arvore Binaria de Busca (BST)\n");
            printf("   - Complexidade: O(n log n) medio / O(n^2) pior caso\n\n");
            printf("2. Block Sort (Grupo B)\n");
            printf("   - Paradigma: Divisao por blocos / Merge in-place\n");
            printf("   - Complexidade: O(n log n) medio / O(n log n) pior caso\n");
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
