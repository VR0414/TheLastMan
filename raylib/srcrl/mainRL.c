#include "raylib.h"
#include "../includerl/mapaRL.h"
#include "../includerl/balasRL.h"
#include "../includerl/soldadoRL.h"

#define CELULA 20
#define LARGURA 800
#define ALTURA 600

int main(void) {

    InitWindow(LARGURA, ALTURA, "The Last Man - Raylib Edition");
    SetTargetFPS(60);

    // ---------------- MAPA ----------------
    Mapa *mapa = criar_mapa(MAPA_LINHAS, MAPA_COLUNAS, ' ');
    preencher_limites(mapa, '#');
    preencher_chao(mapa, '.', '#', '=');
    preencher_abrigo(mapa, '=');

    // ---------------- SOLDADO ----------------
    Soldado jogador;
    iniciar_soldado(&jogador, mapa->linhas - 2, mapa->colunas / 2, 3, '@');

    float spawnInicialL = jogador.linha;
    float spawnInicialC = jogador.coluna;

    // ---------------- LISTA DE BALAS ----------------
    ListaBalas lista;
    inicio_lista_balas(&lista);

    // ---------------- LOOP DO JOGO ----------------
    while (!WindowShouldClose()) {

        // ---- CONTROLES DO JOGADOR ----
        if (IsKeyDown(KEY_UP))    mover_soldado(&jogador, mapa, -0.1f, 0);
        if (IsKeyDown(KEY_DOWN))  mover_soldado(&jogador, mapa,  0.1f, 0);
        if (IsKeyDown(KEY_LEFT))  mover_soldado(&jogador, mapa, 0, -0.1f);
        if (IsKeyDown(KEY_RIGHT)) mover_soldado(&jogador, mapa, 0,  0.1f);

        // ---- ATIRAR BALAS (só para teste) ----
        if (IsKeyPressed(KEY_SPACE)) {
            Bala *nova = criar_bala(0, GetRandomValue(1, mapa->colunas - 2), 0.2f, 0, 300);
            inserir_bala_inicio(&lista, nova);
        }

        // ---- ATUALIZAR BALAS ----
        atualizar_balas(&lista, mapa, '#', '=');

        // ---- COLISÃO JOGADOR X BALAS ----
        Bala *b = lista.head;
        while (b) {
            if ((int)b->linha == (int)jogador.linha &&
                (int)b->coluna == (int)jogador.coluna) {

                aplicar_dano_soldado(&jogador, 1, spawnInicialL, spawnInicialC);
                break;
            }
            b = b->proxima;
        }

        // ---- VERIFICAR CHEGADA NO TOPO (OBJETIVO) ----
        if ((int)jogador.linha <= 1) {
            reset_posicao_soldado(&jogador, spawnInicialL, spawnInicialC);
        }

        // -------------- DESENHO --------------
        BeginDrawing();
        ClearBackground(BLACK);

        desenhar_mapa(mapa, CELULA);

        // Desenha o soldado
        DrawCircle(
            (int)(jogador.coluna * CELULA + CELULA / 2),
            (int)(jogador.linha * CELULA + CELULA / 2),
            CELULA / 2,
            GREEN
        );

        // Desenha as balas
        desenhar_balas(&lista, CELULA);

        DrawText("Use as setas para mover. SPACE = gerar bala.", 10, 10, 20, WHITE);
        DrawText(TextFormat("Vida: %d", jogador.vida), 10, 40, 20, RED);

        EndDrawing();
    }

    // ------ ENCERRAR ------
    liberar_mapa(mapa);
    CloseWindow();

    return 0;
}

