/*
Trabalho 1 de Software Basico (Ligador)
Alunos:
- Filipe Maia Soares  15/0051409
- Carolina Araujo Carneiro  12/0113422

Instrucoes de Compilacao:
Usando g++ 7.2.0 (Ubuntu 17.10)
g++ -std=c++11 ligador.cpp -o ligador

Instrucoes de Execucao:
./ligador arquivo1.obj arquivo2.obj arquivo3.obj arquivo4.obj

Arquivo de saída = arquivo1.e
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

//Declaração de variáveis globais

ifstream arquivo_obj1, arquivo_obj2, arquivo_obj3, arquivo_obj4;


bool ligador(int num_arquivo_obj, vector<ifstream*> elemento_vetor, ofstream &saida)
{

    //Declaração das variáveis a serem usadas na função

    vector <int> fator_de_correcao;

    vector <vector<string> > tabela_uso_global, tabela_def_glob, codigo_gerado;

    vector <vector<string> >::iterator vetor_iterador_2;

    vector <string>::iterator vetor_iterador_1;

    string linhas, palavras;

    string::iterator it;



    for (int i = 0; i < num_arquivo_obj; ++i){			//Loop for de acordo com o número de arquivos objetos passados

        //Vetores usados no for

        vector<string> tabela_uso, tabela_global_definicoes, codigo, auxiliar;

        short int contador = 0;

        while(getline(*elemento_vetor[i],linhas)){		

            if(linhas.find("TABLE USE") != string::npos){		//Encontra tabela de uso

                while(getline(*elemento_vetor[i],linhas) && linhas.find("TABLE DEFINITION") == string::npos){ //Permanece no loop enquanto ainda for tabela de uso

                    istringstream is(linhas);

                    while(getline(is, palavras,' ')){	//Pega e coloca no vetor as palavras dentro da linha

                        tabela_uso.push_back(palavras);	//Função para passar pro vetor
                    }

                }
            }

            if(linhas.find("TABLE DEFINITION") != string::npos){	//Mesma lógica da função anterior

                while(getline(*elemento_vetor[i],linhas) && linhas.find("RELATIVE") == string::npos){

                    istringstream is(linhas);

                    while(getline(is, palavras,' ')){

                        tabela_global_definicoes.push_back(palavras);

                    }

                }
            }



            if(linhas.find("CODE") != string::npos){		//Similiar as funções anteriores

                while(getline(*elemento_vetor[i],linhas, ' '))	
                {
                    codigo.push_back(linhas);		
                }
            }
        }

        contador = codigo.size(); //A partir do vetor codigo, pega-se o seu tamanho para calcular o fator de correção

        if(i == 0){

            fator_de_correcao.push_back(0);

            fator_de_correcao.push_back(contador);
        }

        else if(i == 1){		

            fator_de_correcao.push_back(contador+(fator_de_correcao[1]));
        }


        else if(i == 2){   

            fator_de_correcao.push_back(contador+(fator_de_correcao[2]));
        }

        if(i != 0) { //Condição para aplicar o fator de correção

            for (unsigned int j = 1;
                 j < tabela_global_definicoes.size(); j += 2) { 

                tabela_global_definicoes[j] = to_string(stoi(tabela_global_definicoes[j]) + fator_de_correcao[i]);

            }



        }


        tabela_uso_global.push_back(tabela_uso);		

        tabela_def_glob.push_back(tabela_global_definicoes);

        codigo_gerado.push_back(codigo);
    }

    for(unsigned int i = 0; i < tabela_uso_global.size(); i++){		//For para resolver as pendências cruzadas

        for(unsigned int j = 0; j < tabela_uso_global[i].size(); j+=2){

            bool achou_aux = false;

            for(unsigned int g = 0; g < tabela_def_glob.size(); g++){

                for(unsigned int k = 0; k < tabela_def_glob[g].size(); k+=2){

                    if(tabela_uso_global[i][j] == tabela_def_glob[g][k]){

                        int temp = stoi(codigo_gerado[i][stoi(tabela_uso_global[i][j+1])]) + stoi(tabela_def_glob[g][k+1]);

                        codigo_gerado[i][stoi(tabela_uso_global[i][j+1])] = to_string(temp);

                        achou_aux = true;

                    }
                }
            }

            if(!achou_aux){

                cerr << "Erro de ligação" << endl;
                return false;
            }
        }
    }

    for(vector <string> vec : codigo_gerado){	//Função para gravar no arquivo de saída o código de saída

        for(string st : vec)
        {
            saida << st << ' ';
        }
    }

    return saida.good();
}


int main(int argc, char const *argv[]) {

    vector<ifstream *> arquivos;

    if (argc < 2 || argc > 5) {


        //argc < 2, esta faltando arquivo de entrada.
        //Para argc maior que 5, há mais que 4 arquivos objeto

        cerr << "Erro! Escolha no máximo 4 arquivos objetos\n" << endl;
        return -1;
    }

    //Funções para abrir os arquivos de acordo com o número de arquivos objeto passado por terminal

    if (argc == 2) {

        arquivo_obj1.open(argv[1]);
        arquivos.push_back(&arquivo_obj1);
    }

    if (argc == 3) {

        arquivo_obj1.open(argv[1]);
        arquivos.push_back(&arquivo_obj1);
        arquivo_obj2.open(argv[2]);
        arquivos.push_back(&arquivo_obj2);
    }
    if (argc == 4) {

        arquivo_obj1.open(argv[1]);
        arquivos.push_back(&arquivo_obj1);
        arquivo_obj2.open(argv[2]);
        arquivos.push_back(&arquivo_obj2);
        arquivo_obj3.open(argv[3]);
        arquivos.push_back(&arquivo_obj3);
    }

    if (argc == 5) {

        arquivo_obj1.open(argv[1]);
        arquivos.push_back(&arquivo_obj1);
        arquivo_obj2.open(argv[2]);
        arquivos.push_back(&arquivo_obj2);
        arquivo_obj3.open(argv[3]);
        arquivos.push_back(&arquivo_obj3);
        arquivo_obj4.open(argv[4]);
        arquivos.push_back(&arquivo_obj4);
    }

    //Funções para formar o nome do arquivo de saída usando o nome do primeiro arquivo .obj

    string filename(argv[1]);

    int pos = filename.find(".");

    filename = filename.substr(0, pos + 1);

    filename.append("e");

    ofstream arquivo_saida_executavel(filename, ios::out | ios::trunc); //Função para abrir o arquivo de saida com o mesmo nome do primeiro arquivo .obj

    ligador((argc - 1), arquivos, arquivo_saida_executavel);  //Executa a função para ligar os arquivos e gerar o arquivo executavel

    return 0;
}
