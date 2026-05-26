#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    
    int num_linhas = 4;
    int i = 0;
    int tam_nomes;
    char nome[30];
    char** mat;
    
    char nomes[4][30];
     
    for (i=0; i<num_linhas; i++){
        printf ("Digite o nome [%d]", i+1);
        scanf(" %[^\n]", nomes[i]);;
    }
    
    
    mat = (char **)malloc(num_linhas * sizeof(char*));
    
    for (i = 0; i < num_linhas; i++)
    {
        tam_nomes = strlen(nomes[i]) + 1;
        mat[i] = (char*)malloc(tam_nomes * sizeof(char));
    }
    
    for (i = 0; i < num_linhas; i++)
    {
        strcpy(mat[i], nomes[i]);
    }
    
    printf("Matriz de strings (nomes inseridos):\n");
    for (int i = 0; i < num_linhas; i++) {
        printf("[%d] %s\n", i, mat[i]);
    } 
    
    for (int i = 0; i < num_linhas; i++) {
        free(mat[i]); // Libera cada linha
    }
    
    free(mat);
    
    return 0;
}