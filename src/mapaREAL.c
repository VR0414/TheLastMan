#include "mapaREAL.h"
#include <stdio.h>
#include <stdlib.h>

Mapa *criar_mapa(int linhas, int colunas, char nulo, char abrigo) {

    if (linhas <= 0 || colunas <= 0)
        return NULL;

    Mapa *mapa = (Mapa *)malloc(sizeof(Mapa));

    mapa->linhas = linhas;
    mapa->colunas = colunas;
    mapa->abrigo = abrigo;

    mapa->celulas = (char *)malloc(linhas * colunas * sizeof(char)); // Aloca todas as células

    return mapa;

}

void liberar_mapa(Mapa *mapa) {

    if (mapa->celulas != NULL) 
        free(mapa->celulas);
    free(mapa);

}

int preencher_limites(Mapa *mapa, char paredes) {

    if (mapa->linhas < 0 || mapa->colunas < 0) 
        return -1;

    // Preenchimento Linha 0 | mapa->colunas = 80;
    for (int LinhaSuperior = 0; LinhaSuperior < mapa->colunas; LinhaSuperior++) { 
        mapa->celulas[LinhaSuperior] = paredes;
    }

    // Preenche Linha (mapa->linhas - 1) -> Última linha | mapa->colunas = 80; 
    for (int LinhaInferior = 0; LinhaInferior < mapa->colunas; LinhaInferior++) { 
        int indice = (mapa->linhas - 1) * mapa->colunas + LinhaInferior;
        mapa->celulas[indice] = paredes;
    }

    // Preenche coluna esquerda (mapa->colunas = 0) e coluna direita (mapa->colunas - 1)
    for (int l = 0; l < mapa->linhas; l++) { 
        int esquerda = l * mapa->colunas;
        int direita = l * mapa->colunas + (mapa->colunas - 1);
        mapa->celulas[esquerda] = paredes;
        mapa->celulas[direita] = paredes;
    }

    return 0;
}

void preencher_chao(Mapa *mapa, char nulo, char paredes, char abrigo) {

    for (int l = 0; l < mapa->linhas; l++) {
        for (int c = 0; c < mapa->colunas; c++) {

            int indice = l * mapa->colunas + c;

            if (mapa->celulas[indice] != paredes || mapa->celulas[indice] != abrigo) {
                mapa->celulas[indice] = nulo;
            }
        }
    }
}


void preencher_abrigo(Mapa *mapa, char abrigo) { // Define a função que preenche o abrigo no mapa usando o caractere 'abrigo'

    int colunas = mapa->colunas; // Lê o número de colunas do mapa
    int linhas = mapa->linhas;   // Lê o número de linhas do mapa 

    int largura_abrigo = 7; // Define a largura desejada do abrigo (número de casas horizontais)
    int centro = colunas / 2; // Calcula a coluna central do mapa (divisão inteira)

    int inicio = centro - (largura_abrigo / 2); // Calcula a coluna inicial do abrigo, deixando no centro
    if (inicio < 0) 
        inicio = 0; // Se o início ficou antes da coluna 0, ajusta para 0 (protege limite esquerdo)

    int fim = centro + (largura_abrigo / 2); // Calcula a coluna final do abrigo (exclusivo ou inclusivo dependendo do uso abaixo)
    if (fim > colunas)
        fim = colunas; // Se o fim ultrapassa o número de colunas, ajusta para o limite direito

    int linha_abrigo = mapa->linhas - 1; // Define a linha onde o abrigo será desenhado (última linha do mapa)

    for (int casa = inicio; casa < fim; casa++) { // Itera pelas colunas do início até (fim-1)
        int a = linha_abrigo * colunas + casa; // Converte coordenadas (linha, coluna) para índice linear no vetor de células
        mapa->celulas[a] = abrigo; // Atribui o caractere 'abrigo' à célula correspondente no mapa
    }
}

void imprimir_mapa(Mapa *mapa) {

    for (int l = 0; l < mapa->linhas; l++) {
        for (int c = 0; c < mapa->colunas; c++) {
            putchar(mapa->celulas[l * mapa->colunas + c]);
        }
        putchar('\n');
    }
}