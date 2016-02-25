#include "IO.h"
#include <clocale>
#include <fstream>
#include <vector>

int main()
{
	setlocale(LC_ALL, "PORTUGUESE"); //para se conseguir ler e escrever com acentuação
	srand(time(NULL)); //para começar a contabilizar o tempo

	ifstream scoresFile;
	scoresFile.open("Scores.txt");
	vector <boardScore> bestPlayers = Func::topScores(scoresFile); 
	scoresFile.close();

	ofstream outFile;
	outFile.open("Scores.txt");
	Func::updateScores(bestPlayers);
	outFile.close();

	while (true)
	{
		Func::menu(bestPlayers); 
		Func::clrscr();
	}

	return 0;
}