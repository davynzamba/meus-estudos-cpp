#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

typedef struct cliente
{
    int id;
    int telefone;
    int ddd;
    char nome[30];
    char endereco[100];
    char cidade[30];

}Tcliente;

void mostra_cliente()
{

}

int main()
{
    Tcliente clientes[10];

    clientes[0].id = 1;
    clientes[0].telefone = 40028922;
    clientes[0].ddd = 74;
    strcpy(clientes[0].nome, "Roberto");
    strcpy(clientes[0].endereco, "Rua XV de NOcembro, 100");
    strcpy(clientes[0].cidade, "Xique-Xique");

    mostra_cliente(clientes);
    //mostra cliente
    printf("\n\nCLiente: %d", clientes[0].id);
    printf("\nNome: %s", clientes[0].nome);
    printf("\nEndereço: %s - %s", clientes[0].endereco, clientes[0].cidade);
    printf("\nTelefone: (%d) %d\n\n", clientes[0].ddd, clientes[0].telefone);

    return 0;
}