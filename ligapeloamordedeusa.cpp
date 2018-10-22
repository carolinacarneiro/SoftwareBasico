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

//Declaração de variáveis

ifstream arquivo_obj1, arquivo_obj2, arquivo_obj3, arquivo_obj4;


bool ligador(int num_arquivo_obj, vector<ifstream*> elemento_vetor, ofstream &saida)
{
	
	vector <int> fator_de_correcao;
	vector <vector <int> > realocacao_vetor;
	vector <vector<string> > tabela_uso_global, table_gd_g, codigo_gerado;
	vector <vector<string> >::iterator vec_vec_it;
	vector <string>::iterator vec_it;
	string linhas, palavras;
	string::iterator it;
	
	

	for (int i = 0; i < num_arquivo_obj; ++i)			//Loop de acordo com o número de arquivos objetos passados
	{
		vector<string> table_use, table_gd, code;
		vector<int> realoca;
		
		short int contador = 0;
		
		while(getline(*elemento_vetor[i],linhas))		//Percorre linha a linha o arquivo objeto
		{	
			if(linhas.find("TABLE USE") != string::npos)		//Achou a tabela de uso
			{
				while(getline(*elemento_vetor[i],linhas) && linhas.find("TABLE DEFINITION") == string::npos) //Enquanto ainda for a tabela de uso
				{
					istringstream is(linhas);
					while(getline(is, palavras,' '))	//Pega as palavras dentro da linha
					{
						table_use.push_back(palavras);	//Coloca no vector
					}

				}
			}
			if(linhas.find("TABLE DEFINITION") != string::npos)	//Funcionamento similar ao if anterior
			{
				while(getline(*elemento_vetor[i],linhas) && linhas.find("TABLE REALOCATION") == string::npos)
				{
					istringstream is(linhas);
					while(getline(is, palavras,' '))
					{
						table_gd.push_back(palavras);
						//cout << palavras << endl;
					}

				}
			}
			if(linhas.find("TABLE REALOCATION") != string::npos)	//Acha a tabela com os bits de realocação
			{
				getline(*elemento_vetor[i],linhas);
				for(it = linhas.begin(); it < linhas.end(); ++it)
				{
					realoca.push_back(int(*it) - 48);		//Converte pra int e joga no vector
					//cout << (int(*it)-48) << endl;
					++contador;									//Conta o tamanho do código para calcular o fator de correção
				}	
			}
			if(linhas.find("CODE") != string::npos)		//Acha a seção CODE
			{
				while(getline(*elemento_vetor[i],linhas, ' '))	//Separa a seção CODE por espaços
				{
					code.push_back(linhas);		//Envia pro vector
				}
			}
		}

		if(i == 0)			//Calcula o fator de correção no segundo arquivo
		{
			fator_de_correcao.push_back(0);
			fator_de_correcao.push_back(contador);
		}
		else if(i == 1)		//Calcula o fator de correção no terceiro arquivo
		{
			fator_de_correcao.push_back(contador+(fator_de_correcao[1]));
		}

		if(i != 0)			//Aplica o fator de correção
		{
			for(unsigned int j = 1; j < table_gd.size(); j+=2) // Aplica o fator de correção na tabela de definições
			{
				table_gd[j] = to_string(stoi(table_gd[j]) + fator_de_correcao[i]);
				//cout << table_gd[j] << endl;
			}
			for(unsigned int cont = 0; cont < code.size(); cont++) //Aplica na seção CODE dependendo da tabela de realocação
			{
				if(realoca[cont])
				{
					code[cont] = to_string(fator_de_correcao[i] + stoi(code[cont]));
					//cout << code[cont] << endl;
				}
			}
		}
		tabela_uso_global.push_back(table_use);		//Salva todas as informações do arquivo[i]
		realocacao_vetor.push_back(realoca);
		table_gd_g.push_back(table_gd);
		codigo_gerado.push_back(code);
	}

	for(unsigned int i = 0; i < tabela_uso_global.size(); i++)		//Resolve pendências cruzadas
	{
		for(unsigned int j = 0; j < tabela_uso_global[i].size(); j+=2)
		{
			//cout << tabela_uso_global[i][j] << ' ';
			bool flag_achou = false;
			for(unsigned int g = 0; g < table_gd_g.size(); g++)
			{
				for(unsigned int k = 0; k < table_gd_g[g].size(); k+=2)
				{
					if(tabela_uso_global[i][j] == table_gd_g[g][k])
					{
						//cout << table_gd_g[g][k] << "  achou" << endl;
						int temp = stoi(codigo_gerado[i][stoi(tabela_uso_global[i][j+1])]) + stoi(table_gd_g[g][k+1]);
						codigo_gerado[i][stoi(tabela_uso_global[i][j+1])] = to_string(temp);
						flag_achou = true;
						//cout << codigo_gerado[i][stoi(tabela_uso_global[i][j+1])] << " conteudo " << endl;
					}
				}
			}
			if(!flag_achou)
			{
				cerr << "Erro de ligação: Símbolo " << tabela_uso_global[i][j] << " não definido" << endl;
				return false;
			}
		}
	}
	for(vector <string> vec : codigo_gerado)	//Passa a saída pro arquivo de saída
	{
		for(string st : vec)
		{
			saida << st << ' ';
		}
	}

	return saida.good();
}


int main(int argc, char const *argv[]){

	vector<ifstream*> arquivos;

	if(argc < 3 || argc > 6){ 


		//argc < 3, esta faltando ou arquivo de saida ou arquivo de entrada. 
		//Para argc maior que 6, há mais que 4 arquivos objeto

		cerr << "Erro! Escolha no máximo 4 arquivos objetos e um arquivo de saída.\n" << endl;
		return -1;
	}		
	
	if(argc == 3){					//Abre 1 arquivo obj
	
		arquivo_obj1.open(argv[1]);
		arquivos.push_back(&arquivo_obj1);
	}
	
	if(argc == 4){					//Abre 2 arquivos obj
	
		arquivo_obj1.open(argv[1]);
		arquivos.push_back(&arquivo_obj1);
		arquivo_obj2.open(argv[2]);
		arquivos.push_back(&arquivo_obj2);
	}
	if(argc == 5){ 					// Abre 3 arquivos obj
	
		arquivo_obj1.open(argv[1]);
		arquivos.push_back(&arquivo_obj1);
		arquivo_obj2.open(argv[2]);
		arquivos.push_back(&arquivo_obj2);
		arquivo_obj3.open(argv[3]);
		arquivos.push_back(&arquivo_obj3);
	}

	if(argc == 6){ 					// Abre 4 arquivos obj
	
		arquivo_obj1.open(argv[1]);
		arquivos.push_back(&arquivo_obj1);
		arquivo_obj2.open(argv[2]);
		arquivos.push_back(&arquivo_obj2);
		arquivo_obj3.open(argv[3]);
		arquivos.push_back(&arquivo_obj3);
		arquivo_obj4.open(argv[4]);
		arquivos.push_back(&arquivo_obj4);
	}


	string filename(argv[argc-1]);	//Checa se o último parâmetro passado é arquivo de saída (.e)
	
	if(filename.substr(filename.find(".")+1) != "e"){
	
		cerr << "Nome do arquivo de saída inválido!\nEscolha um arquivo de saída no formato: saída.e" << endl;
		return -1;
	}
	
	ofstream arquivo_saida_executavel(argv[argc-1], ios::out|ios::trunc); //Abre o arquivo de saída


	ligador((argc - 2), arquivos, arquivo_saida_executavel);  //Executa a função para ligar os arquivos e gerar o arquivo executavel

	return 0;
}