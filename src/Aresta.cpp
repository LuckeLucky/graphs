#include "Aresta.h"
Aresta::Aresta() {
}

Aresta::~Aresta() {
}

void Aresta::Set_Peso(double i) {
	peso = i;

}
void Aresta::Set_Destino(Fronteira *f) {
	fronteira = f;
}
int Aresta::Get_Destino() {
	return fronteira->Get_Numero();
}
double Aresta::Get_Peso() {
	return peso;
}
Fronteira *Aresta::Get_Fronteira() {
	return fronteira;
}
void Aresta::EscreverXML(ofstream &f,Fronteira *front) {

		f << "\t" << "<Aresta>" << endl;
		f << "\t\t" << "<Inicio>" << front->Get_Numero() << "</Inicio>" << endl;
		f << "\t\t" << "<Destino>" << Get_Destino() << "</Destino>" << endl;
		f << "\t\t" << "<Peso>" << peso<< "</Peso>" << endl;
		f << "\t" << "</Aresta>" << endl;
}
bool Aresta::Constroi(int &inicio, int &destino, string dados) {
	stringstream ss(dados);
	string sub, elementos[3];
	int i = 0;
	while (getline(ss, sub, ';'))
	{
		elementos[i++] = sub;
	}
	if (i == 3) {
		inicio = atoi(elementos[0].c_str());
		destino = atoi(elementos[1].c_str());
		peso=atoi(elementos[2].c_str());
		return true;
	}
	else
		return false;
}
bool Aresta::LerXML(int &inicio,int &destino,ifstream &f) {
	if (!f)
		return false;
	int i = 0;
	int fecha, abre;
	string linha, elementos[3];
	while (i < 3) {
		getline(f, linha);
		fecha = (int)linha.find_first_of(">");//Encontra o primeiro > na linha
		linha.erase(0, fecha + 1);
		abre = (int)linha.find_last_of("<");//Encontra o ultimo < na linha como a primeira parte <..> ja apagada
		linha.erase(abre, linha.size());
		elementos[i] = linha;
		i++;
	}
	if (i == 3) {
		inicio = atoi(elementos[0].c_str());
		destino = atoi(elementos[1].c_str());
		peso = atoi(elementos[2].c_str());
		return true;
	}
	return false;
}
