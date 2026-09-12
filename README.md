# Trabalho Prático: Análise Empírica e Assintótica de Algoritmos de Ordenação

Projeto desenvolvido para a disciplina de **Projeto e Análise de Algoritmos (PAA)** do curso de **Sistemas de Informação** da **Universidade Federal do Piauí (UFPI - CSHNB)**, sob a orientação do **Prof. Raí Araújo de Miranda**.

O trabalho tem como objetivo a implementação na linguagem **C**, análise teórica de complexidade (notação assintótica) e medição empírica de desempenho de dois algoritmos de ordenação sorteados, confrontando a teoria com a prática sob diferentes volumetrias e disposições de dados.

---

## 📌 Algoritmos Analisados

| Algoritmo            |  Grupo  |          Tipo / Paradigma          |        Melhor Caso        |        Caso Médio        |         Pior Caso         | Memória Auxiliar | Estabilidade |
| :------------------- | :-----: | :---------------------------------: | :-----------------------: | :-----------------------: | :-----------------------: | :----------------: | :----------: |
| **Treesort**   | Grupo A |   Árvore Binária de Busca (BST)   | $\mathcal{O}(n \log n)$ | $\mathcal{O}(n \log n)$ |   $\mathcal{O}(n^2)$   | $\mathcal{O}(n)$ |   Estável   |
| **Block Sort** | Grupo B | Divisão em Blocos / Merge in-place |    $\mathcal{O}(n)$    | $\mathcal{O}(n \log n)$ | $\mathcal{O}(n \log n)$ | $\mathcal{O}(1)$ |   Estável   |

---

## 🚀 Como Compilar e Executar

### 1. Compilação (GCC)

Execute o comando abaixo na raiz do projeto para compilar os módulos com as flags de verificação:

```bash
gcc -Wall -Wextra -std=c99 -Iinclude main.c src/*.c -lm -o trabalho.exe
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

---

## 👥 Autores

Desenvolvido por:

- [Alysson](https://github.com/aIlyson)
- [Maria Júlia](https://github.com/Maju-sousa)

---

## 📄 Licença

Este projeto está sob a licença [MIT](LICENSE).
