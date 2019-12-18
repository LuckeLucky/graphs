#include "Grafo.h"
Grafo::Grafo()
{
	//construtor grafo
}
//Destrutor grafo
Grafo::~Grafo()
{
	Apaga_Conteudo();
	for (list<Pessoa*>::iterator itp = lista_pessoas.begin(); itp != lista_pessoas.end();) {
		delete(*itp);//primeiro apagamos o elemento referenciado por *Pessoa que é Pessoa
		itp = lista_pessoas.erase(itp);//eliminamos o *Pessoa da lista
	}
}

//-------------------------------------------------------------------
//Método: Load
//Parametros:
// Entrada:
//     fich_grafo: Ficheiro dos dados do Grafo
//     fich_pessoas: Ficheiro das pessoas que querem fazer as viagens entre as fronteiras
// Retorno:
//    true/false, mediante a leitura correcta!
//-------------------------------------------------------------------
bool Grafo::Load(const string &fich_grafo, const string &fich_pessoas)
{
	int as = 0;
	int n_fronteiras, n_arestas;
	string linha;
	Fronteira *f = NULL;
	Aresta *a = NULL;
	Pessoa *p = NULL;
	//Leitura do grafo
	ifstream file_grafo;
	file_grafo.open(fich_grafo);
	if (file_grafo.is_open()) {//Verifica se o ficheiro foi aberto com sucesso
		file_grafo >> n_fronteiras;
		file_grafo >> n_arestas;
		while ((getline(file_grafo, linha))) {
			if (linha.find(';') != string::npos) {//se a linha tiver o separador ';' é onde temos os dados para ler
				if (n_fronteiras) {//fronteiras para ler
					f = f->Constroi(linha);
					if (!f)
						return false;
					n_fronteiras--;
					map_caminhos[f];
				}
				else {
					if (n_arestas){//passamos a ler as arestas
					if(!Inserir_Aresta(linha))
						return false;
					n_arestas--;
					}
				}
			}
		}
		file_grafo.close();
	}
	else
		return false;
	//Leitura das pessoas
	ifstream file_pessoas;
	file_pessoas.open(fich_pessoas);
	if (file_pessoas.is_open()) {//Verifica se o ficheiro foi aberto com sucesso
		while ((getline(file_pessoas, linha))) {
			if (linha.find(';') != string::npos) {//se a linha tiver o separador ';' é onde temos os dados para ler
				p = new Pessoa;
				if (!p->Constroi(linha))
					return false;
				lista_pessoas.push_back(p);
			}
		}
		file_pessoas.close();
	}
	else
		return false;
	return true;//Ficheiros lidos com sucesso
}
//-------------------------------------------------------------------
//Método: ContarNos
//Parametros:
// Entrada:--
// Retorno:
//    numero de chaves do mapa,neste caso  numero de fronteiras
//-------------------------------------------------------------------
int Grafo::ContarNos()
{
	return (int)map_caminhos.size();//size do map e igual ao numero de fronteiras que temos guardadas no nosso map
}
//-------------------------------------------------------------------
//Método: ContarArcos
//Parametros:
// Entrada:--
// Retorno:
//    numero de arestas do mapa
//-------------------------------------------------------------------
int Grafo::ContarArcos()
{
	int conta_arcos = 0;
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {//percorre no a no e conta o size da lista de arestas desse no
		conta_arcos += (int)it->second.size();
	}
	return conta_arcos;
}
//-------------------------------------------------------------------
//Método: Memoria
//Parametros:
// Entrada:--
// Retorno:
//    memoria ocupada pelo grafo
//-------------------------------------------------------------------
int Grafo::Memoria()
{
	size_t m_total = 0;
	string nome;
	//Comecar por contar o tamanho ocupado pelo Grafo
	m_total += sizeof(Grafo);
	//O grafo tem uma lista de pessoas por isso temos que contar o espaco ocupado por elas
	for (list<Pessoa*>::iterator itp = lista_pessoas.begin(); itp != lista_pessoas.end(); ++itp) {
		m_total+=sizeof(Pessoa*);//A lista é de ponteiros
		m_total += sizeof(Pessoa);//Os ponteiros apontam para pessoas
		nome=(*itp)->Get_Nome();//A Pesso tem um nome
		m_total+=(sizeof(char)*(nome.size()+1));//Memoria ocupada pelo nome da pessoa
	}
	//O grafo tem um mapa que tem como chave ponteiro para fronteira e como valor uma lista de ponteiros para arestas
	for (map<Fronteira *, list<Aresta*>>::iterator itm = map_caminhos.begin(); itm != map_caminhos.end(); ++itm) {
		m_total+=sizeof(Fronteira*);//O map tem como chave ponteiro para fronteira
		m_total+=sizeof(Fronteira);//ponteiro aponta para fronteira
		for (list<Aresta*>::iterator ita = itm->second.begin(); ita != itm->second.end(); ++ita) {//o map tem para cada no uma lista de arestas adjacentes
		m_total+=sizeof(Aresta*);//lista tem ponteiro para aresta
		m_total+=sizeof(Aresta);//aponta para aresta
		}
	}
	return (int)m_total;
}
//-------------------------------------------------------------------
//Método: NoMaisArcos
//Parametros:
// Entrada:--
// Retorno:
//    lista de inteiros com o numero dos nós com mais arcos
//-------------------------------------------------------------------
list<int> *Grafo::NoMaisArcos()
{
	list<int> *mais_arcos = new list<int>;//iniciar a lista de inteiros
	int maior_valor = 0;//valor que serve para dizer se certo nó entra na lista x ou nao
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {
		if ((int)it->second.size() >maior_valor) {//se o size da lista de arestas desse nó for maior que o maior valor registado ate agora :
			maior_valor = (int)it->second.size();      //o maior_valor passa a ser o size dessa lista de arestas
			mais_arcos->clear();                           //damos clear a nossa lista de nos,ja que todos os nos têm menor valor que o atual
			mais_arcos->push_back(it->first->Get_Numero());//inserimos o novo no que neste caso vai ser sempre o primeiro da lista
		}
		else if (it->second.size() == maior_valor) {//se houver nos com o mesmo numero de arcos basta inserir na lista de inteiros
			mais_arcos->push_back(it->first->Get_Numero());
		}
	}
	return mais_arcos;
}
//-------------------------------------------------------------------
//Método: Adjacencia
//Parametros:
// Entrada:
//			v1:vertice 1
//			v2:vertice 2
// Retorno:
//    true ou false dependendo se ha adjacencia ou nao
//-------------------------------------------------------------------
bool Grafo::Adjacencia(int v1, int v2)
{
	Fronteira *f;
	f = Encontra_Vertice(v1);//encontrar no mapa o v1
	if (f) {//se encontramos o vertice basta saber se temos a aresta que nos liga ao vertice 2
		if (Encontra_Aresta(f, v2))
			return true;
	}

	return false;
}
//-------------------------------------------------------------------
//Método: VerticesIsolados
//Parametros:
// Entrada:
//			v1:vertice inicial
//			v2:vertice destino
//			&custo_total:guarda o custo do caminho de v1 para v2
// Retorno:
//    lista de fronteiras em que passamos para fazer o caminho de v1 a v2
//-------------------------------------------------------------------
list<int> *Grafo::Caminho(int v1, int v2, double &custo_total)
{
	if (!Verificacoes(v1, v2))
		return NULL;
	list<int> *x = new list<int>;
	SitiosInacessiveisAPartirDe(v1, *x);
	map<int, bool> visitado;
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {//percorrer o map
		if (Pertence_A_Lista(it->first->Get_Numero(), *x))//nao consideramos vertices  inacessives a partir de v1
			continue;
		visitado[it->first->Get_Numero()] = false;
	}
	delete(x);
	list<int> *caminho = new list<int>;
	if (Rec_Caminho(v1, v2, caminho, visitado, custo_total)) {//funcao recursiva para descobrir o caminho
		caminho->push_front(v1);//como o vertice inicial é o primeiro a ser visitado
		return caminho;
	}
	else{
		delete(caminho);
		return NULL;
	}
}
//-------------------------------------------------------------------
//Método: VerticesIsolados
//Parametros:
// Entrada:--
// Retorno:
//    lista de fronteiras isoladas(não têm arestas ligadas a elas)
//-------------------------------------------------------------------
list<int> *Grafo::VerticesIsolados()
{
	list<int> *isolados = new list<int>;//iniciar a lista de inteiros
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {
		if ((int)it->second.size() ==0) {  //se a fronteira nao tiver arestas quer dizer que o vertice é isolado             
			isolados->push_back(it->first->Get_Numero());
		}
	}
	return isolados;
}
//-------------------------------------------------------------------
//Método: Search
//Parametros:
// Entrada:--
// Retorno:
//    verdadeiro ou falso dependendo se o vertice esta no grafo ou não
//-------------------------------------------------------------------
bool Grafo::Search(int v)
{
	if (Encontra_Vertice(v))
		return true;
	return false;
}
//-------------------------------------------------------------------
//Método: RemoverVertice
//Parametros:
// Entrada:
//			v:vertice a ser removido
// Retorno:
//    verdadeiro ou falso dependendo se conseguimos remover o vertice dado
//------------------------------------------------------------------
bool Grafo::RemoverVertice(int v)
{
	Fronteira *f;
	Aresta *a = NULL;
	if (!(f=Encontra_Vertice(v)))//o vertice nao se encontra no grafo
		return false;
	//remover as arestas que têm como destino o v
	for (map<Fronteira *, list<Aresta*>>::iterator itm = map_caminhos.begin(); itm != map_caminhos.end(); ++itm) {
		for (list<Aresta*>::iterator ita = itm->second.begin(); ita != itm->second.end();) {
			if ((*ita)->Get_Destino() == v) {
				a = *ita;
				delete(*ita);//primeiro apagamos a Aresta
				ita = itm->second.erase(ita);
			}//apagar o *Aresta do map
			else
				++ita;
		}
	}
	//remover as arestas ligadas a esse vertice
	for (list<Aresta*>::iterator ita = map_caminhos[f].begin(); ita != map_caminhos[f].end();) {

		delete(*ita);//primeiro apagamos a Aresta
		ita = map_caminhos[f].erase(ita);
	}
	map_caminhos.erase(f);//destroi Fronteira *
	delete(f);//destroi a fronteira
	return true;
}
//-------------------------------------------------------------------
//Método: RemoverAresta
//Parametros:
// Entrada:
//			v1:vertice inicio
//			v2:vertice destino
// Retorno:
//			true ou false dependendo se conseguimos remover a aresta que liga v1 a v2 ou nao
//-------------------------------------------------------------------
bool Grafo::RemoverAresta(int v1, int v2)
{
	Fronteira *f = Encontra_Vertice(v1);
	bool arestaA_B = false, arestaB_A = false;
	if (!f)//se o vertice nao pertence ao grafo nao e possivel remover
		return false;
	for (list<Aresta*>::iterator ita = map_caminhos[f].begin(); ita != map_caminhos[f].end();) {
		if ((*ita)->Get_Destino() == v2) {
			delete(*ita);//apagar a aresta
			ita = map_caminhos[f].erase(ita);
			arestaA_B = true;
			break;
		}
		else
			++ita;
	}
	f = Encontra_Vertice(v2);
	if (!f)//se o vertice nao pertence ao grafo nao e possivel remover
		return false;
	for (list<Aresta*>::iterator ita = map_caminhos[f].begin(); ita != map_caminhos[f].end();) {
		if ((*ita)->Get_Destino() == v1) {
			delete(*ita);//apagar a aresta
			ita = map_caminhos[f].erase(ita);
			arestaB_A = true;
			break;
		}
		else
			++ita;
	}
	if (arestaA_B && arestaB_A)
		return true;
	return false;
}
//-------------------------------------------------------------------
//Método: EscreverXML
//Parametros:
// Entrada:
//     &s:nome do ficheiro onde vamos escrever XML
// Retorno:--
//-------------------------------------------------------------------
void Grafo::EscreverXML(const string &s)
{
	ofstream ficheiro;
	ficheiro.open(s);//abertura do ficheiro
	if (ficheiro.is_open()) {
		ficheiro << "<Grafo>" << endl;//inicio abrimos grafo
		for (map<Fronteira *, list<Aresta*>>::iterator itm = map_caminhos.begin(); itm != map_caminhos.end(); ++itm)//escrevemos primeiro as fronteiras
			itm->first->EscreverXML(ficheiro);
		for (map<Fronteira *, list<Aresta*>>::iterator itm = map_caminhos.begin(); itm != map_caminhos.end(); ++itm) {//escrevemos as arestas
			for (list<Aresta*>::iterator ita = itm->second.begin(); ita != itm->second.end(); ++ita)
				(*ita)->EscreverXML(ficheiro,itm->first);
		}
		ficheiro << "</Grafo>" << endl;//fechamos grafo
		ficheiro.close();
	}
	else
		return;
}
//-------------------------------------------------------------------
//Método: LerXML
//Parametros:
// Entrada:
//     &s:nome do ficheiro onde vamos ler o XML
// Retorno:
//		true ou false dependendo se conseguimos ou nao ler o ficheiro xml
//-------------------------------------------------------------------
bool Grafo::LerXML(const string &s)
{
	if (!Apaga_Conteudo())//se nao conseguirmos apagar o conteudo do grafo
		return false;
	ifstream ficheiro;
	int n_linhas = 0;
	string linha;
	Fronteira *f = NULL;
	Aresta *a = NULL;
	int inicio, destino;
	ficheiro.open(s);
	if (ficheiro.is_open()) {
		n_linhas++;
		while (getline(ficheiro, linha)) {
			if (linha.find("</Grafo>") != string::npos &&n_linhas==1)
				continue;;
			if (linha.find("<Fronteira>") != string::npos) {//se na linha actual estiver aberta fronteira lemos fronteira
				f = f->LerXML(ficheiro);
				if (!f)
					return false;
				map_caminhos[f];
				continue;
			}
			if (linha.find("<Aresta>") != string::npos) {//se na linha actual estiver aberta aresta lemos aresta
				a = new Aresta;
				if (!a->LerXML(inicio, destino, ficheiro))
					return false;
				if(!(f = Encontra_Vertice(destino)))
					return false;
				a->Set_Destino(f);
				if (!(f = Encontra_Vertice(inicio)))
					return false;
				map_caminhos[f].push_back(a);
			}
			if(linha.find("</Grafo>") != string::npos)
				break;
		}
		ficheiro.close();
		return true;
	}
	return false;
		
}
//-------------------------------------------------------------------
//Método: DevolveVericesTipo
//Parametros:
// Entrada:
//			&tipo: string com o tipo de fronteira:1,2 ou 3;
// Retorno:
//			lista dos vertices desse mesmo tipo
//------------------------------------------------------------------
list<int> *Grafo::DevolveVerticesTipo(const string &tipo)
{
	int t = atoi(tipo.c_str());//converter a string para inteiro
	if (t < 0 || t>3)//caso o tipo da fronteira seja algo diferente dos 4 tipo 1 2 3  e 0 que é o default
		return NULL;
	list<int> *vertices_tipo = new list<int>;
	for (map<Fronteira *,list<Aresta*>>::iterator it_front = map_caminhos.begin(); it_front != map_caminhos.end(); ++it_front) {//percorrer o map
		if (it_front->first->Get_Tipo() == t) {//só nos interessa a chave
			vertices_tipo->push_back(it_front->first->Get_Numero());
		}
	}
	return vertices_tipo;
}
//-------------------------------------------------------------------
//Método: CaminhoMinimo
//Parametros:
// Entrada:
//			v1:vertice inicial
//			v2:vertice destino
//			&custo_total:custo percorrer o caminho
// Retorno:
//			lista do caminho minimo
//------------------------------------------------------------------
list<int> *Grafo::CaminhoMinimo(int v1, int v2, double &custo_total)
{
	if (!Verificacoes(v1, v2))
		return NULL;
	list<int> *x = new list<int>;
	SitiosInacessiveisAPartirDe(v1,*x);
	map<int, pair<int, double>> caminho_min;//cada vertice vai ter a distancia minima ate si  e de onde vem com a distancia minima
	map<int, bool> visitado;
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {//percorrer o map e guardar apenas os vertices que nos interessam
		if(Pertence_A_Lista(it->first->Get_Numero(),*x))
			continue;
		caminho_min[it->first->Get_Numero()] = make_pair(-1, INT_MAX);
		visitado[it->first->Get_Numero()] = false;
		}
	delete(x);
	Fronteira *f;
	int v_act;
	//o primeiro vertice tem caminho ate ele "0" e distancia "0"
	caminho_min[v1].first = 0;
	caminho_min[v1].second = 0;
	for (int vertices = 0; vertices < (int)caminho_min.size(); ++vertices) {//percorrer todos os vertices
		v_act = Min_Distancia(caminho_min, visitado);
		if (v_act == -1)
			return NULL;
		if (!(f = Encontra_Vertice(v_act)))
			return NULL;
		visitado[v_act] = true;
		for (list<Aresta *>::iterator it = map_caminhos[f].begin(); it != map_caminhos[f].end(); ++it) {
			if (!visitado[(*it)->Get_Destino()] && caminho_min[v_act].second + (*it)->Get_Peso() < caminho_min[(*it)->Get_Destino()].second) {//se o vertice adjacente nao tiver sido visitado e a distancia do vertice actual +a distancia da aresta for menor que a distancia ja guardada no vertice destino 
				caminho_min[(*it)->Get_Destino()].first = v_act;//actualizamos o vertice de onde vem com o peso minimo
				caminho_min[(*it)->Get_Destino()].second = caminho_min[v_act].second + (*it)->Get_Peso();//actualizamos o novo peso para chegar a este vertice
			}
		}
	}
	//recontruir o caminho minimo começando no vertice 2
	list<int> *caminho = new list<int>;
	custo_total = caminho_min[v2].second;
	v_act = v2;
	while (1) {
		caminho->push_front(v_act);
		v2 = caminho_min[v_act].first;
		v_act = v2;
		if (v_act == v1)
			break;
		if(v_act==-1){
			delete(caminho);
			return NULL;
		}
	}
	caminho->push_front(v1);
	return caminho;

}
//-------------------------------------------------------------------
//Método: CaminhoMaximo
//Parametros:
// Entrada:
//			v1:vertice inicial
//			v2:vertice destino
//			&custo_total:custo percorrer o caminho
// Retorno:
//			lista do caminho maximo
//------------------------------------------------------------------
list<int> *Grafo::CaminhoMaximo(int v1, int v2, double &custo_total)
{
	if (!Verificacoes(v1, v2))
		return NULL;
	list<int> *x = new list<int>;
	SitiosInacessiveisAPartirDe(v1, *x);
	custo_total = 0;
	double custo_act = 0;
	map<int, bool>  visitado;
	list<int> *caminho = new list<int>;
	list<int> caminho_actual;
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {//percorrer o map e guardar apenas os vertices que nos interessam
		if (Pertence_A_Lista(it->first->Get_Numero(), *x))
			continue;
		visitado[it->first->Get_Numero()] = false;
	}
	delete(x);
	Rec_Caminho_Max(v1,v2,visitado,custo_act,custo_total,caminho_actual,*caminho);
	if(caminho->empty())
		return NULL;
	return caminho;
}
//-------------------------------------------------------------------
//Método: PossivelCaminho
//Parametros:
// Entrada:
//			v1:vertice 1
//			v2:vertice 2
//			TipoFronteira:tipo da fronteira em que temos que passar
// Retorno:
//			true ou false dependendo se conseguimos ter caminho de v1 a v2 a passar fronteiras por esse tipo
//------------------------------------------------------------------
bool Grafo::PossivelCaminho(int v1, int v2, int TipoFronteira)
{
	if (!Verificacoes(v1, v2))
		return false;
	//verifica se o v1 ou v2 pertence a lista de vertices desse Tipo de fronteira
	list<int> *x;
	x=DevolveVerticesTipo(to_string(TipoFronteira));
	if(!Pertence_A_Lista(v1,*x) || !Pertence_A_Lista(v2,*x)){
		delete(x);
		return false;
	}
	map<int,bool>  visitado;
	Fronteira *f_act;
	list<int> fila;
	fila.push_back(v1);
	//marcar todos os vertices como nao visitados
	for(list<int>::iterator it=x->begin();it!=x->end();++it){
		visitado[*it]=false;
	}
	delete(x);
	visitado[v1]=true;//como comecamos no vertice v1 temos que o marcar como visitado
	while(!fila.empty()){
		v1=fila.front();//acedemos ao primeiro elemento da nossa fila
		fila.pop_front();//eliminar esse elemento
		f_act=Encontra_Vertice(v1);//encontrar no nosso map esse vertice
		if(!f_act)
			return false;
		for (list<Aresta *>::iterator it = map_caminhos[f_act].begin(); it != map_caminhos[f_act].end(); ++it) {//percorrer as arestas dessa fronteira
			if((*it)->Get_Fronteira()->Get_Tipo()==TipoFronteira){//se o destino da aresta tiver esse tipo
				if(visitado[(*it)->Get_Fronteira()->Get_Numero()])//se o destino ja estiver como visitado nao precisamos de o processar novamente
					continue;
				if((*it)->Get_Fronteira()->Get_Numero()==v2)// se chegamos ao v2 devolvemos true
					return true;
				else{
					fila.push_back((*it)->Get_Fronteira()->Get_Numero());//inserimos na fila o destino desta aresta que é o mesmo tipo
					visitado[(*it)->Get_Fronteira()->Get_Numero()]=true;//marcar esse destino como visitado
				}
			}
		}
	
	}
	return false;	
}
//-------------------------------------------------------------------
//Método: FronteirasMenosVisitadas
//Parametros:
// Entrada:
//			&Lv:lista onde vai ser guardada as fronteiras menos visitadas;
// Retorno:
//			--
//------------------------------------------------------------------
void Grafo::FronteirasMenosVisitadas(list<int> &Lv)
{
	if (Lv.size()!=0) {//se a lista não estiver vazia limpa-se a lista
		Lv.clear();
	}
	int visitas = 0;
	map<int, int> n_visitas;
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {//percorrer o map e inicializar o n_visitas de cada vertice a 0
		n_visitas[it->first->Get_Numero()] = 0;
	}
	list<int> *caminho;
	Fronteira *f;
	double custo;
	for (list<Pessoa *>::iterator itp = lista_pessoas.begin(); itp != lista_pessoas.end(); ++itp){//percorrer a lista de pessoas
		caminho=CaminhoMinimo((*itp)->Get_Inicio(), (*itp)->Get_Destino(),custo);//assumimos que as pessoas preferem o caminho minimo ate ao destino
		if (!caminho)
			continue;
		else {
			for (list<int>::iterator itc = caminho->begin(); itc != caminho->end(); ++itc) {//percorrer esse caminho
				f = Encontra_Vertice(*itc);
				if (!f)
					break;
				if (f->Get_Tipo() == 2) {
					(*itp)->Incrementa_Peso();
				}
				n_visitas[f->Get_Numero()]++;
			}
		}
		delete(caminho);
	}
	for (map<int, int>::iterator it = n_visitas.begin(); it != n_visitas.end(); ++it) {
		if (visitas == it->second) {
			Lv.push_back(it->first);
		}
		else {
			if (visitas > it->second) {
				visitas = it->second;
				Lv.clear();
				Lv.push_back(it->first);
			}
		}
	}
}

//-------------------------------------------------------------------
//Método: SistiosInacessiveisAPartirDe
//Parametros:
// Entrada:
//			v1:vertice onde começamos
//			&LV
// Retorno:
//    verdadeiro ou falso dependendo se o vertic
//------------------------------------------------------------------
void Grafo::SitiosInacessiveisAPartirDe(int V1, list<int> &Lv)
{
	if (!Lv.empty()) {//se a lista não estiver vazia limpa-se a lista
		Lv.clear();
	}
	if (V1 == 0)
		return;
	map<int,bool> visitado;
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {//guarda se o vertice ja foi visitado ou nao
		visitado[it->first->Get_Numero()] = false;
	}
	Fronteira *f_act;
	list<int> fila;
	visitado[V1] = true;//primeiro vertice comeca como visitado
	fila.push_back(V1);
	while (!fila.empty()) {
		V1 = fila.front();
		fila.pop_front();
		f_act = Encontra_Vertice(V1);
		if (!f_act)
			return;
		for (list<Aresta *>::iterator it = map_caminhos[f_act].begin(); it != map_caminhos[f_act].end(); ++it) {
			if (!visitado[(*it)->Get_Destino()]) {
				visitado[(*it)->Get_Destino()] = true;
				fila.push_back((*it)->Get_Destino());
			}
		}
	}
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {
		if (!visitado[it->first->Get_Numero()]) {
			Lv.push_back(it->first->Get_Numero());
		}
	}
}
///////////////////////////////////
///////////////////////////////////
//////////////private//////////////
///////////////////////////////////
///////////////////////////////////
bool Grafo::Inserir_Aresta(string dados) {
	int inicio, destino;
	Aresta *a, *b;
	Fronteira *fi,*fd;
	fi = fd = NULL;//inciar as fronteiras
	//a vai ter incio e destino como lemos do ficheiro
	a = new Aresta;
	//b vai ser ao contrario(destino de a=inicio de b ,inicio de a=destino de b)
	b = new Aresta;
	if (!a->Constroi(inicio, destino, dados))
		return false;
	if (!(fi = Encontra_Vertice(inicio)))
		return false;
	if (!(fd = Encontra_Vertice(destino)))
		return false;
	a->Set_Destino(fd);
	b->Set_Peso(a->Get_Peso());
	b->Set_Destino(fi);
	map_caminhos[fi].push_back(a);
	map_caminhos[fd].push_back(b);
	return true;

}
Fronteira *Grafo::Encontra_Vertice(int v) {
	for (map<Fronteira *, list<Aresta*>>::iterator it = map_caminhos.begin(); it != map_caminhos.end(); ++it) {
		if (it->first->Get_Numero() == v)
			return it->first;//encontramos o vertice
	}
	return NULL;//nao encontramos o vertice
}
Aresta *Grafo::Encontra_Aresta(Fronteira *f, int v) {
	if (!f)
		return NULL;
	for (list<Aresta*>::iterator it = map_caminhos[f].begin(); it != map_caminhos[f].end(); ++it) {
		if ( (*it)->Get_Destino()==v) {
			return (*it);
		}
	}
	return NULL;
}
bool Grafo::Rec_Caminho(int v1, int v2, list<int> *caminho, map<int, bool> &visitado, double &custo_total) {
	//Marcar o vertice 1 como visitado
	visitado[v1] = true;
	Fronteira *f_act = Encontra_Vertice(v1);
	if (!f_act)//se o vertice nao estiver no grafo nao podemos fazer nada
		return false;
	for (list<Aresta*>::iterator it = map_caminhos[f_act].begin(); it != map_caminhos[f_act].end(); ++it) {
		if (visitado[(*it)->Get_Destino()])//se ja visitamos o destino desta aresta não vale a pena utiliza-la
			continue;
		if ((*it)->Get_Destino() == v2) {
			caminho->push_front(v2);
			custo_total= (*it)->Get_Peso();
			return true;
		}
		else {
			if (Rec_Caminho((*it)->Get_Destino(), v2, caminho, visitado, custo_total)) {//se encontramos caminho
				caminho->push_front((*it)->Get_Destino());
				custo_total += (*it)->Get_Peso();
				return true;
			}
		}
	}
	return false;
}
bool Grafo::Apaga_Conteudo() {
	//O grafo tem um mapa que tem como chave ponteiro para fronteira e como valor uma lista de ponteiros para arestas
	for (map<Fronteira *, list<Aresta*>>::iterator itm = map_caminhos.begin(); itm != map_caminhos.end();) {
		for (list<Aresta*>::iterator ita = itm->second.begin(); ita != itm->second.end();) {
			delete(*ita);//primeiro apagamos a Aresta
			ita = itm->second.erase(ita);//apagar o *Aresta do map
		}
		delete(itm->first);//remove a Fronteira
		itm = map_caminhos.erase(itm);//remove *Fronteira
	}
	return true;
}
int Grafo::Min_Distancia(map<int,pair<int,double>> &mapa,map<int,bool> &visitado){
	int indice_menor=-1;
	double min = INT_MAX;
	for (map<int, pair<int, double>>::iterator it = mapa.begin(); it != mapa.end(); ++it) {
		if (visitado[it->first] == false && it->second.second <= min) {
			min = it->second.second;
			indice_menor = it->first;
		}
	}
	return indice_menor;
}
bool Grafo::Pertence_A_Lista(int v, list<int> &x){
	list<int>::iterator it;
	it=std::find(x.begin(),x.end(),v);
	if(it==x.end()){//o vertice nao pertence a lista
		return false;
	}
	return true;
}
bool Grafo::Verificacoes(int v1, int v2) {
	if (!Encontra_Vertice(v1) || !Encontra_Vertice(v2))//Vertices nao pertencem ao grafo
		return false;
	list<int> *x;
	//verifica se o v1 ou v2 pertence a lista de vertices isolados
	x = VerticesIsolados();
	if (x) {
		if (Pertence_A_Lista(v1, *x) || Pertence_A_Lista(v2, *x)) {
			delete(x);
			return false;
		}
	}
	//e possivel chegar de v1 a v2 ?
	 SitiosInacessiveisAPartirDe(v1,*x);
	 if (x) {
		 if (Pertence_A_Lista(v2, *x)) {
			 delete(x);
			 return false;
		 }
	 }
	 delete(x);
	 return true;

}
void Grafo::Rec_Caminho_Max(int v1, int v2, map<int,bool> &visitado,double &custo,double &custo_max,list<int> &caminho_actual,list<int> &caminho_max) {
	visitado[v1] = true;
	caminho_actual.push_back(v1);
	if (v1 == v2) {
		if (custo > custo_max) {
			caminho_max.assign(caminho_actual.begin(), caminho_actual.end());
			custo_max = custo;
		}
	}
	else {
		Fronteira *f;
		f = Encontra_Vertice(v1);
		if (!f)
			return;
		for (list<Aresta*>::iterator it = map_caminhos[f].begin(); it != map_caminhos[f].end(); ++it) {
			if (!visitado[(*it)->Get_Destino()]) {
				custo += (*it)->Get_Peso();
				Rec_Caminho_Max((*it)->Get_Destino(), v2, visitado,custo,custo_max,caminho_actual,caminho_max);
				custo -= (*it)->Get_Peso();
			}
		}
	}
	caminho_actual.pop_back();
	visitado[v1] = false;
}