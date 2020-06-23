#include <iostream>
#include <fstream>
#include <cstring>
#include <assert.h>
#include "Huffman.h"

using namespace std;

int main(int argc, char**argv) {

	if(strcmp(argv[1],"c")==0) {

		//Cria um fluxo de entrada e abre o arquivo	
		ifstream fin(argv[2]); 
		assert(fin.is_open());

		fin.seekg (0, fin.end);		//Direciona o ponteiro get para o fim do arquivo
		int size = fin.tellg();		//Tamanho do arquivo vai ser a posicao do get
		fin.seekg (0, fin.beg);		//Direciona o get no inicio do arquivo novamente para leitura
		
		unsigned char* buffer = new unsigned char[size];		//Devemos armazenar em unsigned char para nao acessarmos posicoes invalidas
		fin.read(reinterpret_cast <char*> (buffer),size);		//Le o arquivo e armazena em buffer

		vector<char> in(size);
		vector<bool> out;				
		int freqs[256] = {0};
		
		for(int i=0;i<size;i++)
			in[i] = buffer[i];	//Colocando o arquivo lido em um MyVec<char>
		
		for(int i=0;i<size;i++)
			freqs[buffer[i]]++;	//Contabiliza a frequencia de cada caractere que aparece no vetor

		HuffManTree ht(freqs);		//Cria a arvore de Huffman
		ht.comprimir(out,in);		//Comprime o arquivo em bits e salva em out
		
		//Cria um fluxo de saida
		ofstream fout(argv[3]);

		int bytes, bits = out.size();		//Numero de bits e o tamanho de MyVec<bool>
		char c=0; 		   //Cria um byte 00000000

		//Armazena o numero de bits e o vetor de frequencias no arquivo de saida para recuperacao do arquivo original
		fout << bits;				
		fout.write(reinterpret_cast <char*> (freqs), 256*sizeof(int));
		
		//Separando os bits em 'blocos' de 8
		if(bits%8==0) bytes=bits/8;
		else bytes=bits/8+1;				//Se nao for divisivel por 8, temos um bloco a mais que sera preenchido com zeros

		for(int i=0;i<bytes;i++) {   
			for(int j=0;j<8 && (i*8)+j<bits;j++)
				//Liga os bits de c e grava o byte representado por ele no arquivo
				c |= out[(i*8)+j] << j;			//Operacao logica OR entre um char vazio e cada posicao do vetor de bits
			fout << c;
			c=0;
		}
		
		//Fecha os fluxos e desaloca memoria alocada
		fin.close();
		fout.close();
		delete []buffer;

	} else if(strcmp(argv[1],"d")==0) {

		//Cria um fluxo de entrada e abre o arquivo	
		ifstream fin(argv[2]); 
		assert(fin.is_open());

		int bits, bytes;
		fin >> bits;			//Le o numero de bits armazenado no inicio do arquivo

		//Separando os bits em 'blocos' de 8
		if(bits%8==0) bytes=bits/8;
		else bytes=bits/8+1;				//Se nao for divisivel por 8, temos um bloco a mais que sera preenchido com zeros

		//Le o vetor de frequencias armazenado no inicio do arquivo
		int freqs[256];
		fin.read(reinterpret_cast <char*> (freqs), 256*sizeof(int));
			 		
		char* buffer = new char[bytes];
		fin.read(buffer,bytes); 			//Le os bytes que representam um arquvio comprimido

		vector<bool> in;
	
		for(int i=0;i<bytes;i++) {
			for(int j=0;j<8 && (i*8)+j<bits;j++)  
				//Operacao logica AND entre o i-esimo byte do arquivo e o byte 00000001 deslocado j vezes pra esquerda, j=0...8
				//Retorna 1 se o j-esimo bit do i-esimo byte estiver ligado, e 0 caso contrario; insere o resultado no vetor de bits
				in.push_back(1<<j & buffer[i]);
		}

		vector<char> out;

		HuffManTree ht(freqs);		//Cria a arvore de Huffman
		ht.descomprimir(out,in);    //Descomprime o arquivo e salva em out

		//Cria um fluxo de saida
		ofstream fout(argv[3]);

		//Grava no arquivo de saida o arquivo descomprimido
		for(int i=0;i<out.size();i++)
			fout << out[i];

		//Fecha os fluxos e desaloca memoria alocada
		fin.close();
		fout.close();
		delete []buffer;
	}

	return 0;
}