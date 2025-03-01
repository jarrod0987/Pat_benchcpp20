#include<iostream>
#include<string>
#include<random>
#include<functional>
#include<algorithm>
#include<numeric>
#include <array>
#include <vector>
//#include <map>
#include <utility>
//#include<windows.h>
#include <fstream>

#include "Scoring.h"
#include "Quadragram.h"

export module Pat_module;

//Converts string of capital letters to numbers 0-25 for performance improvement.
array<int, 120> encode_letters(string s)
{
	array<int, 120> output;
	for (int i = 0; i < s.size(); i++) { output[i] = s[i] - 'A'; }
	return output;
}

//Converts array of numbers 0-25 back to letters so humans can read them.
string decode_letters(array<int, 120> nums, int len)
{
	string output;
	for (int i = 0; i < len; i++) { output+= nums[i] + 'A'; }
	return output;
}

//Decryption function that works with numbers 0-25 instead of characters. Improves performance.
array<int, 120> decrypt_nums(array<int, 120>& nums, int length, array<int, 26>& key)
{
	array<int, 120> output{ 0 };
	for (int i = 0; i < length; i++)
	{
		output[i] = find(key.begin(), key.end(), nums[i]) - key.begin();
	}

	return output;
}

//Reads lines from a .txt file into a vector.
export vector<string> read_list_from_file(string file_name)
{
	vector<string> output;
	ifstream in_file;
	in_file.open(file_name);

	if (!in_file)
	{
		cout << file_name << " not found...\n";
		return output;
	}

	string current_line;
	while (getline(in_file, current_line)) { output.push_back(current_line); }
	in_file.close();
	return output;
}

//Preprocess the lines of text from the ACA format into one this program can work with.
export vector<string> process_digital_risties()
{
	vector<string> raw_aristos = read_list_from_file("risties_in.txt");
	vector<string> proccessed_aristos;
	string under_construction;
	for (const auto& line : raw_aristos)
	{
		if (line.size() == 0)//Line is empty;
		{
			if (under_construction.at(under_construction.size() - 1) == ' ')
			{
				under_construction.erase(under_construction.size() - 1);
			}

			proccessed_aristos.push_back(under_construction);
			under_construction.clear();
		}
		else if (line.at(1) == '-') { continue; } //Line containing tip.
		else
		{
			under_construction += line;
			if (line.at(line.size() - 1) != ' ') { under_construction += ' '; }
		}
	}
	if (proccessed_aristos.back() != under_construction)
	{
		proccessed_aristos.push_back(under_construction);
	}
	return proccessed_aristos;
}

//Removes spaces from the ristie.
export vector<string> convert_aristo_to_pat(vector<string> aristos)
{
	vector<string> pats;
	for (auto& a : aristos)
	{
		string output;
		for (auto& b : a)
		{
			if (b >= 'A' and b <= 'Z') { output += b; }
		}
		pats.push_back(output);
	}
	return pats;
}

//Adds spaces and symbols back into the decrypt to match the original puzzle.
export string space_decrypted_aristo(string& aristo, string& decrypt)
{
	string output;
	int index = 0;
	for (auto& c : aristo)
	{
		if (c >= 'A' and c <= 'Z')
		{
			output += decrypt.at(index);
			index++;
		}
		else
		{
			output += c;
		}
	}

	return output;
}

//Determines if a ristie is an aristocrat or patristocrat.
export bool detect_patristocrats(string& s)
{
	if (s[5] == ' ' and s[11] == ' ' and s[17] == ' ' and s[23] == ' ' and s[29] == ' ') { return true; }
	return false;
}

//Hill climbs the encoded cipher text to find the plain text.
export string hill_climb(string& ct_in)
{
	//Converts the letters of the cipher text to numbers for perfomance purposes.
	int len = ct_in.size();
	array<int, 120> encoded_ct{encode_letters(ct_in)};

	//Needed Arrays.
	array<int, 120> best_decrypt{ 0 }, current_decrypt{ 0 };
	array<int, 26> best_key{ 0 };

	// Setting up RNG.
	random_device rd;
	default_random_engine re(rd());
	uniform_int_distribution<int> dist_a(0, 25);
	uniform_real_distribution<double> dist_b(0.000001, 1);
	auto twentyfive = bind(dist_a, re);
	auto oh_one = bind(dist_b, re);

	//Variables
	double global_best_score = -1000000.0, current_score = 0.0, dF = 0.0, thresh = 0.0, fitness = 0.0;
	int num = 0, inner_count = 0, outer_count = 0, a = 0, b = 0;
	array<int, 26> old_key{ 0 };
	iota(old_key.begin(), old_key.end(), 0);
	array<int, 26> new_key(old_key);

	//Scoring Table
	shared_ptr<Quadragram> q = make_shared<Quadragram>();

	while (outer_count < 1200)//How many times to try to climb on a random key.
	{
		shuffle(old_key.begin(), old_key.end(), re);

		inner_count = 0;
		double best_score = -100000.0;

		while (inner_count < 600)//How many times to swap 2 random letters.
		{
			new_key = old_key;
			inner_count++;

			a = twentyfive();
			b = twentyfive();

			swap(new_key.at(a), new_key.at(b));

			//Decrypt and score message with new key.
			current_decrypt = decrypt_nums(encoded_ct, len, new_key);
			current_score = q->score_nums(current_decrypt, len);

			//Update scoring data.
			if (current_score > best_score)
			{
				best_score = current_score;
				old_key = new_key;
				inner_count = 0;

				if (current_score > global_best_score)
				{
					global_best_score = current_score;
					best_key = new_key;
					best_decrypt = current_decrypt;
				}
			}
		}
		outer_count++;
	}
	//Returns the best result found.
	return decode_letters(best_decrypt, len);
}