#include "raylib.h"
#include "../includerl/mapaRL.h"
#include "../includerl/balasRL.h"
#include "../includerl/soldadoRL.h"
#include <stdlib.h>
#include <stdio.h> 

typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_FIM
} EstadoJogo;

int main(void) {
    const int SCREEN_WIDTH = GetMonitorWidth(0);
    const int SCREEN_HEIGHT = GetMonitorHeight(0);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Last Man - Edição Raylib");
    ToggleFullscreen();
    SetTargetFPS(60);

    EstadoJogo estadoAtual = TELA_MENU;

    float balaWidth = 10.0f;
    float balaHeight = 10.0f;

    Image imgBala = LoadImage("resources/bala.png");
    ImageResize(&imgBala, (int)balaWidth, (int)balaHeight);
    Texture2D texturaBala = LoadTextureFromImage(imgBala);
    UnloadImage(imgBala);

    Rectangle areaVitoria = { 0, 0, SCREEN_WIDTH, 50 };
    Mapa *mapa = criar_mapa("resources/campo.png", areaVitoria, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!mapa) { return 1; }

    Vector2 posInicialSoldado = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 60.0f };
    float soldWidth = 32.0f;
    float soldHeight = 32.0f;
    float soldVel = 250.0f; 
    Soldado *jogador = criar_soldado(posInicialSoldado, 3, soldVel, soldWidth, soldHeight, "resources/soldado.png");
    if (!jogador) { return 1; }

    ListaBalas listaBalas;
    inicio_lista_balas(&listaBalas);
    
    float spawnTimer = 0.0f;
    float spawnIntervalo = 0.3f; 
    float tempoDeJogo = 0.0f;

    while (!WindowShouldClose()) {

        switch (estadoAtual) {
            
            case TELA_MENU:
            {
                if (IsKeyPressed(KEY_ENTER)) {
                    jogador->vida = 3;
                    reset_posicao_soldado(jogador);
                    liberar_lista_balas(&listaBalas);
                    tempoDeJogo = 0.0f;
                    estadoAtual = TELA_JOGO;
                }
            
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("THE LAST MAN", SCREEN_WIDTH / 2 - MeasureText("THE LAST MAN", 40) / 2, SCREEN_HEIGHT / 3, 40, WHITE);
                DrawText("Pressione ENTER para começar", SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para começar", 20) / 2, SCREEN_HEIGHT / 2, 20, GRAY);
                DrawText("W, A, S, D para mover", SCREEN_WIDTH / 2 - MeasureText("W, A, S, D para mover", 20) / 2, SCREEN_HEIGHT / 2 + 30, 20, GRAY);
                EndDrawing();
            }
            break;
            
            case TELA_JOGO:
            {
                atualizar_soldado(jogador, SCREEN_WIDTH, SCREEN_HEIGHT);
                atualizar_balas(&listaBalas, SCREEN_WIDTH, SCREEN_HEIGHT);
                
                spawnTimer += GetFrameTime();
                if (spawnTimer >= spawnIntervalo) {
                    spawnTimer = 0.0f;
                    
                    float posY = (float)GetRandomValue(60, SCREEN_HEIGHT - 100);
                    float velX = (float)GetRandomValue(150, 300);
                    
                    Vector2 pos, vel;
                    
                    if (GetRandomValue(0, 1) == 0) {
                        pos = (Vector2){ -balaWidth, posY }; 
                        vel = (Vector2){ velX, 0 };
                    } else {
                        pos = (Vector2){ (float)SCREEN_WIDTH, posY };
                        vel = (Vector2){ -velX, 0 };
                    }
                    
                    Bala *b = criar_bala(pos, vel, 1, balaWidth, balaHeight);
                    adicionar_bala(&listaBalas, b);
                }
                
                Bala *b = listaBalas.head;
                while (b) {
                    Bala *prox = b->proxima;
                    
                    if (CheckCollisionRecs(jogador->colisao, b->colisao)) {
                        aplicar_dano_soldado(jogador, b->dano);
                        remover_bala(&listaBalas, b);
                        
                        if (jogador->vida <= 0) {
                            estadoAtual = TELA_FIM;
                        }
                    }
                    b = prox;
                }

                if (checar_vitoria_mapa(mapa, jogador->colisao)) {
                    estadoAtual = TELA_FIM;
                }
                
                tempoDeJogo += GetFrameTime();

                BeginDrawing();
                ClearBackground(BLACK);
                
                desenhar_mapa(mapa);
                
                BeginBlendMode(BLEND_ALPHA);
                desenhar_soldado(jogador);
                desenhar_balas(&listaBalas, texturaBala);
                EndBlendMode();
                
                DrawText(TextFormat("VIDA: %d", jogador->vida), 10, SCREEN_HEIGHT - 30, 20, RED);
                DrawText(TextFormat("TEMPO: %.1f", tempoDeJogo), SCREEN_WIDTH - 150, SCREEN_HEIGHT - 30, 20, WHITE);
                
                EndDrawing();
            }
            break;

            case TELA_FIM:
            {
                if (IsKeyPressed(KEY_ENTER)) {
                    estadoAtual = TELA_MENU;
                }
                
                BeginDrawing();
                desenhar_mapa(mapa);
                
                BeginBlendMode(BLEND_ALPHA);
                desenhar_soldado(jogador);
                EndBlendMode();

                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
                
                const char* textoFim;
                if (jogador->vida <= 0) {
                    textoFim = "VOCÊ MORREU";
                } else {
                    textoFim = "MISSÃO CUMPRIDA";
                    DrawText(TextFormat("Seu tempo: %.2f segundos", tempoDeJogo), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Seu tempo: %.2f segundos", tempoDeJogo), 20) / 2, SCREEN_HEIGHT / 2, 20, WHITE);
                }
                
                DrawText(textoFim, SCREEN_WIDTH / 2 - MeasureText(textoFim, 40) / 2, SCREEN_HEIGHT / 3, 40, WHITE);
                DrawText("Pressione ENTER para voltar ao Menu", SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 20) / 2, SCREEN_HEIGHT / 2 + 40, 20, GRAY);
                
                EndDrawing();
            }
            break;
        }
    }

    liberar_mapa(mapa);
    liberar_soldado(jogador);
    liberar_lista_balas(&listaBalas);
    UnloadTexture(texturaBala);
    
    CloseWindow();
    return 0;
}
