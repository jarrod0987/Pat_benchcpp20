#include "Quadragram.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

//Initializes the quadragram scoring table with values calculated from the corpus file. 
void Quadragram::build_table()
{
	//Load the corpus.
	string corpus = read_file("corpus.txt");

	//Counts the  frequency of each quadragram.
	double count = 0.0;
	for (int i = 0; i < corpus.size()-4; i++)
	{
		int a = corpus.at(i) - 'A';
		int b = corpus.at(i+1) - 'A';
		int c = corpus.at(i+2) - 'A';
		int d = corpus.at(i+3) - 'A';
		table.at(a).at(b).at(c).at(d)++;
		count += 1.0;
	}

	//Converts the frequencys to a more suitable number.
	auto convert = [&](double& d) 
		{ 
			if (d == 0.0) { return -24.0; }
			else {return log(d) / count; }
		};
	
	for (int w = 0; w < 26; w++)
	{
		for (int x = 0; x < 26; x++)
		{
			for (int y = 0; y < 26; y++)
			{
				for (int z = 0; z < 26; z++)
				{
					table[w][x][y][z] = convert(table[w][x][y][z]);
				}
			}
		}
	}
}

//Returns score of a specific quadragram from table.
double Quadragram::get_table_value(string quad)
{
	if (quad.size() != 4) { return 9999.9999; }
	
	int a = quad.at(0) - 'A';
	int b = quad.at(1) - 'A';
	int c = quad.at(2) - 'A';
	int d = quad.at(3) - 'A';
	
	return table.at(a).at(b).at(c).at(d);
}

//Scores as entire string of characters.
double Quadragram::score(string& s)
{
	double score = 0.0;
	for (int i = 0; i < s.size()-4; i++) 
	{ 
		string temp = s.substr(i, 4);
		score += get_table_value(temp);
	}
	return score;
}

//Returns score of a specific quadragram converted to numbers 0-25. Improves performance.
double Quadragram::score_nums(array<int, 120>& nums, int length)
{
	double score = 0.0;
	for (int i = 0; i < length - 4; i++)
	{
		int a = nums[i];
		int b = nums[i+1];
		int c = nums[i+2];
		int d = nums[i+3];

		score += table[a][b][c][d];
	}
	return score;
}