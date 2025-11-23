# 🪖 TheLastMan 💥

TheLastMan é um jogo em **C**, no estilo **Top-Down**, inspirado no clássico *Frogger*.

O jogador controla um **soldado**, que precisa atravessar um campo de batalha cheio de balas de tiroteio e chegar a um **abrigo seguro**, evitando colisões.

O objetivo do jogo é chegar ao topo da tela em segurança, completando a missão no menor tempo possível.

---

## 🕹️ Gameplay

- O jogador controla o **soldado** usando as teclas **W, A, S, D** ou setas do teclado.
- Balas aparecem aleatoriamente no campo de batalha e o jogador deve **desviar de todas**.
- Ao chegar ao **abrigo**, o tempo de conclusão é registrado em um arquivo de **recordes**.
- O jogo possui **uma missão/fase única**, com colisão e movimento em tempo real.

---

## 📁 Estrutura do Projeto

Abaixo está a estrutura de arquivos principal utilizada no projeto:

TheLastMan/
├── data/ # Arquivos de recorde
├── resources/ # Arquivos para imagens (.png) do jogo
└── raylib/
    ├── includerl/ # Headers (.h) das funções e structs (mapaRL.h, soldadoRL.h, etc.)
    └── srcrl/ # Código-fonte do jogo (.c)

---

## ▶️ Executando o Jogo

Para executar o **TheLastMan**, siga estas etapas no terminal:

### 1. Compilação (Onde o Código Está)

Navegue até o diretório de código-fonte e compile, incluindo todos os módulos e as flags do Raylib (`-I../includerl` e `-lraylib`):

```bash
# Navegue para o diretório de código-fonte
cd raylib/srcrl

# Compile o programa, gerando o binário 'jogo'
gcc -o jogo mainRL.c mapaRL.c soldadoRL.c balasRL.c scoresRL.c -I../includerl -lraylib -lm
