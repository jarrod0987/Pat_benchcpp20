#include<iostream>
#include<vector>
#include<iomanip>
#include<windows.h>
#include "Timer.h"

import Patristocrat;
import Crypto;

using namespace std;

int main()
{
    //Encrypts the plain text to create a Patristocrat for testing.
    string pt = "THEREISNOTHINGONTHISEARTHMORETOBEPRIZEDTHENATRUEFRIENDSAIDTHEPHILOSOPHERANDPRIESTTHOMASAQUINAS";
    string keyword = "FRESHAPPLESAUCE";
    string enc_key = make_keyed_alphabet(keyword);
    string ct = encrypt(pt, enc_key);
    vector<string> decrypts;
    
    Timer t; //For timing how long it takes for 10 hill climbs.
    t.start();

    //Hill Climbs the Patristocrat 10 times.
    for (int i = 0; i < 10; i++) { decrypts.push_back(hill_climb(ct)); }
    
    t.stop('s');


    //Evaluates how accurate the decrypts are.
    float possible_points = pt.size() * 10.0;
    int actual_points = 0;
    
    for (auto& decrypt:decrypts)
    {
        for (int i = 0; i < pt.size(); i++) 
        {
            if (decrypt[i] == pt[i]) { actual_points++; }
        }
    }

    float percentage_correct = actual_points * 100.0 / possible_points;

    //Displays time elapsed and accuracy% of decrypts.
    cout << setprecision(2) <<  percentage_correct << endl;

    ////Keeps Window Open.
    //cout << "Press E to EXIT..." << endl;
    //bool end = false;
    //while (end == false)
    //{
    //    if (GetAsyncKeyState('E') & 0x8000)
    //    {
    //        end = true;
    //    }
    //}

    return 0;
}

//79-80 Seconds for 10 Hill Climb on my basline C++ 20 code. Single Thread

