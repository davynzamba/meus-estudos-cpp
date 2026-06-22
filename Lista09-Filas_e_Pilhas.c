//1. Estrutura de fila é uma forma de salvar ordenar prioridades de operções e salvamento na memoria
//2. Entrada de dados que precisam ser trarados, tentativas criptograficas e teste de verdadeiros
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

//incicialização

#define MAX 10
int fila[MAX];
int ini = 0;
int fim = 0;

// -------------------------------
// Adicionando elemento a fila
void qstore(int i) {

    if (fim >= MAX)
    {
        printf("Fila cheia.\n");
        return;
    }

    fila[fim] = i;
    fim++;

}

// -------------------------------
// Removendo elemento a fila
int qretrivie(){
    
    if(ini == fim) {
        printf ("Fila vazia.\n");
        return 0;
    }

    ini++;
    return fila[ini-1];
}
//---------------------------------
// Mostra fila
void mostra_fila()
{
    printf("\nFila: Inicio --> ");
    for (int i = ini; i < fim; i++)
    {
        printf("%d ", fila[i]);
    }
    printf("<-- Fim");
}

// -------------------------------
// Invertendo os elementos
void inverte_fila(){
    int aux = 0;
    for (int i = ini; i <= fim/2; i++)
    {
        int j = fim - i;

        aux = fila[i];
        fila[i] = fila[j];
        fila[j] = aux;
    
    }
}

// -------------------------------
int main(){

    int opcao = 0;
    int valor;

    // Colocando 0 em todas as posições da fila
    for(int i =0; i < fim; i++)
        fila[i] = 0;


    do{
        printf("\n----------Menu----------\n");
        printf("Digite - 1 - para inserir um elemento a fila\n");
        printf("Digite - 2 - para remover um elemento da fila\n");
        printf("Digite - 3 - para inverter a fila\n");
        printf("Digite - 3 - para sair\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
                printf("Valor para inserir: ");
                scanf("%d", &valor);
                qstore(valor);
                mostra_fila();
                break;

            case 2:
                valor = qretrivie();
                if(ini != fim){
                    printf("Na vez: %d", valor);
                    mostra_fila();
                }
                break;

            case 3:
                inverte_fila();
                mostra_fila();
                break;
        }
        
    }while (opcao != 4);

    return 0;
}
