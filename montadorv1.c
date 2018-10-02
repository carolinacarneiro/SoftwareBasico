#include <stdio.h>

int main (){
FILE *instrucoes;
char letra, palavra[100], rotulo[15], operacao[10], operando1[10], operando2[10];
int i, j, letrint, parametro;

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
        for (j=0; j<i; j++){
          rotulo[j] = palavra[j];
          printf("%c", rotulo[j]);
        }
        printf(" ");
      }
      else {
        if (parametro == 1){
          printf("operacao: ");
          for (j=0; j<i; j++){
            operacao[j] = palavra[j];
            printf("%c", operacao[j]);
          }
          printf(" ");
        }
        if (parametro == 2){
          printf("operando1: ");
          for (j=0; j<i; j++){
            operando1[j] = palavra[j];
            printf("%c", operando1[j]);
          }
          printf(" ");
        }
          if (parametro == 3){
            printf("operando2: ");
            for (j=0; j<i; j++){
              operando2[j] = palavra[j];
              printf("%c", operando2[j]);
            }
          }
          parametro++;
        }
      /*printf (" acabou\n");*/
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
