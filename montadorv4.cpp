#include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>
using namespace std;

typedef struct simb {
  char rotulo[15];
  int endereco;
} simb;

void substituipalavra (int i, char palavra[], char newword[]) {
  int j;
  for (j=0; j<i; j++) {
      newword[j] = palavra[j];
      printf("%c", newword[j]);
  }
  newword[i] = '\0';
  printf("\t");
}

int descobreoperacao (char instrucao[]) {

  if(strcmp(instrucao,"add")==0){
    return 1;
  }
  else if(strcmp(instrucao,"sub")==0){
    return 2;
  }
  else if(strcmp(instrucao,"mult")==0){
    return 3;
  }
  else if(strcmp(instrucao,"div")==0){
    return 4;
  }
  else if(strcmp(instrucao,"jmp")==0){
    return 5;
  }
  else if(strcmp(instrucao,"jmpn")==0){
    return 6;
  }
  else if(strcmp(instrucao,"jmpp")==0){
    return 7;
  }
  else if(strcmp(instrucao,"jmpz")==0){
    return 8;
  }
  else if(strcmp(instrucao,"copy")==0){
    return 9;
  }
  else if(strcmp(instrucao,"load")==0){
    return 10;
  }
  else if(strcmp(instrucao,"store")==0){
    return 11;
  }
  else if(strcmp(instrucao,"input")==0){
    return 12;
  }
  else if(strcmp(instrucao,"output")==0){
    return 13;
  }
  else if(strcmp(instrucao,"stop")==0){
    return 14;
  }
  else {
    return 0;
  }
}

int main (){
  FILE *instrucoes;
  char letra, palavra[100], rotulo[15], operacao[10], operando1[10], operando2[10];
  int i, letrint, parametro, tam_rot, num_op, contadorpos=0, contadorlinha=0, mudaop=0;
  list<simb> tab_simb;
  simb elemento_simb;//, elemento1
  list<simb>::iterator iterador;

//  triangulo[8] = 'o';
// strcmp(palavra, "section");
//  substituipalavra(9, triangulo, elemento1.rotulo);
//  tab_simb.push_back(elemento1);

  instrucoes = fopen("instrucoes.txt", "r");
  if (instrucoes == NULL){
    printf("Este arquivo nao foi encontrado!\n");
  }

  parametro=1;
  i=0;
  while(!feof(instrucoes)){
    letra = getc(instrucoes);
    if (letra == ';'){ //se for comentario, ignora tudo ate o \n//
      while (letra != '\n') {
        letra = getc(instrucoes);
      }
    }

    letrint = letra;
    if (letrint > 64 && letrint < 91){ //verifica se letra eh maiuscula e, se sim, troca por minuscula//
      letrint = letrint + 32;
      letra = letrint;
    }

    if (letra != ' ' && letra != '\n' && letra != '\t'){ //verifica se o char faz parte de alguma palavra//
      palavra[i]=letra;
      palavra[i+1] = '\0';
      i++;
      if(strcmp(palavra,"section")==0) { //se for section, ignora//
        i=0;
        while (letra != '\n') {
          letra = getc(instrucoes);
        }
      }
    }

    if(i>0){ //verifica se ja tem alguma palavra sendo formada//
      if ((letra == ' ' || letra == '\n' || letra == '\t')&&(palavra[i-1] != ' ')){ //verifica se eh termino de palavra//
        if (palavra[i-1] == ':'){ //eh rotulo?//
          i = i-1;
          tam_rot = i;
          printf("rotulo: ");
          substituipalavra(i, palavra, rotulo);
        }
        else {
          if (parametro == 1){
            mudaop = 1;
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
    if (letra == '\n'){ //se a linha terminou//
      if (strcmp(elemento_simb.rotulo, rotulo)!=0){ //compara se eh um rotulo diferente do anterior//
        substituipalavra(tam_rot, rotulo, elemento_simb.rotulo);
        elemento_simb.endereco = contadorpos;
        iterador = tab_simb.begin();
        while(strcmp(iterador->rotulo, rotulo)!=0 && iterador != tab_simb.end()){ //tenta achar rotulo na lista de simbolos//
          iterador++;
        }
        if(iterador != tab_simb.end()) { //achou elemento//
          printf("Erro! \n");
        }
        else {
          printf("pos: %d", elemento_simb.endereco);
          tab_simb.push_back(elemento_simb);
        }
      }
      if (mudaop == 1) {
        num_op = descobreoperacao(operacao);
        if ((num_op>=1 && num_op<=8)||(num_op>=10 && num_op<=13)){
          contadorpos = contadorpos+2;
        }
        else if(num_op == 9) {
          contadorpos = contadorpos+3;
        }
        else if(num_op == 14) {
           contadorpos++;
        }
        contadorlinha++;
      }

      printf("cpos: %d", contadorpos);
      parametro = 1;
      printf(" \n");
      mudaop = 0;
    }
  }
  return 0;
}
