#ifndef HUFFMANT_H
#define HUFFMANT_H

#include <vector>
#include <algorithm>
using namespace std;

class Node {
	public:
		Node(unsigned char elem_= 0, int p_= 0): right(NULL), left(NULL), parent(NULL) {
			elem.first = elem_; elem.second = p_;
		}
		//Cada nodo vai armazenar um caractere e sua frequencia no arquivo...
		pair<unsigned char,int> elem;
		Node *parent;
		Node *left, *right;
};

class HuffManTree {
	public:
		HuffManTree(int freqs[256]);
		~HuffManTree();
		HuffManTree(const HuffManTree&);
		HuffManTree& operator=(const HuffManTree&);
		void comprimir(vector<bool> &out, const vector<char> &in) const;
		void descomprimir(vector<char> &out, const vector<bool> &in) const;

	private:
		Node* root;	

		void deleteNodes(const Node*);
		Node* copyNodes(const Node*, Node*);
};

#endif