#include "Huffman.h"
#include <queue>
#include <iostream>

using namespace std;

void HuffManTree::deleteNodes(const Node* ptr) {
	if(!ptr) return;
	deleteNodes(ptr->left);			//Deleta os nodos da sub-arvore esquerda
	deleteNodes(ptr->right);		//Deleta os nodos da sub-arvore direita
	delete ptr;					//Depois deleta o pai das sub-arvores
}

Node* HuffManTree::copyNodes(const Node* ptr, Node* parent) {
	if(!ptr) return NULL;			//Caso base...
	Node* ans = new Node(ptr->elem.first,ptr->elem.second);
	ans->parent = parent;
	ans->right = copyNodes(ptr->right,ans);    //Pai do filho direito sera o nodo atual
	ans->left = copyNodes(ptr->left,ans);	   //Pai do filho esquerdo sera o nodo atual
	return ans;
}

HuffManTree::HuffManTree(int freqs[256]) {
	priority_queue<Node*> PQ;
	Node *a, *b;
	
	for(int i=0;i<256;i++)
		if(freqs[i]!=0) 	//Para todo caractere presente no arquivo cria uma arvore de um nodo e coloca na fila de prioridade
			PQ.push(new Node((unsigned char)i,freqs[i]));
	
	while(PQ.size()>1) {
		a = PQ.top(); PQ.pop();			//Pega os elementos com menor prioridade
		b = PQ.top(); PQ.pop();
		Node* ans = new Node(0,a->elem.second + b->elem.second);	//A nova arvore tera a soma de a e b como prioridade
		ans->left = a;			//Filho esquerdo da nova arvore sera a
		ans->right = b;			//Filho direito sera b
		PQ.push(ans);
	}

	//A raiz da HuffManTree sera a raiz da unica arvore que restou na fila de prioridade
	if(PQ.size()!=0) root = PQ.top();
	else root = NULL;					//Se nao houve nada na fila, arquivo vazio...
}

HuffManTree::~HuffManTree() {
	deleteNodes(root);
}

HuffManTree::HuffManTree(const HuffManTree &other) {
	root = NULL;
	*this = other;		//Utilizando o operador de atribuicao
}

HuffManTree& HuffManTree::operator=(const HuffManTree &other) {
	if(this==&other) return *this; 			//Testa auto-atribuicao
	deleteNodes(root);
	root = copyNodes(other.root,NULL);	//Pai da nova raiz sera NULL
	return *this;
}

void geraCodes(const Node* p, string s, string codes[]) {
	if(!p) return;		//Arquivo vazio
	//Caso base...
	if(!p->left && !p->right) {	
		//Arvore tem apenas um caractere
		if(s.empty()) codes[p->elem.first] = "0";
		//Se o nodo for uma folha, coloca a string com sua codificacao no vetor e retorna
		else codes[p->elem.first] = s;
		return;
	}
	geraCodes(p->left,s+"0",codes);				//Indo para o filho esquerdo, acresentamos 0 na codificacao
	geraCodes(p->right,s+"1",codes);			//Indo para o filho direito, acresentamos 1 na codificacao
}

void HuffManTree::comprimir(vector<bool> &out, const vector<char> &in) const {
	string codes[256];
	string s;

	//Gera a codificacao de Huffman...
	geraCodes(root,s,codes);

	for(unsigned char i:in) {			//Para todo caractere do arquivo
		for(char j:codes[i]) {			//Para cada 'bit' da string
			if(j=='1') out.push_back(1);		
			else out.push_back(0);
		}
	}
}

void HuffManTree::descomprimir(vector<char> &out, const vector<bool> &in) const {
	Node* p = root;

	//Para todo 'bit' do arquivo
	for(bool i:in) {
		if(i) p = p->right;		//Se o bit for 1, vamos para a direita na arvore
		else p = p->left;		//Se for 0, vamos para a esquerda
		if(!p) p = root;		//Se chegamos em NULL, ou seja, passamos de uma folha, quer dizer que o arquivo tem apenas um caractere, que e a raiz da arvore
		if(!p->right && !p->left) {		//Quando chegar em uma folha, grava o caratere correspondente e volta para a raiz
			out.push_back(p->elem.first);
			p = root;
		}
	}
}