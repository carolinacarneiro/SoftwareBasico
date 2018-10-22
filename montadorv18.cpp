#include <stdio.h>
#include <ctype.h>
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
  int ehzero = 0;
  int ehlabel;
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

int tratahexa(char palavra[]) { // funcao que identifica o tipo de numero contido na string e converte pra int
   int i, hexa, novo_num;
   hexa = 1;

   if (palavra[0] != '0' || palavra[1] != 'x') { // verifica se esta na notacao 0x---
     hexa = 0;
   }
   else {
     for(i=2; palavra[i] != '\0'; i++) { // se o resto da palavra for de hexadecimais
       if (isxdigit(palavra[i]) == 0) {
         hexa = 0;
       }
     }
   }

   if (hexa == 1) { // devolve numero int de string de hexa
     novo_num = (int)strtol(palavra, NULL, 16);
   }
   else { // devolve numero int de string de inteiro
     novo_num = atoi(palavra);
   }
   return novo_num;
}

int errolexico(char palavra[]) { // funcao que identifica erros na escrita dos tokens
  int tem_erro = 0, i=1;

  tem_erro = 0; // primeiramente, a palavra nao tem erro nenhum
  if (isalpha(palavra[0]) == 0 && palavra[0] != '_') { // o primeiro char nao pode ser nada alem do alfabeto
    tem_erro = 1;
  }
  while (palavra[i] != '\0') { // loop que verifica se os argumentos do meio da palavra sao alfabeticas
    if(isalpha(palavra[i]) == 0 && palavra[i] != '_' && isdigit(palavra[i]) == 0) {
      printf("erro em: %c\t", palavra[i]);
      tem_erro = 1;
    }
    i++;
  }
  return tem_erro;
}

void preencheparametro(int *i, int *parametro, int *mudaadic, int *mudarot, int *mudaop, int *mudaop1, int *mudaop2, int tam_rot, char palavra[], char rotulo[], char operacao[], char operando1[], char operando2[], char letra, char adicionado[]) {
  if(*i>0 || letra == '+'){ //verifica se ja tem alguma palavra sendo formada//
    if ((letra == ' ' || letra == '\n' || letra == '\t' || letra == ':' || letra == '+')){ //verifica se eh termino de palavra//
      if (letra == ':'){ //eh rotulo?//
        *mudarot = *mudarot + 1;
        *i = *i-1;
        tam_rot = *i;
        printf("rotulo: ");
        strcpy(rotulo, palavra); // completa o rotulo
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

int descobreinstrucao (char instrucao[]) { // funcao que descobre a instrucao

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
  else if(strcmp(instrucao, "begin")==0) {
    return 6;
  }
  else if (strcmp(instrucao, "end")==0) {
    return 7;
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
} // funcao que preenche cada argumento (rotulo, instrucao etc) com o contido no vetor palavra

void preproc (auto strarquivo_entrada, auto strarquivo_saida0)  { // funcao que realiza o preprocessamento
  FILE *instrucoes, *instrucoes0;
  char letra, letrant, palavra[100], rotulo[50], operacao[50], operando1[50], operando2[50], operando3[10][50], adicionado[50];
  int i, j,letrint, parametro, tam_rot, mudarot=0, mudaop=0, mudaop1=0, mudaop2=0, ignoralinha=0, mudaadic=0, contadorlinha=1;
  list<diretiva> tab_preproc;
  diretiva elemento_preproc;
  list<diretiva>::iterator iterador_p;

  instrucoes = fopen(strarquivo_entrada.c_str(), "r");
  instrucoes0 = fopen(strarquivo_saida0.c_str(), "w+");

  parametro=1;
  i=0;
  while(!feof(instrucoes)){ // enquanto o arquivo nao terminar
    letrant = letra;
    letra = getc(instrucoes);
    if (letra == ';'){ //se for comentario, ignora tudo ate o \n//
      while (letra != '\n') {
        letra = getc(instrucoes);
      }
    }
    if(letra == '+' && (letrant == '\t'|| letrant == ' ')){ // se o char for + e a letra anterior for espaco, eh porque a palavra anterior ja foi preenchida
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
          if (mudarot > 1) { // se tiver mais de um rotulo, ja printa no arquivo pre processado
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
        if (strcmp(operacao, "equ")==0){ // se for equ, armazena nome do rotulo e valor
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

  printf ("\nfim pre processamento!");
  printf("\n -------------------------------------------- \n");
}

void primeirapassagem (list<simb> *tab_simb, list<def> *tab_def, int *eh_modulo, auto strarquivo_saida0){ // funcao que realiza a primeira passagem (monta a tabela de simbolos)
  FILE *instrucoes, *instrucoes1;
  char letra, palavra[50], rotulo[50], operacao[50], operando1[50], operando2[50], adicionado[50];
  int i, letrint, parametro, tam_rot, num_op, contadorpos=0, contadorlinha=1, mudaop=0,
      mudarot=0, mudaop1=0, mudaop2=0, num_dir=0, mudaadic=0, secao_atual = 0;
  simb elemento_simb;
  def elemento_def;
  list<simb>::iterator iterador;

  instrucoes1 = fopen(strarquivo_saida0.c_str(), "r");
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
          printf("Erro Semantico na linha %d. Simbolo %s Redefinido\n", contadorlinha, iterador->rotulo);
        }
        else {
          if (descobrediretiva(operacao) == 5) { //detectar se a diretiva eh extern //
            elemento_simb.endereco = 0;
            elemento_simb.externa = 1;
            elemento_simb.secao_atual = 1;
          }
          else {
            elemento_simb.endereco = contadorpos;
            elemento_simb.externa = 0;
            elemento_simb.secao_atual = secao_atual;
            if (descobrediretiva(operacao) == 2) {
              if (atoi(operando1) == 0) { // detecta se o operando1 vale zero para detectar futuramente fivisao por zero
                elemento_simb.ehzero = 1;
              }
              else {
                elemento_simb.ehzero = 2;
              }
            }
            else {
              elemento_simb.ehzero = 0;
            }
            if (strcmp(operacao, "space") !=0 && strcmp(operacao, "const") != 0) {
              elemento_simb.ehlabel = 1;
            }
            else {
              elemento_simb.ehlabel = 0;
            }
            strcpy(elemento_def.rotulo, rotulo);
            elemento_def.endereco = contadorpos;
            tab_def->push_back(elemento_def);
          }
          tab_simb->push_back(elemento_simb);
        }
      }
      if (mudaop == 1) {
        num_op = descobreinstrucao(operacao);
        num_dir = descobrediretiva(operacao);
        if (num_op == 0 && (num_dir >= 0 && num_dir <= 2) && secao_atual == 1) {
          printf("Erro Semantico na linha %d. Diretiva ou Instrucao na secao errada\n", contadorlinha);
        }
        if (secao_atual == 2 && num_dir != 2 && num_dir != 3 && num_dir != 6 && num_dir != 7) {
          printf("Erro Semantico na linha %d. Diretiva ou Instrucao na secao errada\n", contadorlinha);
        }
        if (secao_atual == 3 && num_dir != 1 && num_dir != 3 && num_dir != 6 && num_dir != 7) {
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
          if (num_dir == 2) { // diretiva const
            contadorpos++;
          }
          if (num_dir == 3) { // diretiva section
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
          if (num_dir == 6) {
            *eh_modulo = *eh_modulo + 1;
          }
          if (num_dir == 7) {
            *eh_modulo = *eh_modulo + 1;
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
  printf ("\nfim primeira passagem!");
  printf("\n -------------------------------------------- \n");
}

void segundapassagem (list<simb> tab_simb, list<def> tab_def, int eh_modulo, auto strarquivo_saida0, auto strarquivo_saida1){
  FILE *instrucoes1, *instrucoes2, *instrucoes3;
  char letra, palavra[50], rotulo[50], operacao[50], operando1[50], operando2[50], adicionado[50], c_novo;
  int i, tam_rot, parametro, num_op, contadorpos=0, contadorlinha=1, mudaop=0,
      mudarot=0, mudaop1=0, mudaop2=0, num_dir=0, mudaadic=0, end_uso = 0, tem_erro = 0, k, l,
      relativo[100], end_relativo, novo_num;
  simb elemento_simb;
  list<simb>::iterator iterador, iterador2, iterador_simb;
  list<def>::iterator iterador_def;
  list<uso> tab_uso;
  list<uso>::iterator iterador_uso;
  uso elemento_uso;

  iterador_simb = tab_simb.begin(); //adiciona elementos externos a tabela de uso
  while(iterador_simb != tab_simb.end()) {
    if (iterador_simb->externa == 1) {
      strcpy(elemento_uso.rotulo, iterador_simb->rotulo);
      elemento_uso.indice_end = 0;
      tab_uso.push_back(elemento_uso);
    }
    iterador_simb++;
  }

  instrucoes2 = fopen ("auxiliar.txt", "w+"); // arquivo auxiliar para escrever o arquivo objeto na ordem correta
  instrucoes1 = fopen(strarquivo_saida0.c_str(), "r");

  if (instrucoes1 == NULL){
    printf("Este arquivo nao foi encontrado!\n");
  }
  if (eh_modulo == 2) { // se o arquivo contem begin e end
    fputs("CODE\n", instrucoes2);
  }

  end_relativo = 0;
  parametro=1;
  i=0;
  while(!feof(instrucoes1)){
    letra = getc(instrucoes1);

    if (letra != ' ' && letra != '\n' && letra != '\t'  && letra != '+' && letra != ':'){ //verifica se o char faz parte de alguma palavra//
      palavra[i]=letra;
      palavra[i+1] = '\0';
      i++;
    }

    preencheparametro(&i, &parametro, &mudaadic, &mudarot, &mudaop, &mudaop1, &mudaop2, tam_rot, palavra, rotulo, operacao, operando1, operando2, letra, adicionado);

    if (letra == '\n'){ //se a linha terminou//
      preencheuso (&tab_uso, tab_simb, operando1, operando2, mudaop1, mudaop2, contadorpos);
      num_op = descobreinstrucao(operacao);
      num_dir = descobrediretiva(operacao);

      if (mudarot == 1) {
        tem_erro = errolexico(rotulo);
        if (tem_erro == 1) {
          printf("Erro Lexico no rotulo da linha %d. Token invalido!\n", contadorlinha);
        }
      }
      if (mudaop == 1) {
        tem_erro = errolexico(operacao);
        if (tem_erro == 1) {
          printf("Erro Lexico na operacao da linha %d. Token invalido!\n", contadorlinha);
        }
      }
      if (mudaop1 == 1 && num_dir != 1 && num_dir != 2) {
        tem_erro = errolexico(operando1);
        if (tem_erro == 1) {
          printf("Erro Lexico no operando 1 da linha %d. Token invalido!\n", contadorlinha);
        }
      }
      if (mudaop2 == 1) {
        tem_erro = errolexico(operando2);
        if (tem_erro == 1) {
          printf("Erro Lexico no operando 2 da linha %d. Token invalido!\n", contadorlinha);
        }
      }

      if ((num_op>=1 && num_op<=8)||(num_op>=10 && num_op<=13)){
        relativo[end_relativo] = contadorpos + 1; // armazena os enderecos relativos
        end_relativo ++;

        if (isdigit(operando1[0]) != 0) { // se o primeiro char for um numero
          printf("Erro Sintatico do operando 1 na linha %d. Tipo de argumento invalido\n", contadorlinha);
        }

        if (mudaop2 == 1) {
          printf("Erro Sintatico na linha %d. Quantidade de operandos invalida!\n", contadorlinha);
        }
        iterador = tab_simb.begin();
        while(strcmp(iterador->rotulo, operando1)!=0 && iterador != tab_simb.end()){ //tenta achar rotulo na lista de simbolos//
          iterador++;
        }
        if (iterador == tab_simb.end()){
          printf("Erro Semantico na linha %d! Operando %s Indefinido!\t", contadorlinha, operando1);
        }
        else {
          if (num_op == 12 || num_op == 11) {
            if (iterador->ehzero != 0) {
              printf("Erro Semantico na linha %d. Modificacao de um valor constante\n", contadorlinha);
            }
          }
          if (num_op == 4 && iterador->ehzero == 1) {
            printf("Erro Semantico na linha %d. Divisao por zero\n", contadorlinha);
          }
          if(num_op >= 5 && num_op <= 8) { //algum tipo de jmp
            if (iterador->ehlabel != 1) {
              printf("Erro Semantico na linha %d. Pulo para rotulo invalido\n", contadorlinha);
            }
            if(iterador->secao_atual != 1) {
              printf("Erro Semantico na linha %d. Pulo para secao errada!\n", contadorlinha);
            }
          }
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
        relativo[end_relativo] = contadorpos + 1; // armazena valores relativos
        end_relativo ++;
        relativo[end_relativo] = contadorpos + 2;
        end_relativo ++;

        if (isdigit(operando1[0]) != 0) { // se o primeiro char for um numero
          printf("Erro Sintatico do operando 1 na linha %d. Tipo de argumento invalido\n", contadorlinha);
        }
        if (isdigit(operando2[0]) != 0) { // se o primeiro char for um numero
          printf("Erro Sintatico do operando 2 na linha %d. Tipo de argumento invalido\n", contadorlinha);
        }

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
          if (iterador2->ehzero != 0) {
            printf("Erro Semantico na linha %d. Modificacao de um valor constante\n", contadorlinha);
          }
          fprintf(instrucoes2, "%d", num_op);
          fputs(" ", instrucoes2);
          fprintf(instrucoes2, "%d", iterador->endereco);
          fputs(" ", instrucoes2);
          fprintf(instrucoes2, "%d", iterador2->endereco);
          fputs(" ", instrucoes2);
        }
        contadorpos = contadorpos+3;
      }
      else if(num_op == 14) { // instrucao stop
        if (mudaop1 == 1) {
          printf("Erro Sintatico na linha %d. Quantidade de operandos invalida!\n", contadorlinha);
        }
        fprintf(instrucoes2, "%d", num_op);
        fputs(" ", instrucoes2);
        contadorpos++;
      }
      else if (num_op == 0) { // nao achou instrucao
        if (mudaop1 == 1) {
          if (num_dir == 1 || num_dir == 2) {
            if (isdigit(operando1[0]) == 0) {
              printf("Erro Sintatico do operando 1 na linha %d. Tipo de argumento invalido\n", contadorlinha);
            }
          }
          else {
            if (isdigit(operando1[0]) != 0){
            printf("Erro Sintatico do operando 1 na linha %d. Tipo de argumento invalido\n", contadorlinha);
            }
          }
        }
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
          novo_num = tratahexa (operando1);
          fprintf(instrucoes2, "%d ", novo_num);
          fputs(" ", instrucoes2);
        }
        if (num_dir == 0) {
          printf("Erro Sintatico na linha %d,  Diretiva ou Instucao invalida!\t", contadorlinha);
        }
      }
      contadorlinha++;
      printf("cpos: %d", contadorpos);
      parametro = 1;
      printf(" \n");
      mudaop = 0; mudarot = 0; mudaop1 = 0; mudaop2 = 0; mudaadic = 0;
    }
  }
  fclose(instrucoes2);

  if(eh_modulo == 2) { // se o arquivo for um modulo, insere no file as tabelas de uso, definicao e enderecos relativos
    instrucoes2 = fopen ("auxiliar.txt", "r");
    instrucoes3 = fopen (strarquivo_saida1.c_str(), "w+");
    fputs("TABLE USE\n", instrucoes3);
    iterador_uso = tab_uso.begin();
    while(iterador_uso != tab_uso.end()){
      for (k=0; k < iterador_uso->indice_end; k++) {
        fprintf(instrucoes3, "%s ", iterador_uso->rotulo);
        fprintf(instrucoes3, "%d\n", iterador_uso->endereco[k]);
      }
      iterador_uso++;
    }
    fputs("TABLE DEFINITION\n", instrucoes3);
    iterador_def = tab_def.begin();
    while(iterador_def != tab_def.end()){
      fprintf(instrucoes3, "%s ", iterador_def->rotulo);
      fprintf(instrucoes3, "%d\n", iterador_def->endereco);
      iterador_def++;
    }
    fputs("RELATIVE\n", instrucoes3);
    for (l=0; l < end_relativo; l++) {
      fprintf(instrucoes3, "%d ", relativo[l]);
    }
    fputs("\n", instrucoes3);
    c_novo = fgetc(instrucoes2);
    while (c_novo != EOF) {
      fputc(c_novo, instrucoes3);
      c_novo = fgetc(instrucoes2);
    }
    fclose(instrucoes2);
    fclose(instrucoes3);
    remove("auxiliar.txt");
  }
  else {
    instrucoes2 = fopen ("auxiliar.txt", "r");
    instrucoes3 = fopen (strarquivo_saida1.c_str(), "w+");
    c_novo = fgetc(instrucoes2);
    while (c_novo != EOF) {
      fputc(c_novo, instrucoes3);
      c_novo = fgetc(instrucoes2);
    }
    fclose(instrucoes2);
    fclose(instrucoes3);
    remove("auxiliar.txt");
  }
  fclose(instrucoes1);
  printf ("\nfim segunda passagem!");
  printf("\n -------------------------------------------- \n");
}

int main (int argc, char *argv[]){
  auto arquivo_entrada = argv[1];
  auto arquivo_saida0 = argv[1];
  auto arquivo_saida1 = argv[1];
  auto strarquivo_entrada = std::string(arquivo_entrada);
  auto strarquivo_saida0 = std::string(arquivo_saida0);
  auto strarquivo_saida1 = std::string(arquivo_saida1);
  int eh_modulo;
  list<simb> tab_simb;
  list<def> tab_def;
  list<simb>::iterator iterador;

  if(argc != 2) {
    printf("Erro no numero de argumentos!\n");
    return 0;
  }

  eh_modulo = 0; // variavel que eh incrementada se acordo com o numero de begins e ends
  strarquivo_entrada.append(".asm");
  strarquivo_saida0.append(".pre");
  strarquivo_saida1.append(".obj");

  preproc(strarquivo_entrada, strarquivo_saida0);
  primeirapassagem (&tab_simb, &tab_def, &eh_modulo, strarquivo_saida0);
  segundapassagem (tab_simb, tab_def, eh_modulo, strarquivo_saida0, strarquivo_saida1);

  return 0;
}
