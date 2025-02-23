#include<iomanip>
#include "Timer.h"

import Pat_module;

//RESULTS: (Accuracy vs Time, Single CPU Thread)
//74 Seconds for 10 Hill Climb on my basline C++ 20 code. Single Thread
//73-74 Seconds Removed B==A while loop.
//50-57 Seconds Set outer loop to 1000 and inner loop to 500.
//11 Seconds Byte Array Implemented. (around 92-95% Accuracy)
//Alternate Scoring Table 8 Seconds at 51.87% Accuracy. Not Ideal

//Single Thread Testing
//1200 Outer Loop, 600 Inner Loop ~95% Accuracy.
//RESULTS:
//7 Climbs Single Thread = 9,943 ms
//8 Climbs Single Thread = 11,390 ms
//9 Climbs Single Thread = 13,120 ms
//10 Climbs Single Thread = 14,238 ms
//100 Climbs Single Thread = 144,419 ms

//7 Climbs par_unseq Policy = 1,999 ms (4.97x, 20.1%)
//8 Climbs par_unseq Policy = 2,018 ms (5.64x, 17.72%)
//9 Climbs par_unseq Policy = 2,107 ms (6.23x, 16.06%)
//10 Climbs par_unseq Policy = 2,257 ms (6.3x, 15.85%)
//100 Climbs par_unseq Policy = 19,746 ms (7.31x, 13.67%)



using namespace std;

int main()
{
    //Encrypts the plain text to create a Patristocrat for testing.
    Patristocrat p;
    p.plain_text = "THEREISNOTHINGONTHISEARTHMORETOBEPRIZEDTHENATRUEFRIENDSAIDTHEPHILOSOPHERANDPRIESTTHOMASAQUINAS";
    string keyword = "FRESHAPPLESAUCE";
    string enc_key = make_keyed_alphabet(keyword);
    p.encrypt(enc_key);
    p.ct_length = p.cipher_text.size();
    
    //Converts the letters of the cipher text to numbers for perfomance purposes.
    p.encode();
    
    //Array to hold hill climber outputs for evaluation after.
    vector<array<int, 100>> decrypts(10, p.encoded_ct);
    
    Timer t; //For timing how long it takes for 10 hill climbs.
    t.start();//Starts timer just before the hill climbs start.

    //Hill Climbs the Patristocrats on a single thread.
    //for (auto& x : decrypts) { x = p.hill_climb(x, p.ct_length); }
    
    //Hill Climbs the Patristocrats on multiple-threads.
    p.batch_climb_CPU(decrypts);
    
    t.stop('m');//Stops timer after climbs all finish.

    //Evaluates how accurate the decrypts are.
    float possible_points = p.ct_length * decrypts.size() * 1.0;
    float actual_points = 0.0;

    //Converts the letters of the plain text to numbers for perfomance purposes.
    array<int, 100> encoded_pt{ 0 };
    for (int i = 0; i < p.ct_length; i++) { encoded_pt[i] = p.plain_text[i] - 'A'; }
    
    //Evaluates how accurate the hill climbs were.
    for (auto& decrypt:decrypts)
    {
        for (int i = 0; i < p.ct_length; i++) 
        {
            if (decrypt[i] == encoded_pt[i]) { actual_points++; }
        }
    }

    float percentage_correct = actual_points * 100.0 / possible_points;

    //Displays time elapsed and accuracy% of decrypts.
    cout << setprecision(4) <<  percentage_correct << "% Accuracy" << endl;

    //Keeps Window Open.
    //hold_window();
    
    return 0;
}