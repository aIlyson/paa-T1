# Trabalho Prático: Análise Empírica e Assintótica de Algoritmos de Ordenação

Projeto desenvolvido para a disciplina de **Projeto e Análise de Algoritmos (PAA)** do curso de **Sistemas de Informação** da **Universidade Federal do Piauí (UFPI - CSHNB)**, sob a orientação do **Prof. Raí Araújo de Miranda**.

O trabalho tem como objetivo a implementação na linguagem **C**, análise teórica de complexidade (notação assintótica) e medição empírica de desempenho de dois algoritmos de ordenação sorteados, confrontando a teoria com a prática sob diferentes volumetrias e disposições de dados.

---

## 📌 Algoritmos Analisados

| Algoritmo      | Grupo |                     Tipo / Paradigma                     | Melhor Caso |    Caso Médio     |    Pior Caso     |  Memória Auxiliar   | Estabilidade |
| :------------- | :---: | :------------------------------------------------------: | :---------: | :---------------: | :--------------: | :-----------------: | :----------: |
| **Treesort**   |   A   |                 Árvore Binária de Busca (BST)            | O(n log n)  |    O(n log n)     |      O(n²)       |        O(n)         |    Sim       |
| **Block Sort** |   B   |     Subdivisão em Blocos / Merge in-place (rotações)     | O(n log n)   |   O(n log² n)     |   O(n log² n)    |       O(log n)       |    Sim       |

> **Notas de implementação**
>
> - **Treesort:** inserção, percurso em-ordem e liberação da árvore são **100% iterativos** (pilha explícita), o que elimina stack overflow nos vetores degenerados de 60.000 e 80.000 elementos. Valores iguais são inseridos à direita, preservando a estabilidade.
> - **Block Sort:** o merge é realizado por **rotações in-place** (busca binária `lower_bound`/`upper_bound` + triplo-reverse), **estável**, sem vetor auxiliar de tamanho N; a pilha de recursão ocupa O(log n). Como essas rotações podem percorrer uma quantidade linear de elementos e o procedimento de intercalação é executado em múltiplos níveis, a estratégia implementada apresenta O(n log² n) no caso médio e no pior caso, e O(n log n) no melhor caso. Para entradas já ordenadas, cada intercalação detecta que os blocos já estão na ordem correta e evita as rotações — uma otimização de caminho feliz que explica os tempos muito baixos medidos nesse cenário, sem que a complexidade anunciada seja inferior a O(n log n). Existem variantes mais sofisticadas de Block Merge Sort (ex.: WikiSort, com intercâmbio de blocos, que atinge O(n log n)) que conseguem melhores limites, mas não são a implementação utilizada neste trabalho.

---

## 🗂️ Estrutura do Projeto

```
paa-T1/
├── main.c                              # ponto de entrada (menu)
│
├── include/                            # sistema — cabeçalhos (tree_sort, block_sort, utils, measure_time, menu, security)
├── src/                                # sistema — implementações
│
├── experimentos/                       # EXPERIMENTO — entradas e saídas
│   ├── dados/                          # ENTRADAS: 9 conjuntos (20k/60k/80k × crescente/decrescente/aleatório)
│   ├── resultados/
│   │   └── resultados_comparativos.csv # SAÍDAS numéricas: 18 medições (2 algoritmos × 9 cenários)
│   └── graficos/
│       └── grafico_*.png               # VISUALIZAÇÕES: 5 PNGs (3 lineares + painel geral + razão em escala logarítmica)
│
├── tools/                              # ferramentas auxiliares
│   ├── gerador.c                       # gera os vetores de teste (seed fixa)
│   └── graficos.py                     # gera os gráficos a partir do CSV
│
├── README.md
├── LICENSE
└── .gitignore
```

---

## 🚀 Como Compilar e Executar

### 1. Compilação (GCC)

Execute o comando abaixo na raiz do projeto para compilar os módulos com as flags de verificação:

```bash
gcc -Wall -Wextra -std=c99 -Iinclude -O2 main.c src/*.c -lm -o trabalho.exe
```

### 2. Execução

- **Windows (PowerShell / CMD):**

  ```powershell
  .\trabalho.exe
  ```
- **Linux / macOS:**

  ```bash
  ./trabalho.exe
  ```

### 3. Menu

| Opção | Descrição |
| :---: | :-------- |
| `1` | Demonstração do **Tree Sort** passo a passo (vetor pequeno: inserções na BST + percurso em-ordem) |
| `2` | Demonstração do **Block Sort** passo a passo (vetor pequeno: blocos, busca binária, rotação e merge) |
| `3` | **Medição de desempenho:** 9 cenários × 2 algoritmos = 18 execuções → `experimentos/resultados/resultados_comparativos.csv` |
| `4` | Informações teóricas (complexidade, memória) e ambiente de execução (SO, CPU, RAM, compilador) |
| `0` | Sair |

### 4. Medição de desempenho

A opção `3` carrega cada um dos 9 arquivos de `experimentos/dados/` e roda os dois algoritmos sobre **cópias do mesmo vetor original**, validando ordenação **e integridade** (checksum soma + XOR) antes de gravar o CSV:

```csv
algoritmo,tamanho,tipo,tempo_segundos
Tree Sort,20000,crescente,0.473093
Block Sort,20000,crescente,0.000037
...
```

O tempo é medido com `QueryPerformanceCounter`/`QueryPerformanceFrequency` (resolução submicrosegundo — tipicamente ~100 ns por tique no Windows —, bem superior aos 1 ms de `clock()` no MinGW).

### 5. Gráficos

Com o CSV gerado (opção `3`), produza os 5 gráficos comparativos: 3 lineares individuais por tipo (N vs. tempo em segundos), o painel 2×2 (os 3 lineares + a razão Tempo(Tree Sort)/Tempo(Block Sort) em log) e o gráfico dedicado da razão Tempo(Tree Sort)/Tempo(Block Sort) em escala logarítmica:

```bash
python tools/graficos.py
```

> Requer `matplotlib` (`pip install matplotlib`). O script lê somente o CSV atual (nunca o altera), valida as 18 linhas e salva os PNGs em `experimentos/graficos/`.

### 6. Gerador de dados (opcional)

Os 9 arquivos de teste já estão em `experimentos/dados/`. Para regenerá-los (na raiz do projeto):

```bash
gcc -Wall -Wextra -std=c99 -O2 tools/gerador.c -o gerador.exe
.\gerador.exe   # grava em experimentos/dados/
```

---

## 👥 Autores

Desenvolvido por:

- [Alysson](https://github.com/aIlyson)
- [Maria Júlia](https://github.com/Maju-sousa)

---

## 📄 Licença

Este projeto está sob a licença [MIT](LICENSE).
