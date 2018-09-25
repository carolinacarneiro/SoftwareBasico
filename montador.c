#include <stdio.h>

int main (){
FILE *instrucoes;
char letra, palavra[100]/*, rotulo[15], operacao[10], operando1[10], operando2[10]*/;
int i, j;

instrucoes = fopen("instrucoes.txt", "r");
if (instrucoes == NULL){
  printf("Este arquivo nao foi encontrado!\n");
}

i=0;
while(!feof(instrucoes)){
  letra = getc(instrucoes);
  if (letra != ' ' && letra != ':' && letra != '\n'){
    palavra[i]=letra;
    i++;
  }
  if(i>0){
    if ((letra == ' ' || letra == '\n')&&(palavra[i-1] != ' ')){
      if (palavra[i-1] == ':'){
        i = i-1;
      }
      for (j=0; j<i; j++){
        printf("%c", palavra[j]);
      }
      printf (" acabou\n");
      i=0;
    }
  }
}


return 0;
}
