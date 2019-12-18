#include "Pessoa.h"

Pessoa::Pessoa() {
	peso = 0;//Todas as pessoas começam com peso de carga igual a 0
}
Pessoa::~Pessoa() {
}
	
bool Pessoa::Constroi(string dados) {
	stringstream ss(dados);
	string sub, elementos[4];
	int i = 0;
	while (getline(ss, sub, ';'))
	{
		elementos[i++] = sub;
	}
	if (i == 4) {
		n_pessoa = atoi(elementos[0].c_str());
		nome = elementos[1];
		f_inicio = atoi(elementos[2].c_str());
		f_destino = atoi(elementos[3].c_str());
		return true;
	}
	else
		return false;
}
int Pessoa::Get_Inicio() {
	return f_inicio;
}
string Pessoa::Get_Nome() {
	return nome;
}
bool Pessoa::Incrementa_Peso() {
	if (peso == 10)
		return false;
	peso++;
	return true;
}
int Pessoa::Get_Destino() {
	return f_destino;
}