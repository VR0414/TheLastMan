#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#include "mapa.h"
#include "jogador.h"
#include "bala.h"

#define LINHAS 20
#define COLUNAS 40

// parâmetros de spawn
#define TICKS_PARA_POSSIVEL_SPAWN 3   // a cada N ticks avaliamos spawn
#define PROB_SPAWN_PERCENT 50         // probabilidade (0-100) de spawn quando chegar o tick

struct termios oldt;

void ativar_modo_sem_buffer(){
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restaurar_modo(){
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int ler_tecla_nao_bloqueante(){
    unsigned char c;
    if(read(STDIN_FILENO, &c, 1) == 1) return c;
    return 0;
}

// Spawn: adiciona uma bala na borda lateral (esquerda ou direita) em linha aleatória
void spawn_bala_lateral(ListaBalas *l, Mapa *m){
    // escolher linha entre 1 e linhas-2 (evita topo e base)
    int linha_min = 1;
    int linha_max = m->linhas - 2;
    if(linha_max < linha_min) return;
    int linha = linha_min + rand() % (linha_max - linha_min + 1);

    // escolher lado: 0 = esquerda (dx = +1), 1 = direita (dx = -1)
    int lado = rand() % 2;
    if(lado == 0){
        // esquerda: coluna 0, dx = +1
        adicionar_bala(l, linha, 0, +1);
    } else {
        // direita: coluna m->colunas -1, dx = -1
        adicionar_bala(l, linha, m->colunas - 1, -1);
    }
}

// inicializar sem balas fixas; spawn será contínuo
void inicializar_padrao_balas(ListaBalas *l, Mapa *m){
    // se quiser, pode criar um pequeno conjunto inicial opcional
    // por enquanto deixamos vazio para spawn contínuo
}

int main(void){
    srand(time(NULL));
    Mapa *mapa = criar_mapa(LINHAS, COLUNAS);
    if(!mapa){ fprintf(stderr, "Erro ao criar mapa\n"); return 1; }

    // colocar abrigo único no topo (centro)
    int centro = COLUNAS / 2;
    for(int c=0;c<COLUNAS;c++) definir_celula(mapa, 0, c, ' '); // limpa topo
    definir_celula(mapa, 0, centro, '#'); // único abrigo

    // criar soldado e posicionar (na base, coluna central)
    Soldado *soldado = criar_soldado(LINHAS - 1, centro);
    posicionar_soldado_no_mapa(soldado, mapa);

    // lista de balas
    ListaBalas *balas = criar_lista_balas();
    inicializar_padrao_balas(balas, mapa);
    desenhar_balas_em_mapa(balas, mapa);

    ativar_modo_sem_buffer();

    int rodando = 1;
    int tick = 0;
    int colisao = 0;
    time_t inicio = time(NULL);

    while(rodando){
        usleep(60000); // ~60ms (aumente/diminua para ajustar velocidade)
        tick++;

        int k = ler_tecla_nao_bloqueante();
        if(k){
            if(k == 'q' || k == 'Q') { rodando = 0; break; }
            else if(k == 'w' || k == 'W') {
                int r = mover_soldado(soldado, -1, 0, mapa);
                if(r == 1) colisao = 1;
                else if(r == 2){
                    desenhar_mapa(mapa);
                    printf("\nVocê alcançou o abrigo! Vitória!\n");
                    break;
                }
            }
            else if(k == 's' || k == 'S') {
                if(mover_soldado(soldado, 1, 0, mapa) == 1) colisao = 1;
            }
            else if(k == 'a' || k == 'A') {
                if(mover_soldado(soldado, 0, -1, mapa) == 1) colisao = 1;
            }
            else if(k == 'd' || k == 'D') {
                if(mover_soldado(soldado, 0, 1, mapa) == 1) colisao = 1;
            }
        }

        // spawn contínuo: a cada TICKS_PARA_POSSIVEL_SPAWN avaliamos spawn com probabilidade
        if(tick % TICKS_PARA_POSSIVEL_SPAWN == 0){
            int r = rand() % 100;
            if(r < PROB_SPAWN_PERCENT){
                spawn_bala_lateral(balas, mapa);
            }
        }

        // atualizar balas a cada 1 tick (ou condicione se quiser desacelerar)
        atualizar_balas(balas, mapa, &colisao, soldado->linha, soldado->coluna);

        desenhar_mapa(mapa);
        printf("\nUse W A S D para mover. Q para sair. Tempo: %lds\n", (long)(time(NULL)-inicio));

        if(colisao){
            printf("\nVocê foi atingido! Fim de jogo.\n");
            break;
        }
    }

    restaurar_modo();
    liberar_lista_balas(balas);
    liberar_soldado(soldado);
    liberar_mapa(mapa);
    return 0;
}
