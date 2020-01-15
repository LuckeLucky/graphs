#pragma once
#include <string>
#include <sstream>

using namespace std;
class Pessoa
{
private:
	string nome;
	int f_inicio, f_destino, peso, n_pessoa;
public:
	Pessoa();
	~Pessoa();
	bool Constroi(string dados);
	int Get_Inicio();
	string Get_Nome();
	bool Incrementa_Peso();
	int Get_Destino();
};
