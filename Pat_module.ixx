#include<iostream>
#include<random>
#include<functional>
#include<algorithm>
#include<numeric>
#include <array>
#include <vector>
#include <map>
#include <utility>
#include<windows.h>
#include <fstream>
#include <execution>

#include "Scoring.h"
#include "Quadragram.h"

export module Pat_module;

export class Patristocrat
{
public:
	string plain_text, cipher_text;
	array<int, 100> encoded_ct{ 0 };
	int ct_length = 0;

	//Encrypts plaint text to cipher text with a key.
	void encrypt(string key)
	{
		string output;
		for (char& c : plain_text)
		{
			output += key.at(c - 'A');
		}
		cipher_text = output;
	}

	//Converts string of capital letters to numbers 0-25 for performance improvement.
	void encode()
	{
		for (int i = 0; i < ct_length; i++) { encoded_ct[i] = cipher_text[i] - 'A'; }
	}

	//Decryption function that works with numbers 0-25 instead of characters. Improves performance.
	array<int, 100> decrypt_nums(array<int, 100>& nums, int length, array<int, 26>& key)
	{
		array<int, 100> output{ 0 };
		for (int i = 0; i < length; i++)
		{
			output[i] = find(key.begin(), key.end(), nums[i]) - key.begin();
		}

		return output;
	}

	//Hill climbs the encoded cipher text to find the plain text.
	array<int, 100> hill_climb(array<int, 100>& encodedxx, int len)
	{
		array<int, 100> best_decrypt{ 0 }, current_decrypt{ 0 };
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
				current_decrypt = decrypt_nums(encodedxx, len, new_key);
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

						//Display to screen. Testing Only
						//cout << current_score << endl;
					}
				}
			}
			outer_count++;
		}
		return best_decrypt;
	}

	//Hill climbs a batch of patristocrats concurrently on CPU.
	void batch_climb_CPU(vector<array<int, 100>>& pats)
	{
		//This Lambda expression is a needed wrapper to make for_each work.
		auto lam = [&](array<int, 100>& a) { a = hill_climb(a, ct_length); };

		for_each(execution::par_unseq, pats.begin(), pats.end(), lam);
	}
};

//Keeps terminal window open until user presses E.
export void hold_window()
{
	cout << "Press E to EXIT..." << endl;
	bool end = false;
	while (end == false)
	{
		if (GetAsyncKeyState('E') & 0x8000)
		{
			end = true;
		}
	}
}

//Creates a standard keyed alphabet from a keyword.
export string make_keyed_alphabet(string& keyword)
{
	string output;
	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (auto& c : keyword)
	{
		if (output.find(c) == string::npos) { output.push_back(c); }
	}

	for (auto& c : alphabet)
	{
		if (output.find(c) == string::npos) { output.push_back(c); }
	}
	return output;
}