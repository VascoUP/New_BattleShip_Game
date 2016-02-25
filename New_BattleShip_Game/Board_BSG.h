#ifndef Board_BSG
	#define Board_BSG
#include "Types.h"
#include "Bomb_BSG.h"
#include "Ship_BSG.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Board
{
public:
	Board(const string &filename); //carrega o tabuleiro através do ficheiro "filename"
	Position_Type<int> get_size() const; //dimensão do tabuleiro
	vector <Ship> get_ships() const;
	int shipsArea() const; //calcula a área do respetivo barco para depois se calcular a pontuação
	bool putShip(const Ship &s); //coloca o barco
	void removeShip(const Ship &s);//remove o barco do tabuleiro
	void moveShip(); //move o barco
	bool attack(const Bomb &b); //ataca o tabuleiro/barco
	bool endGame(); // para verificar se o jogo acabou, ou seja, se um tabuleiro está ou não vazio 
	friend ostream &operator<<(ostream& out, const Board&); //overloading do operator<< permitindo a construção do tabuleiro com toda a informação recolhida anteriormente
	void show() const; //mostra os atributos do tabuleiro (por debugging)
private:
	int numLines, numColumns;
	int ship_Position; //o indice de um certo barco no vetor dos barcos (só é usado no putShip)
	vector<Ship> ships;
	vector <vector <int>> board;
};


#endif