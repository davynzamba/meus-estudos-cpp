#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct tipo_produto
{
	int codigo; // C�digo do produto
	double preco; // Pre�o do produto
	struct tipo_produto* proximo; /* Pr�ximo
	elemento da lista encadeada de Produtos */
} TProduto;

void listar(TProduto* noatual)
{
	int i = 0;
	printf("\n ------------------");
	/* Enquanto n�o chega no fim da lista */
	while (noatual != NULL)
	{
		i++;
		printf("\n\nProduto numero %d\n", i);
		printf("Codigo: %d\n", noatual->codigo);
		printf("Preco: R$ %.2lf", noatual->preco);
		noatual = noatual->proximo;
		/* Faz noatual apontar para o pr�ximo n� */
	}
	printf("\n\n ------------------");
}

//---------------------------------------------
void inserir(TProduto** cabeca)
{
	TProduto* noatual, * novono;
	int cod;
	double preco;
	printf("\nCodigo do novo produto: ");
	scanf("%d", &cod);
	printf("\nPreco do produto: R$ ");
	scanf("%lf", &preco);
	/* Se ainda n�o existe nenhum produto na lista */
	if (*cabeca == NULL)
	{/* cria o n� cabe�a */
		*cabeca = (TProduto*)malloc(sizeof(TProduto));
		(*cabeca)->codigo = cod;
		(*cabeca)->preco = preco;
		(*cabeca)->proximo = NULL;
	}
	else
	{ /* Se j� existem elementos na lista, deve percorre-la at� o
	seu final e inserir o novo elemento */
		noatual = *cabeca;
		/* Ao final do while noatual aponta para o ultimo n� */
		while (noatual->proximo != NULL)
			noatual = noatual->proximo;
		/* Aloca mem�ria para o novo n� */
		novono = (TProduto*)malloc(sizeof(TProduto));
		novono->codigo = cod;
		novono->preco = preco;
		novono->proximo = NULL;
		/* Faz o ultimo n� apontar para o novo n� */
		noatual->proximo = novono;
	}
}
//--------------------------------------------
void remove(TProduto** cabeca)
{
	int cod, esta_na_lista = 0; // 0 -> N�o est�; 1 --> Est�
	TProduto* noatual = NULL;
	TProduto* noanterior = NULL;

	printf("\n Digite o codigo do produto a ser removido: ");
	scanf("%d", &cod);

	noatual = *cabeca;
	//percorre a lista at� o final
	while (noatual != NULL)//
	{
		//compara os c�digo da lista com o c�digo a ser removido
		if (noatual->codigo == cod)
		{
			esta_na_lista = 1;
			break;
		}
		else
		{
			//armazena o n� anterior para conseguir remover um n� do meio da lista.
			noanterior = noatual;
			noatual = noatual->proximo;
		}
	}
	if (esta_na_lista)
	{
		// Se for remover o n� cabe�a
		if (*cabeca == noatual)
		{
			*cabeca = noatual->proximo;
			free(noatual);
		}
		else
		{
			// remove o �ltimo n� da lista
			if (noatual->proximo == NULL)
			{
				noanterior->proximo = NULL;
				free(noatual);
			}
			else
			{
				noanterior->proximo = noatual->proximo;
				free(noatual);
			}
		}
		printf("\nProduto codigo %d removido com sucesso.", cod);		
	
	}
	else
		printf("\nProduto nao esta na lista!");
}

//--------------------------------------------
void main()
{
	/* Ponteiro para a cabeca da lista */
	TProduto* cabeca = NULL;
	TProduto* noatual; /* Ponteiro a ser usado para
	percorrer a lista no momento de desalocar seus
	elementos*/
	char q; // Caractere para receber a op��o do usu�rio
	do 
	{
		printf("\n\nOpcoes: \nI -> para inserir novo produto; \nL -> para listar os produtos; \nR -> para remover um produto; \nS -> para sair. \nDigite a sua opcao: ");
		scanf("%c", &q); /* Le a op��o do usu�rio */
		switch (q)
		{
		case 'i': case 'I':
			inserir(&cabeca);
			break;
		case 'l': case 'L':
			listar(cabeca);
			break;
		case 'r': case 'R':
			remove(&cabeca);
			break;
		case 's': case 'S':
			break;
		default:
			printf("\n\n Opcao invalida");
		}
		getchar();
	} while ((q != 's') && (q != 'S'));

	/* Desaloca a mem�ria que foi alocada para 
	os elementos da lista */
	noatual = cabeca;
	while (noatual != NULL)
	{
		cabeca = noatual->proximo;
		free(noatual);
		noatual = cabeca;
	}
}