# 🪖 TheLastMan 💥

TheLastMan é um jogo em **C**, no estilo **Top-Down**, inspirado no clássico *Frogger*.

O jogador controla um **soldado**, que precisa atravessar um campo de batalha cheio de balas de tiroteio e chegar a um **abrigo seguro**, evitando colisões.

O objetivo do jogo é chegar ao topo da tela em segurança, completando a missão no menor tempo possível.

---

## 🕹️ Gameplay

* O jogador controla o **soldado** usando as teclas **W, A, S, D** ou setas do teclado.
* Balas aparecem aleatoriamente no campo de batalha e o jogador deve **desviar de todas**.
* Ao chegar ao **abrigo**, o tempo de conclusão é registrado em um arquivo de **recordes**.
* O jogo possui **uma missão/fase única**, com colisão e movimento em tempo real.

---

## 📁 Estrutura do Projeto

Abaixo está a estrutura de arquivos principal utilizada no projeto:

```text
TheLastMan/
├── data/           # Arquivos de recorde
├── resources/      # Arquivos para imagens (.png) do jogo
└── raylib/
    ├── includerl/  # Headers (.h) das funções e structs (mapaRL.h, soldadoRL.h, etc.)
    └── srcrl/      # Código-fonte do jogo (.c)
```

---

## ▶️ Executando o Jogo

### ⚠️ Pré-requisito importante

- Para compilar o jogo, você precisa ter o Raylib instalado no seu sistema.

Mais informações sobre como instalar o **Raylib**, acesse: https://www.raylib.com/

### Para executar o **TheLastMan**, siga estas etapas no terminal:

### 1. Clonando Repositório

No Ubuntu, abra o terminal e digite:

```bash
git clone https://github.com/VR0414/TheLastMan.git
```

Entra no diretório:

```bash
cd TheLastMan
```

### 2. Compilação (Pasta `raylib/srcrl/`)

Você precisa compilar dentro da pasta de código-fonte e incluir todos os módulos e as flags do Raylib.

1. Navegue para o diretório de código-fonte:

```bash
cd raylib/srcrl
```

2. Compile o programa:

```bash
gcc -o jogo mainRL.c mapaRL.c soldadoRL.c balasRL.c scoresRL.c -I../includerl -lraylib -lm
```

### 2. Execução (Pasta TheLastMan - Raiz)

Volte para a pasta raiz para que o jogo encontre a pasta `resources/` e rode o binário corretamente.

1. Volte para o diretório raiz do projeto:

```bash
cd ../..
```

2. Rode o programa:

```bash
./raylib/srcrl/jogo
```

Divirta-se!

---

## 🎮 Demonstração do Jogo

Assista abaixo ao jogo em execução:

https://github.com/user-attachments/assets/bf7a0156-f6e3-4792-907c-bbb98e0df96b

---

## 📝 Apresentação de Slides

Acesse os slides completos da apresentação:

[TheLastMan.pdf](https://github.com/user-attachments/files/23754467/TheLastMan.pdf)

---

# Como Contribuir com o TheLastMan 🪖

Obrigado pelo interesse em contribuir com o **TheLastMan**! Para garantir que suas alterações funcionem corretamente e sejam integradas, siga este guia passo a passo.

---

## 🛠️ Pré-requisitos

Antes de começar, certifique-se de ter instalado:
1. **Compilador C:** Recomendamos o **GCC**.
2. **Biblioteca Raylib:** Necessária para o funcionamento gráfico e sonoro. Certifique-se de que ela está configurada no `PATH` ou acessível pelo linker.

---

## 🚀 Guia de Desenvolvimento

Como o projeto depende de caminhos de arquivos específicos (imagens e dados), siga **exatamente** esta ordem para configurar, compilar e rodar.

---

### 1. Configuração Inicial

1. Faça um **Fork** deste repositório no GitHub.
2. Clone o seu fork localmente:

    ```bash
    git clone https://github.com/SEU-USUARIO/TheLastMan.git
    cd TheLastMan
    ```

3. Crie uma **Branch** para sua alteração (não use a main!):

    ```bash
    git checkout -b feature/minha-melhoria
    ```

---

### 2. Compilação

O código-fonte está na pasta `raylib/srcrl`, mas os headers estão em `raylib/includerl`.  
Entre na pasta correta e rode o comando de compilação:

```bash
cd raylib/srcrl

# Comando obrigatório (inclui math lib e raylib)
gcc -o jogo mainRL.c mapaRL.c soldadoRL.c balasRL.c scoresRL.c -I../includerl -lraylib -lm
```

### 3. 📝 Commit das alterações

Adicione os arquivos modificados e crie um commit com uma mensagem clara explicando o que foi feito:

```bash
# Adiciona todas as alterações
git add .

# Cria o commit (substitua a mensagem pelo que você fez)
git commit -m "Feat: Adiciona novo obstáculo na fase 1"
```

### 4. 🚀 Enviando para o GitHub (Push)

Envie a branch com suas alterações para o seu fork no GitHub:

```bash
# Envia a branch 'feature/minha-melhoria' para o remoto 'origin'
git push origin feature/minha-melhoria
```

### 5. Abrindo o Pull Request (PR)

	1.	Acesse a página do seu repositório fork no GitHub.
	2.	Clique no botão verde “Compare & pull request”.
	3.	Verifique se:
		•	base repository: é o projeto original
		•	head repository: é o seu fork
	4.	Escreva um título e uma descrição explicando o que mudou e por quê.
	5.	Clique em “Create pull request”.

---

Pronto! Agora é só aguardar a revisão. 💥

---

## 🙏 Agradecimento

Muito obrigado por dedicar tempo e esforço para contribuir com o **TheLastMan**.  
Cada melhoria, sugestão e correção ajuda o projeto a evoluir e ficar ainda melhor.  
A sua colaboração faz a diferença! 💛💥
