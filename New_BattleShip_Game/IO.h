#ifndef IO
	#define IO
#include "Types.h"
#include "Bomb_BSG.h"
#include "Ship_BSG.h"
#include "Player_BSG.h"
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

//A classe Func tem presente todas as fun�oes que servem de auxiliares a outras fun��es de outras classes, respetivamente
class Func
{
public:
	static void clrscr(); //"apaga" o ecr�
	static void setcolor(unsigned int color, unsigned int background_color); //muda a cor
	static unsigned int convert_color(unsigned int color); //converte a cor
	static void waitUntil(const time_t nextMove); //cria um desfazamento temporal entre o display dos tabuleiros dos jogadores para que se veja se o barco foi ou n�o atacado
	//------//
	static int search_char(const char letter); //converte o char da coordenada no seu �ndice
	static void ship2tab(vector <vector <int>> &board, Ship s, unsigned int i); //coloca barco no tabuleiro
	static bool pos_possible(Position_Type<int> Position, unsigned int size, char orientation, int lineMin, int columnMin, int lineMax, int columnMax); //verifica se � poss�vel a posi�o dada
	static bool checkShip(vector <vector <int>> board, Ship s); //verifica o respetivo barco
	static vector <vector <int>> tab(const vector <Ship> &ships, const int Lines, const int Columns); //coloca os barcos mediante as novas posi��es
	static int searchPartNumber(const Position_Type<int> &pos, const Ship &s); //procura a vari�vel PartNumber
	static bool EqualShips(vector <Ship> Ships, vector <Ship_Type> sameShips);
	static bool compShips(vector <Ship> p1Ships, vector <Ship> p2Ships);
	static bool PlayerPossible(Player p1, Player p2);
	//-----//
	static void menu(vector <boardScore> &bestPlayers); //cria��o do menu
	static void instructions(); //instru��es do jogo
	static void playMenu(vector <boardScore> &bestPlayers); //informa��es dos jogadores e ordem de jogada
	static void Game(const string Player1, Player &tab1, const string Player2, Player &tab2, vector <boardScore> &bestPlayers);
	static void PlayerTurn(const string Player1, Player &tab1, const string Player2, Player &tab2, vector <boardScore> &bestPlayers); //p�r os jogadores a jogar segundo a ordem que lhes foi atribu�da
	static vector <boardScore> topScores(ifstream &scoresFile); //cria��o do ficheiro com os "scores"
	static void showBestScores(const vector <boardScore> bestScores); //mostra os melhores "scores"
	static void newBest(vector <boardScore> &bestPlayers, const boardScore &Player); //coloca��o de um novo "best score" num vetor para depois colocar no ficheiro
	static void updateScores(const vector <boardScore> &bestPlayers); //atualiza��o dos "scores"
};

#endif