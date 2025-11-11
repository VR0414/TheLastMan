#include "raylib.h"
#include "mapaRL.h"
#include "balasRL.h"

int main(void) {
    InitWindow(800, 600, "The Last Man - Raylib Edition");
    SetTargetFPS(60);

    Mapa *mapa = criar_mapa(MAPA_LINHAS, MAPA_COLUNAS, '=');
    preencher_limites(mapa, '#');
    preencher_chao(mapa, '.', '#', '=');
    preencher_abrigo(mapa, '=');

    ListaBalas lista;
    inicio_lista_balas(&lista);

    while (!WindowShouldClose()) {
        // Cria bala com espaço
        if (IsKeyPressed(KEY_SPACE)) {
            Bala *nova = criar_bala(20, 40, -0.3f, 0, 200);
            inserir_bala_inicio(&lista, nova);
        }

        atualizar_balas(&lista, mapa, '#', '=');

        BeginDrawing();
        ClearBackground(BLACK);

        desenhar_mapa(mapa, 10);
        desenhar_balas(&lista, 10);

        DrawText("Pressione ESPAÇO para atirar", 10, 10, 20, WHITE);
        EndDrawing();
    }

    liberar_mapa(mapa);
    CloseWindow();
    return 0;
}
