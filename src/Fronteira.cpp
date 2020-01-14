#include "Fronteira.h"

Fronteira::Fronteira() {
}

Fronteira::~Fronteira() {
}

int Fronteira::Get_Numero() {
	return n_fronteira;
}
Fronteira *Fronteira::Constroi(string dados) {
	stringstream ss(dados);
	string sub, elementos[4];
	int i = 0;
	while (getline(ss, sub, ';'))
	{
		elementos[i++] = sub;
	}
	if (i == 4) {
		Fronteira *f;
		f = Escolhe_Tipo(atoi(elementos[3].c_str()));
		f->n_fronteira = atoi(elementos[0].c_str());
		f->coord_x = atoi(elementos[1].c_str());
		f->coord_y = atoi(elementos[2].c_str());
		return f;
	}
	return NULL;;
}
Fronteira *Fronteira::Escolhe_Tipo(int tipo) {
	Fronteira *f;
	switch (tipo) {
	case 1:f = new Fronteira1; break;
	case 2:f = new Fronteira2; break;
	case 3:f = new Fronteira3; break;
	default:f = new Fronteira; break;
	}
	return f;
}
void Fronteira::EscreverXML(ofstream &f) {
	if (!f)
		return;
	f << "\t" << "<Fronteira>" << endl;
	f << "\t\t" << "<Numero>" << n_fronteira << "</Numero>" << endl;
	f << "\t\t" << "<X>" << coord_x << "</X>" << endl;
	f << "\t\t" << "<Y>" << coord_y << "</Y>" << endl;
	f << "\t\t" << "<Tipo>" << Get_Tipo() << "</Tipo>" << endl;
	f << "\t" << "</Fronteira>" << endl;
}
Fronteira *::Fronteira::LerXML(ifstream &f) {
	if (!f)
		return NULL;
	int i = 0;
	int fecha, abre;
	string linha, elementos[4];
	while (i<4) {
		getline(f, linha);
		fecha=(int)linha.find_first_of(">");//Encontra o primeiro > na linha
		linha.erase(0, fecha+1);
		abre = (int)linha.find_last_of("<");//Encontra o ultimo < na linha como a primeira parte <..> ja apagada
		linha.erase(abre,linha.size());
		elementos[i] = linha;
		i++;
	}
	if (i == 4) {
		Fronteira *f;
		f = Escolhe_Tipo(atoi(elementos[3].c_str()));
		f->n_fronteira = atoi(elementos[0].c_str());
		f->coord_x = atoi(elementos[1].c_str());
		f->coord_y = atoi(elementos[2].c_str());
		return f;
	}
	return NULL;;
}
