#!/usr/bin/env python3
"""Gera os graficos comparativos a partir do CSV dos experimentos.

- Le exclusivamente os valores atuais de
  experimentos/resultados/resultados_comparativos.csv (somente leitura).
- Nunca altera o CSV e nao reutiliza resultados de execucoes anteriores.
- Valida as 18 linhas contra o CSV antes de gerar qualquer imagem.

Execute a partir da raiz do projeto: python tools/graficos.py
"""

import csv
import os
import sys

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

ARQUIVO_CSV = os.path.join("experimentos", "resultados", "resultados_comparativos.csv")
PASTA_SAIDA = os.path.join("experimentos", "graficos")
CAMPOS_ESPERADOS = ["algoritmo", "tamanho", "tipo", "tempo_segundos"]
ALGORITMOS = ["Tree Sort", "Block Sort"]
TIPOS = ["crescente", "decrescente", "aleatorio"]
TAMANHOS = [20000, 60000, 80000]
CORES_ALGORITMO = {"Tree Sort": "#1f77b4", "Block Sort": "#ff7f0e"}
CORES_TIPO = {"crescente": "#9467bd", "decrescente": "#8c564b", "aleatorio": "#e377c2"}
ESTILOS_TIPO = {"crescente": "-", "decrescente": "--", "aleatorio": ":"}


def falhar(mensagem):
    print(f"Erro: {mensagem}", file=sys.stderr)
    sys.exit(1)


def formatar_eixo(valor, _posicao):
    return f"{int(valor):,}".replace(",", ".")


def formatar_razao_eixo(valor, _posicao):
    return f"{valor:,.0f}".replace(",", ".")


def formatar_razao_texto(valor):
    texto = f"{valor:.1f}"
    if "." in texto:
        inteiro, decimal = texto.split(".")
    else:
        inteiro, decimal = texto, "0"
    return f"{int(inteiro):,}".replace(",", ".") + "," + decimal + "x"


def carregar_dados(caminho):
    linhas = []
    dados = {}

    try:
        with open(caminho, newline="", encoding="utf-8") as arquivo:
            leitor = csv.DictReader(arquivo)
            if leitor.fieldnames != CAMPOS_ESPERADOS:
                falhar(f"cabecalho do CSV inesperado: {leitor.fieldnames}")
            for linha in leitor:
                linhas.append(linha)
                chave = (linha["algoritmo"], linha["tipo"])
                dados.setdefault(chave, []).append(
                    (int(linha["tamanho"]), float(linha["tempo_segundos"]))
                )
    except FileNotFoundError:
        falhar(f"'{caminho}' nao encontrado. Rode antes a opcao 3 (Medicao de Desempenho).")
    except (TypeError, ValueError, KeyError) as erro:
        falhar(f"linha invalida em '{caminho}': {erro}")

    for pontos in dados.values():
        pontos.sort()

    return dados, linhas


def exibir_valores(linhas):
    print(f"Valores lidos de {ARQUIVO_CSV}:")
    for linha in linhas:
        print(f"  {linha['algoritmo']:<11} | {linha['tamanho']:>5} | "
              f"{linha['tipo']:<11} | {linha['tempo_segundos']} s")


def validar_dados(dados):
    combos = {(algoritmo, tipo) for algoritmo in ALGORITMOS for tipo in TIPOS}
    obtidos = set(dados.keys())

    if obtidos != combos:
        faltam = sorted(combos - obtidos)
        sobram = sorted(obtidos - combos)
        falhar(f"combinações algoritmo/tipo divergentes. Faltam: {faltam}; Sobram: {sobram}")

    total = 0
    for (algoritmo, tipo), pontos in sorted(dados.items()):
        tamanhos = [tamanho for tamanho, _ in pontos]
        if len(pontos) != len(TAMANHOS) or tamanhos != TAMANHOS:
            falhar(f"tamanhos divergentes para {algoritmo}/{tipo}: "
                   f"{tamanhos} (esperado {TAMANHOS}; duplicatas ou ausentes)")
        for tamanho, tempo in pontos:
            if tempo <= 0:
                falhar(f"tempo nao positivo em {algoritmo}/{tipo}/{tamanho}: "
                       f"{tempo} (impossivel plotar em escala logaritmica)")
        total += len(pontos)

    esperado_total = len(ALGORITMOS) * len(TIPOS) * len(TAMANHOS)
    if total != esperado_total:
        falhar(f"total de registros = {total}, esperado {esperado_total}")


def serie(dados, algoritmo, tipo):
    return dados.get((algoritmo, tipo), [])


def pontos_razao(dados, tipo):
    razoes = []
    arvore = serie(dados, "Tree Sort", tipo)
    bloco = serie(dados, "Block Sort", tipo)

    for (n_arvore, t_arvore), (n_bloco, t_bloco) in zip(arvore, bloco):
        if n_arvore != n_bloco:
            falhar(f"tamanhos inconsistentes entre algoritmos no tipo {tipo}")
        razoes.append((n_arvore, t_arvore / t_bloco))

    return razoes


def desenhar_serie_linear(eixo, dados, algoritmo, tipo):
    pontos = serie(dados, algoritmo, tipo)
    eixo.plot([p[0] for p in pontos], [p[1] for p in pontos],
              marker="o", color=CORES_ALGORITMO[algoritmo], label=algoritmo)


def configurar_eixo_linear(eixo, titulo):
    eixo.set_title(titulo)
    eixo.set_xlabel("Tamanho do vetor (N)")
    eixo.set_ylabel("Tempo de execucao (s)")
    eixo.set_xticks(TAMANHOS)
    eixo.xaxis.set_major_formatter(FuncFormatter(formatar_eixo))
    eixo.grid(True, linestyle="--", alpha=0.5)
    eixo.legend()


def plotar_tipo(dados, tipo):
    caminho = os.path.join(PASTA_SAIDA, f"grafico_{tipo}.png")
    fig, eixo = plt.subplots(figsize=(8, 5))

    for algoritmo in ALGORITMOS:
        desenhar_serie_linear(eixo, dados, algoritmo, tipo)

    configurar_eixo_linear(eixo, f"Tree Sort vs Block Sort - dados {tipo} (escala linear)")
    fig.tight_layout()
    fig.savefig(caminho, dpi=150)
    plt.close(fig)
    print(f"Gerado: {caminho}")


def desenhar_razao(eixo, dados):
    for tipo in TIPOS:
        pontos = pontos_razao(dados, tipo)
        eixo.plot([p[0] for p in pontos], [p[1] for p in pontos],
                  marker="o", color=CORES_TIPO[tipo], label=tipo)
        for tamanho, razao in pontos:
            eixo.annotate(formatar_razao_texto(razao), (tamanho, razao),
                          textcoords="offset points", xytext=(0, 7),
                          ha="center", fontsize=7)

    eixo.axhline(1.0, color="gray", linestyle=":", linewidth=1,
                 label="empate (1x)")
    eixo.set_yscale("log")
    eixo.set_title("Razao de tempo Tree Sort / Block Sort (log)")
    eixo.set_xlabel("Tamanho do vetor (N)")
    eixo.set_ylabel("Tempo(Tree Sort) / Tempo(Block Sort)")
    eixo.set_xticks(TAMANHOS)
    eixo.xaxis.set_major_formatter(FuncFormatter(formatar_eixo))
    eixo.yaxis.set_major_formatter(FuncFormatter(formatar_razao_eixo))
    eixo.grid(True, linestyle="--", alpha=0.5)
    eixo.legend()


def plotar_painel(dados):
    caminho = os.path.join(PASTA_SAIDA, "grafico_geral_comparativo.png")
    fig, eixos = plt.subplots(2, 2, figsize=(12, 8))

    for eixo, tipo in zip(eixos.flat[:3], TIPOS):
        for algoritmo in ALGORITMOS:
            desenhar_serie_linear(eixo, dados, algoritmo, tipo)
        configurar_eixo_linear(eixo, f"Tipo: {tipo} (escala linear)")

    desenhar_razao(eixos.flat[3], dados)

    fig.suptitle("Comparação de desempenho: Tree Sort x Block Sort", fontsize=14)
    fig.tight_layout(rect=(0, 0, 1, 0.96))
    fig.savefig(caminho, dpi=150)
    plt.close(fig)
    print(f"Gerado: {caminho}")


def plotar_escala_logaritmica(dados):
    caminho = os.path.join(PASTA_SAIDA, "grafico_escala_logaritmica.png")
    fig, eixo = plt.subplots(figsize=(9, 5.5))

    for tipo in TIPOS:
        pontos = pontos_razao(dados, tipo)
        eixo.plot([p[0] for p in pontos], [p[1] for p in pontos],
                  marker="o", linestyle=ESTILOS_TIPO[tipo],
                  color=CORES_TIPO[tipo],
                  label=tipo)
        for tamanho, razao in pontos:
            eixo.annotate(formatar_razao_texto(razao), (tamanho, razao),
                          textcoords="offset points", xytext=(0, 7),
                          ha="center", fontsize=7)

    eixo.axhline(1.0, color="gray", linestyle=":", linewidth=1,
                 label="empate (1x)")
    eixo.set_yscale("log")
    eixo.set_title("Razao Tempo(Tree Sort) / Tempo(Block Sort) - escala logaritmica")
    eixo.set_xlabel("Tamanho do vetor (N)")
    eixo.set_ylabel("Razao Tempo(Tree Sort) / Tempo(Block Sort)")
    eixo.set_xticks(TAMANHOS)
    eixo.xaxis.set_major_formatter(FuncFormatter(formatar_eixo))
    eixo.yaxis.set_major_formatter(FuncFormatter(formatar_razao_eixo))
    eixo.grid(True, linestyle="--", alpha=0.5)
    eixo.legend(fontsize=8)
    fig.tight_layout()
    fig.savefig(caminho, dpi=150)
    plt.close(fig)
    print(f"Gerado: {caminho}")


def main():
    dados, linhas = carregar_dados(ARQUIVO_CSV)
    exibir_valores(linhas)
    validar_dados(dados)
    print(f"Validacao OK: {len(linhas)} registros conferem com o CSV.\n")

    os.makedirs(PASTA_SAIDA, exist_ok=True)

    for tipo in TIPOS:
        plotar_tipo(dados, tipo)
    plotar_painel(dados)
    plotar_escala_logaritmica(dados)

    print(f"\nTodos os graficos foram gerados na pasta '{PASTA_SAIDA}/'.")


if __name__ == "__main__":
    main()
