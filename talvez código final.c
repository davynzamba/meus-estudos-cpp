#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

/* Configurações de Dataset e Hiperparâmetros */
#define TOTAL_ROWS 70692
#define TOTAL_COLS 22
#define TRAIN_SIZE 56554
#define TEST_SIZE 14138
#define MAX_LINE_LENGTH 2048
#define NUM_TREES 10
#define LEARNING_RATE 0.3f
#define REG_LAMBDA 1.0f

/* Estrutura de Árvore de Decisão (Stump) */
typedef struct No {
    int feature_idx;
    float limiar;
    float peso_folha;
    struct No* esquerda, * direita;
} No;

/* --- Funções de Memória e Árvore --- */

No* criar_no(int f_idx, float lim, float peso) {
    No* novo = (No*)malloc(sizeof(No));
    novo->feature_idx = f_idx;
    novo->limiar = lim;
    novo->peso_folha = peso;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

void liberar_arvore(No* raiz) {
    if (raiz == NULL) return;
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}

float prever_arvore(No* raiz, float* amostra) {
    if (raiz->esquerda == NULL && raiz->direita == NULL) return raiz->peso_folha;
    if (amostra[raiz->feature_idx] <= raiz->limiar)
        return prever_arvore(raiz->esquerda, amostra);
    else
        return prever_arvore(raiz->direita, amostra);
}

/* --- Lógica Core XGBoost --- */

float sigmoid(float x) {
    return 1.0f / (1.0f + expf(-x));
}

// Cálculo do Ganho XGBoost: G = 0.5 * [(GL^2 / (HL + λ)) + (GR^2 / (HR + λ)) - ((GL+GR)^2 / (HL+HR + λ))]
float calcular_ganho(float g_l, float h_l, float g_r, float h_r) {
    float ganho = 0.5f * (
        (g_l * g_l) / (h_l + REG_LAMBDA) +
        (g_r * g_r) / (h_r + REG_LAMBDA) -
        ((g_l + g_r) * (g_l + g_r)) / (h_l + h_r + REG_LAMBDA)
        );
    return ganho;
}

No* treinar_nova_arvore(float* features, float* g, float* h, int n_amostras, int n_features) {
    int melhor_f = -1;
    float melhor_limiar = 0.0f;
    float melhor_ganho = -FLT_MAX;

    // Busca exaustiva pelo melhor split (Ganho Máximo)
    for (int f = 0; f < n_features; f++) {
        for (int i = 0; i < n_amostras; i++) {
            float limiar_atual = features[i * n_features + f];
            float gl = 0, hl = 0, gr = 0, hr = 0;

            for (int j = 0; j < n_amostras; j++) {
                if (features[j * n_features + f] <= limiar_atual) {
                    gl += g[j]; hl += h[j];
                }
                else {
                    gr += g[j]; hr += h[j];
                }
            }

            float ganho_atual = calcular_ganho(gl, hl, gr, hr);
            if (ganho_atual > melhor_ganho) {
                melhor_ganho = ganho_atual;
                melhor_f = f;
                melhor_limiar = limiar_atual;
            }
        }
    }

    // Cálculo dos pesos das folhas: w = -G / (H + λ)
    float gl_final = 0, hl_final = 0, gr_final = 0, hr_final = 0;
    for (int i = 0; i < n_amostras; i++) {
        if (features[i * n_features + melhor_f] <= melhor_limiar) {
            gl_final += g[i]; hl_final += h[i];
        }
        else {
            gr_final += g[i]; hr_final += h[i];
        }
    }

    float w_l = -gl_final / (hl_final + REG_LAMBDA);
    float w_r = -gr_final / (hr_final + REG_LAMBDA);

    No* raiz = criar_no(melhor_f, melhor_limiar, 0.0f);
    raiz->esquerda = criar_no(-1, 0, w_l * LEARNING_RATE);
    raiz->direita = criar_no(-1, 0, w_r * LEARNING_RATE);
    return raiz;
}

/* --- Processamento de Dados e Persistência --- */

void aplicar_normalizacao(float* train, float* test, int col_idx, int n_feat) {
    float min_v = FLT_MAX, max_v = -FLT_MAX;
    for (int i = 0; i < TRAIN_SIZE; i++) {
        float v = train[i * n_feat + col_idx];
        if (v < min_v) min_v = v;
        if (v > max_v) max_v = v;
    }
    float diff = max_v - min_v;
    if (diff < 1e-7) return;

    for (int i = 0; i < TRAIN_SIZE; i++) train[i * n_feat + col_idx] = (train[i * n_feat + col_idx] - min_v) / diff;
    for (int i = 0; i < TEST_SIZE; i++) test[i * n_feat + col_idx] = (test[i * n_feat + col_idx] - min_v) / diff;
}

void salvar_modelo(No** ensemble, int n_arvores, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) return;
    fprintf(f, "XGBOOST MODEL - DIABETES CLASSIFIER\n");
    for (int i = 0; i < n_arvores; i++) {
        fprintf(f, "TREE_%d: F_IDX=%d LIM=%.4f WL=%.4f WR=%.4f\n",
            i, ensemble[i]->feature_idx, ensemble[i]->limiar,
            ensemble[i]->esquerda->peso_folha, ensemble[i]->direita->peso_folha);
    }
    fclose(f);
}

void exibir_metricas(int tp, int fp, int tn, int fn) {
    int total = tp + fp + tn + fn;
    float err = (float)(fp + fn) / total;
    float acc = (float)(tp + tn) / total;
    float pre = (float)tp / (tp + fp + 1e-7);
    float rec = (float)tp / (tp + fn + 1e-7);
    float f1 = 2.0f * (pre * rec) / (pre + rec + 1e-7);

    printf("\n======================================================\n");
    printf("         RELATORIO DE PERFORMANCE - UTFPR             \n");
    printf("======================================================\n");
    printf("Matriz de Confusao:\n");
    printf("TP: %d | FP: %d\n", tp, fp);
    printf("TN: %d | FN: %d\n\n", tn, fn);
    printf("Métricas Calculadas:\n");
    printf("1. Erro de Previsao (Err): %.4f\n", err);
    printf("2. Acuracia (Acc)       : %.4f\n", acc);
    printf("3. Precisao (Pre)       : %.4f\n", pre);
    printf("4. Recall (Rec)         : %.4f\n", rec);
    printf("5. F1-Score             : %.4f\n", f1);
    printf("======================================================\n");
}

int main() {
    const int num_feat = TOTAL_COLS - 1;
    float* train_x = (float*)malloc(TRAIN_SIZE * num_feat * sizeof(float));
    float* train_y = (float*)malloc(TRAIN_SIZE * sizeof(float));
    float* test_x = (float*)malloc(TEST_SIZE * num_feat * sizeof(float));
    float* test_y = (float*)malloc(TEST_SIZE * sizeof(float));
    float* base_preds = (float*)calloc(TRAIN_SIZE, sizeof(float));

    FILE* csv = fopen("diabetes_binary_5050split_health_indicators_BRFSS2015.csv", "r");
    if (!csv) { printf("Erro ao abrir CSV.\n"); return 1; }

    char buffer[MAX_LINE_LENGTH];
    fgets(buffer, MAX_LINE_LENGTH, csv); // Pular cabeçalho

    for (int i = 0; i < TOTAL_ROWS; i++) {
        if (!fgets(buffer, MAX_LINE_LENGTH, csv)) break;
        char* token = strtok(buffer, ",");
        for (int j = 0; j < TOTAL_COLS; j++) {
            float val = (float)atof(token);
            if (i < TRAIN_SIZE) {
                if (j == 0) train_y[i] = val;
                else train_x[i * num_feat + (j - 1)] = val;
            }
            else {
                int ti = i - TRAIN_SIZE;
                if (j == 0) test_y[ti] = val;
                else test_x[ti * num_feat + (j - 1)] = val;
            }
            token = strtok(NULL, ",");
        }
    }
    fclose(csv);

    // Normalização Min-Max (Colunas não-binárias especificados pelo RACI)
    int norm_cols[] = { 3, 13, 14, 15, 18, 19, 20 }; // BMI, GenHlth, MentHlth, PhysHlth, Age, Education, Income
    for (int k = 0; k < 7; k++) aplicar_normalizacao(train_x, test_x, norm_cols[k], num_feat);

    // Loop de Treinamento
    No* ensemble[NUM_TREES];
    float* g = (float*)malloc(TRAIN_SIZE * sizeof(float));
    float* h = (float*)malloc(TRAIN_SIZE * sizeof(float));

    printf("Treinando XGBoost...");
    for (int t = 0; t < NUM_TREES; t++) {
        for (int i = 0; i < TRAIN_SIZE; i++) {
            float p = sigmoid(base_preds[i]);
            g[i] = p - train_y[i];
            h[i] = p * (1.0f - p);
        }
        ensemble[t] = treinar_nova_arvore(train_x, g, h, TRAIN_SIZE, num_feat);
        for (int i = 0; i < TRAIN_SIZE; i++) {
            base_preds[i] += prever_arvore(ensemble[t], &train_x[i * num_feat]);
        }
        printf(".");
    }

    salvar_modelo(ensemble, NUM_TREES, "modelo_diabetes.txt");

    // Avaliação no Conjunto de Teste
    int tp = 0, fp = 0, tn = 0, fn = 0;
    for (int i = 0; i < TEST_SIZE; i++) {
        float score = 0.0f;
        for (int t = 0; t < NUM_TREES; t++) score += prever_arvore(ensemble[t], &test_x[i * num_feat]);
        int pred = (sigmoid(score) >= 0.5f) ? 1 : 0;
        int real = (int)test_y[i];
        if (pred == 1 && real == 1) tp++;
        else if (pred == 1 && real == 0) fp++;
        else if (pred == 0 && real == 0) tn++;
        else if (pred == 0 && real == 1) fn++;
    }

    exibir_metricas(tp, fp, tn, fn);

    // Limpeza de Recursos
    for (int t = 0; t < NUM_TREES; t++) liberar_arvore(ensemble[t]);
    free(train_x); free(train_y); free(test_x); free(test_y);
    free(base_preds); free(g); free(h);

    return 0;
}