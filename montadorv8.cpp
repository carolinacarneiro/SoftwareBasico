#include <stdio.h>
#include <stdlib.h>
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

void preencheparametro(int *i, int *parametro, int *mudarot, int *mudaop, int *mudaop1, int *mudaop2, int tam_rot, char palavra[], char rotulo[], char operacao[], char operando1[], char operando2[], char letra) {
  if(*i>0){ //verifica se ja tem alguma palavra sendo formada//
    //printf("palavra[i-1]: %c\n", palavra[*i-1]);
    if ((letra == ' ' || letra == '\n' || letra == '\t' || letra == ':')&&(palavra[*i-1] != ' ')){ //verifica se eh termino de palavra//
      if (letra == ':'){ //eh rotulo?//
        *mudarot = 1;
        *i = *i-1;
        tam_rot = *i;
        printf("rotulo: ");
        strcpy(rotulo, palavra);
        printf("%s\t", rotulo);
      }
      else {
        if (*parametro == 1){
          *mudaop = 1;
          printf("operacao: ");
          strcpy(operacao, palavra);
          printf("%s\t", operacao);
        }
        if (*parametro == 2){
          *mudaop1 = 1;
          printf("operando1: ");
          strcpy(operando1, palavra);
          printf("%s\t", operando1);
        }
          if (*parametro == 3){
            *mudaop2 = 1;
            printf("operando2: ");
            strcpy(operando2, palavra);
            printf("%s\t", operando2);
          }
          *parametro = *parametro + 1;
        }
      *i=0;
    }
  }
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

void preproc ()  { // funcao que realiza o preprocessamento

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

    if (letra != ' ' && letra != '\n' && letra != '\t' && letra != ':' && letra != ','){ //verifica se o char faz parte de alguma palavra//
      palavra[i]=letra;
      palavra[i+1] = '\0';
      i++;
    }

    preencheparametro(&i, &parametro, &mudarot, &mudaop, &mudaop1, &mudaop2, tam_rot, palavra, rotulo, operacao, operando1, operando2, letra);

    if (letra == '\n'){ //se a linha terminou//
      if (mudaop == 1){
        if (strcmp(operacao, "equ")==0){
          printf("rotulo: %s\n", rotulo);
          strcpy(elemento_preproc.nome, rotulo);
          elemento_preproc.valor = atoi(operando1); //corrigir o inteiro dado pela tabela ascii
          printf("elemento_nome: %s\n", elemento_preproc.nome);
          printf("valor: %d\t", elemento_preproc.valor);
          tab_preproc.push_back(elemento_preproc);
        }
        else if (strcmp(operacao, "if")==0){
          iterador_p = tab_preproc.begin();
          while(strcmp(iterador_p->nome, rotulo)!=0 && iterador_p != tab_preproc.end()){ //tenta achar rotulo na lista de simbolos//
            printf("iterador->nome: %s , rotulo: %s\n", iterador_p->nome, rotulo);
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
            printf("mudarot: %d\t", mudarot);
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

void primeirapassagem (list<simb> *tab_simb){
  FILE *instrucoes1;
  char letra, palavra[50], rotulo[50], operacao[50], operando1[50], operando2[50];
  int i, letrint, parametro, tam_rot, num_op, contadorpos=0, contadorlinha=0, mudaop=0,
      mudarot=0, mudaop1=0, mudaop2=0, num_dir=0;
  simb elemento_simb;
  list<simb>::iterator iterador;

  instrucoes1 = fopen("instrucoes0.txt", "r");
  if (instrucoes1 == NULL){
    printf("Este arquivo nao foi encontrado!\n");
  }

  parametro=1;
  i=0;
  while(!feof(instrucoes1)){
    letra = getc(instrucoes1);

    if (letra != ' ' && letra != '\n' && letra != '\t' && letra != ':'){ //verifica se o char faz parte de alguma palavra//
      palavra[i]=letra;
      palavra[i+1] = '\0';
      i++;
    }

    preencheparametro(&i, &parametro, &mudarot, &mudaop, &mudaop1, &mudaop2, tam_rot, palavra, rotulo, operacao, operando1, operando2, letra);

    if (letra == '\n'){ //se a linha terminou//
      printf("mudarot: %d\t", mudarot);
      if (mudarot == 1){
        strcpy(elemento_simb.rotulo, rotulo);
        elemento_simb.endereco = contadorpos;
        iterador = tab_simb->begin();
        while(strcmp(iterador->rotulo, rotulo)!=0 && iterador != tab_simb->end()){ //tenta achar rotulo na lista de simbolos//
          printf("iterador->rotulo: %s\t", iterador->rotulo);
          iterador++;
        }
        if(iterador != tab_simb->end()) { //achou elemento//
          printf("Erro! Simbolo Redefinido\n");
        }
        else {
          printf("pos: %d ", elemento_simb.endereco);
          tab_simb->push_back(elemento_simb);
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
      mudaop = 0; mudarot = 0;
    }
  }
  fclose(instrucoes1);
  printf ("fim primeira passagem!\n");
  printf("\n -------------------------------------------- \n");
}

void segundapassagem (list<simb> tab_simb){
  FILE *instrucoes1, *instrucoes2;
  char letra, palavra[50], rotulo[50], operacao[50], operando1[50], operando2[50];
  int i, tam_rot, parametro, num_op, contadorpos=0, contadorlinha=0, mudaop=0,
      mudarot=0, mudaop1=0, mudaop2=0, num_dir=0;
  simb elemento_simb;
  list<simb>::iterator iterador, iterador2;


  instrucoes2 = fopen ("instrucoes2.txt", "w+");
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
    }

    preencheparametro(&i, &parametro, &mudarot, &mudaop, &mudaop1, &mudaop2, tam_rot, palavra, rotulo, operacao, operando1, operando2, letra);

    if (letra == '\n'){ //se a linha terminou//
      num_op = descobreinstrucao(operacao);
      if ((num_op>=1 && num_op<=8)||(num_op>=10 && num_op<=13)){
        iterador = tab_simb.begin();
        while(strcmp(iterador->rotulo, operando1)!=0 && iterador != tab_simb.end()){ //tenta achar rotulo na lista de simbolos//
          iterador++;
        }
        if (iterador == tab_simb.end()){
          printf("Erro! Simbolo Indefinido1!\t");
        }
        else {
          fprintf(instrucoes2, "%d", num_op);
          fputs(" ", instrucoes2);
          fprintf(instrucoes2, "%d", iterador->endereco);
          fputs(" ", instrucoes2);
        }
        contadorpos = contadorpos+2;
      }
      else if(num_op == 9) {
        iterador = tab_simb.begin();
        while(strcmp(iterador->rotulo, operando1)!=0 && iterador != tab_simb.end()){ //tenta achar rotulo na lista de simbolos//
          iterador++;
        }
        if (iterador == tab_simb.end()){
          printf("Erro! Simbolo Indefinido2!\t");
        }
        else {
          iterador2 = tab_simb.begin();
          while(strcmp(iterador2->rotulo, operando2)!=0 && iterador != tab_simb.end()){ //tenta achar rotulo na lista de simbolos//
            iterador2++;
          }
          if (iterador2 == tab_simb.end()){
            printf("Erro! Simbolo Indefinido!\t");
          }
          else {
            fprintf(instrucoes2, "%d", num_op);
            fputs(" ", instrucoes2);
            fprintf(instrucoes2, "%d", iterador->endereco);
            fputs(" ", instrucoes2);
            fprintf(instrucoes2, "%d", iterador2->endereco);
            fputs(" ", instrucoes2);
          }
        }
        contadorpos = contadorpos+3;
      }
      else if(num_op == 14) {
        fprintf(instrucoes2, "%d", num_op);
        fputs(" ", instrucoes2);
        contadorpos++;
      }
      else if (num_op == 0) {
        num_dir = descobrediretiva(operacao);
        if (num_dir == 1){
          printf("00 ");
          fputs("00", instrucoes2);
          fputs(" ", instrucoes2);
          contadorpos++;
        }
        if (num_dir == 2) {
          fputs(operando1, instrucoes2);
          fputs(" ", instrucoes2);
        }
        if (num_dir == 0) {
          printf("Erro! Nao eh operacao valida!\t");
        }
      }
      contadorlinha++;
      printf("cpos: %d", contadorpos);
      parametro = 1;
      printf(" \n");
      mudaop = 0;
    }
  }

  fclose(instrucoes1);
  fclose(instrucoes2);
}

int main (){
  FILE *instrucoes1;
  char letra, palavra[50], rotulo[50], operacao[50], operando1[50], operando2[50];
  int i, letrint, parametro, tam_rot, num_op, contadorpos=0, contadorlinha=0, mudaop=0,
      mudarot=0, mudaop1=0, mudaop2=0, num_dir=0;
  list<simb> tab_simb;
  simb elemento_simb;
  list<simb>::iterator iterador;

  preproc();
  primeirapassagem (&tab_simb);
  segundapassagem (tab_simb);

  return 0;
}
