#include "raylib.h"            
#include "mapaRL.h"            // mapa (funções e tipos relacionados ao mapa)
#include "balasRL.h"           // balas (lista, criação, atualização, desenho)

int main(void) {              
    InitWindow(800, 600, "The Last Man - Raylib Edition"); //janela de 800x600 com título
    SetTargetFPS(60);  // deixando a 60 FPS
    Mapa *mapa = criar_mapa(MAPA_LINHAS, MAPA_COLUNAS, '='); // Alocando e inicializando mapa com as dimensões predefinidas e caractere padrão '='
    preencher_limites(mapa, '#'); // Preenche as bordas/limites com o caractere '#'
    preencher_chao(mapa, '.', '#', '='); // Preenche o chão do mapa com '.', possivelmente usando '#' e '=' como parâmetros auxiliares
    preencher_abrigo(mapa, '='); // Cria/define estruturas de abrigo no mapa usando o caractere '='

    ListaBalas lista;          // Declara a lista de balas (estrutura que guarda as balas ativas)
    inicio_lista_balas(&lista); // Inicializa a lista de balas (configura ponteiros e contadores iniciais)

    while (!WindowShouldClose()) { // Loop Do Jogo(enquanto a janela não deve fechar)
        // Cria bala com espaço
        if (IsKeyPressed(KEY_SPACE)) { // Se a tecla ESPAÇO for pressionada (apenas no momento do pressionamento)
            Bala *nova = criar_bala(20, 40, -0.3f, 0, 200); // Cria uma nova bala com parâmetros (x, y, vx, vy, vida) — exemplo: posição (20,40), velocidade x -0.3, sem velocidade y, vida 200
            inserir_bala_inicio(&lista, nova); // Insere a nova bala no início da lista de balas
        }

        atualizar_balas(&lista, mapa, '#', '='); // Atualiza posição/estado das balas, trata colisões com o mapa usando caracteres '#' e '='

        BeginDrawing();          // Inicia o bloco de desenho do Raylib
        ClearBackground(BLACK);  // Limpa a tela com a cor preta

        desenhar_mapa(mapa, 10); // Desenha o mapa na tela, possivelmente com escala ou tamanho de célula = 10
        desenhar_balas(&lista, 10); // Desenha todas as balas da lista, possivelmente com a mesma escala = 10

        DrawText("Pressione ESPAÇO para atirar", 10, 10, 20, WHITE); // Desenha uma instrução de texto no canto superior esquerdo
        EndDrawing();            // Finaliza o bloco de desenho e apresenta na janela
    }

    liberar_mapa(mapa);        // Libera a memória/alocação do mapa
    CloseWindow();             // Fecha a janela do Raylib e limpa recursos
    return 0;                  // Retorna 0 indicando execução bem-sucedida
} // fim de main
