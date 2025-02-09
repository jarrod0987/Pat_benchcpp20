#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <numeric>
#include <map>
#include <algorithm>
#include <utility>

export module Crypto;

using namespace std;

//Returns Index of Coincidence. (Must be only capital A-Z.)
export double get_ic(string text)
{
    array<int, 26> freqs{ 0 };
    int phi_observed = 0;
    for (char c : text) { freqs[c - 'A']++; }
    for (int i = 0; i < 26; i++) { phi_observed += freqs[i] * (freqs[i] - 1); }
    double phi_random = 0.0385 * text.size() * (text.size() - 1);
    return phi_observed / phi_random;
}

export int get_period(string text)
{
    auto get_avg_ic = [&](int test_period)
        {
            vector<string> columns(test_period);
            vector<double> column_ics(test_period);

            int index = 0;
            for (char& c : text)
            {
                columns.at(index) += c;
                index = (index + 1) % test_period;
            }

            for (string& column : columns) { column_ics.push_back(get_ic(column)); }
            double average_ic = accumulate(column_ics.begin(), column_ics.end(), 0.0) / test_period;
            return average_ic;
        };

    double best_average_ic = 0.0;

    for (int i = 1; i < 15; i++)
    {
        double average_ic = get_avg_ic(i);
        cout << setprecision(2) << "Period of " << i << ":\tavg. ioc is " << average_ic << endl;
    }

    int choice = 0;
    while (choice < 1 or choice > 14)
    {
        if (choice != 0) { cout << "Bad Input\n"; }
        cout << "\nChoose a period to test(1-14):\n";
        cin >> choice;
    }

    cout << "You chose a period of: " << choice << endl;
    return choice;
}

export vector<string> make_columns(string text, int n_columns)
{
    vector<string> columns(n_columns);
    int index = 0;
    for (auto& c : text)
    {
        columns.at(index).push_back(c);
        index = (index + 1) % n_columns;
    }
    return columns;
}

export string combine_columns(vector<string>& columns)
{
    string output;
    int width = columns.size();
    int shorter_length = columns.at(width - 1).size();

    //Writes the output string and handles columns of different lengths.
    for (int i = 0; i < shorter_length; i++)
    {
        for (auto column : columns) { output += column.at(i); }
    }

    for (auto column : columns)
    {
        if (column.size() > shorter_length) { output += column.at(shorter_length); }
    }
    return output;
}

export void guess_letters(string& alphabet)//Tries to fill plain alphabet sections of a keyed alphabet.
{
    string pt_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<pair<char, char>> letter_pairs;
    
    //Finds pairs of letters that are likely begining and end of a plain alphabet key segment.
    for (auto& first_letter : alphabet)
    {
        if (first_letter != ' ')
        {
            for (auto& second_letter : alphabet)
            {
                if (second_letter != ' ' and
                    alphabet.find(second_letter) > first_letter - 'A' and
                    second_letter - 'A' > first_letter - 'A')
                {
                    letter_pairs.push_back(make_pair(first_letter, second_letter));
                    break;
                }
            }
            continue;
        }
    }

    //Populates the plain alphabet letters that would run in between those pairs.
    for (auto& pair : letter_pairs)
    {
        int d = alphabet.find(pair.second) - alphabet.find(pair.first);
        if (d == pt_alphabet.find(pair.second) - pt_alphabet.find(pair.first))
        {
            int start = alphabet.find(pair.first);

            for (int i = start; i < start + d; i++)
            {
                alphabet.at(i) = pair.first + (i - start);
            }
        }
    }
}

export string get_word_pattern(string& s)
{
    string output_pattern;
    map<char, char> m;
    char index = 'a';

    for (auto& c : s)
    {
        if (m.find(c) == m.end())//Not Found
        {
            m.insert(make_pair(c, index));
            index++;
        }
        output_pattern += m.at(c);
    }
    return output_pattern;
}

export string get_word_pattern(string_view s)
{
    string output_pattern;
    map<char, char> m;
    char index = 'a';

    for (auto& c : s)
    {
        if (m.find(c) == m.end())//Not Found
        {
            m.insert(make_pair(c, index));
            index++;
        }
        output_pattern += m.at(c);
    }
    return output_pattern;
}

export bool word_pattern_match(string& a, string& b)
{
    if (get_word_pattern(a) == get_word_pattern(b)) { return true; }
    return false;
}

export  string make_non_conflicting_alphabet(string& pt_letters, string& ct_letters)
{
    if (pt_letters.size() not_eq ct_letters.size()) { return "PT list size not equal to CT list size!"; }
    
    string selected_pt_letters, selected_ct_letters;
    
    for (int i = 0; i < pt_letters.size(); i++)
    {
        //Select this letter pair.
        if (selected_pt_letters.find(pt_letters.at(i)) == selected_ct_letters.find(ct_letters.at(i)))
        {
            if (selected_pt_letters.find(pt_letters.at(i)) == -1)
            {
                selected_pt_letters += pt_letters.at(i);
                selected_ct_letters += ct_letters.at(i);
            }
            else { continue; }
        }
        //Pair is a conflict with an existing pair.
        else { return "Failed"; }
    }

    string alphabet = "                          ";

    for (int i = 0; i < selected_pt_letters.size(); i++)
    {
        alphabet.at(selected_pt_letters.at(i) - 'A') = selected_ct_letters.at(i);
    }
    
    return { alphabet };
}

export string get_missing_letters(string partial_alphabet)
{
    string missing_letters;
    for (char c = 'A'; c <= 'Z'; c++)
    {
        if (partial_alphabet.find(c) == -1) { missing_letters += c; }
    }
    return missing_letters;
}

export vector<string> anagram_string(string& s)
{
    vector<string> output;
    sort(s.begin(), s.end());
    do { output.push_back(s); }
    while (next_permutation(s.begin(), s.end()));
    return output;
}

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

export bool periodic_can_place(string& cipher_text, string& crib, int period, int starting_index)
{
    vector<string> ct_columns(period);
    int index(starting_index);
    for (auto& c : cipher_text)
    {
        ct_columns.at(index) += c;
        index = (index + 1) % period;
    }
    string ct_pattern;
    for (auto& column : ct_columns) { ct_pattern += get_word_pattern(column); }

    index = starting_index;
    vector<string> crib_columns(period);
    for (auto& c : crib)
    {
        crib_columns.at(index) += c;
        index = (index + 1) % period;
    }
    string crib_pattern;
    for (auto& column : crib_columns) { crib_pattern += get_word_pattern(column); }

    return ct_pattern == crib_pattern;
}

export vector<string>merge_alphabets(const vector<string> tableau_in)
{
    vector<string> failed{"FAILED"};
    vector<string> old_tableau(tableau_in);
    
    auto new_tablaeu(old_tableau);
    do
    {
        vector<string> new_tablaeu(old_tableau);
        for (auto& c : "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        {
            for (int i = 0; i < tableau_in.size() - 1; i++)
            {
                if (new_tablaeu.at(i).find(c) != string::npos)
                {
                    for (int j = i + 1; j < tableau_in.size(); j++)
                    {
                        if (new_tablaeu.at(j).find(c) != string::npos)
                        {
                            //Merge the 2 alphabets here.
                            int home_letter_position_a = new_tablaeu.at(i).find(c);
                            int home_letter_position_b = new_tablaeu.at(j).find(c);

                            //Send alphabet a letters to alphabet b
                            for (int k = 1; k < 26; k++)
                            {
                                int x = (home_letter_position_a + k) % 26;
                                if (new_tablaeu.at(i).at(x) != ' ')
                                {
                                    char temp = new_tablaeu.at(i).at(x);
                                    int y = (home_letter_position_b + k) % 26;

                                    //If alphabets have a conflict, return fail.
                                    if (new_tablaeu.at(j).at(y) != temp and new_tablaeu.at(j).at(y) != ' ')
                                    {
                                        return failed;
                                    }
                                    //If no conflic then place the letter.
                                    else { new_tablaeu.at(j).at(y) = temp; }
                                }
                            }

                            //Send alphabet b letters to alphabet a
                            for (int k = 1; k < 26; k++)
                            {
                                int x = (home_letter_position_b + k) % 26;
                                if (new_tablaeu.at(j).at(x) != ' ')
                                {
                                    char temp = new_tablaeu.at(j).at(x);
                                    int y = (home_letter_position_a + k) % 26;

                                    //If alphabets have a conflict, return fail.
                                    if (new_tablaeu.at(i).at(y) != temp and new_tablaeu.at(i).at(y) != ' ')
                                    {
                                        return failed;
                                    }
                                    //If no conflic then place the letter.
                                    else { new_tablaeu.at(i).at(y) = temp; }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (new_tablaeu != old_tableau) { old_tableau = new_tablaeu; }
       
    } while (new_tablaeu == old_tableau);
    return(old_tableau);
}

export bool all_alphabets_merged(vector<string> alphabets)
{
    vector<int> counts;
    for (auto& a : alphabets)
    {
        int count = 0;
        for (auto& c : a)
        {
            if (c not_eq ' ') { count++; }
        }
        counts.push_back(count);
    }
    return adjacent_find(counts.begin(), counts.end(), not_equal_to<int>()) == counts.end();
}

export vector<pair<int, int>>find_mirrored_alphabets(vector<string>& tableau)
{
    vector<pair<int, int>> output;
    for (int i = 0; i < tableau.size() - 1; i++)
    {
        for (int j = i + 1; j < tableau.size(); j++)
        {
            for (int k = 0; k < 26; k++)
            {
                if (
                    (tableau.at(i).at(k) not_eq ' ' or
                        tableau.at(j).at(k) not_eq ' ') and
                    (tableau.at(i).at(k) == tableau.at(j).at(k))
                    )
                {
                    output.push_back(make_pair(i, j));
                    break;
                }
            }
        }
    }
    return output;
}

export void fill_mirrored_alphabets(vector<string>& tableau, vector<pair<int, int>>& matched_alphabets)
{
    for (auto& match:matched_alphabets)
    {
        for (int i = 0; i < 26; i++)
        {
            if (tableau.at(match.first).at(i) == ' ' and tableau.at(match.second).at(i) != ' ')
            {
                tableau.at(match.first).at(i) = tableau.at(match.second).at(i);
            }
            else if (tableau.at(match.second).at(i) == ' ' and tableau.at(match.first).at(i) != ' ')
            {
                tableau.at(match.second).at(i) = tableau.at(match.first).at(i);
            }
        }
    }
}

export  vector<pair<int, int>>find_recipricol_alphabets(vector<string>& tableau)
{
    vector<pair<int, int>> output;
    for (int i = 0; i < tableau.size() - 1; i++)
    {
        for (int j = i + 1; j < tableau.size(); j++)
        {
           //Verifies if there are at least 2 matching letters in each alphabet under test.
            string combined_alphabets(tableau.at(i));
            combined_alphabets += tableau.at(j);
            string common_letters;
            for (char letter = 'A'; letter <= 'Z'; letter++)
            {
                int num = count(combined_alphabets.begin(), combined_alphabets.end(), letter);
                if (num > 1) { common_letters.push_back(letter); }
            }
            if (common_letters.size() == 0) { continue; }
            
            //Checks if first letter that was in both alphabets is a recipricol pair.
            //If it is, all the pairs must be.
            int a = tableau.at(i).find(common_letters.at(0));
            char letter_a = tableau.at(j).at(a);
            if (letter_a == ' ') { continue; }
            int b = tableau.at(i).find(letter_a);
            if (a == b or b == -1) { continue; }//Filters out mirror alphabets.
            char letter_b = tableau.at(j).at(b);
            if (letter_b == ' ') { continue; }

            if (letter_b == common_letters.at(0))
            {
                output.push_back(make_pair(i, j));
            }
            else { continue; }
        }
    }
    return output;
}

export void fill_recipricol_alphabets(vector<string>& tableau, vector<pair<int, int>>& recipricol_alphabets)
{
    for (auto& match : recipricol_alphabets)
    {
        for (int i = 0; i < 26; i++)
        {
            if (tableau.at(match.first).at(i) == ' ' and tableau.at(match.second).at(i) != ' ')
            {
                int x = tableau.at(match.first).find(tableau.at(match.second).at(i));
                if (x == -1) { continue; }//Ignore if letter not discovered in other alphabet.
                else { tableau.at(match.first).at(i) = tableau.at(match.second).at(x); }
            }
            else if (tableau.at(match.second).at(i) == ' ' and tableau.at(match.first).at(i) != ' ')
            {
                int x = tableau.at(match.second).find(tableau.at(match.first).at(i));
                if (x == -1) { continue; } //Ignore if letter not discovered in other alphabet.
                else { tableau.at(match.second).at(i) = tableau.at(match.first).at(x); }
            }
        }
    }
}

export void merge_redundant_strings(vector<string>& strings_to_merge)
{
    int old_size = strings_to_merge.size();
    int new_size(old_size);

    do
    {
        new_size = old_size;
        for (int i = 0; i < strings_to_merge.size() - 1; i++)
        {
            for (int j = i + 1; j < strings_to_merge.size(); j++)
            {
                if (strings_to_merge.at(i).find(strings_to_merge.at(j)) != string::npos)
                {
                    auto it = find(strings_to_merge.begin(), strings_to_merge.end(), strings_to_merge.at(j));
                    strings_to_merge.erase(it);
                }
            }
        }

        if (strings_to_merge.size() < new_size) { old_size = strings_to_merge.size(); }

    } while (old_size != new_size);
}