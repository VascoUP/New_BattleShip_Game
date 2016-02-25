#ifndef Ship_BSG
	#define Ship_BSG
#include "Types.h"
#include <string>
#include <fstream>

using namespace std;

class Ship
{
public:
	Ship(char symbol, Position_Type<char> position, char orientation, unsigned int size, unsigned int color); //criação do barco/ship
	char get_symbol(const int &partNumber) const; //símbolo do barco
	int get_size() const; //tamanho do barco
	char get_orientation() const; //orientação do barco
	Position_Type<int> get_position() const; //posição do barco
	unsigned int get_color() const; //cor do barco
	bool move(char direction, bool rotate, unsigned int lineMin, unsigned int columnMin, unsigned int lineMax, unsigned int columnMax); //move o barco
	bool moveRand(unsigned int lineMin, unsigned int columnMin, unsigned int lineMax, unsigned int columnMax); //cria o movimento e rotação do barco
	bool attack(size_t partNumber); //ataca o barco
	bool isDestroyed() const; //destroi o barco
	friend ostream &operator<<(ostream& out, const Ship&); //overloading do operator<< permitindo mostrar os atributos do ship (para debugging) 
private:
	char ship_symbol;
	Position_Type<int> ship_position;
	char ship_orientation;
	unsigned int ship_size;
	unsigned int ship_color;
	string status;
};

#endif