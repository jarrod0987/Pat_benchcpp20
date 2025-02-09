#include "Quadragram.h"
#include <fstream>

void Quadragram::build_table()
{
	//Load the corpus.
	string corpus = read_file("corpus.txt");

	//Process the quadragrams.
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

	//Convert values in table
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
					table.at(w).at(x).at(y).at(z) = convert(table.at(w).at(x).at(y).at(z));
					//if (this->table[w][x][y][z] == 0) this->table[w][x][y][z] = -24.0;
					//else { this->table[w][x][y][z] = log((double)this->table[w][x][y][z] / (double)quad_count); }
				}
			}
		}
	}
}

double Quadragram::get_table_value(string quad)
{
	if (quad.size() != 4) { return 9999.9999; }
	
	int a = quad.at(0) - 'A';
	int b = quad.at(1) - 'A';
	int c = quad.at(2) - 'A';
	int d = quad.at(3) - 'A';
	
	return table.at(a).at(b).at(c).at(d);
}

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