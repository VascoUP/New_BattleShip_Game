#ifndef Bomb_BSG
	#define Bomb_BSG
#include "Types.h"
#include <fstream>

using namespace std;

class Bomb
{
public:
	Bomb(Position_Type<char> targetPosition); //criação da bomba
	Position_Type<char> getTargetPosition() const; //posição do alvo
	friend ostream &operator<<(ostream& out, const Bomb&); //overloading do operator<< permitindo mostrar os atributos da bomba (para debugging)
private:
	char targetLine, targetColumn;
};

#endif