// ===================================================
// ARQUIVO: mainRL.c (COMPLETO E CORRIGIDO)
// ===================================================

#include "raylib.h"
#include "../includerl/mapaRL.h"
#include "../includerl/balasRL.h"
#include "../includerl/soldadoRL.h"
#include <stdlib.h> // Para NULL
#include <stdio.h>  // Para TextFormat

// ----------------------------------------------------
// ESTADOS DO JOGO
// ----------------------------------------------------
typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_FIM
} EstadoJogo;

// ----------------------------------------------------
// DIMENSÕES (Pode ajustar como quiser)
// ----------------------------------------------------
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 900;

// ----------------------------------------------------
// MAIN
// ----------------------------------------------------
int main(void) {
    // --- INICIALIZAÇÃO DA JANELA ---
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Last Man - Edição Raylib");
    SetTargetFPS(60);

    // --- ESTADO INICIAL ---
    EstadoJogo estadoAtual = TELA_MENU;

    // --- CARREGAR RECURSOS (Texturas) ---
    // !!! VOCÊ PRECISA CRIAR ESSES ARQUIVOS DE IMAGEM !!!
    // Crie uma pasta 'resources' e coloque as imagens lá.
    // 
    // 
    // 
    Texture2D texturaBala = LoadTexture("resources/bala.png");
    // (Texturas do mapa e soldado são carregadas dentro de criar_mapa/criar_soldado)

    // --- CRIAR OBJETOS DO JOGO (Usando as novas funções) ---
    
    // 1. MAPA
    // O "abrigo" será uma faixa de 50 pixels no topo da tela
    Rectangle areaVitoria = { 0, 0, SCREEN_WIDTH, 50 };
    Mapa *mapa = criar_mapa("resources/campo.png", areaVitoria);
    if (!mapa) { /* Erro ao carregar mapa */ }

    // 2. SOLDADO
    // Posição inicial: centro, na parte de baixo da tela
    Vector2 posInicialSoldado = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 60.0f };
    // Dimensões do soldado (ex: 32x32 pixels) - AJUSTE PARA O TAMANHO DA SUA IMAGEM
    float soldWidth = 32.0f;
    float soldHeight = 32.0f;
    float soldVel = 250.0f; // 250 pixels por segundo
    Soldado *jogador = criar_soldado(posInicialSoldado, 3, soldVel, soldWidth, soldHeight, "resources/soldado.png");
    if (!jogador) { /* Erro ao carregar jogador */ }

    // 3. BALAS
    ListaBalas listaBalas;
    inicio_lista_balas(&listaBalas);
    // Dimensões da bala (ex: 10x10 pixels) - AJUSTE PARA O TAMANHO DA SUA IMAGEM
    float balaWidth = 10.0f;
    float balaHeight = 10.0f;
    
    // Timer para spawn de balas
    float spawnTimer = 0.0f;
    float spawnIntervalo = 0.3f; // Nova bala a cada 0.3 segundos

    // Timer do jogo (para recorde)
    float tempoDeJogo = 0.0f;

    // --- LOOP PRINCIPAL (Máquina de Estados) ---
    while (!WindowShouldClose()) {

        switch (estadoAtual) {
            
            // ==========================================
            // TELA_MENU (O "Lobby")
            // ==========================================
            case TELA_MENU:
            {
                // --- LÓGICA DO MENU ---
                if (IsKeyPressed(KEY_ENTER)) {
                    // Resetar jogo para começar
                    jogador->vida = 3;
                    reset_posicao_soldado(jogador);
                    liberar_lista_balas(&listaBalas); // Limpa balas se houver
                    tempoDeJogo = 0.0f;
                    estadoAtual = TELA_JOGO;
                }
            
                // --- DESENHO DO MENU ---
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("THE LAST MAN", SCREEN_WIDTH / 2 - MeasureText("THE LAST MAN", 40) / 2, SCREEN_HEIGHT / 3, 40, WHITE);
                DrawText("Pressione ENTER para começar", SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para começar", 20) / 2, SCREEN_HEIGHT / 2, 20, GRAY);
                DrawText("W, A, S, D para mover", SCREEN_WIDTH / 2 - MeasureText("W, A, S, D para mover", 20) / 2, SCREEN_HEIGHT / 2 + 30, 20, GRAY);
                EndDrawing();
            }
            break;
            
            // ==========================================
            // TELA_JOGO (O Jogo Rodando)
            // ==========================================
            case TELA_JOGO:
            {
                // --- LÓGICA DE UPDATE ---
                
                // 1. Atualizar Soldado (Input e Movimento)
                atualizar_soldado(jogador, SCREEN_WIDTH, SCREEN_HEIGHT);
                
                // 2. Atualizar Balas (Movimento e Despawn)
                atualizar_balas(&listaBalas, SCREEN_WIDTH, SCREEN_HEIGHT);
                
                // 3. Spawn de Novas Balas (Vindo dos lados)
                spawnTimer += GetFrameTime();
                if (spawnTimer >= spawnIntervalo) {
                    spawnTimer = 0.0f;
                    
                    // Posição Y aleatória (evita topo e base)
                    float posY = (float)GetRandomValue(60, SCREEN_HEIGHT - 100);
                    // Velocidade X aleatória
                    float velX = (float)GetRandomValue(150, 300);
                    
                    Vector2 pos, vel;
                    
                    if (GetRandomValue(0, 1) == 0) {
                        // Nasce na Esquerda
                        pos = (Vector2){ -balaWidth, posY }; // Fora da tela
                        vel = (Vector2){ velX, 0 }; // Move para direita
                    } else {
                        // Nasce na Direita
                        pos = (Vector2){ (float)SCREEN_WIDTH, posY }; // Fora da tela
                        vel = (Vector2){ -velX, 0 }; // Move para esquerda
                    }
                    
                    Bala *b = criar_bala(pos, vel, 1, balaWidth, balaHeight);
                    adicionar_bala(&listaBalas, b);
                }
                
                // 4. Checar Colisão (Soldado vs Balas)
                Bala *b = listaBalas.head;
                while (b) {
                    Bala *prox = b->proxima; // Salva o próximo (importante!)
                    
                    // A "mágica" da Raylib
                    if (CheckCollisionRecs(jogador->colisao, b->colisao)) {
                        aplicar_dano_soldado(jogador, b->dano);
                        remover_bala(&listaBalas, b); // Remove a bala que colidiu
                        
                        if (jogador->vida <= 0) {
                            estadoAtual = TELA_FIM;
                        }
                    }
                    b = prox; // Vai para o próximo (seguro)
                }

                // 5. Checar Vitória (Soldado vs Abrigo)
                if (checar_vitoria_mapa(mapa, jogador->colisao)) {
                    // (Aqui você salvaria o recorde 'tempoDeJogo')
                    estadoAtual = TELA_FIM;
                }
                
                // 6. Atualizar timer
                tempoDeJogo += GetFrameTime();

                // --- DESENHO DO JOGO ---
                BeginDrawing();
                ClearBackground(BLACK);
                
                desenhar_mapa(mapa);        // Desenha a imagem de fundo
                desenhar_soldado(jogador);  // Desenha o soldado (textura)
                desenhar_balas(&listaBalas, texturaBala); // Desenha todas as balas (textura)
                
                // HUD (Interface)
                DrawText(TextFormat("VIDA: %d", jogador->vida), 10, SCREEN_HEIGHT - 30, 20, RED);
                DrawText(TextFormat("TEMPO: %.1f", tempoDeJogo), SCREEN_WIDTH - 150, SCREEN_HEIGHT - 30, 20, WHITE);
                
                EndDrawing();
            }
            break;

            // ==========================================
            // TELA_FIM (Game Over ou Vitória)
            // ==========================================
            case TELA_FIM:
            {
                // --- LÓGICA DO FIM ---
                if (IsKeyPressed(KEY_ENTER)) {
                    estadoAtual = TELA_MENU;
                }
                
                // --- DESENHO DO FIM ---
                BeginDrawing();
                // (Opcional) Desenha a última tela do jogo por trás
                desenhar_mapa(mapa);
                desenhar_soldado(jogador);
                // Escurece a tela
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
                
                const char* textoFim;
                if (jogador->vida <= 0) {
                    textoFim = "VOCÊ MORREU";
                } else {
                    textoFim = "MISSÃO CUMPRIDA";
                    // Esta é a linha que estava quebrada (AGORA CORRIGIDA)
                    DrawText(TextFormat("Seu tempo: %.2f segundos", tempoDeJogo), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Seu tempo: %.2f segundos", tempoDeJogo), 20) / 2, SCREEN_HEIGHT / 2, 20, WHITE);
                }
                
                DrawText(textoFim, SCREEN_WIDTH / 2 - MeasureText(textoFim, 40) / 2, SCREEN_HEIGHT / 3, 40, WHITE);
                DrawText("Pressione ENTER para voltar ao Menu", SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 20) / 2, SCREEN_HEIGHT / 2 + 40, 20, GRAY);
                
                EndDrawing();
            }
            break;
        }
    }

    // --- LIMPEZA (Muito importante) ---
    liberar_mapa(mapa);
    liberar_soldado(jogador);
    liberar_lista_balas(&listaBalas);
    UnloadTexture(texturaBala);
    
    CloseWindow();
    return 0;
}