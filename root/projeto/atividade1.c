    // /*
 * Ponteiros + Alocação Dinâmica (vetor e matriz) com raylib
 * ---------------------------------------------------------------
 * Este programa desenha:
 *   1) Uma matriz dinâmica (grade de células coloridas) alocada com malloc,
 *      onde cada linha é um ponteiro para um vetor de inteiros.
 *   2) Um um vetor dinâmico de bolinhas (struct Bola) que se movem na tela,
 *      manipuladas via ponteiros.
 *
 * Conceitos praticados:
 *   - malloc / free
 *   - ponteiro para ponteiro (int **) para representar matriz
 *   - vetor de structs alocado dinamicamente
 *   - passagem de ponteiros para funções (evita cópias, permite alterar
 *     o dado original)
 *   - aritmética de ponteiros ( *(p + i) é equivalente a p[i] )
 *
 * Compilar (Linux, com raylib instalada):
 *   gcc exemplo_ponteiros_raylib.c -o exemplo -lraylib -lm -lpthread -ldl -lrt -lX11
 *
 * Compilar (Windows, MinGW):
 *   gcc exemplo_ponteiros_raylib.c -o exemplo.exe -lraylib -lgdi32 -lwinmm
 */
#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define TAM_CELULA     40   // tamanho de cada célula da grade (matriz)


typedef struct {
    Vector2 pos;
    Vector2 vel;
    float   raio;
    Color   cor;
} Bola;
/* ---------------------------------------------------------------
 * cria uma MATRIZ dinâmica de inteiros (linhas x colunas)
 * Retorna um ponteiro para ponteiro (int **): cada posição do
 * vetor externo aponta para um vetor de inteiros (uma linha).
 * --------------------------------------------------------------- */
int **criarMatriz(int linhas, int colunas) {
    // aloca o vetor de ponteiros (um ponteiro por linha)
    int **matriz = (int **)malloc(linhas * sizeof(int *));
    if (matriz == NULL) return NULL;

    for (int i = 0; i < linhas; i++) {
        // aloca cada linha como um vetor de inteiros
        matriz[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            // Exercício 2: começa com todas as células não visitadas
            matriz[i][j] = 0;
        }
    }
    return matriz;
}
/* libera a memória da matriz: primeiro cada linha, depois o vetor de linhas */
void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);   // libera cada linha
    }
    free(matriz);           // libera o vetor de ponteiros
}
/* desenha a matriz na tela, célula por célula */
void desenharMatriz(int **matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            // Exercício 2: célula visitada fica mais clara
            Color cor = (matriz[i][j] == 1) ? (Color){40, 80, 120, 255}
                                             : (Color){15, 30, 55, 255};
            DrawRectangle(j * TAM_CELULA, i * TAM_CELULA,
                           TAM_CELULA - 2, TAM_CELULA - 2, cor);
        }
    }
}
/* ---------------------------------------------------------------
 * cria o vetor dinâmico de bolas
 * --------------------------------------------------------------- */
Bola *criarBolas(int quantidade) {
    Bola *bolas = (Bola *)malloc(quantidade * sizeof(Bola));
    if (bolas == NULL) return NULL;
    for (int i = 0; i < quantidade; i++) {
        // usar (bolas + i) é o mesmo que &bolas[i]: aqui acessamos
        // o campo via ponteiro para deixar explícito o conceito.
        Bola *b = (bolas + i);
        b->pos = (Vector2){ GetRandomValue(50, LARGURA_JANELA - 50),
                             GetRandomValue(50, ALTURA_JANELA - 50) };
        b->vel = (Vector2){ (float)GetRandomValue(-4, 4),
                             (float)GetRandomValue(-4, 4) };
        b->raio = (float)GetRandomValue(10, 25);
        b->cor  = (Color){ GetRandomValue(100,255), GetRandomValue(100,255),
                            GetRandomValue(100,255), 255 };
    }
    return bolas;
}
/* atualiza a posição de UMA bola: recebe um PONTEIRO para a struct,
 * então as alterações afetam diretamente o vetor original (sem cópia) */
void atualizarBola(Bola *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    // rebate nas bordas
    if (b->pos.x - b->raio < 0 || b->pos.x + b->raio > LARGURA_JANELA)
        b->vel.x *= -1;
    if (b->pos.y - b->raio < 0 || b->pos.y + b->raio > ALTURA_JANELA)
        b->vel.y *= -1;
}

int main(void) {
    srand((unsigned int)time(NULL));
    InitWindow(LARGURA_JANELA, ALTURA_JANELA,
               "Ponteiros e Alocacao Dinamica - raylib");
    SetTargetFPS(60);

    int linhas   = ALTURA_JANELA / TAM_CELULA;
    int colunas  = LARGURA_JANELA / TAM_CELULA;
    int **grade  = criarMatriz(linhas, colunas);   // matriz dinâmica

    int quantidadeBolas = 12;
    Bola *bolas = criarBolas(quantidadeBolas);      // vetor dinâmico

    // Exercício 2: contador de células visitadas (variável int, não ponteiro)
    int celulasVisitadas = 0;

    while (!WindowShouldClose()) {
        // Exercício 1: ESPAÇO adiciona uma nova bola usando realloc.
        if (IsKeyPressed(KEY_SPACE)) {
            int novaQuantidade = quantidadeBolas + 1;
            Bola *temp = (Bola *)realloc(bolas, novaQuantidade * sizeof(Bola));

            if (temp != NULL) {
                bolas = temp;
                quantidadeBolas = novaQuantidade;

                Bola *b = bolas + (quantidadeBolas - 1);
                b->pos = (Vector2){ GetRandomValue(50, LARGURA_JANELA - 50),
                                     GetRandomValue(50, ALTURA_JANELA - 50) };
                b->vel = (Vector2){ (float)GetRandomValue(-4, 4),
                                     (float)GetRandomValue(-4, 4) };
                b->raio = (float)GetRandomValue(10, 25);
                b->cor  = (Color){ GetRandomValue(100,255), GetRandomValue(100,255),
                                    GetRandomValue(100,255), 255 };
            }
        }

        // Exercício 1: BACKSPACE remove a última bola usando realloc.
        if (IsKeyPressed(KEY_BACKSPACE) && quantidadeBolas > 0) {
            int novaQuantidade = quantidadeBolas - 1;

            if (novaQuantidade == 0) {
                free(bolas);
                bolas = NULL;
                quantidadeBolas = 0;
            } else {
                Bola *temp = (Bola *)realloc(bolas, novaQuantidade * sizeof(Bola));

                if (temp != NULL) {
                    bolas = temp;
                    quantidadeBolas = novaQuantidade;
                }
            }
        }

        // percorre o vetor usando aritmética de ponteiros:
        // (bolas + i) aponta para o i-ésimo elemento do vetor
        for (int i = 0; i < quantidadeBolas; i++) {
            atualizarBola(bolas + i);

            // Exercício 2: converte a posição (x, y) em coluna e linha.
            int coluna = (int)(bolas[i].pos.x / TAM_CELULA);
            int linha  = (int)(bolas[i].pos.y / TAM_CELULA);

            // Verifica os limites antes de acessar grade[linha][coluna].
            if (linha >= 0 && linha < linhas &&
                coluna >= 0 && coluna < colunas) {

                // Só conta a célula na primeira vez que ela é visitada.
                if (grade[linha][coluna] == 0) {
                    grade[linha][coluna] = 1;
                    celulasVisitadas++;
                }
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            desenharMatriz(grade, linhas, colunas);

            for (int i = 0; i < quantidadeBolas; i++) {
                DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
            }

            DrawText("Matriz (int**) e vetor de structs (Bola*) alocados com malloc",
                     10, 10, 18, WHITE);
            DrawText(TextFormat("Celulas visitadas: %d", celulasVisitadas),
                     10, 35, 18, WHITE);
            DrawText(TextFormat("Quantidade de bolas: %d", quantidadeBolas),
                     10, 60, 18, WHITE);
            DrawText("ESPACO: adicionar | BACKSPACE: remover",
                     10, 85, 18, WHITE);
            DrawText("Pressione ESC para sair", 10, ALTURA_JANELA - 25, 16, WHITE);

        EndDrawing();
    }

    // libera TODA a memória alocada dinamicamente antes de encerrar
    free(bolas);
    liberarMatriz(grade, linhas);

    CloseWindow();
    return 0;
}
