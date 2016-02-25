#include "IO.h"
#include "Types.h"
#include "Bomb_BSG.h"
#include "Ship_BSG.h"
#include "Board_BSG.h"
#include "Player_BSG.h"
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <fstream>
#include <ctime>

//-------------Fun��es gerais--------------//

void Func::clrscr() //"apaga" o ecr�
{
	COORD coordScreen = { 0, 0 }; 
	DWORD cCharsWritten;
	DWORD dwConSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hCon, coordScreen);
}

void Func::setcolor(unsigned int color, unsigned int background_color) //muda a cor
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | background_color * 16 + color);
}

unsigned int Func::convert_color(unsigned int color) //converte a cor
{
	unsigned int converted_color;

	switch (color)
	{
	case 0:
		converted_color = BLACK;
		break;
	case 1:
		converted_color = BLUE;
		break;
	case 2:
		converted_color = GREEN;
		break;
	case 3:
		converted_color = CYAN;
		break;
	case 4:
		converted_color = RED;
		break;
	case 5:
		converted_color = MAGENTA;
		break;
	case 6:
		converted_color = BROWN;
		break;
	case 7:
		converted_color = LIGHTGRAY;
		break;
	case 8:
		converted_color = DARKGRAY;
		break;
	case 9:
		converted_color = LIGHTBLUE;
		break;
	case 10:
		converted_color = LIGHTGREEN;
		break;
	case 11:
		converted_color = LIGHTCYAN;
		break;
	case 12:
		converted_color = LIGHTRED;
		break;
	case 13:
		converted_color = LIGHTMAGENTA;
		break;
	case 14:
		converted_color = YELLOW;
		break;
	case 15:
		converted_color = WHITE;
		break;
	}

	return converted_color;
}

void Func::waitUntil(const time_t nextMove) //cria um desfazamento temporal entre o display dos tabuleiros dos jogadores para que se veja se o barco foi ou n�o atacado
{
	time_t beggining = time(NULL);
	time_t end = time(NULL);
	time_t elapsedTime = end - beggining;

	while (elapsedTime < nextMove)
	{
		end = time(NULL);
		elapsedTime = end - beggining;
	}
}

//----------Funcionamento do jogo----------//

int Func::search_char(const char letter) //converte o char da coordenada no seu �ndice
{
	int l = 0;
	char sLetter = letter;

	if (!islower(letter))
		sLetter = tolower(sLetter);

	while (l < alfabeto)
	{
		if (sLetter == minusculas[l])
			break;
		else
			l++;
	}

	return l;
}

void Func::ship2tab(vector <vector <int>> &board, Ship s, unsigned int i) //coloca barco no tabuleiro
{
	int size = s.get_size();
	char orientation = s.get_orientation();
	Position_Type<int> position = s.get_position();

	if (orientation == 'H' || orientation == 'h')
	{
		for (int index = 0; index < size; index++)
			board[position.lin][position.col + index] = i;
	}
	else if (orientation == 'V' || orientation == 'v')
	{
		for (int index = 0; index < size; index++)
			board[position.lin + index][position.col] = i;
	}
}

bool Func::pos_possible(Position_Type<int> Position, unsigned int size, char orientation, int lineMin, int columnMin, int lineMax, int columnMax) //verifica se � poss�vel a posi�o dada
{
	bool possible = true;

	if (orientation == 'H')
	{
		int c = Position.col;
		c = c + size - 1;
		if (!(c <= columnMax && Position.col >= columnMin && Position.lin >= lineMin && Position.lin <= lineMax))
			possible = false;
	}
	else if (orientation == 'V')
	{
		int l = Position.lin;
		l = l + size - 1;
		if (!(Position.col <= columnMax && Position.col >= columnMin && Position.lin >= lineMin && l <= lineMax))
			possible = false;
	}

	return possible;
}

bool Func::checkShip(vector <vector <int>> board, Ship s) //verifica o respetivo barco
{
	//j� se verificou anteriormente que o barco cabe no tabuleiro nesta posi��o

	Position_Type<int> position = s.get_position();

	if (s.get_orientation() == 'H')
	{
		for (int i = 0; i < s.get_size(); i++)
		{
			if (board[position.lin][position.col + i] != -1)
				return false;
		}
	}
	else
	{
		for (int i = 0; i < s.get_size(); i++)
		{
			if (board[position.lin + i][position.col] != -1)
				return false;
		}
	}
	return true;
}

vector < vector <int>> Func::tab(const vector <Ship> &ships, const int Lines, const int Columns) //coloca os barcos mediante as novas posi��es
{
	vector < vector <int>> board(Lines, vector <int> (Columns, -1));

	for (size_t i = 0; i < ships.size(); i++)
		Func::ship2tab(board, ships[i], i);

	return board;
}

int Func::searchPartNumber(const Position_Type<int> &pos, const Ship &s) //procura a vari�vel PartNumber
{
	int partNumber;
	Position_Type<int> ship_pos = s.get_position();

	if (s.get_orientation() == 'H')
		partNumber = pos.col - ship_pos.col;
	else
		partNumber = pos.lin - ship_pos.lin;

	return partNumber;
}

bool Func::EqualShips(vector <Ship> Ships, vector <Ship_Type> sameShips)
{
	for (size_t i = 0; i < sameShips.size(); i++)
	{
		int count = 0;
		Ship_Type ship = sameShips[i];

		for (size_t l = 0; l < Ships.size(); l++)
		{
			if (ship.letter == Ships[l].get_symbol(0) && ship.size == Ships[l].get_size())
				count++;
		}

		if (count != ship.numShips)
			return false;
	}

	return true;
}

bool Func::compShips(vector <Ship> p1Ships, vector <Ship> p2Ships)
{
	vector <Ship_Type> sameShips;

	for (size_t i = 0; i < p1Ships.size(); i++)
	{
		bool found = false;
		Ship s = p1Ships[i];

		for (size_t l = 0; l < sameShips.size(); l++)
		{
			if (s.get_symbol(0) == sameShips[l].letter && s.get_size() == sameShips[l].size)
			{
				sameShips[l].numShips++;
				found = true;
				break;
			}
		}

		if (!found)
		{
			Ship_Type ship;
			ship.letter = s.get_symbol(0);
			ship.size = s.get_size();
			ship.numShips = 1;
			sameShips.push_back(ship);
		}
	}

	if (!Func::EqualShips(p2Ships, sameShips))
		return false;

	return true;
}

//------------Menu e Ordem de Jogo------------//

void Func::menu(vector <boardScore> &bestPlayers) //cria��o do menu
{
	int answer;

	Func::setcolor(LIGHTRED, BLACK);
	cout << "*********************" << endl;
	cout << "    BATALHA NAVAL    " << endl;
	cout << "*********************" << endl;

	cout << endl << endl << "MENU:\n\n";
	Func::setcolor(WHITE, BLACK);

	cout<< "1: Jogar\n"
		<< "2: Melhores Pontua��es\n"
		<< "3: Instru��es\n"
		<< "4: Sair\n\n"
		<< "Op��o desejada: ";
	cin >> answer;

	switch (answer){
	case 1:
		Func::playMenu(bestPlayers);
		break;
	case 2:
		Func::showBestScores(bestPlayers);
		break;
	case 3:
		Func::instructions();
		break;
	case 4:
		Func::updateScores(bestPlayers);
		exit(0);
	default:
		cin.clear();
		cin.ignore(1000, '\n');
		Func::setcolor(LIGHTRED, BLACK);
		cout << "\nATEN��O:";
		Func::setcolor(WHITE, BLACK);
		cerr << "\nPara a pr�xima escolha uma das op��es anteriores.\n";
		system("PAUSE");
		break;
	}
}

void Func::instructions() //instru��es do jogo
{
	Func::clrscr();

	Func::setcolor(LIGHTRED, BLACK);
	cout << setw(45) << "INSTRU��ES:" << endl << endl;
	Func::setcolor(WHITE, BLACK);

	cout << "1� - Ao escolher a op��o 1 do Menu principal, ter� que adicionar o seu nome e\nficheiro de configura��o do seu tabuleiro. Estes passos tamb�m ter�o que ser \nexecutados pelo jogador advers�rio, respetivamente;\n\n"
		<< "2� - Os nomes dos jogadores devem ser diferente;\n\n"
		<< "3� - O ficheiro de configura��o deve ser previamente preenchido e indicado no \njogo com a extens�o .txt;\n\n"
		<< "4� - A ordem de jogada ser� escolhida aleatoriamente pelo computador;\n\n"
		<< "5� - A cada jogada s� ter� acesso ao tabuleiro do jogador advers�rio;\n\n"
		<< "6� - Sempre que ataca o seu advers�rio deve indicar as coordenadas da sua bomba atrav�s de letras (mai�sculas para a linha e min�sculas para a coluna, que se \nencontrem dentro dos limites do tabuleiro);\n\n"
		<< "7� - O jogo termina quando o tabuleiro de um dos jogadores estiver vazio, saindovencedor o jogador que esvaziou esse tabuleiro;\n\n"
		<< "8� - A pontua��o � atribu�da ao jogador vencedor seguindo a f�rmula de c�lculo \nT*S/A, em que T � o tempo que o jogador demora a vencer, S a �rea total ocupada pelos barcos e A a �rea do tabuleiro, respetivamente. Quanto menor for a \npontua��o melhor � o classifica��o do vencedor;\n\n"
		<< "9� - Na op��o 2 do Menu principal tem acesso �s 10 melhores pontua��es dos \nvencedores deste jogo.\n\n";

	system("PAUSE");
}

bool Func::PlayerPossible(Player p1, Player p2)
{
	Position_Type<int> T1Size = p1.boardSize();
	Position_Type<int> T2Size = p2.boardSize();

	if (T1Size.col != T2Size.col || T1Size.lin != T2Size.lin)
		return false;

	vector <Ship> p1Ships = p1.Ships();
	vector <Ship> p2Ships = p2.Ships();

	if (!Func::compShips(p1Ships, p2Ships))
		return false;

	return true;
}

void Func::playMenu(vector <boardScore> &bestPlayers) //informa��es dos jogadores e ordem de jogada
{
	string Player1, Player2, file1, file2;

	Func::clrscr();

	//leitura dos dados dos jogadores:
	Func::setcolor(MAGENTA, BLACK);
	cout << "Jogador 1:" << endl;
	Func::setcolor(WHITE, BLACK);
	cout << "Nome: "; cin >> Player1;
	cin.ignore(1000, '\n');
	cout << endl;
	cout << "Ficheiro: "; cin >> file1;
	cout << endl;

	Func::setcolor(CYAN, BLACK);
	cout << "Jogador 2:" << endl;
	Func::setcolor(WHITE, BLACK);
	cout << "Nome: "; cin >> Player2;
	cin.ignore(1000, '\n');
	cout << endl;
	cout << "Ficheiro: "; cin >> file2;
	cout << endl;

	if (Player1 == Player2) //caso os nomes sejam iguais
	{
		Func::clrscr();

		while (Player1 == Player2)
		{
			Func::setcolor(LIGHTRED, BLACK);
			cout << "ATEN��O: ";
			Func::setcolor(WHITE, BLACK);

			cerr << "Os nomes dos jogadores devem ser diferentes e os ficheiros de \nconfigura��o devem de existir e ter a extens�o .txt\n\n\n";
			Func::setcolor(MAGENTA, BLACK);
			cout << "Jogador 1:" << endl;
			Func::setcolor(WHITE, BLACK);
			cout << "Nome: "; cin >> Player1;
			cin.ignore(1000, '\n');
			cout << endl;
			cout << "Ficheiro: "; cin >> file1;
			cout << endl;

			Func::setcolor(CYAN, BLACK);
			cout << "Jogador 2:" << endl;
			Func::setcolor(WHITE, BLACK);
			cout << "Nome: "; cin >> Player2;
			cin.ignore(1000, '\n');
			cout << endl;
			cout << "Ficheiro: "; cin >> file2;
			cout << endl;

		}
	}

	//cria��o dos respetivos tabuleiros dos jogadores
	Player tab1(Player1, file1);
	Player tab2(Player2, file2);

	if (!Func::PlayerPossible(tab1, tab2))
	{
		setcolor(LIGHTRED, BLACK);
		cout << "ATEN��O: ";
		setcolor(WHITE, BLACK);

		cout << "Os tabuleiros dos jogadores t�m de ter as mesma dimens�es e o mesmo\ntipo de barcos\n"
			<< "Por exemplo, se o jogador 1 tiver o tabuleiro com dimens�es 10 x 10, o jogador 2\nter� tamb�m de ter um tabuleiro 10 x 10\n"
			<< "O mesmo acontece para os barcos. Se o jogador 1 tiver 2 barcos do tipo 'L', o\njogador 2 ter� de ter 2 barcos do tipo 'L'\n\n"
			<< "Construa 2 tabuleiros com as mesma carater�sticas\n\n";
		system("PAUSE");
	}
	else
	{
		Func::clrscr();
		Func::Game(Player1, tab1, Player2, tab2, bestPlayers);
	}
}

void Func::Game(const string Player1, Player &tab1, const string Player2, Player &tab2, vector <boardScore> &bestPlayers)
{
	//para verificar a ordem de jogada:
	int first;

	if (tab1.firstPlayer() == 0)
	{
		cout << "1� a jogar � ";
		Func::setcolor(MAGENTA, BLACK);
		cout << Player1 << endl << endl;
		Func::setcolor(WHITE, BLACK);
		first = 1;
	}

	else
	{
		cout << "1� a jogar � ";
		Func::setcolor(CYAN, BLACK);
		cout << Player2 << endl << endl;
		Func::setcolor(WHITE, BLACK);
		first = 2;
	}

	if (first == 1)
	{
		while (!tab1.endgame() && !tab2.endgame()) //enquanto nenhum acabar o tabuleiro do advers�rio 
		{
			Func::clrscr();
			Func::PlayerTurn(Player1, tab1, Player2, tab2, bestPlayers);
			if (!tab2.endgame())
				Func::PlayerTurn(Player2, tab2, Player1, tab1, bestPlayers);
		}
	}
	else if (first == 2)
	{
		while (!tab1.endgame() && !tab2.endgame()) //enquanto nenhum acabar o tabuleiro do advers�rio 
		{
			Func::clrscr();
			Func::PlayerTurn(Player2, tab2, Player1, tab1, bestPlayers);
			if (!tab1.endgame())
				Func::PlayerTurn(Player1, tab1, Player2, tab2, bestPlayers);

		}
	}

	system("PAUSE");
}

void Func::PlayerTurn(const string Player1, Player &tab1, const string Player2, Player &tab2, vector <boardScore> &bestPlayers) //p�r os jogadores a jogar segundo a ordem que lhes foi atribu�da
{
	const time_t waitingTime = 2;

	cout << "� a vez de ";
	Func::setcolor(MAGENTA, BLACK);
	cout << Player1;
	Func::setcolor(WHITE, BLACK);
	cout << ":" << endl << endl;

	cout << "Tabuleiro de ";
	Func::setcolor(CYAN, BLACK);
	cout << Player2;
	Func::setcolor(WHITE, BLACK);
	cout << ":" << endl << endl;
	cout << tab2; //vai fazer o showBoard() do tabuleiro do jogador 2 devido ao overloading do operator<< feito na antiga fun��o showBoard() na classe Player
	cout << endl << endl;
	Bomb bomba = tab2.getBomb();

	Func::clrscr();

	cout << "� a vez de ";
	Func::setcolor(MAGENTA, BLACK);
	cout << Player1;
	Func::setcolor(WHITE, BLACK);
	cout << ":" << endl << endl;

	cout << "Tabuleiro de ";
	Func::setcolor(CYAN, BLACK);
	cout << Player2;
	Func::setcolor(WHITE, BLACK);
	cout << ":" << endl << endl;
	tab2.attackBoard(bomba);

	Func::waitUntil(waitingTime);

	Func::clrscr();

	if (tab2.endgame())
	{
		cout << "O Jogador ";
		Func::setcolor(MAGENTA, BLACK);
		cout << Player1;
		Func::setcolor(WHITE, BLACK);
		cout << " venceu! Parab�ns!\n\n";

		time_t end = time(NULL);
		time_t elapsedTime = tab2.getTime();
		Position_Type<int> size = tab2.boardSize();
		int tabArea = size.lin*size.col; //�rea do tabuleiro
		int points = elapsedTime * tabArea/ tab2.areaOcupied(); //c�lculo da pontua��o como um integer
		boardScore Player;
		Player.boatsArea = tab2.areaOcupied();
		Player.numColumns = size.col;
		Player.numLines = size.lin;
		Player.PlayerName = Player1;
		Player.score = points;
		Func::newBest(bestPlayers, Player);
		Func::updateScores(bestPlayers);
	}
}

vector <boardScore> Func::topScores(ifstream &scoresFile) //cria��o do ficheiro com os "scores"
{
	vector <boardScore> bestPlayers;
	string name, trash;
	int points, lines, columns, area;

	while (!scoresFile.eof())
	{
		scoresFile >> name >> points >> lines >> trash >> columns >> area;
		boardScore Player;
		Player.PlayerName = name;
		Player.score = points;
		Player.numLines = lines;
		Player.numColumns = columns;
		Player.boatsArea = area;

		scoresFile.ignore(1000, '\n');

		if (points < 0)
			break;

		bestPlayers.push_back(Player);
	}

	return bestPlayers;
}

void Func::showBestScores(const vector <boardScore> bestScores) //mostra os melhores "scores"
{
	Func::clrscr();

	Func::setcolor(LIGHTRED, BLACK);
	cout << "**************************\n"
		<< " BEST SCORES OF ALL TIME: \n"
		<< "**************************\n\n\n";
	Func::setcolor(WHITE, BLACK);

	if (bestScores.size() == 0)
	{
		cout << "N�o h� registos de jogos feitos.";
	}
	else
	{
		for (size_t i = 0; i < bestScores.size(); i++)
		{
			cout << setw(2) << i + 1 << "� Lugar - " << bestScores[i].PlayerName << " - "
				<< bestScores[i].score << endl;
		}
	}

	cout << endl << endl << endl;

	system("PAUSE");
}

void Func::newBest(vector <boardScore> &bestPlayers, const boardScore &Player) //coloca��o de um novo "best score" num vetor para depois colocar no ficheiro
{
	vector <boardScore> newScores;
	const unsigned int maxNumber = 9;
	int index_p = 0, index_v = 0;
	bool PlayerSaved = false;

	if (bestPlayers.size() == 0)
	{
		newScores.push_back(Player);
		PlayerSaved = true;
	}
	else
	{
		while (index_p <= maxNumber)
		{
			if (index_v >= bestPlayers.size())
			{
				if (!PlayerSaved)
				{
					newScores.push_back(Player);
					PlayerSaved = true;
				}
				else
					break;
			}
			else
			{
				if (!PlayerSaved)
				{
					if (bestPlayers[index_v].score < Player.score)
					{
						newScores.push_back(bestPlayers[index_v]);
						index_v++;
					}
					else
					{
						newScores.push_back(Player);
						PlayerSaved = true;
					}
				}
				else
				{
					newScores.push_back(bestPlayers[index_v]);
					index_v++;
				}
			}
			index_p++;
		}
	}

	bestPlayers = newScores;
}

void Func::updateScores(const vector <boardScore> &bestPlayers) //atualiza��o dos "scores"
{
	ofstream outFile;
	outFile.open("Scores.txt");

	for (size_t i = 0; i < bestPlayers.size(); i++)
	{
		outFile << bestPlayers[i].PlayerName << " " << bestPlayers[i].score << " " << bestPlayers[i].numLines
			<< " x " << bestPlayers[i].numLines << " " << bestPlayers[i].boatsArea << " ";
		if (i == bestPlayers.size() - 1)
			break;
		outFile << endl;
	}

	outFile.close();
}