#include <stdio.h>

void substituipalavra (int i, char palavra[], char newworld[]) {
  int j;

  for (j=0; j<i; j++) {
      newworld[j] = palavra[j];
      printf("%c", newworld[j]);
  }
  printf("\t");

}

int main (){
FILE *instrucoes;
char letra, palavra[100], rotulo[15], operacao[10], operando1[10], operando2[10];
int i, letrint, parametro;

instrucoes = fopen("instrucoes.txt", "r");
if (instrucoes == NULL){
  printf("Este arquivo nao foi encontrado!\n");
}

parametro=1;
i=0;
while(!feof(instrucoes)){
  letra = getc(instrucoes);
  letrint = letra;
  if (letrint > 64 && letrint < 91){ /*verifica se letra eh maiuscula e, se sim, troca por minuscula*/
    letrint = letrint + 32;
    letra = letrint;
  }

  if (letra != ' ' && letra != '\n' && letra != '\t'){ /*verifica se o char faz parte de alguma palavra*/
    palavra[i]=letra;
    i++;
  }

  if(i>0){ /*verifica se ja tem alguma palavra sendo formada*/
    if ((letra == ' ' || letra == '\n' || letra == '\t')&&(palavra[i-1] != ' ')){ /*verifica se eh termino de palavra*/
      if (palavra[i-1] == ':'){ /*eh rotulo?*/
        i = i-1;
        printf("rotulo: ");
        substituipalavra(i, palavra, rotulo);
      }
      else {
        if (parametro == 1){
          printf("operacao: ");
          substituipalavra(i, palavra, operacao);
        }
        if (parametro == 2){
          printf("operando1: ");
          substituipalavra(i, palavra, operando1);
        }
          if (parametro == 3){
            printf("operando2: ");
            substituipalavra(i, palavra, operando2);
          }
          parametro++;
        }
      i=0;
    }
  }
  if (letra == '\n'){
    parametro = 1;
    printf(" \n");
  }
}
return 0;
}
