#include "../include/security.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void limpar_tela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar_tela()
{
    printf("\nPressione [ENTER] para continuar...");
    fflush(stdout);
    getchar();
    limpar_tela();
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static WORD cor_padrao = 0xFFFF;

static HANDLE console_handle(void)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (cor_padrao == 0xFFFF)
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(h, &info))
            cor_padrao = info.wAttributes;
    }
    return h;
}

static void aplicar_cor(WORD cor)
{
    SetConsoleTextAttribute(console_handle(), cor);
}

static void restaurar_cor(void)
{
    SetConsoleTextAttribute(console_handle(), cor_padrao);
}

void cor_azul(void)
{
    aplicar_cor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
}

void cor_amarela(void)
{
    aplicar_cor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
}

void cor_vermelha(void)
{
    aplicar_cor(FOREGROUND_INTENSITY | FOREGROUND_RED);
}

void cor_verde(void)
{
    aplicar_cor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
}

void cor_normal(void)
{
    restaurar_cor();
}

void print_azul(char *mensagem)
{
    aplicar_cor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    printf("%s", mensagem);
    restaurar_cor();
}

void print_amarelo(char *mensagem)
{
    aplicar_cor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
    printf("%s", mensagem);
    restaurar_cor();
}

void print_vermelho(char *mensagem)
{
    aplicar_cor(FOREGROUND_INTENSITY | FOREGROUND_RED);
    printf("%s", mensagem);
    restaurar_cor();
}

void print_verde(char *mensagem)
{
    aplicar_cor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    printf("%s", mensagem);
    restaurar_cor();
}

#else

void print_azul(char *mensagem)
{
    printf("\033[1;34m%s\033[0m", mensagem);
}

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

void cor_azul(void)
{
    printf("\033[1;34m");
}

void cor_amarela(void)
{
    printf("\033[1;33m");
}

void cor_vermelha(void)
{
    printf("\033[1;31m");
}

void cor_verde(void)
{
    printf("\033[1;32m");
}

void cor_normal(void)
{
    printf("\033[0m");
}

#endif

void pausa_passo(void)
{
#ifdef _WIN32
    Sleep(160);
#else
    struct timespec atraso;
    atraso.tv_sec = 0;
    atraso.tv_nsec = 160000000L;
    nanosleep(&atraso, NULL);
#endif
}

void pausa_fase(void)
{
#ifdef _WIN32
    Sleep(550);
#else
    struct timespec atraso;
    atraso.tv_sec = 0;
    atraso.tv_nsec = 550000000L;
    nanosleep(&atraso, NULL);
#endif
}

void mensagem_erro(char *mensagem)
{
    print_vermelho("ERRO: ");
    print_vermelho(mensagem);
    printf("\n");
}

void mensagem_sucesso(char *mensagem)
{
    print_verde("Sucesso: ");
    print_verde(mensagem);
    printf("\n");
}

void mensagem_info(char *mensagem)
{
    print_amarelo("INFO: ");
    print_amarelo(mensagem);
    printf("\n");
}

void print_titulo(char *titulo)
{
    printf("\n");
    print_azul("=== ");
    print_azul(titulo);
    print_azul(" ===\n");
}

void verificar_alocacao(void *ponteiro)
{
    if (ponteiro == NULL)
    {
        mensagem_erro("Erro: Falha na alocacao de memoria.");
        exit(1);
    }
}
