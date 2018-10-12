#include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>
using namespace std;

typedef struct diretiva {
  char nome[50];
  int valor;
} diretiva;

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

void preproc ()  {

  FILE *instrucoes, *instrucoes0;
  char letra, palavra[100], rotulo[50], operacao[50], operando1[50], operando2[50];
  int i, letrint, parametro, tam_rot, mudarot=0, mudaop=0, mudaop1=0, mudaop2=0, ignoralinha=0;
  list<diretiva> tab_preproc;
  diretiva elemento_preproc;
  list<diretiva>::iterator iterador_p;

  instrucoes = fopen("instrucoes.txt", "r");
  instrucoes0 = fopen("instrucoes0.txt", "w+");

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
    }

    if(i>0){ //verifica se ja tem alguma palavra sendo formada//
      if ((letra == ' ' || letra == '\n' || letra == '\t')&&(palavra[i-1] != ' ')){ //verifica se eh termino de palavra//
        if (palavra[i-1] == ':'){ //eh rotulo?//
          mudarot = 1;
          i = i-1;
          tam_rot = i;
          substituipalavra(i, palavra, rotulo);
        }
        else {
          if (parametro == 1){
            mudaop = 1;
            substituipalavra(i, palavra, operacao);
          }
          if (parametro == 2){
            mudaop1 = 1;
            substituipalavra(i, palavra, operando1);
          }
            if (parametro == 3){
              mudaop2 = 1;
              substituipalavra(i, palavra, operando2);
            }
            parametro++;
          }
        i=0;
      }
    }
    if (letra == '\n'){ //se a linha terminou//
      if (mudaop == 1){
        if (strcmp(operacao, "equ")==0){
          substituipalavra(tam_rot, rotulo, elemento_preproc.nome);
          elemento_preproc.valor = operando1[0] - 48;
          printf("valor: %d\t", elemento_preproc.valor);
          tab_preproc.push_back(elemento_preproc);
        }
        else if (strcmp(operacao, "if")==0){
          iterador_p = tab_preproc.begin();
          while(strcmp(iterador_p->nome, rotulo)!=0 && iterador_p != tab_preproc.end()){ //tenta achar rotulo na lista de simbolos//
            iterador_p++;
          }
          if (iterador_p == tab_preproc.end()){
            printf("Erro! Rotulo nao encontrado!\t");
          }
          else {
            if (iterador_p->valor != 1) {
              ignoralinha = 1;
            }
          }
        }
        else{
          if (ignoralinha == 0) {
            if (mudarot == 1) {
              fputs(rotulo, instrucoes0);
              fputs(":", instrucoes0);
              fputs("\t", instrucoes0);
            }
            fputs(operacao, instrucoes0);
            fputs("\t", instrucoes0);
            if (mudaop1 == 1) {
              fputs(operando1, instrucoes0);
              fputs("\t", instrucoes0);
            }
            if (mudaop2 == 1) {
              fputs(operando2, instrucoes0);
            }
            fputs("\n", instrucoes0);
          }
          else {
            ignoralinha = 0;
          }
        }
      }
      parametro = 1;
      printf(" \n");
      mudarot = 0; mudaop = 0; mudaop1 = 0; mudaop2 = 0;
    }
  }
  fclose(instrucoes);
  fclose(instrucoes0);
  printf ("fim pre processamento!\n");
  printf("\n -------------------------------------------- \n");
}

int descobreinstrucao (char instrucao[]) {

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

int descobrediretiva (char instrucao[]) {

  if(strcmp(instrucao,"space")==0){
    return 1;
  }
  else if(strcmp(instrucao,"const")==0){
    return 2;
  }
  else if(strcmp(instrucao,"section")==0) {
    return 3;
  }
  else {
    return 0;
  }
}

int main (){
  FILE *instrucoes1;
  char letra, palavra[50], rotulo[50], operacao[50], operando1[50], operando2[50];
  int i, letrint, parametro, tam_rot, num_op, contadorpos=0, contadorlinha=0, mudaop=0, num_dir=0;
  list<simb> tab_simb;
  simb elemento_simb;
  list<simb>::iterator iterador;

  preproc();

  instrucoes1 = fopen("instrucoes0.txt", "r");
  if (instrucoes1 == NULL){
    printf("Este arquivo nao foi encontrado!\n");
  }

  parametro=1;
  i=0;
  while(!feof(instrucoes1)){
    letra = getc(instrucoes1);

    if (letra != ' ' && letra != '\n' && letra != '\t'){ //verifica se o char faz parte de alguma palavra//
      palavra[i]=letra;
      palavra[i+1] = '\0';
      i++;
//      if(strcmp(palavra,"section")==0) { //se for section, ignora//
//        i=0;
//        while (letra != '\n') {
//          letra = getc(instrucoes1);
//        }
//      }
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
      if (strcmp(operacao, "space")==0 || strcmp(operacao, "const")==0){
        substituipalavra(tam_rot, rotulo, elemento_simb.rotulo);
        elemento_simb.endereco = contadorpos;
        iterador = tab_simb.begin();
        while(strcmp(iterador->rotulo, rotulo)!=0 && iterador != tab_simb.end()){ //tenta achar rotulo na lista de simbolos//
          iterador++;
        }
        if(iterador != tab_simb.end()) { //achou elemento//
          printf("Erro! Simbolo Redefinido\n");
        }
        else {
          printf("pos: %d ", elemento_simb.endereco);
          tab_simb.push_back(elemento_simb);
        }
      }
      if (mudaop == 1) {
        num_op = descobreinstrucao(operacao);
        if ((num_op>=1 && num_op<=8)||(num_op>=10 && num_op<=13)){
          contadorpos = contadorpos+2;
        }
        else if(num_op == 9) {
          contadorpos = contadorpos+3;
        }
        else if(num_op == 14) {
           contadorpos++;
        }
        else if (num_op == 0) {
          num_dir = descobrediretiva(operacao);
          if (num_dir == 1 || num_dir == 2){
            contadorpos++;
          }
          if (num_dir == 0) {
            printf("Erro! Nao eh operacao valida!\t");
          }
        }
        contadorlinha++;
      }

      printf("cpos: %d", contadorpos);
      parametro = 1;
      printf(" \n");
      mudaop = 0;
    }
  }
  fclose(instrucoes1);
  return 0;
}
