#include<iomanip>
#include <execution>
#include "Timer.h"

import Pat_module;

//RESULTS of various testing done during development.

//Accuracy vs Time, Single CPU Thread:
//74 Seconds for 10 Hill Climb on my basline C++ 20 code. Single Thread
//73-74 Seconds Removed B==A while loop.
//50-57 Seconds Set outer loop to 1000 and inner loop to 500.
//11 Seconds Byte Array Implemented. (around 92-95% Accuracy)
//Alternate Scoring Table 8 Seconds at 51.87% Accuracy. Not Ideal

//Single Thread Testing:
//1200 Outer Loop, 600 Inner Loop ~95% Accuracy.
//RESULTS:
//7 Climbs Single Thread = 9,943 ms
//8 Climbs Single Thread = 11,390 ms
//9 Climbs Single Thread = 13,120 ms
//10 Climbs Single Thread = 14,238 ms
//100 Climbs Single Thread = 144,419 ms

//Testing multiple thread counts:
//7 Climbs par_unseq Policy = 1,999 ms (4.97x, 20.1%)
//8 Climbs par_unseq Policy = 2,018 ms (5.64x, 17.72%)
//9 Climbs par_unseq Policy = 2,107 ms (6.23x, 16.06%)
//10 Climbs par_unseq Policy = 2,257 ms (6.3x, 15.85%)
//100 Climbs par_unseq Policy = 19,746 ms (7.31x, 13.67%)

using namespace std;

int main()
{
    //Load "Risties" from ACA provided digital format.
    vector<string> risties = process_digital_risties();
    vector<string> pats = convert_aristo_to_pat(risties);
    
    //Detect if Aristocrat or Patristocrat
    bool is_patristocrat = detect_patristocrats(risties[1]);
        
    //Array to hold hill climber outputs for evaluation after.
    vector<string> decrypts{pats};
    
    Timer t; //For timing how long it takes for 10 hill climbs.
    t.start();//Starts timer just before the hill climbs start.
        
    //This Lambda expression is a needed wrapper.
    auto lam = [&](string& s)->string{ return hill_climb(s); };
    
    //Hill climbs a batch of patristocrats concurrently on CPU.
    transform(execution::par_unseq, pats.begin(), pats.end(), decrypts.begin(), lam);
        
    t.stop('m');//Stops timer after climbs all finish.

    //Displays decryption results.
    for (int i= 0; i < pats.size(); i++)
    {
        if (is_patristocrat) { cout << risties[i] << endl << decrypts[i] << endl << endl; }
        else
        {
            cout << risties[i] << endl << space_decrypted_aristo(risties[i], decrypts[i]) << endl << endl;
        }
    }
        
    return 0;
}