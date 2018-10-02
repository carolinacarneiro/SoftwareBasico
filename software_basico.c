#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main()
{

	
	
	char nome_arquivo_leitura[100], ch;
	FILE *arquivo_leitura;
	FILE *arquivo_aux;

	printf("Digite o nome do arquivo:\n");
	scanf("%s", nome_arquivo_leitura);
	printf("O arquivo escolhido foi: %s \n", nome_arquivo_leitura);

	arquivo_leitura = fopen(nome_arquivo_leitura, "r");
	arquivo_aux = fopen("nome_arquivo_aux.txt", "w");

	if (arquivo_leitura == NULL)
	{
		printf("Não foi possível abrir o arquivo!\n");
	}
	else 
	{
		printf("Arquivo aberto com sucesso!\n");
	}


	while(ch != EOF)
	{
		ch = getc(arquivo_leitura);

		if((97<=ch)&&(ch<=122))
		{
			printf("Letra lida: %c\nCódigo ascii: %d \n", ch, ch);
			printf("Letra maiuscula: %c\nCódigo ascii %d", (ch-32), (ch-32));
			putc((ch-32), arquivo_aux);

		}
		else
		{
			printf("%c\n", ch);
			putc(ch, arquivo_aux);
		}
		

	}


	fclose(arquivo_leitura);
	fclose(arquivo_aux);

	return 0;






}