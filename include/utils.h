#ifndef UTILS_H
#define UTILS_H


int *ler_arquivo(const char *caminho, int *tamanho);


int *copiar_vetor(const int *origem, int tamanho);

double tempo_atual(void);

int esta_ordenado(const int *vetor, int tamanho);

#endif
