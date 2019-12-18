#include <iostream>
using namespace std;

#include "Grafo.h"

int main()
{
		list<int> *x;
		double custo = 0;
		Grafo *GF;
		GF = new Grafo();
		if (!GF->Load("grafo_3.txt", "pessoas.txt")) {
			cout << "ERRO A LER FICHEIROS" << endl;
		}
		else
			cout << "FICHEIROS LIDOS COM SUCESSO" << endl;
			/*cout << "**ContarNos" << endl;
			cout << GF->ContarNos() << endl;
			cout << "**ContarArcos" << endl;
			cout << GF->ContarArcos() << endl;
			cout << "**Memoria" << endl;
			cout << GF->Memoria() <<"bytes"<< endl;
			cout << "**Adjacencia" << endl;
			cout << GF->Adjacencia(13,12) << endl;
			cout << "**NoMaisArcos" << endl;
			x=GF->NoMaisArcos();
			if (x) {
				for (list<int>::iterator lit = (*x).begin(); lit != (*x).end(); ++lit)
					cout << *lit << ",";
				cout << endl;
				delete(x);
			}
			cout << "**VericesIsolados" << endl;
			x = GF->VerticesIsolados();
			if (x) {
				for (list<int>::iterator lit = (*x).begin(); lit != (*x).end(); ++lit)
					cout << *lit << ",";
				cout << endl;
				delete(x);
			}
			else
				cout << "Nao ha vertices Isolados" << endl;
			cout << "**Search" << endl;
			cout << GF->Search(3) << endl;
			cout << "**Caminho" << endl;*/
			x=GF->Caminho(1,23,custo);
			if (x ) {
				for (list<int>::iterator lit = (*x).begin(); lit != (*x).end(); ++lit)
					cout << *lit << "->";
				cout << endl;
				delete(x);
				cout << "custo:" << custo << endl;
			}
			else
				cout << "Nao ha caminho" << endl;

			/*cout << "**DevolveVerticesTipo" << endl;
			x = GF->DevolveVerticesTipo("3");
			if (x) {
				for (list<int>::iterator lit = (*x).begin(); lit != (*x).end(); ++lit)
					cout << *lit << ",";
				cout << endl;
				delete(x);
			}
			else
				cout << "Nao ha vertices desse tipo" << endl;
			cout<< "**SitiosInacessiveisAPartirDe" << endl;
			x = new list<int>;
			GF->SitiosInacessiveisAPartirDe(25, *x);
			if (x) {
				for (list<int>::iterator lit = (*x).begin(); lit != (*x).end(); ++lit)
					cout << *lit << ",";
				cout << endl;
				delete(x);
			}
			else
				cout << "Nao ha vertices inacessiveis" << endl;
			cout << "**RemoverVertice" << endl;
			cout << GF->RemoverVertice(26) << endl;
			cout << "**RemoverAresta" << endl;
			cout << GF->RemoverAresta(1,2) << endl;
			/*GF->EscreverXML("xml.txt");
			GF->LerXML("xml.txt");
			cout << GF->ContarNos() << endl;
			cout << GF->ContarArcos() << endl;
			cout<<"**PossivelCaminho"<<endl;
			cout<<GF->PossivelCaminho(1,5,1)<<endl;*/
			cout<<"**CaminhoMinimo"<<endl;
			x=GF->CaminhoMinimo(3,8,custo);
			if (x) {
				for (list<int>::iterator lit = (*x).begin(); lit != (*x).end(); ++lit)
					cout << *lit << ",";
				cout << "\ncusto:" << custo << endl;
				cout << endl;
				delete(x);
			}
			else
				cout << "Nao ha caminho minimo" << endl;
			cout<<"**CaminhoMaximo"<<endl;
			x=GF->CaminhoMaximo(3,8,custo);
			if (x) {
				for (list<int>::iterator lit = (*x).begin(); lit != (*x).end(); ++lit)
					cout << *lit << ",";
				cout << "\ncusto:" << custo << endl;
				cout << endl;
				delete(x);
			}
			else
				cout << "Nao ha caminho maximo" << endl;
		x = new list<int>;
		cout << "**FronteirasMenosVisitadas" << endl;
		GF->FronteirasMenosVisitadas(*x);
		if (x) {
			for (list<int>::iterator lit = (*x).begin(); lit != (*x).end(); ++lit)
				cout << *lit << ",";
			cout << endl;
			delete(x);
		}
		else
			cout << "Nao ha fronteiras visitadas" << endl;
		getchar();
		delete GF;
	return 1;
}