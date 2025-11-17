#include "raylib.h"
#include "../includerl/mapaRL.h"
#include "../includerl/balasRL.h"
#include "../includerl/soldadoRL.h"
#include <stdlib.h>

#define CELULA 20

int main(void) {
    // --- tela cheia do monitor
    int screenW = GetMonitorWidth(0);
    int screenH = GetMonitorHeight(0);
    InitWindow(screenW, screenH, "The Last Man - Raylib Edition");
    SetTargetFPS(60);

    // Calcula dimensões do mapa (ajusta CELULA para caber se quiser)
    int mapa_cols = MAPA_COLUNAS;
    int mapa_lines = MAPA_LINHAS;
    // se a grade não ocupar a tela toda, pode ajustar CELULA ou desenhar centralizado.
    // aqui usaremos CELULA definido e centralizaremos o mapa na tela.
    int tam_cel = CELULA;
    int mapa_pixel_w = mapa_cols * tam_cel;
    int mapa_pixel_h = mapa_lines * tam_cel;
    int offset_x = (screenW - mapa_pixel_w) / 2;
    int offset_y = (screenH - mapa_pixel_h) / 2;

    // ---------------- MAPA ----------------
    Mapa *mapa = criar_mapa(mapa_lines, mapa_cols, CEL_ABRIGO, CEL_PAREDE, CEL_CHAO);
    preencher_limites(mapa);
    preencher_chao(mapa);
    preencher_abrigo(mapa);

    // ---------------- SOLDADO ----------------
    Soldado jogador;
    iniciar_soldado(&jogador, mapa->linhas - 2, mapa->colunas / 2, 3, '@');
    float spawnInicialL = jogador.linha;
    float spawnInicialC = jogador.coluna;

    // ---------------- LISTA DE BALAS ----------------
    ListaBalas lista;
    inicio_lista_balas(&lista);

    // timer de spawn automático de balas
    float spawnTimer = 0.0f;
    const float spawnInterval = 0.4f; // a cada 0.4s gera uma bala

    // estados: 0 = menu inicial, 1 = jogando
    int estado = 0;

    while (!WindowShouldClose()) {

        // --- entrada e updates ---
        if (estado == 0) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                estado = 1;
            }
        } else if (estado == 1) {
            // contornos do mapa para mover em "grade" usando floats.
            if (IsKeyDown(KEY_UP))    mover_soldado(&jogador, mapa, -0.12f, 0);
            if (IsKeyDown(KEY_DOWN))  mover_soldado(&jogador, mapa,  0.12f, 0);
            if (IsKeyDown(KEY_LEFT))  mover_soldado(&jogador, mapa, 0, -0.12f);
            if (IsKeyDown(KEY_RIGHT)) mover_soldado(&jogador, mapa, 0,  0.12f);

            // teste spawn manual rápido
            if (IsKeyPressed(KEY_SPACE)) {
                Bala *nova = criar_bala(0, (float)(GetRandomValue(1, mapa->colunas - 2)), 0.4f, 0.0f, 1);
                adicionar_bala(&lista, nova);
            }

            // spawn automático
            spawnTimer += GetFrameTime();
            if (spawnTimer >= spawnInterval) {
                spawnTimer = 0;
                float col = (float)GetRandomValue(1, mapa->colunas - 2);
                Bala *nova = criar_bala(0.0f, col, 0.25f, 0.0f, 1);
                adicionar_bala(&lista, nova);
            }

            // atualiza balas (movimento + remoções)
            atualizar_balas(&lista, mapa);

            // checa colisões soldado x balas (e remove a bala)
            Bala *b = lista.head;
            Bala *anterior = NULL;
            while (b) {
                if ((int)b->linha == (int)jogador.linha &&
                    (int)b->coluna == (int)jogador.coluna) {

                    aplicar_dano_soldado(&jogador, b->dano, spawnInicialL, spawnInicialC);

                    // remover bala atingida
                    Bala *rem = b;
                    if (anterior) anterior->proxima = b->proxima;
                    else lista.head = b->proxima;
                    b = b->proxima;
                    free(rem);
                    continue;
                }
                anterior = b;
                b = b->proxima;
            }

            // se chegar no topo -> vencer / reset para testar
            if ((int)jogador.linha <= 1) {
                // aqui você pode adicionar pontuação; por enquanto reset
                reset_posicao_soldado(&jogador, spawnInicialL, spawnInicialC);
            }
        }

        // --- desenhar ---
        BeginDrawing();
        ClearBackground(BLACK);

        if (estado == 0) {
            // TELA INICIAL
            const char *title = "THE LAST MAN";
            int fontSize = 80;
            int subSize = 24;
            int tw = MeasureText(title, fontSize);
            DrawText(title, (screenW - tw) / 2, screenH / 3, fontSize, RAYWHITE);
            DrawText("Press ENTER or SPACE to start", (screenW - MeasureText("Press ENTER or SPACE to start", subSize)) / 2,
                     screenH / 2, subSize, BEIGE);
            DrawText("Use arrow keys to move", (screenW - MeasureText("Use arrow keys to move", subSize)) / 2,
                     screenH / 2 + 32, subSize, BEIGE);
        } else {
            // desenha mapa centralizado
            DrawRectangle(offset_x - 2, offset_y - 2, mapa_pixel_w + 4, mapa_pixel_h + 4, DARKGRAY);
            // desenhar mapa em offset (desenhar_mapa usa posições em 0..n, então vamos traduzir)
            // Para facilitar, temporariamente transformamos a matriz para tela desenhando cada célula com offset.
            for (int l = 0; l < mapa->linhas; l++) {
                for (int c = 0; c < mapa->colunas; c++) {
                    char cel = mapa->celulas[l * mapa->colunas + c];
                    Color cor = WHITE;
                    if (cel == mapa->parede) cor = DARKGRAY;
                    else if (cel == mapa->abrigo) cor = ORANGE;
                    else if (cel == mapa->vazio) cor = BEIGE;
                    DrawRectangle(offset_x + c * tam_cel, offset_y + l * tam_cel, tam_cel, tam_cel, cor);
                }
            }

            // Soldado (usando offset)
            float sx = offset_x + jogador.coluna * tam_cel + tam_cel / 2.0f;
            float sy = offset_y + jogador.linha * tam_cel + tam_cel / 2.0f;
            DrawCircleV((Vector2){sx, sy}, tam_cel * 0.45f, GREEN);

            // Desenhar balas levando offset em conta
            Bala *bb = lista.head;
            while (bb) {
                float bx = offset_x + bb->coluna * tam_cel + tam_cel / 2.0f;
                float by = offset_y + bb->linha * tam_cel + tam_cel / 2.0f;
                DrawCircleV((Vector2){bx, by}, tam_cel * 0.25f, RED);
                bb = bb->proxima;
            }

            // HUD
            DrawText("The Last Man", 10, 10, 24, RAYWHITE);
            DrawText(TextFormat("Vida: %d", jogador.vida), 10, 40, 20, RED);
        }

        EndDrawing();
    }

    // cleanup
    liberar_lista_balas(&lista);
    liberar_mapa(mapa);
    CloseWindow();
    return 0;
}

