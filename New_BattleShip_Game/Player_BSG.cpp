#include "Player_BSG.h"
#include "Bomb_BSG.h"
#include "Ship_BSG.h"
#include "Board_BSG.h"
#include "IO.h"
#include <iostream>
#include <clocale>

Player::Player(string PlayerName, string boardFilename) :board(boardFilename), name(PlayerName) //cria��o do "jogador" atrav�s do seu nome e respetivo ficheiro de configura��o
{
	Time = 0;
}

Position_Type<int> Player::boardSize() const //tamanho do barco
{
	Position_Type<int> size = board.get_size();
	return size;
}

int Player::areaOcupied() const //�rea ocupada pelos barcos
{
	int area = board.shipsArea();
	return area;
}

time_t Player::getTime() const //tempo de cada jogada
{
	time_t elapsedTime;
	return elapsedTime = Time;
}

vector <Ship> Player::Ships() const
{
	vector <Ship> ships = board.get_ships();
	return ships;
}

ostream &operator<<(ostream& out, const Player& Player) //faz o display do tabuleiro (board) atrav�s do overloading do operator <<
{
	out << Player.board;
	out << endl;

	return out;
}

Bomb Player::getBomb() //recebe as coordenadas da bomba e recebe as posi��es finais da respetiva
{
	Position_Type<char> coordinates;
	Position_Type<int> BoardSize = board.get_size();

	cout << "Insira as coordenadas de alvo da sua bomba: ";
	time_t beggining = time(NULL);
	cin >> coordinates.lin >> coordinates.col;

	while (!islower(coordinates.col) || !isupper(coordinates.lin) || Func::search_char(coordinates.lin) >= BoardSize.lin || Func::search_char(coordinates.col) >= BoardSize.col)
	//caso n�o se insira bem as coordenadas volta a pedir novamente as mesmas at� estar correto
	{
		Func::setcolor(LIGHTRED, BLACK);
		cout << "ATEN��O: ";
		Func::setcolor(WHITE, BLACK);

		cout << "\n1 - As coordenadas de alvo da sua bomba t�m que estar no intervalo entre [A..." << maiusculas[BoardSize.lin - 1]
			<< "]\npara a linha e [a..." << minusculas[BoardSize.col - 1] << "] para a coluna\n"
			<< "\n2 - As coordenadas de alvo da sua bomba t�m que ser letras:\nLINHA - mai�scula  COLUNA-min�scula\n"
			<< "\nInsira as coordenadas de alvo da sua bomba: ";
		cin >> coordinates.lin >> coordinates.col;
	}

	//soma do tempo de cada jogada
	time_t end = time(NULL);
	Time = Time + end - beggining;

	Bomb bomba(coordinates);
	bomba.getTargetPosition();

	return bomba;
}

void Player::attackBoard(const Bomb &b) //ataca o tabuleiro
{
	board.moveShip();
	board.attack(b);
	cout << board; //faz-se refer�ncia ao display do board atrav�s do cout uma vez que esta fun��o resulta do overloador do operator<<
}

bool Player::endgame() //verifica se o tabuleiro est� vazio e por isso o jogo j� acabou (endgame ainda por testar)
{
	return board.endGame();
}

int Player::firstPlayer() const //cria a ordem de jogada
{
	srand(time(NULL));
	unsigned int n;
	n = rand() % 2;

	return n;
}