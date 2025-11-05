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

TheLastMan/

├── src/ # Código-fonte do jogo

├── include/ # Headers (.h) das funções e structs

├── data/ # Arquivos de recorde e configuração

└── cli-lib/ # Biblioteca CLI para renderizar o jogo no terminal

## ▶️ Executando o Jogo

Para executar o **TheLastMan**, siga estas etapas:

1. Clone este repositório em sua máquina:

git@github.com:VR0414/TheLastMan.git

2. Entre na pasta do projeto:

cd TheLastMan

3. Compile o programa:

gcc ./src/*.c cli-lib/src/*.c -I./include -I./cli-lib/include -o TheLastMan -std=c99 -Wall -Wextra

4. Rode o programa:

./TheLastMan

5. Divirta-se!
