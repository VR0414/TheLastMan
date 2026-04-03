#include "raylib.h"
#include "rlgl.h"  // ADICIONAR ESTA LINHA
#include "../includerl/mapaRL.h"
#include "../includerl/balasRL.h"
#include "../includerl/soldadoRL.h"
#include "../includerl/scoresRL.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_FIM
} EstadoJogo;

// Sistema de partículas para efeitos visuais
typedef struct Particula {
    Vector2 posicao;
    Vector2 velocidade;
    Color cor;
    float vida;
    float vidaMax;
    float tamanho;
    struct Particula *proxima;
} Particula;

typedef struct {
    Particula *head;
    int count;
} SistemaParticulas;

void adicionar_particula(SistemaParticulas *sistema, Vector2 pos, Vector2 vel, Color cor, float vida, float tamanho) {
    Particula *nova = (Particula*)malloc(sizeof(Particula));
    nova->posicao = pos;
    nova->velocidade = vel;
    nova->cor = cor;
    nova->vida = vida;
    nova->vidaMax = vida;
    nova->tamanho = tamanho;
    nova->proxima = sistema->head;
    sistema->head = nova;
    sistema->count++;
}

void atualizar_particulas(SistemaParticulas *sistema, float delta) {
    Particula **atual = &sistema->head;
    while (*atual) {
        Particula *p = *atual;
        p->vida -= delta;
        
        if (p->vida <= 0) {
            *atual = p->proxima;
            free(p);
            sistema->count--;
        } else {
            p->posicao.x += p->velocidade.x * delta;
            p->posicao.y += p->velocidade.y * delta;
            p->velocidade.y += 200.0f * delta; // Gravidade
            atual = &p->proxima;
        }
    }
}

void desenhar_particulas(SistemaParticulas *sistema) {
    Particula *p = sistema->head;
    while (p) {
        float alpha = (p->vida / p->vidaMax) * 255.0f;
        Color cor = p->cor;
        cor.a = (unsigned char)alpha;
        DrawCircleV(p->posicao, p->tamanho, cor);
        p = p->proxima;
    }
}

void liberar_particulas(SistemaParticulas *sistema) {
    while (sistema->head) {
        Particula *temp = sistema->head;
        sistema->head = sistema->head->proxima;
        free(temp);
    }
}

void criar_explosao_particulas(SistemaParticulas *sistema, Vector2 pos, Color cor, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        float angulo = (float)GetRandomValue(0, 360) * DEG2RAD;
        float velocidade = (float)GetRandomValue(50, 150);
        Vector2 vel = { cosf(angulo) * velocidade, sinf(angulo) * velocidade - 100.0f };
        float tamanho = (float)GetRandomValue(2, 6);
        adicionar_particula(sistema, pos, vel, cor, (float)GetRandomValue(5, 15) / 10.0f, tamanho);
    }
}

// Função para desenhar texto com sombra
void DrawTextShadow(const char *text, int posX, int posY, int fontSize, Color textColor, Color shadowColor, int shadowOffset) {
    DrawText(text, posX + shadowOffset, posY + shadowOffset, fontSize, shadowColor);
    DrawText(text, posX, posY, fontSize, textColor);
}

// Função para desenhar botão estilizado
bool DrawStyledButton(const char *text, int posX, int posY, int fontSize, Color baseColor, Color hoverColor) {
    int textWidth = MeasureText(text, fontSize);
    Rectangle btnRect = { posX - textWidth/2 - 20, posY - 10, textWidth + 40, fontSize + 20 };
    
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), btnRect);
    Color currentColor = isHovered ? hoverColor : baseColor;
    
    // Desenhar background do botão
    DrawRectangleRounded(btnRect, 0.3f, 10, Fade(currentColor, 0.3f));
    DrawRectangleRoundedLines(btnRect, 0.3f, 10, currentColor); // CORRIGIDO: removido o parâmetro '2'
    
    // Desenhar texto
    DrawTextShadow(text, posX - textWidth/2, posY, fontSize, currentColor, Fade(BLACK, 0.5f), 2);
    
    return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

int main(void) {
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Last Man - Modern Edition");
    
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
    float soldVel = 85.0f;
    
    Soldado *jogador = criar_soldado(posInicial, 3, soldVel, soldW, soldH, "resources/soldado.png");
    if (!jogador) return 1;

    ListaBalas listaBalas;
    inicio_lista_balas(&listaBalas);
    
    float spawnTimer = 0.0f;
    float spawnIntervalo = 0.15f;
    float tempoDeJogo = 0.0f;

    Score *topScores = NULL;
    topScores = carregar_scores("scores.txt");

    // Sistema de partículas
    SistemaParticulas particulas = { NULL, 0 };

    // Variáveis de animação
    float menuPulse = 0.0f;
    float hudAlpha = 0.0f;

    // Shake da tela quando leva dano
    float screenShake = 0.0f;
    Vector2 shakeOffset = { 0, 0 };

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // Atualizar efeito de shake
        if (screenShake > 0) {
            screenShake -= deltaTime * 5.0f;
            shakeOffset.x = ((float)GetRandomValue(-10, 10)) * screenShake;
            shakeOffset.y = ((float)GetRandomValue(-10, 10)) * screenShake;
        } else {
            shakeOffset = (Vector2){ 0, 0 };
        }

        // Atualizar partículas
        atualizar_particulas(&particulas, deltaTime);

        switch (estadoAtual) {
            
            case TELA_MENU:
            {
                menuPulse += deltaTime * 3.0f;
                hudAlpha = 0.0f;
                
                if (IsKeyPressed(KEY_ENTER) || DrawStyledButton("", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 20, 0, GREEN, LIME)) {
                    jogador->vida = 3;
                    reset_posicao_soldado(jogador);
                    liberar_lista_balas(&listaBalas);
                    tempoDeJogo = 0.0f;
                    liberar_particulas(&particulas);
                    estadoAtual = TELA_JOGO;
                }
                
                // Partículas de fundo
                if (GetRandomValue(0, 30) == 0) {
                    Vector2 pos = { (float)GetRandomValue(0, SCREEN_WIDTH), (float)GetRandomValue(0, SCREEN_HEIGHT) };
                    Vector2 vel = { (float)GetRandomValue(-20, 20), (float)GetRandomValue(-50, -10) };
                    adicionar_particula(&particulas, pos, vel, Fade(WHITE, 0.3f), 2.0f, 2.0f);
                }

                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(texturaMenu, 0, 0, Fade(WHITE, 0.8f));
                
                // Overlay escuro
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.4f));
                
                // Desenhar partículas de fundo
                desenhar_particulas(&particulas);

                // Título com efeito de brilho
                float pulseScale = 1.0f + sinf(menuPulse) * 0.05f;
                int titleSize = (int)(70 * pulseScale);
                const char *titulo = "THE LAST MAN";
                int titleWidth = MeasureText(titulo, titleSize);
                
                DrawRectangle(SCREEN_WIDTH/2 - titleWidth/2 - 30, SCREEN_HEIGHT/3 - 50, 
                             titleWidth + 60, titleSize + 40, Fade(BLACK, 0.7f));
                
                DrawTextShadow(titulo, SCREEN_WIDTH/2 - titleWidth/2, SCREEN_HEIGHT/3 - 20, 
                              titleSize, 
                              ColorAlpha(RED, 0.8f + sinf(menuPulse) * 0.2f), 
                              Fade(BLACK, 0.8f), 4);

                // Botão de início com animação
                float btnPulse = (sinf(menuPulse * 1.5f) + 1.0f) / 2.0f;
                Color btnColor = ColorFromHSV(0, 0.8f, 0.5f + btnPulse * 0.5f);
                
                DrawStyledButton("PRESSIONE ENTER PARA INICIAR", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 20, 28, btnColor, RED);

                // Top 3 com design moderno
                if (topScores) {
                    int panelY = SCREEN_HEIGHT - 250;
                    int panelH = 230;
                    
                    DrawRectangle(SCREEN_WIDTH/2 - 200, panelY, 400, panelH, Fade(BLACK, 0.8f));
                    DrawRectangleLinesEx((Rectangle){SCREEN_WIDTH/2 - 200, panelY, 400, panelH}, 2, GOLD);
                    
                    DrawTextShadow("TOP 3 RECORDES", 
                                  SCREEN_WIDTH/2 - MeasureText("TOP 3 RECORDES", 28)/2, 
                                  panelY + 15, 28, GOLD, Fade(BLACK, 0.8f), 2);

                    Score *atual = topScores;
                    int count = 0;
                    int startY = panelY + 60;
                    
                    while (atual != NULL && count < 3) {
                        Color medalColor = count == 0 ? GOLD : count == 1 ? LIGHTGRAY : (Color){205, 127, 50, 255};
                        
                        char texto[64];
                        sprintf(texto, "%d.  %.2f s", count + 1, atual->tempo);
                        
                        DrawRectangle(SCREEN_WIDTH/2 - 180, startY + count * 45, 360, 40, Fade(medalColor, 0.2f));
                        DrawTextShadow(texto, 
                                      SCREEN_WIDTH/2 - MeasureText(texto, 24)/2,
                                      startY + count * 45 + 8, 
                                      24, 
                                      medalColor, Fade(BLACK, 0.8f), 2);
                        atual = atual->prox;
                        count++;
                    }
                }
                
                EndDrawing();
            }
            break;
            
            case TELA_JOGO:
            {
                // Fade in do HUD
                if (hudAlpha < 1.0f) hudAlpha += deltaTime * 2.0f;
                
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
                        // Criar explosão de partículas vermelhas
                        Vector2 impacto = { jogador->colisao.x + jogador->colisao.width/2, 
                                           jogador->colisao.y + jogador->colisao.height/2 };
                        criar_explosao_particulas(&particulas, impacto, RED, 20);
                        
                        aplicar_dano_soldado(jogador, b->dano);
                        remover_bala(&listaBalas, b);
                        
                        // Ativar shake
                        screenShake = 1.0f;
                        
                        if (jogador->vida <= 0) estadoAtual = TELA_FIM;
                    }
                    b = prox;
                }

                if (checar_vitoria_mapa(mapa, jogador->colisao)) {
                    // Explosão de vitória
                    Vector2 posVitoria = { jogador->colisao.x + jogador->colisao.width/2, 
                                          jogador->colisao.y + jogador->colisao.height/2 };
                    criar_explosao_particulas(&particulas, posVitoria, GREEN, 50);
                    
                    adicionar_score_ordenado(&topScores, tempoDeJogo);
                    salvar_scores("scores.txt", topScores);
                    estadoAtual = TELA_FIM;
                }
                
                tempoDeJogo += GetFrameTime();

                BeginDrawing();
                ClearBackground(BLACK);
                
                // Aplicar offset do shake
                rlPushMatrix();
                rlTranslatef(shakeOffset.x, shakeOffset.y, 0);
                
                desenhar_mapa(mapa);
                
                BeginBlendMode(BLEND_ALPHA);
                desenhar_soldado(jogador);
                desenhar_balas(&listaBalas, texturaBala);
                EndBlendMode();
                
                // Desenhar partículas
                desenhar_particulas(&particulas);
                
                rlPopMatrix();
                
                // HUD moderno com fade in
                Color hudColor = ColorAlpha(WHITE, hudAlpha);
                Color hudBgColor = ColorAlpha(BLACK, 0.7f * hudAlpha);
                
                // Painel de vidas
                int vidasPanelW = 200;
                int vidasPanelH = 60;
                DrawRectangle(10, SCREEN_HEIGHT - vidasPanelH - 10, vidasPanelW, vidasPanelH, hudBgColor);
                DrawRectangleLinesEx((Rectangle){10, SCREEN_HEIGHT - vidasPanelH - 10, vidasPanelW, vidasPanelH}, 
                                    2, ColorAlpha(RED, hudAlpha));
                
                DrawTextShadow("VIDAS", 20, SCREEN_HEIGHT - vidasPanelH - 5 + 5, 22, 
                              ColorAlpha(LIGHTGRAY, hudAlpha), hudBgColor, 2);
                
                // Corações para vidas
                for (int i = 0; i < jogador->vida; i++) {
                    DrawText("♥", 25 + i * 50, SCREEN_HEIGHT - 45, 40, ColorAlpha(RED, hudAlpha));
                }

                // Painel de tempo
                char tempoTexto[32];
                sprintf(tempoTexto, "TEMPO: %.1f", tempoDeJogo);
                int tempoWidth = MeasureText(tempoTexto, 30);
                int tempoPanelW = tempoWidth + 40;
                int tempoPanelH = 60;
                
                DrawRectangle(SCREEN_WIDTH - tempoPanelW - 10, SCREEN_HEIGHT - tempoPanelH - 10, 
                             tempoPanelW, tempoPanelH, hudBgColor);
                DrawRectangleLinesEx((Rectangle){SCREEN_WIDTH - tempoPanelW - 10, SCREEN_HEIGHT - tempoPanelH - 10, 
                                                 tempoPanelW, tempoPanelH}, 
                                    2, ColorAlpha(SKYBLUE, hudAlpha));
                
                DrawTextShadow(tempoTexto, 
                              SCREEN_WIDTH - tempoPanelW + 10, 
                              SCREEN_HEIGHT - 45, 
                              30, 
                              ColorAlpha(SKYBLUE, hudAlpha), 
                              hudBgColor, 2);
                
                // Barra de progresso no topo
                float progresso = (SCREEN_HEIGHT - jogador->colisao.y) / (float)SCREEN_HEIGHT;
                int barWidth = SCREEN_WIDTH - 40;
                DrawRectangle(20, 10, barWidth, 20, Fade(BLACK, 0.7f * hudAlpha));
                DrawRectangle(20, 10, (int)(barWidth * progresso), 20, ColorAlpha(GREEN, hudAlpha));
                DrawRectangleLinesEx((Rectangle){20, 10, barWidth, 20}, 2, ColorAlpha(WHITE, hudAlpha));
                
                EndDrawing();
            }
            break;

            case TELA_FIM:
            {
                if (IsKeyPressed(KEY_ENTER)) {
                    liberar_particulas(&particulas);
                    estadoAtual = TELA_MENU;
                }
                
                // Partículas de celebração se venceu
                if (jogador->vida > 0 && GetRandomValue(0, 10) == 0) {
                    Vector2 pos = { (float)GetRandomValue(0, SCREEN_WIDTH), 0 };
                    Vector2 vel = { (float)GetRandomValue(-30, 30), (float)GetRandomValue(50, 150) };
                    Color cores[] = { GOLD, YELLOW, ORANGE, GREEN };
                    adicionar_particula(&particulas, pos, vel, cores[GetRandomValue(0, 3)], 3.0f, (float)GetRandomValue(3, 8));
                }

                BeginDrawing();
                ClearBackground(BLACK);
                
                desenhar_mapa(mapa);
                desenhar_soldado(jogador);
                
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.85f));
                
                // Desenhar partículas
                desenhar_particulas(&particulas);
                
                if (jogador->vida <= 0) {
                    const char *gameOver = "GAME OVER";
                    int goWidth = MeasureText(gameOver, 70);
                    DrawTextShadow(gameOver, SCREEN_WIDTH/2 - goWidth/2, SCREEN_HEIGHT/3, 70, 
                                  RED, Fade(DARKGRAY, 0.8f), 5);
                } else {
                    const char *vitoria = "MISSÃO CUMPRIDA!";
                    int vWidth = MeasureText(vitoria, 60);
                    
                    // Efeito de brilho
                    float brilho = (sinf(GetTime() * 5.0f) + 1.0f) / 2.0f;
                    DrawTextShadow(vitoria, SCREEN_WIDTH/2 - vWidth/2, SCREEN_HEIGHT/3, 60, 
                                  ColorFromHSV(120, 0.7f, 0.5f + brilho * 0.5f), 
                                  Fade(DARKGREEN, 0.8f), 5);
                    
                    char tempoFinal[64];
                    sprintf(tempoFinal, "Tempo: %.2f segundos", tempoDeJogo);
                    int tfWidth = MeasureText(tempoFinal, 35);
                    DrawTextShadow(tempoFinal, SCREEN_WIDTH/2 - tfWidth/2, SCREEN_HEIGHT/2, 35, 
                                  GOLD, Fade(BLACK, 0.8f), 3);
                }

                // Painel de scores
                if (topScores) {
                    int panelY = SCREEN_HEIGHT/2 + 70;
                    int panelH = 180;
                    
                    DrawRectangle(SCREEN_WIDTH/2 - 220, panelY, 440, panelH, Fade(BLACK, 0.9f));
                    DrawRectangleLinesEx((Rectangle){SCREEN_WIDTH/2 - 220, panelY, 440, panelH}, 3, GOLD);
                    
                    DrawTextShadow("TOP 3 RECORDES:", 
                                  SCREEN_WIDTH/2 - MeasureText("TOP 3 RECORDES:", 32)/2, 
                                  panelY + 10, 32, GOLD, Fade(BLACK, 0.8f), 3);

                    Score *atual = topScores;
                    int count = 0;
                    
                    while (atual != NULL && count < 3) {
                        Color medalColor = count == 0 ? GOLD : count == 1 ? LIGHTGRAY : (Color){205, 127, 50, 255};
                        char scoreText[32];
                        sprintf(scoreText, "%d.  %.2fs", count + 1, atual->tempo);
                        
                        DrawTextShadow(scoreText, 
                                      SCREEN_WIDTH/2 - MeasureText(scoreText, 28)/2,
                                      panelY + 55 + count * 35, 
                                      28, 
                                      medalColor, Fade(BLACK, 0.8f), 2);
                        atual = atual->prox;
                        count++;
                    }
                }

                // Botão de retorno
                DrawStyledButton("[ ENTER ] Voltar ao Menu", SCREEN_WIDTH/2, SCREEN_HEIGHT - 80, 24, LIGHTGRAY, WHITE);
                
                EndDrawing();
            }
            break;
        }
    }

    liberar_mapa(mapa);
    liberar_soldado(jogador);
    liberar_lista_balas(&listaBalas);
    liberar_particulas(&particulas);
    UnloadTexture(texturaBala);
    UnloadTexture(texturaMenu);
    liberar_scores(topScores);
    
    CloseWindow();
    return 0;
}