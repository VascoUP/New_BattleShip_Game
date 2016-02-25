#ifndef Types
	#define Types
#include <string>

using namespace std;

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

const int alfabeto = 26;
static char maiusculas[alfabeto] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
static char minusculas[alfabeto] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

template<typename P> //template para substituir PositionInt e PositionChar
struct Position_Type
{
	P lin;
	P col;
};

struct boardScore //informação do ficheiro com "scores"
{
	string PlayerName;
	int score, numLines, numColumns, boatsArea;
};

struct Ship_Type
{
	char letter;
	int size, numShips;
};

#endif