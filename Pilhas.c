// ConsoleApplication1.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX 10
int pilha[MAX];
int topo = 0;

//--------------------------
// Insere na Pilha
void push(int i) {
    if (topo >= MAX) {
        printf("Pilha cheia.\n");
        return;
    }

    pilha[topo] = i;
    topo++;
}

//--------------------------
// Remove da Pilha
int pop() {
    topo--;
    if (topo < 0) {
        printf("Pilha vazia.\n");
        topo = 0;
        return 0;
    }
    return pilha[topo];
}

//--------------------------
// Mostra Pilha
void mostra_pilha( )
{
    printf("\nPilha: ");
    for (int i = topo - 1; i >= 0; i--)
        printf("\n%d", pilha[i]);
    printf("\n");
}

//--------------------------
// Main
int main()
{
    int valor = 0;
    int opcao = 0;
    int pali = 1; // se é palíndromo = 1, caso contrário 0
    int tam = 0; // tamanho do palíndromo
    do
    {
        printf("\n\nDigite - 1 - para inserir na pilha;");
        printf("\nDigite - 2 - para remover da pilha;");
        printf("\nDigite - 3 - para verificar palindromo;");
        printf("\nDigite - 4 - para sair;");
        printf("\nOpcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\nValor para inserir: ");
            scanf("%d", &valor);
            push(valor);
            mostra_pilha();
            tam++;
            break;

        case 2:
            valor = pop();
            printf("\nValor removido da pilha: %d", valor);
            mostra_pilha();
            tam--;
            break;

        case 3:
            for (int i = 0; i < tam / 2; i++)
                if (pilha[i] != pilha[tam - 1 - i])
                {
                    pali = 0;
                    break;
                }
            if (pali == 1)
                printf("\nEh um palindromo!");
            else
                printf("\nNao eh um palindromo!");

            break;

        }


    } while (opcao != 4);
}
