#include<iostream>
#include<chrono>
#include<random>
#include<functional>
#include<algorithm>

#include "Scoring.h"
#include "Quadragram.h"

export module Patristocrat;

using namespace std;

export string encrypt(string plain_text, string key)
{
	string output;
	for (char& c : plain_text)
	{
		output += key.at(c - 'A');
	}
	return output;
}

string decrypt(string cipher_text, string key)
{
	string output;
	for (auto& c : cipher_text) { output += key.find(c) + 'A'; }
	return output;
}

export string hill_climb(string cipher_text)
{
	string best_decrypt, current_decrypt, best_key;

	// Setting up RNG.
	auto seed{ chrono::system_clock::now().time_since_epoch().count() };
	default_random_engine re{ default_random_engine((unsigned int)seed) };
	uniform_int_distribution<int> dist_a(0, 25);
	uniform_real_distribution<double> dist_b(0.000001, 1);
	auto twentyfive = bind(dist_a, re);
	auto oh_one = bind(dist_b, re);

	//Variables
	double global_best_score = -1000000.0, current_score = 0.0, dF = 0.0, thresh = 0.0, fitness = 0.0;
	int num = 0, inner_count = 0, outer_count = 0, a = 0, b = 0;
	string old_key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", new_key(old_key);
	bool end = false;

	//Scoring Table
	shared_ptr<Quadragram> q = make_shared<Quadragram>();

	while (outer_count < 1000)//How many times to try to climb on a random key.
	{
		shuffle(old_key.begin(), old_key.end(), re);

		inner_count = 0;
		double best_score = -100000.0;

		while (inner_count < 1000)//How many times to swap 2 random letters.
		{
			new_key = old_key;
			inner_count++;

			a = twentyfive();
			b = twentyfive();

			while (b == a) { b = twentyfive(); }
			swap(new_key.at(a), new_key.at(b));

			//Decrypt and score message with new key.
			current_decrypt = decrypt(cipher_text, new_key);
			current_score = q->score(current_decrypt);

			//Update scoring data.

			if (current_score > best_score)
			{
				best_score = current_score;
				old_key = new_key;

				if (current_score > global_best_score)
				{
					global_best_score = current_score;
					best_key = new_key;
					best_decrypt = current_decrypt;

					//Display to screen. Testing Only
					//cout << current_decrypt << endl << endl;
				}

				inner_count = 0;
			}
			//else
			//{
			//	dF = current_score - best_score;
			//	thresh = oh_one(); //Random threshold.
			//	fitness = pow(2.71828, (dF / 18.0)); //18.0 Determined experimentaly.
			//	if (fitness > thresh)
			//	{
			//		best_score = current_score;
			//		old_key = new_key;
			//	}
			//}
		}
		outer_count++;
	}

	new_key = best_key;
	return best_decrypt;
}