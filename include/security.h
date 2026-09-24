#ifndef SECURITY_H
#define SECURITY_H

#include <stddef.h>

void limpar_buffer();
void pausar_tela();
void limpar_tela();

void pausa_passo(void);
void pausa_fase(void);

void cor_azul(void);
void cor_amarela(void);
void cor_vermelha(void);
void cor_verde(void);
void cor_normal(void);

void print_azul(char *mensagem);
void print_amarelo(char *mensagem);
void print_vermelho(char *mensagem);
void print_verde(char *mensagem);

void mensagem_erro(char *mensagem);
void mensagem_sucesso(char *mensagem);
void mensagem_info(char *mensagem);

void print_titulo(char *titulo);
void verificar_alocacao(void *ponteiro);

#endif
