#include "raylib.h"
#include "../includerl/mapaRL.h"
#include "../includerl/balasRL.h"
#include "../includerl/soldadoRL.h"
#include "../includerl/scoresRL.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_FIM
} EstadoJogo;

int main(void) {
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Last Man");
    
    int monitorW = GetMonitorWidth(0);
    int monitorH = GetMonitorHeight(0);
    SetWindowPosition((monitorW - SCREEN_WIDTH)/2, (monitorH - SCREEN_HEIGHT)/2);

    SetTargetFPS(60);

    EstadoJogo estadoAtual = TELA_MENU;

    Image imgMenu = LoadImage("resources/menu.png");
    ImageResize(&imgMenu, SCREEN_WIDTH, SCREEN_HEIGHT);
    Texture2D texturaMenu = LoadTextureFromImage(imgMenu);
    UnloadImage(imgMenu);

    float balaVisualW = 65.0f;
    float balaVisualH = 20.0f;

    float balaHitboxW = 35.0f;
    float balaHitboxH = 12.0f;

    Image imgBala = LoadImage("resources/bala.png");
    ImageResize(&imgBala, (int)balaVisualW, (int)balaVisualH);
    Texture2D texturaBala = LoadTextureFromImage(imgBala);
    UnloadImage(imgBala);

    Rectangle areaVitoria = { 0, 0, SCREEN_WIDTH, 80 };
    Mapa *mapa = criar_mapa("resources/campo.png", areaVitoria, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!mapa) return 1;

    Vector2 posInicial = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 80.0f };
    float soldW = 70.0f;
    float soldH = 70.0f;
    float soldVel = 90.0f;
    
    Soldado *jogador = criar_soldado(posInicial, 3, soldVel, soldW, soldH, "resources/soldado.png");
    if (!jogador) return 1;

    ListaBalas listaBalas;
    inicio_lista_balas(&listaBalas);
    
    float spawnTimer = 0.0f;
    float spawnIntervalo = 0.3f;
    float tempoDeJogo = 0.0f;

    Score *topScores = NULL;
    topScores = carregar_scores("scores.txt");

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
                DrawTexture(texturaMenu, 0, 0, WHITE);

                DrawText("THE LAST MAN", SCREEN_WIDTH/2 - MeasureText("THE LAST MAN", 60)/2, SCREEN_HEIGHT/3, 60, BLACK);
                DrawText("PRESSIONE ENTER", SCREEN_WIDTH/2 - MeasureText("PRESSIONE ENTER", 30)/2, SCREEN_HEIGHT/2, 30, YELLOW);

                if (topScores) {
                    DrawText("TOP 3 RECORDES", SCREEN_WIDTH/2 - MeasureText("TOP 3 RECORDES", 25)/2, SCREEN_HEIGHT - 220, 25, RED);

                    Score *atual = topScores;
                    int count = 0;
                    int startY = SCREEN_HEIGHT - 190;
                    
                    while (atual != NULL && count < 3) {
                        DrawText(TextFormat("%d. %.2f s", count + 1, atual->tempo), 
                                 SCREEN_WIDTH/2 - MeasureText("9. 99.99 s", 20)/2,
                                 startY + count * 30, 
                                 20, 
                                 WHITE);
                        atual = atual->prox;
                        count++;
                    }
                }
                
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
                    
                    int linha = GetRandomValue(0, MAPA_LINHAS - 1);
                    
                    if (mapa->grade[linha][0] == 1) {
                        float posY = (linha * mapa->tileHeight) + (mapa->tileHeight / 2) - (balaVisualH / 2);
                        float velX = (float)GetRandomValue(200, 400);
                        Vector2 pos, vel;
                        
                        if (GetRandomValue(0, 1) == 0) {
                            pos = (Vector2){ -balaVisualW, posY };
                            vel = (Vector2){ velX, 0 };
                        } else {
                            pos = (Vector2){ (float)SCREEN_WIDTH, posY };
                            vel = (Vector2){ -velX, 0 };
                        }

                        adicionar_bala(&listaBalas, criar_bala(pos, vel, 1, balaHitboxW, balaHitboxH));
                    }
                }
                
                Bala *b = listaBalas.head;
                while (b) {
                    Bala *prox = b->proxima;
                    if (CheckCollisionRecs(jogador->colisao, b->colisao)) {
                        aplicar_dano_soldado(jogador, b->dano);
                        remover_bala(&listaBalas, b);
                        if (jogador->vida <= 0) estadoAtual = TELA_FIM;
                    }
                    b = prox;
                }

                if (checar_vitoria_mapa(mapa, jogador->colisao)) {
                    adicionar_score_ordenado(&topScores, tempoDeJogo);
                    salvar_scores("scores.txt", topScores);
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
                
                DrawText(TextFormat("VIDAS: %d", jogador->vida), 20, SCREEN_HEIGHT - 50, 40, RED);
                DrawText(TextFormat("TEMPO: %.1f", tempoDeJogo), SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, 40, WHITE);
                
                EndDrawing();
            }
            break;

            case TELA_FIM:
            {
                
                if (IsKeyPressed(KEY_ENTER)) estadoAtual = TELA_MENU;
                
                BeginDrawing();
                desenhar_mapa(mapa);
                desenhar_soldado(jogador);
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.85f));
                
                if (jogador->vida <= 0) {
                    DrawText("GAME OVER", SCREEN_WIDTH/2 - MeasureText("GAME OVER", 60)/2, SCREEN_HEIGHT/3, 60, RED);
                } else {
                    DrawText("MISSÃO CUMPRIDA!", SCREEN_WIDTH/2 - MeasureText("MISSÃO CUMPRIDA!", 50)/2, SCREEN_HEIGHT/3, 50, GREEN);
                    DrawText(TextFormat("Tempo: %.2f segundos", tempoDeJogo), SCREEN_WIDTH/2 - MeasureText(TextFormat("Tempo: %.2f segundos", tempoDeJogo), 30)/2, SCREEN_HEIGHT/2, 30, WHITE);
                }

                if (topScores) {
                    DrawText("TOP 3 RECORDES:", SCREEN_WIDTH/2 - MeasureText("TOP 3 RECORDES:", 30)/2, SCREEN_HEIGHT/2 + 50, 30, RED);

                    Score *atual = topScores;
                    int count = 0;
                    
                    while (atual != NULL && count < 3) {
                        DrawText(TextFormat("%d. %.2fs", count + 1, atual->tempo), 
                                 SCREEN_WIDTH/2 - MeasureText("9. 99.99s", 25)/2,
                                 SCREEN_HEIGHT/2 + 90 + count * 30, 
                                 25, 
                                 WHITE);
                        atual = atual->prox;
                        count++;
                    }
                }

                DrawText("[ ENTER ] Voltar ao Menu", SCREEN_WIDTH/2 - MeasureText("[ ENTER ] Voltar ao Menu", 20)/2, SCREEN_HEIGHT - 100, 20, LIGHTGRAY);
                EndDrawing();
            }
            break;
        }
    }

    liberar_mapa(mapa);
    liberar_soldado(jogador);
    liberar_lista_balas(&listaBalas);
    UnloadTexture(texturaBala);
    UnloadTexture(texturaMenu);

    liberar_scores(topScores);
    
    CloseWindow();
    return 0;
}
