#include "Bomb_BSG.h"
#include "Board_BSG.h"
#include "IO.h"
#include "Types.h"
#include <fstream>
#include <cstdlib>

Bomb::Bomb(Position_Type<char> targetPosition) //criação da bomba
{
	srand(time(NULL));
	const int numMax = 25;
	const int numMin = 0;
	int move = rand() % 5;

	int line = Func::search_char(targetPosition.lin);
	int column = Func::search_char(targetPosition.col);

	//posição final da bomba:
	switch (move){
	case 0:
		targetLine = targetPosition.lin;
		targetColumn = targetPosition.col;
		break;
	case 1: 
		if (line > 0)
			targetLine = maiusculas[line - 1];
		else
			targetLine = maiusculas[numMin];
		targetColumn = targetPosition.col;
		break;
	case 2:
		if (line < 25)
			targetLine = maiusculas[line + 1];
		else
			targetLine = maiusculas[numMax];
		targetColumn = targetPosition.col; 
		break;
	case 3:
		targetLine = targetPosition.lin;
		if (column < 25)
			targetColumn = minusculas[column + 1];
		else
			targetColumn = minusculas[numMax];
		break;
	case 4:
		targetLine = targetPosition.lin;
		if (column > 0)
			targetColumn = minusculas[column - 1];
		else
			targetColumn = minusculas[numMin];
		break;
	default:
		break;
	}
	
}

Position_Type<char> Bomb::getTargetPosition() const //posição do alvo
{
	Position_Type<char> position;
	
	position.lin = targetLine;
	position.col = targetColumn;

	return position;
}

ostream &operator<<(ostream& out, const Bomb& bomb) //overloading do operator<< permitindo mostrar os atributos da bomba (para debugging)
{
	out << bomb.targetLine << " " << bomb.targetColumn;
	return out;
}