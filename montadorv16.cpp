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
  char rotulo[50];
  int endereco;
  int externa;
  int secao_atual = 0;
} simb;

typedef struct def {
  char rotulo[50];
  int endereco;
} def;

typedef struct uso {
  char rotulo[50];
  int endereco[50];
  int indice_end = 0;
} uso;

void preencheparametro(int *i, int *parametro, int *mudaadic, int *mudarot, int *mudaop, int *mudaop1, int *mudaop2, int tam_rot, char palavra[], char rotulo[], char operacao[], char operando1[], char operando2[], char letra, char adicionado[]) {
  if(*i>0 || letra == '+'){ //verifica se ja tem alguma palavra sendo formada//
    //printf("palavra[i-1]: %c\n", palavra[*i-1]);
    if ((letra == ' ' || letra == '\n' || letra == '\t' || letra == ':' || letra == '+')){ //verifica se eh termino de palavra//
      if (letra == ':'){ //eh rotulo?//
        *mudarot = *mudarot + 1;
        *i = *i-1;
        tam_rot = *i;
        printf("rotulo: ");
        strcpy(rotulo, palavra);
        printf("%s\t", rotulo);
      }
      else if (*mudaadic == 1) {
        strcpy(adicionado, palavra);
        printf("adicionado: %s\t", adicionado);
      }
      else {
        if (letra == '+') {
          *mudaadic = 1;
        }
        if (*parametro == 1 && *mudaop == 0){
          *mudaop = 1;
          printf("operacao: ");
          strcpy(operacao, palavra);
          printf("%s\t", operacao);
        }
        if (*parametro == 2 && *mudaop1 == 0){
          *mudaop1 = 1;
          printf("operando1: ");
          strcpy(operando1, palavra);
          printf("%s\t", operando1);
        }
          if (*parametro == 3 && *mudaop2 == 0){
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
  else if(strcmp(instrucao,"public")==0) {
    return 4;
  }
  else if(strcmp(instrucao,"extern")==0) {
    return 5;
  }
  else {
    return 0;
  }
}

void preencheuso (list<uso> *tab_uso, list<simb> tab_simb, char operando1[], char operando2[], int mudaop1, int mudaop2, int contadorpos) {
  list<simb>::iterator iterador_simb;
  list<uso>::iterator iterador_uso;
  uso elemento_uso;

  iterador_uso = tab_uso->begin();
  if (mudaop1 == 1) {
    while(strcmp(iterador_uso->rotulo, operando1)!=0 && iterador_uso != tab_uso->end()){
      iterador_uso++;
    }
    if (iterador_uso != tab_uso->end()) {
      iterador_uso->endereco[iterador_uso->indice_end] = contadorpos + 1;
      iterador_uso->indice_end ++;
    }
  }

  iterador_uso = tab_uso->begin();
  if (mudaop2 == 1) {
    while(strcmp(iterador_uso->rotulo, operando2)!=0 && iterador_uso != tab_uso->end()){ //tenta achar rotulo na lista de simbolos//
      iterador_uso++;
    }
    if (iterador_uso != tab_uso->end()) {
      iterador_uso->endereco[iterador_uso->indice_end] = contadorpos + 2;
      iterador_uso->indice_end ++;
    }
  }
}

void preproc ()  { // funcao que realiza o preprocessamento

  FILE *instrucoes, *instrucoes0;
  char letra, letrant, palavra[100], rotulo[50], operacao[50], operando1[50], operando2[50], operando3[10][50], adicionado[50];
  int i, j,letrint, parametro, tam_rot, mudarot=0, mudaop=0, mudaop1=0, mudaop2=0, ignoralinha=0, mudaadic=0, contadorlinha=1;
  list<diretiva> tab_preproc;
  diretiva elemento_preproc;
  list<diretiva>::iterator iterador_p;

  instrucoes = fopen("instrucoes.txt", "r");
  instrucoes0 = fopen("instrucoes0.txt", "w+");

  parametro=1;
  i=0;
  while(!feof(instrucoes)){
    letrant = letra;
    letra = getc(instrucoes);
    if (letra == ';'){ //se for comentario, ignora tudo ate o \n//
      while (letra != '\n') {
        letra = getc(instrucoes);
      }
    }
    if(letra == '+' && (letrant == '\t'|| letrant == ' ')){
      printf("mudou adic! \n");
      mudaadic = 1;
    }

    letrint = letra;
    if (letrint > 64 && letrint < 91){ //verifica se letra eh maiuscula e, se sim, troca por minuscula//
      letrint = letrint + 32;
      letra = letrint;
    }

    if (letra != ' ' && letra != '\n' && letra != '\t' && letra != ':' && letra != ',' && letra != '+'){ //verifica se o char faz parte de alguma palavra//
      palavra[i]=letra;
      palavra[i+1] = '\0';
      i++;
    }

    if(i>0 || letra == '+'){ //verifica se ja tem alguma palavra sendo formada//
      if ((letra == ' ' || letra == '\n' || letra == '\t' || letra == ':' || letra == '+')){ //verifica se eh termino de palavra//
        if (letra == ':'){ //eh rotulo?//
          mudarot = mudarot + 1;
          if (mudarot > 1) {
            fputs(rotulo, instrucoes0);
            fputs(":", instrucoes0);
            fputs("\t", instrucoes0);
            printf("Erro na linha %d. Mais de um rotulo\n", contadorlinha);
            mudarot = 1;
          }
          printf("rotulo: ");
          strcpy(rotulo, palavra);
          printf("%s\t", rotulo);
        }
        else if (mudaadic == 1) {
          strcpy(adicionado, palavra);
          printf("adicionado: %s\t", adicionado);
        }
        else {
          if (letra == '+') {
            mudaadic = 1;
          }
          if (parametro == 1 && mudaop == 0){
            mudaop = 1;
            printf("operacao: ");
            strcpy(operacao, palavra);
            printf("%s\t", operacao);
          }
          if (parametro == 2 && mudaop1 == 0){
            mudaop1 = 1;
            printf("operando1: ");
            strcpy(operando1, palavra);
            printf("%s\t", operando1);
          }
          if (parametro == 3 && mudaop2 == 0){
            mudaop2 = 1;
            printf("operando2: ");
            strcpy(operando2, palavra);
            printf("%s\t", operando2);
          }
          if (parametro > 3) {
            strcpy(operando3[parametro-4], palavra);
          }
          parametro = parametro + 1;
        }
        i=0;
      }
    }

    if (letra == '\n' && mudaop == 1){ //se a linha terminou//
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
            if (mudarot > 0) {
              fputs(rotulo, instrucoes0);
              fputs(":", instrucoes0);
              fputs("\t", instrucoes0);
            }
            fputs(operacao, instrucoes0);
            if (mudaop1 == 1) {
              fputs("\t", instrucoes0);
              fputs(operando1, instrucoes0);
            }
            if (mudaop2 == 1) {
              fputs("\t", instrucoes0);
              fputs(operando2, instrucoes0);
            }
            if(parametro > 3) {
              for(j=4; j < parametro; j++) {
                fputs("\t", instrucoes0);
                fputs(operando3[j-4], instrucoes0);
              }
            }
            if (mudaadic == 1) {
              fputs("+", instrucoes0);
              fputs(adicionado, instrucoes0);
            }
            fputs("\n", instrucoes0);
          }
          else {
            ignoralinha = 0;
          }
        }
      }
      contadorlinha++;
      parametro = 1;
      printf(" \n");
      mudarot = 0; mudaop = 0; mudaop1 = 0; mudaop2 = 0; mudaadic = 0;
    }
  }
  fclose(instrucoes);
  fclose(instrucoes0);
  printf ("fim pre processamento!\n");
  printf("\n -------------------------------------------- \n");
}

void primeirapassagem (list<simb> *tab_simb, list<def> *tab_def){
  FILE *instrucoes1;
  char letra, palavra[50], rotulo[50], operacao[50], operando1[50], operando2[50], adicionado[50];
  int i, letrint, parametro, tam_rot, num_op, contadorpos=0, contadorlinha=1, mudaop=0,
      mudarot=0, mudaop1=0, mudaop2=0, num_dir=0, mudaadic=0, secao_atual = 0;
  simb elemento_simb;
  def elemento_def;
  list<simb>::iterator iterador;

  instrucoes1 = fopen("instrucoes0.txt", "r");
  if (instrucoes1 == NULL){
    printf("Este arquivo nao foi encontrado!\n");
  }

  parametro=1;
  i=0;
  while(!feof(instrucoes1)){
    letra = getc(instrucoes1);

    if (letra != ' ' && letra != '\n' && letra != '\t' && letra != ':' && letra != '+'){ //verifica se o char faz parte de alguma palavra//
      palavra[i]=letra;
      palavra[i+1] = '\0';
      i++;
    }

    preencheparametro(&i, &parametro, &mudaadic, &mudarot, &mudaop, &mudaop1, &mudaop2, tam_rot, palavra, rotulo, operacao, operando1, operando2, letra, adicionado);

    if (letra == '\n'){ //se a linha terminou//
      printf("mudarot: %d\t", mudarot);
      if (mudarot > 0){
        strcpy(elemento_simb.rotulo, rotulo);
        iterador = tab_simb->begin();
        while(strcmp(iterador->rotulo, rotulo)!=0 && iterador != tab_simb->end()){ //tenta achar rotulo na lista de simbolos//
          iterador++;
        }
        if(iterador != tab_simb->end()) { //achou elemento//
          printf("Erro Semantico na linha %d. Simbolo Redefinido\n", contadorlinha);
        }
        else {
          if (descobrediretiva(operacao) == 5) { //detectar se a diretiva eh extern //
            elemento_simb.endereco = 0;
            elemento_simb.externa = 1;
            elemento_simb.secao_atual = 0;
          }
          else {
            elemento_simb.endereco = contadorpos;
            elemento_simb.externa = 0;
            elemento_simb.secao_atual = secao_atual;
            strcpy(elemento_def.rotulo, rotulo);
            elemento_def.endereco = contadorpos;
            tab_def->push_back(elemento_def);
          }
          printf("pos: %d ", elemento_simb.endereco);
          tab_simb->push_back(elemento_simb);
        }
      }
      if (mudaop == 1) {
        num_op = descobreinstrucao(operacao);
        num_dir = descobrediretiva(operacao);
        if (num_op == 0 && (num_dir >= 0 && num_dir <= 2) && secao_atual == 1) {
          printf("Erro Semantico na linha %d. Diretiva ou Instrucao na secao errada\n", contadorlinha);
        }
        if (secao_atual == 2 && num_dir != 2 && num_dir != 3) {
          printf("Erro Semantico na linha %d. Diretiva ou Instrucao na secao errada\n", contadorlinha);
        }
        if (secao_atual == 3 && num_dir != 1 && num_dir != 3) {
          printf("Erro Semantico na linha %d. Diretiva ou Instrucao na secao errada\n", contadorlinha);
        }
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
          if (num_dir == 1){ //diretiva space //
            if (mudaop1 == 1) { //space com + //
              contadorpos = contadorpos + atoi(operando1);
            }
            else {
            contadorpos++;
            }
          }
          if (num_dir == 2) {
            contadorpos++;
          }
          if (num_dir == 3) {
            if (strcmp(operando1, "text")==0) {
              secao_atual = 1;
            }
            else if (strcmp(operando1, "data")==0) {
              if (secao_atual == 0) {
                printf("Erro Semantico na linha %d. Nao ha section text.\n",contadorlinha);
              }
              secao_atual = 2;
            }
            else if (strcmp(operando1, "bss")==0) {
              if (secao_atual == 0) {
                printf("Erro Semantico na linha %d. Nao ha section text.\n",contadorlinha);
              }
              secao_atual = 3;
            }
            else {
              printf("Erro Sintatico na linha %d. Secao Invalida\n", contadorlinha);
            }
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
      mudaop = 0; mudarot = 0; mudaop1 = 0; mudaop2 = 0; mudaadic = 0;
    }
  }
  fclose(instrucoes1);
  printf ("fim primeira passagem!\n");
  printf("\n -------------------------------------------- \n");
}

void segundapassagem (list<simb> tab_simb){
  FILE *instrucoes1, *instrucoes2;
  char letra, palavra[50], rotulo[50], operacao[50], operando1[50], operando2[50], adicionado[50];
  int i, tam_rot, parametro, num_op, contadorpos=0, contadorlinha=1, mudaop=0,
      mudarot=0, mudaop1=0, mudaop2=0, num_dir=0, mudaadic=0, end_uso = 0;
  simb elemento_simb;
  list<simb>::iterator iterador, iterador2, iterador_simb;
  list<uso> tab_uso;
  list<uso>::iterator iterador_uso;
  uso elemento_uso;

  iterador_simb = tab_simb.begin(); //adiciona elementos externos a tabela de uso
  while(iterador_simb != tab_simb.end()) {
    if (iterador_simb->externa == 1) {
      strcpy(elemento_uso.rotulo, iterador_simb->rotulo);
      tab_uso.push_back(elemento_uso);
    }
    iterador_simb++;
  }

  instrucoes2 = fopen ("instrucoes2.txt", "w+");
  instrucoes1 = fopen("instrucoes0.txt", "r");
  if (instrucoes1 == NULL){
    printf("Este arquivo nao foi encontrado!\n");
  }

  parametro=1;
  i=0;
  while(!feof(instrucoes1)){
    letra = getc(instrucoes1);

    if (letra != ' ' && letra != '\n' && letra != '\t'  && letra != '+'){ //verifica se o char faz parte de alguma palavra//
      palavra[i]=letra;
      palavra[i+1] = '\0';
      i++;
    }

    preencheparametro(&i, &parametro, &mudaadic, &mudarot, &mudaop, &mudaop1, &mudaop2, tam_rot, palavra, rotulo, operacao, operando1, operando2, letra, adicionado);

    if (letra == '\n'){ //se a linha terminou//
      preencheuso (&tab_uso, tab_simb, operando1, operando2, mudaop1, mudaop2, contadorpos);
      num_op = descobreinstrucao(operacao);
      num_dir = descobrediretiva(operacao);
      if ((num_op>=1 && num_op<=8)||(num_op>=10 && num_op<=13)){
        if (mudaop2 == 1) {
          printf("Erro Sintatico na linha %d. Quantidade de operandos invalida!\n", contadorlinha);
        }
        iterador = tab_simb.begin();
        while(strcmp(iterador->rotulo, operando1)!=0 && iterador != tab_simb.end()){ //tenta achar rotulo na lista de simbolos//
          iterador++;
        }
        if(num_op >= 5 && num_op <= 8) { //algum tipo de jmp
          if(iterador->secao_atual != 1) {
            printf("Erro Semantico na linha %d. Pulo para secao errada!\n", contadorlinha);
          }
        }
        if (iterador == tab_simb.end()){
          printf("Erro Semantico! Operando %s Indefinido!\t", operando1);
        }
        else {
          fprintf(instrucoes2, "%d", num_op);
          fputs(" ", instrucoes2);
          if (mudaadic == 1) {
            fprintf(instrucoes2, "%d", iterador->endereco + atoi(adicionado));
          }
          else {
            fprintf(instrucoes2, "%d", iterador->endereco);
          }
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
          printf("Erro Semantico! Operando %s Indefinido!\t", operando1);
        }
        iterador2 = tab_simb.begin();
        while(strcmp(iterador2->rotulo, operando2)!=0 && iterador2 != tab_simb.end()){ //tenta achar rotulo na lista de simbolos//
          iterador2++;
        }
        if (iterador2 == tab_simb.end()){
          printf("Erro Semantico! Operando %s Indefinido!\t", operando2);
        }
        else {
          fprintf(instrucoes2, "%d", num_op);
          fputs(" ", instrucoes2);
          fprintf(instrucoes2, "%d", iterador->endereco);
          fputs(" ", instrucoes2);
          fprintf(instrucoes2, "%d", iterador2->endereco);
          fputs(" ", instrucoes2);
        }
        contadorpos = contadorpos+3;
      }
      else if(num_op == 14) {
        if (mudaop1 == 1) {
          printf("Erro Sintatico na linha %d. Quantidade de operandos invalida!\n", contadorlinha);
        }
        fprintf(instrucoes2, "%d", num_op);
        fputs(" ", instrucoes2);
        contadorpos++;
      }
      else if (num_op == 0) {
        if (num_dir == 1){
          if (mudaop2 == 1) {
            printf("Erro Sintatico na linha %d. Quantidade de operandos invalida!\n", contadorlinha);
          }
          printf("00 ");
          fputs("00", instrucoes2);
          fputs(" ", instrucoes2);
          contadorpos++;
        }
        if (num_dir == 2) {
          if (mudaop2 == 1) {
            printf("Erro Sintatico na linha %d. Quantidade de operandos invalida!\n", contadorlinha);
          }
          fputs(operando1, instrucoes2);
          fputs(" ", instrucoes2);
        }
        if (num_dir == 0) {
          printf("Erro Sintatico na linha %d,  Diretiva invalida!\t", contadorlinha);
        }
        if (num_dir > 5 || num_dir < 0) {
          printf("Erro Sintatico na linha %d, Instrucao invalida!\t", contadorlinha);
        }
      }
      contadorlinha++;
      //printf("cpos: %d", contadorpos);
      parametro = 1;
      printf(" \n");
      mudaop = 0; mudarot = 0; mudaop1 = 0; mudaop2 = 0; mudaadic = 0;
    }
  }

  fclose(instrucoes1);
  fclose(instrucoes2);

  printf("\nTabela de Uso: \n");
    iterador_uso = tab_uso.begin();
    while(iterador_uso != tab_uso.end()){
      printf("elemento: %s, %d, %d, %d\n", iterador_uso->rotulo, iterador_uso->endereco[0], iterador_uso->endereco[1], iterador_uso->endereco[2]);
      iterador_uso++;
    }
    printf("\n");
}

int main (){
  list<simb> tab_simb;
  list<def> tab_def;
  list<simb>::iterator iterador;

//  printf("Tabela de Definições: \n");
//  iterador = tab_def.begin();
//  while(iterador != tab_def.end()){
//    printf("elemento: %s, %d\n", iterador->rotulo, iterador->endereco);
//    iterador++;
//  }
//  printf("\n");

  preproc();
  primeirapassagem (&tab_simb, &tab_def);
   printf("Tabela de Simbolos: \n");
   iterador = tab_simb.begin();
   while(iterador != tab_simb.end()){
     printf("elemento: %s, %d\n", iterador->rotulo, iterador->endereco);
     iterador++;
   }
   printf("\n");
  segundapassagem (tab_simb);

  return 0;
}
