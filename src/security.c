#include "../include/security.h"
#include <stdio.h>
#include <stdlib.h>

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void pausar_tela()
{
    printf("\nPressione [ENTER] para continuar...");
    getchar();
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void print_amarelo(char *mensagem)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
    printf("%s", mensagem);
    SetConsoleTextAttribute(hConsole, 7);
}

void print_vermelho(char *mensagem)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
    printf("%s", mensagem);
    SetConsoleTextAttribute(hConsole, 7);
}

void print_verde(char *mensagem)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    printf("%s", mensagem);
    SetConsoleTextAttribute(hConsole, 7);
}

#else

void print_amarelo(char *mensagem)
{
    printf("\033[1;33m%s\033[0m", mensagem);
}

void print_vermelho(char *mensagem)
{
    printf("\033[1;31m%s\033[0m", mensagem);
}

void print_verde(char *mensagem)
{
    printf("\033[1;32m%s\033[0m", mensagem);
}

#endif

void mensagem_erro(char *mensagem)
{
    print_vermelho("ERRO: ");
    printf("%s\n", mensagem);
}

void mensagem_sucesso(char *mensagem)
{
    print_verde("Sucesso: ");
    printf("%s\n", mensagem);
}

void mensagem_info(char *mensagem)
{
    print_amarelo("INFO: ");
    printf("%s\n", mensagem);
}

void print_titulo(char *titulo)
{
    printf("\n=== %s ===\n", titulo);
}

void verificar_alocacao(void *ponteiro)
{
    if (ponteiro == NULL)
    {
        mensagem_erro("Erro: Falha na alocacao de memoria.");
        exit(1);
    }
}
