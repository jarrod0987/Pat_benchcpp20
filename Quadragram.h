#pragma once
#include <array>
#include "Scoring.h"

class Quadragram :public Scoring
{
private:
	array<array<array<array<double, 26>, 26>, 26>, 26> table{0.0};
	void build_table() override;
	
public:
	Quadragram() { build_table(); }
	double get_table_value(string quad);
	double score(string& s);
	double score_nums(array<int, 120>& nums, int length);
	
};