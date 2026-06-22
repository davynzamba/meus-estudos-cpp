//1. Estrutura de fila é uma forma de salvar ordenar prioridades de operções e salvamento na memoria
//2. Entrada de dados que precisam ser trarados, tentativas criptograficas e teste de verdadeiros
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

//incicialização

#define MAX 10
int pilha[MAX];
int topo = 0;

// -------------------------------
// Adicionando elemento a pilha
void push(int i) {

    if (topo >= MAX)
    {
        printf("Pilha cheia.\n");
        return;
    }

    pilha[topo] = i;
    topo++;

}

// -------------------------------
// Removendo elemento a pilha
int pop(){
    topo--;
    if(topo < 0) {
        printf("Pilha vazia.\n");
        topo = 0;
        return 0;
    }
    return pilha[topo];
}
//---------------------------------
// Mostra pilha
void mostra_pilha()
{
    printf("\nPilha: Inicio\n->  ");
    for (int i = 0; i < topo; i++)
    {
        printf("%d\n", pilha[i]);
    }
    printf("<-- Topo\n");
}


// -------------------------------
int main(){

    char q;
    int valor;

    // Colocando 0 em todas as posições da pilha
    for(int i = 0; i < MAX; i++)
        pilha[i] = 0;


    do{
        printf("\n----------Menu----------\n");
        printf("Digite - I - para inserir um elemento da pilha\n");
        printf("Digite - R - para remover um elemento da pilha\n");
        printf("Digite - S - para sair\n");
        scanf("%c", &q); //lê a opção do usuário

        switch (q)
        {
            case 'I': case 'i':
                printf("Valor para inserir: ");
                scanf("%d", &valor);
                push(valor);
                mostra_pilha();
                break;

            case 'R': case 'r':
                valor = pop();
                printf("Valor removido da plha: %d", valor);
                mostra_pilha();
                break;

            case 'S': case 's':
                break;
        }
        
    }while ((q != 'S') && (q != 's'));

    return 0;
}
