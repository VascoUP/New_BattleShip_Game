#ifndef Player_BSG
	#define Player_BSG
#include "Types.h"
#include "Bomb_BSG.h"
#include "Ship_BSG.h"
#include "Board_BSG.h"
#include <string>

using namespace std;

class Player
{
public:
	Player(string PlayerName, string boardFilename); //criação do "jogador" através do seu nome e respetivo ficheiro de configuração
	Position_Type<int> boardSize() const; //tamanho do tabuleiro
	int areaOcupied() const; //area ocupada pelos barcos
	time_t getTime() const; //tempo de cada jogada
	vector <Ship> Ships() const;
	friend ostream &operator<<(ostream& out, const Player&); //faz o display do tabuleiro (board) através do overloading do operator <<
	Bomb getBomb(); //recebe as coordenadas da bomba e recebe as posições finais da respetiva
	void attackBoard(const Bomb &b);//ataca o tabuleiro
	bool endgame();//verifica se o tabuleiro está vazio e por isso o jogo já acabou (endgame ainda por testar)
	int firstPlayer() const; //cria a ordem de jogada

private:
	string name; 
	Board board;
	time_t Time;
};

#endif