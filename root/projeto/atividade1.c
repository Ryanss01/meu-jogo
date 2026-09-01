#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define TAM_CELULA 40

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float raio;
    Color cor;
} Bola;

int **criarMatriz(int linhas, int colunas) {
    int **matriz = malloc(linhas * sizeof(int *));
    if (matriz == NULL) return NULL;

    for (int i = 0; i < linhas; i++) {
        matriz[i] = malloc(colunas * sizeof(int));
        if (matriz[i] == NULL) {
            for (int k = 0; k < i; k++) free(matriz[k]);
            free(matriz);
            return NULL;
        }
        for (int j = 0; j < colunas; j++)
            matriz[i][j] = 0;
    }
    return matriz;
}

void liberarMatriz(int **matriz, int linhas) {
    if (matriz == NULL) return;
    for (int i = 0; i < linhas; i++) free(matriz[i]);
    free(matriz);
}

void desenharMatriz(int **matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Color cor = matriz[i][j] == 1
                ? (Color){70, 130, 190, 255}
                : (Color){15, 30, 55, 255};

            DrawRectangle(j * TAM_CELULA, i * TAM_CELULA,
                          TAM_CELULA - 2, TAM_CELULA - 2, cor);
        }
    }
}

Bola *criarBolas(int quantidade) {
    Bola *bolas = malloc(quantidade * sizeof(Bola));
    if (bolas == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        Bola *b = bolas + i;

        b->pos = (Vector2){
            GetRandomValue(50, LARGURA_JANELA - 50),
            GetRandomValue(50, ALTURA_JANELA - 50)
        };

        b->vel = (Vector2){
            (float)GetRandomValue(-4, 4),
            (float)GetRandomValue(-4, 4)
        };

        if (b->vel.x == 0) b->vel.x = 2;
        if (b->vel.y == 0) b->vel.y = 2;

        b->raio = (float)GetRandomValue(10, 25);
        b->cor = (Color){
            GetRandomValue(100, 255),
            GetRandomValue(100, 255),
            GetRandomValue(100, 255),
            255
        };
    }
    return bolas;
}

void atualizarBola(Bola *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    if (b->pos.x - b->raio < 0 ||
        b->pos.x + b->raio > LARGURA_JANELA)
        b->vel.x *= -1;

    if (b->pos.y - b->raio < 0 ||
        b->pos.y + b->raio > ALTURA_JANELA)
        b->vel.y *= -1;
}

int main(void) {
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA,
               "Ponteiros e Alocacao Dinamica - Raylib");
    SetTargetFPS(60);

    int linhas = ALTURA_JANELA / TAM_CELULA;
    int colunas = LARGURA_JANELA / TAM_CELULA;

    int **grade = criarMatriz(linhas, colunas);
    if (grade == NULL) {
        CloseWindow();
        return 1;
    }

    int quantidadeBolas = 12;
    Bola *bolas = criarBolas(quantidadeBolas);

    if (bolas == NULL) {
        liberarMatriz(grade, linhas);
        CloseWindow();
        return 1;
    }

    int celulasVisitadas = 0;

    while (!WindowShouldClose()) {

        /* EXERCICIO 1: adicionar bola */
        if (IsKeyPressed(KEY_SPACE)) {
            int novaQuantidade = quantidadeBolas + 1;

            Bola *temp = realloc(
                bolas, novaQuantidade * sizeof(Bola)
            );

            if (temp != NULL) {
                bolas = temp;
                quantidadeBolas = novaQuantidade;

                Bola *b = bolas + (quantidadeBolas - 1);

                b->pos = (Vector2){
                    GetRandomValue(50, LARGURA_JANELA - 50),
                    GetRandomValue(50, ALTURA_JANELA - 50)
                };

                b->vel = (Vector2){
                    (float)GetRandomValue(-4, 4),
                    (float)GetRandomValue(-4, 4)
                };

                if (b->vel.x == 0) b->vel.x = 2;
                if (b->vel.y == 0) b->vel.y = 2;

                b->raio = (float)GetRandomValue(10, 25);
                b->cor = (Color){
                    GetRandomValue(100, 255),
                    GetRandomValue(100, 255),
                    GetRandomValue(100, 255),
                    255
                };
            }
        }

        /* EXERCICIO 1: remover ultima bola */
        if (IsKeyPressed(KEY_BACKSPACE) && quantidadeBolas > 0) {
            int novaQuantidade = quantidadeBolas - 1;

            if (novaQuantidade == 0) {
                free(bolas);
                bolas = NULL;
                quantidadeBolas = 0;
            } else {
                Bola *temp = realloc(
                    bolas, novaQuantidade * sizeof(Bola)
                );

                if (temp != NULL) {
                    bolas = temp;
                    quantidadeBolas = novaQuantidade;
                }
            }
        }

        /* Movimento + EXERCICIO 2 */
        for (int i = 0; i < quantidadeBolas; i++) {
            Bola *b = bolas + i;

            atualizarBola(b);

            int coluna = (int)(b->pos.x / TAM_CELULA);
            int linha = (int)(b->pos.y / TAM_CELULA);

            if (linha >= 0 && linha < linhas &&
                coluna >= 0 && coluna < colunas) {

                if (grade[linha][coluna] == 0) {
                    grade[linha][coluna] = 1;
                    celulasVisitadas++;
                }
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        desenharMatriz(grade, linhas, colunas);

        for (int i = 0; i < quantidadeBolas; i++)
            DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);

        DrawRectangle(5, 5, 370, 75, (Color){0, 0, 0, 170});

        DrawText(TextFormat("Bolas: %d", quantidadeBolas),
                 15, 12, 20, WHITE);

        DrawText(TextFormat("Celulas visitadas: %d", celulasVisitadas),
                 15, 37, 20, WHITE);

        DrawText("ESPACO: adicionar | BACKSPACE: remover",
                 15, 62, 15, WHITE);

        EndDrawing();
    }

    free(bolas);
    liberarMatriz(grade, linhas);
    CloseWindow();

    return 0;
}
