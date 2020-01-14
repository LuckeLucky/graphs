#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Pessoa.h"
using namespace std;
class Fronteira
{
protected:
	int coord_x, coord_y, n_fronteira;
public:
	Fronteira();
	~Fronteira();
	virtual int Get_Tipo() { return 0; };
	int Get_Numero();
	Fronteira * Constroi(string dados);
	Fronteira *Escolhe_Tipo(int tipo);
	void EscreverXML(ofstream &f);
	Fronteira *LerXML(ifstream &f);
};
//Fronteira do tipo 1
class Fronteira1 :public Fronteira
{
public:
	int Get_Tipo() { return 1; }
};
//Fronteira do tipo 2
class Fronteira2 :public Fronteira {
public:
	int Get_Tipo() { return 2; }
};
//Fronteira do tipo 3
class Fronteira3 :public Fronteira {
public:
	int Get_Tipo() { return 3; }
};
