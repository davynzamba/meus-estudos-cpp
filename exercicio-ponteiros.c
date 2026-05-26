#include <stdio.h>
#include <stdlib.h>

// a. Função para alocar a matriz dinamicamente
int** alocar_matriz(int l, int c) {
    // Aloca o vetor de ponteiros (linhas)
    int **matriz = (int **)malloc(l * sizeof(int *));
    if (matriz == NULL) {
        printf("Erro de alocacao.\n");
        exit(1);
    }
    
    // Aloca o vetor de inteiros para cada linha (colunas)
    for (int i = 0; i < l; i++) {
        matriz[i] = (int *)malloc(c * sizeof(int));
        if (matriz[i] == NULL) {
            printf("Erro de alocacao na linha %d.\n", i);
            exit(1);
        }
    }
    
    return matriz;
}

// b. Função para preencher a matriz com números de 1 a (l * c)
void preencher_matriz(int **mat, int l, int c) {
    int contador = 1; // Inicia em 1 conforme exigido
    
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            mat[i][j] = contador;
            contador++;
        }
    }
}

// c. Função para mostrar a matriz na tela no formato tradicional
void mostrar_matriz(int **mat, int l, int c) {
    printf("Matriz preenchida:\n");
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            // O \t (tabulacao) ajuda a alinhar as colunas visualmente
            printf("%d\t", mat[i][j]); 
        }
        printf("\n"); // Quebra de linha ao final de cada linha da matriz
    }
}

int main() {
    int **mat; // Declaração do ponteiro mat na main, conforme exigido
    int l, c;

    // Solicitando as dimensões para testar o código
    printf("Digite o numero de linhas (l): ");
    scanf("%d", &l);
    printf("Digite o numero de colunas (c): ");
    scanf("%d", &c);
    printf("\n");

    // Executando o passo 'a'
    mat = alocar_matriz(l, c);

    // Executando o passo 'b'
    preencher_matriz(mat, l, c);

    // Executando o passo 'c'
    mostrar_matriz(mat, l, c);

    // --- Liberação da Memória Alocada ---
    for (int i = 0; i < l; i++) {
        free(mat[i]); // Libera cada uma das linhas
    }
    free(mat); // Libera o vetor de ponteiros principal

    return 0;
}