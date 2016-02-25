#include "Ship_BSG.h"
#include "IO.h"
#include <fstream>
#include <string>

Ship::Ship(char symbol, Position_Type<char> position, char orientation, unsigned int size, unsigned int color) //criação do barco/ship
{
	ship_symbol = symbol;
	ship_position.col = Func::search_char(position.col);
	ship_position.lin= Func::search_char(position.lin);
	ship_orientation = orientation;
	ship_size = size;
	ship_color = color;
	for (unsigned int i = 0; i < size; i++)
	{
		status = status + symbol;
	}
}

char Ship::get_symbol(const int &partNumber) const //símbolo do barco
{
	char symbol;
	return symbol = status[partNumber];
}

int Ship::get_size() const //tamanho do barco
{
	unsigned int size;
	return size = ship_size;
}

char Ship::get_orientation() const //orientação do barco
{
	char orientation;
	return orientation = ship_orientation;
}

Position_Type<int> Ship::get_position() const //posição do barco
{
	Position_Type<int> position;
	return position = ship_position;
}

unsigned int Ship::get_color() const //cor do barco
{
	unsigned int color;
	return color = ship_color;
}

bool Ship::move(char direction, bool rotate, unsigned int lineMin, unsigned int columnMin, unsigned int lineMax, unsigned int columnMax) //move o barco
{
	bool possible = false;
	Position_Type<int> pos;

	char orientation = ship_orientation;
	pos.lin = ship_position.lin;
	pos.col = ship_position.col;

	if (rotate)
	{
		if (orientation == 'H' || orientation == 'h')
			orientation = 'V';
		else
			orientation = 'H';
	}

	switch (direction)
	{
	case 'N':
		pos.lin--;

		if (Func::pos_possible(pos, ship_size, orientation, lineMin, columnMin, lineMax, columnMax))
		{
			ship_position = pos;
			ship_orientation = orientation;
			possible = true;
		}
		break;
	case 'S':
		pos.lin++;
		if (Func::pos_possible(pos, ship_size, orientation, lineMin, columnMin, lineMax, columnMax))
		{
			ship_position = pos;
			ship_orientation = orientation;
			possible = true;
		}
		break;
	case 'E':
		pos.col++;
		if (Func::pos_possible(pos, ship_size, orientation, lineMin, columnMin, lineMax, columnMax))
		{
			ship_position = pos;
			ship_orientation = orientation;
			possible = true;
		}
		break;
	case 'W':
		pos.col--;
		if (Func::pos_possible(pos, ship_size, orientation, lineMin, columnMin, lineMax, columnMax))
		{
			ship_position = pos;
			ship_orientation = orientation;
			possible = true;
		}
		break;
	default:
		break;
	}

	return possible;
}

bool Ship::moveRand(unsigned int lineMin, unsigned int columnMin, unsigned int lineMax, unsigned int columnMax) //cria o movimento e rotação do barco
{
	char direct;

	int line = ship_position.lin;
	int column = ship_position.col;
	char orientation = ship_orientation;

	int rotation = rand() % 2;
	int direction = rand() % 5;

	switch (direction)
	{
	case 1:
		direct = 'N';
		break;
	case 2:
		direct = 'S';
		break;
	case 3:
		direct = 'E';
		break;
	case 4:
		direct = 'O';
		break;
	default:
		direct = '0';
		break;
	}

	return move(direct, (bool) rotation, lineMin, columnMin, lineMax, columnMax);
}

bool Ship::attack(size_t PartNumber) //ataca o barco
{
	bool attacked = false;
	if (PartNumber < ship_size)
	{
		status[PartNumber] = tolower(status[PartNumber]); //se parte do barco tiver sido atacada, a letra dessa posição fica em minúsculas
		attacked = true;
	}

	return attacked;
}

bool Ship::isDestroyed() const //destroí o barco
{
	bool destruction = false;
	double size = ship_size; //double porque vai dividir por 2 na linha abaixo
	int destroyed = ceil(size / 2);
	int sum = 0;
	for (unsigned int i = 0; i < ship_size; i++)
	{
		if (islower(status[i]))
		{
			sum++;
			if (sum == destroyed)
			{
				destruction = true;
				break;
			}
		}
	}

	return destruction;
}

ostream &operator<<(ostream& out, const Ship& ship) //overloading do operator<< permitindo mostrar os atributos do ship (para debugging)
{
	out << ship.ship_symbol << " " << ship.ship_size << " " << ship.ship_orientation << "\n"
		<< ship.ship_position.lin << " " << ship.ship_position.col << "\n"
		<< ship.status << "\n";

	return out;
}