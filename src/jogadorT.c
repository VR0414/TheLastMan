#include <stdlib.h>
#include "jogador.h"
#include "mapa.h"

Soldado *criar_soldado(int linha, int coluna){
    Soldado *s = malloc(sizeof(Soldado));
    if(!s) return NULL;
    s->linha = linha;
    s->coluna = coluna;
    s->simbolo = 'S';
    return s;
}

void liberar_soldado(Soldado *s){
    if(!s) return;
    free(s);
}

void posicionar_soldado_no_mapa(Soldado *s, void *mapa_void){
    Mapa *m = (Mapa*)mapa_void;
    if(!m || !s) return;
    definir_celula(m, s->linha, s->coluna, s->simbolo);
}

// retorna 0 se ok, 1 se colisao (tocar bala), 2 se alcançou abrigo
int mover_soldado(Soldado *s, int dr, int dc, void *mapa_void){
    Mapa *m = (Mapa*)mapa_void;
    if(!m || !s) return 0;
    int nx = s->linha + dr;
    int ny = s->coluna + dc;
    if(nx < 0) nx = 0;
    if(nx >= m->linhas) nx = m->linhas - 1;
    if(ny < 0) ny = 0;
    if(ny >= m->colunas) ny = m->colunas - 1;

    char destino = obter_celula(m, nx, ny);
    // se destino é '^' (bala) -> colisao
    if(destino == '^') return 1;
    // se destino é '#' (abrigo no topo) e linha 0 -> venceu
    if(destino == '#' && nx == 0) {
        // limpar posição anterior e posicionar em cima
        definir_celula(m, s->linha, s->coluna, ' ');
        s->linha = nx; s->coluna = ny;
        definir_celula(m, s->linha, s->coluna, s->simbolo);
        return 2;
    }

    // mover normalmente
    definir_celula(m, s->linha, s->coluna, ' ');
    s->linha = nx; s->coluna = ny;
    definir_celula(m, s->linha, s->coluna, s->simbolo);
    return 0;
}
