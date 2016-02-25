#include "Board_BSG.h"
#include "Ship_BSG.h"
#include "Bomb_BSG.h"
#include "IO.h"
#include "Types.h"
#include <fstream>
#include <clocale>
#include <iostream>

Board::Board(const string &filename) //carrega o tabuleiro através do ficheiro "filename"
{
	//elementos que irão ser lidos do respetivo ficheiro de configuração:
	char ignore, symbol, column, line, orientation;
	int color, size;

	//ficheiro de configuração:
	ifstream input;

	input.open(filename);

	if (input.fail())
	{
		Func::setcolor(LIGHTRED, BLACK);
		cout << "ATENÇÃO: ";
		Func::setcolor(WHITE, BLACK);

		cerr << "Erro de leitura de um dos ficheiros. O jogo irá encerrar...\n";
		exit(1);
	}
	else if (input.eof())
	{
		Func::setcolor(LIGHTRED, BLACK);
		cout << "ATENÇÃO: ";
		Func::setcolor(WHITE, BLACK);

		cerr << "Um dos ficheiros que forneceram encontra-se vazio. O jogo irá encerrar...\n";
		exit(1);
	}

	input >> numLines >> ignore >> numColumns;
	input.ignore(1000, '\n');

	if (numLines != numColumns || (numLines > 26 || numLines <= 0))
	{
		Func::setcolor(LIGHTRED, BLACK);
		cout << "ATENÇÃO: ";
		Func::setcolor(WHITE, BLACK);

		cerr << "As dimensões do seu tabuleiro não são válidas:\n1 - O tabuleiro tem de ser quadrado;\n"
			<< "2 - Tem de ter dimensões compreendidas entre 1 e 26.\n"
			<< "Configure novamente o seu ficheiro por favor...\n"
			<< "O jogo irá encerrar...\n";
		exit(1);
	}

	while (!input.eof())
	{
		input >> symbol >> size >> line >> column >> orientation >> color;

		Position_Type<char> position;
		position.col = column;
		position.lin = line;
		unsigned int converted_color = Func::convert_color(color);

		Ship boat(symbol, position, orientation, size, converted_color); //criação do barco com a informação recolhida

		input.ignore(1000, '\n');

		if (size > numLines) //caso o tamanho do barco seja grande demais para o tabuleiro
		{
			Func::setcolor(LIGHTRED, BLACK);
			cout << "ATENÇÃO: ";
			Func::setcolor(WHITE, BLACK);

			cerr << "O barco representado com o símbolo " << symbol << " tem um tamanho demasiado grande para o tabuleiro.\n"
				<< "Volte a reconfigurar o seu ficheiro.\n"
				<< "O jogo irá encerrar...\n";
			exit(1);
		}

		ships.push_back(boat); //guarda o barco já construído no vetor ships
	}

	input.close();
	board = Func::tab(ships, numLines, numColumns); //coloca os barcos mediante a nova posição
}

Position_Type<int> Board::get_size() const //dimensão do tabuleiro
{
	Position_Type<int> size;
	size.col = numColumns;
	size.lin = numLines;
	return size;
}

vector <Ship> Board::get_ships() const
{
	vector <Ship> bShips = ships;
	return bShips;
}

int Board::shipsArea() const //calcula a área do respetivo barco para depois se calcular a pontuação
{
	int area = 0;

	for (size_t i = 0; i < ships.size(); i++)
	{
		area = area + ships[i].get_size();
	}

	return area;
}

bool Board::putShip(const Ship &s) //coloca o barco
{
	unsigned int size = s.get_size();
	char orientation = s.get_orientation();
	Position_Type<int> position = s.get_position();

	if (Func::checkShip(board, s))
	{
		position = s.get_position();
		//colocação do barco:
		Func::ship2tab(board, s, ship_Position);
		return true;
	}

	return false;
}

void Board::removeShip(const Ship &s) //remove o barco do tabuleiro
{
	Position_Type<int> position = s.get_position();

	if (s.get_orientation() == 'H')
	{
		for (unsigned int i = 0; i < s.get_size(); i++)
		{
			board[position.lin][position.col + i] = -1;
		}
	}
	else
	{
		for (unsigned int i = 0; i < s.get_size(); i++)
		{
			board[position.lin + i][position.col] = -1;
		}
	}
}

void Board::moveShip() //move o barco
{
	for (size_t i = 0; i < ships.size(); i++)
	{
		Ship s = ships[i];
		ship_Position = i;

		removeShip(ships[i]);
		bool put = false;

		for (unsigned int numTimes = 0; numTimes < 5; numTimes++)
		{
			if (s.moveRand(0, 0, numLines - 1, numColumns - 1))
			{

				if (putShip(s))
				{
					ships[i] = s;
					put = true;
					break;
				}
			}
			
			s = ships[i];
		}

		if (!put)
			putShip(ships[i]);
	}
}

bool Board::attack(const Bomb &b) //ataca o tabuleiro/barco
{
	Position_Type<char> target = b.getTargetPosition();
	Position_Type<int> position;
	int l = Func::search_char(target.lin);
	int c = Func::search_char(target.col);
	position.lin = l;
	position.col = c;

	//verificar limites (ver se a bomba cai fora do tabuleiro)
	if (l < 0)
		l = 0;
	else if (l > numLines - 1)
		l = numLines - 1;
	if (c < 0)
		c = 0;
	else if (c > numColumns - 1)
		c = numColumns - 1;

	if (board[l][c] == -1)
		return false;
	else 
	{
		int partNumber = Func::searchPartNumber(position, ships[board[l][c]]);
		ships[board[l][c]].attack(partNumber);
	}

	return true;
}

bool Board::endGame() // para verificar se o jogo acabou, ou seja, se um tabuleiro está ou não vazio 
{
	for (size_t i = 0; i < ships.size(); i++)
	{
		if (!ships[i].isDestroyed())
			return false;
	}

	return true;
}

ostream &operator<<(ostream& out, const Board& board) //overloading do operator<< permitindo a construção do tabuleiro com toda a informação recolhida anteriormente
{
	out << "  ";

	for (int i = 0; i < board.numColumns; i++)
	{
		out << minusculas[i] << " ";
	}

	for (size_t l = 0; l < board.board.size(); l++)

	{

		out << endl << maiusculas[l];

		Func::setcolor(BLUE, LIGHTGRAY);
		out << " ";

		for (size_t c = 0; c < board.board[l].size(); c++)
		{
			if (board.board[l][c] == -1)
			{
				Func::setcolor(BLUE, LIGHTGRAY);
				out << ". ";
			}
			else
			{
				Position_Type<int> position;
				position.lin = l;
				position.col = c;

				Ship s = board.ships[board.board[l][c]];

				if (s.isDestroyed())
				{
					Func::setcolor(BLUE, LIGHTGRAY);
					out << ". ";
				}
				else
				{

					int partNumber = Func::searchPartNumber(position, s);

					Func::setcolor(s.get_color(), LIGHTGRAY);
					out << s.get_symbol(partNumber) << " ";
				}
			}

			Func::setcolor(WHITE, BLACK);
		}
	}

	return out;
}

void Board::show() const //mostra os atributos do tabuleiro (por debugging)
{
	cout << numLines << " x " << numColumns << endl;
}