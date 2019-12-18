#pragma once
#include "Fronteira.h"
#include <fstream>
class Aresta
{
private:
	Fronteira *fronteira;//fronteira de destino da aresta
	double peso;
public:
	Aresta();
	~Aresta();
	//Atribui peso a aresta
	void Set_Peso(double i);
	void Set_Destino(Fronteira *f);
	bool Constroi(int &inicio, int &destino, string dados);
	int Get_Destino();
	double Get_Peso();
	Fronteira *Get_Fronteira();
	void EscreverXML(ofstream &f,Fronteira *front);
	bool LerXML(int &inicio, int &destino,ifstream &f);
};
