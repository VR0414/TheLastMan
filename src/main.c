#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#include "mapa.h"
#include "jogador.h"
#include "bala.h"

// configurações do mapa
#define LINHAS 20
#define COLUNAS 40

// funções utilitárias para input não-bloqueante
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

// inicializa um padrão simples de balas em algumas colunas
void inicializar_padrao_balas(ListaBalas *l, Mapa *m){
    // adiciona balas em duas linhas diferentes
    for(int c = 2; c < m->colunas - 2; c += 6){
        adicionar_bala(l, m->linhas - 3, c);
        adicionar_bala(l, m->linhas - 6, c+2);
    }
}

int main(void){
    srand(time(NULL));
    Mapa *mapa = criar_mapa(LINHAS, COLUNAS);
    if(!mapa){ fprintf(stderr, "Erro ao criar mapa\n"); return 1; }

    // desenhar abrigo no topo
    for(int c=0;c<COLUNAS;c++) definir_celula(mapa, 0, c, '#');

    // criar soldado e posicionar
    Soldado *soldado = criar_soldado(LINHAS - 1, COLUNAS/2);
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
        usleep(80000); // ~80ms
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
            // future: processar setas (sequência \033[A ...)
        }

        // mover balas a cada 3 ticks
        if(tick % 3 == 0){
            atualizar_balas(balas, mapa, &colisao, soldado->linha, soldado->coluna);
        }

        desenhar_mapa(mapa);
        printf("\nUse W A S D para mover. Q para sair. Tempo: %lds\n", (long)(time(NULL)-inicio));

        if(colisao){
            printf("\nVocê foi atingido! Fim de jogo.\n");
            break;
        }
    }

    // liberar recursos
    restaurar_modo();
    liberar_lista_balas(balas);
    liberar_soldado(soldado);
    liberar_mapa(mapa);
    return 0;
}
