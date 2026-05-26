//Prova 2
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE* arq = NULL;
    char nomes[30];
    float valor = 0, valor_maior = 0;
    int menu = 0;
    char nome_maior[30] = ""; // Inicializado vazio para evitar lixo de memória

    do
    {
        printf("\n------- Menu -------\n");
        printf("1 - Inserir um novo lance\n");
        printf("2 - Consultar o maior lance\n");
        printf("3 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &menu);
        
        switch (menu) {

            case 1:
                arq = fopen("lance.txt", "a+");

                if (arq != NULL) {
                    printf("Arquivo aberto com sucesso!\n");
                    printf("Digite o seu primeiro nome (Max. 30 caracteres):\n");
                    
                    // Limpa o buffer antes de ler a string
                    getchar(); 
                    
                    // fgets substitui o gets_s de forma segura e padrão
                    fgets(nomes, sizeof(nomes), stdin);
                    
                    // Remove a quebra de linha (\n) que o fgets armazena automaticamente
                    nomes[strcspn(nomes, "\n")] = '\0';

                    printf("Digite o seu valor do lance:\n");
                    scanf("%f", &valor);
                    
                    fprintf(arq, "%s\t%.2f\n", nomes, valor);
                    fclose(arq);
                } else {
                    printf("Erro ao abrir o arquivo.\n");
                }
                break;

            case 2:
                arq = fopen("lance.txt", "r");
                
                if (arq != NULL) {
                    printf("\nArquivo aberto com sucesso!\n");
                    
                    // Resetando o maior valor antes da leitura para sempre atualizar corretamente
                    valor_maior = 0; 
                    
                    // Corrigido a leitura: em %s passamos 'nomes' diretamente, sem o '&' e o '[0]'
                    while(fscanf(arq, "%s\t%f\n", nomes, &valor) != EOF)
                    {                                                                                                       
                        if (valor > valor_maior) {
                            valor_maior = valor;
                            strcpy(nome_maior, nomes);
                        }
                    }
                    fclose(arq);
                    
                    if (valor_maior > 0) {
                        printf("Este foi o maior lance: %s R$ %.2f\n", nome_maior, valor_maior);
                    } else {
                        printf("Nenhum lance registrado ainda.\n");
                    }
                   
                } else {
                    printf("\nNenhum arquivo de lances encontrado.\n");
                }
                break;
                
            case 3:
                printf("Saindo do programa...\n");
                return 0;
                break;
                
            default:
                printf("Opcao invalida!\n");
                break;
        } 

    } while (menu != 3);

    return 0;
}
