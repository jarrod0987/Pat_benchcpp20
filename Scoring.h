#pragma once
#include <iostream>

using namespace std;

class Scoring
{
private:
	virtual void build_table() = 0;
public:
	string read_file(string file_name);
	void write_file(string& s, string file_name);
};

