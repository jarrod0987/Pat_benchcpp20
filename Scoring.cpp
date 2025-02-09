#include "Scoring.h"
#include <fstream>
#include <string>

string Scoring::read_file(string file_name)
{
    string s, current_line;
    ifstream in_file;

    in_file.open(file_name);

    if (!in_file) { cout << file_name << " not found...\n"; }
    else
    {
        while (getline(in_file, current_line)) { s += current_line; }
        in_file.close();
    }
    
    return s;
}

void Scoring::write_file(string& s, string file_name)
{
    ofstream out_file;
    out_file.open(file_name);

    if (!out_file) { cout << "Cannot open " << file_name << endl; }
    else
    {
        out_file << s;
        out_file.close();
    }
}