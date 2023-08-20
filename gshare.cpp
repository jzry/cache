//////////////////////////////////////////////////////////////////////
// BRANCH PREDICTOR SIMULATOR USING GSHARE, PROJECT #2
// Prediction index for branch taken or not calculated by:
// 1. Remove 2 least significant bits from PC/Branch Address.
// 2. Take the M least significant bits from result in step 1.
// 3. Add M-N zeros to the current content of the global register to make M bits.
// 4. XOR the result to from steps 2 and 3 to get the index.
// 5. Go to the index in the global buffer table, and check if it matches with
// prediction (taken / not) and increment / decrement appropriately.
// 6. Shift global history register / buffer by one right to discard the 1st bit from the right.
// Place the outcome of the prediction / actual (1 if true, 0 if false in the GHB) in the very left most bit.
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <bitset>

using namespace std;

//////////////////////////////////////////////////////////////////////
// TO DISPLAY DEBUGGING INFORMATION, CHANGE DEBUG MACRO TO 1.
//////////////////////////////////////////////////////////////////////

#define DEBUG 0
#define STRONG_NOT_TAKE 0
#define WEAK_NOT_TAKE 1
#define WEAK_TAKE 2
#define STRONG_TAKE 3

// Keeps track of predictions for mispredictions rate.
int num_taken = 0, num_not_taken = 0, num_predict = 0, mispredict = 0;

// Global history buffer is initialized to all zeros at start of simulation.
int GHB = 0;

// Convert a hex string into long long.
long long int hexStringToLongLong(const string& hexString)
{
    stringstream converter;
    converter << hex << hexString;
    long long int result;
    converter >> result;
    return result;
}

// Check if taken or not taken and compare to actual history value. Update value accordingly.
// Update change statistics.
// If branch status is taken and value in index is 2 or 3, increment by 1 but keep at max 3.
// If branch status is taken and value in index is 0 or 1, increment by 1.
// If branch status is not taken and value in index is 2 or 3, decremement by 1.
// If branch status is not taken and value in index is 0 or 1, decremement by 1 but keep at 0 min.
int update(string branch_status, int index, vector<int> &table)
{
    // Increase the number of predictions.
    num_predict++;

    if (table[index] >= 2 && branch_status == "t")
    {
        table[index]++;
        num_taken++;

        if (table[index] > 3)
        {
            table[index] = 3;
        }
        return 1;
    }
    else if (table[index] >= 2 && branch_status == "n")
    {
        table[index]--;
        mispredict++;
        num_not_taken++;
        return 0;
    }
    else if (table[index] < 2 && branch_status == "t")
    {
        table[index]++;
        mispredict++;
        num_taken++;
        return 1;
    }
    else // table[index] < 2 && branch_status == "n"
    {
        table[index]--;
        num_not_taken++;

        if (table[index] < 0)
        {
            table[index] = 0;
        }
        return 0;
    }
}

// Convert the address from hex to long long int.
// Select the bottom M-N of the hex address.
// Use the GHB and XOR with the bottom M-N of the address.
// XOR result is converted from hex to decimal for the address in the table.
// Go to address and check if TAKEN predict matches actual branch status, increment accordingly.
// Update global num_taken, num_not_taken, num_predict vars.
// Change state of the table according to addresses taken and not taken.
void access_table(string branch, string branch_status, vector<int> &table, int M, int N)
{
    long long int PC;

    // Convert hex of branch to long long int.
    PC = hexStringToLongLong(branch);

    if (DEBUG == 1)
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "PC dec =  \t" << PC << endl;
        cout << "PC bin =  \t" << bitset<32>(PC)  << endl;
        cout << "GHB begin =  \t" << bitset<32>(GHB)  << endl;
    }

    // Remove least 2 significant bits.
    PC = PC >> 2;

    // Mask only the M least significant bits from PC (zero out everything else).
    long long int one = 1;
    long long int mask = (one << M) - one;
    long long int maskedPC = PC & mask;

    // Taking the upper N bits.
    GHB = GHB << (M-N);
    
    if (DEBUG == 1)
    {
        cout << "GHB << (M-N) =\t" << bitset<32>(GHB) << endl;
    }

    // Adding the so only the part that matches with the XOR is used.
    GHB = GHB & mask;

    if (DEBUG == 1)
    {
        cout << "GHB & mask =\t" << bitset<32>(GHB) << endl;
    }

    // Do an XOR of GHB and the last M bits of the shifted PC value.
    long long int XOR = (maskedPC ^ GHB);

    if (DEBUG == 1)
    {
        cout << "PC >> 2:\t" << bitset<32>(PC) << endl;
        cout << "mask (LSB M):\t" << bitset<32>(mask) << endl;
        cout << "maskedPC:\t" << bitset<32>(maskedPC) << endl;
        cout << "maskedPC ^ GHB:\t" << bitset<32>(XOR) << endl;
        cout << "XOR/Index:\t" << XOR << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }

    // Update the status within the table.
    int GHB_shift = update(branch_status, XOR, table);

    if (DEBUG == 1)
    {
        (GHB_shift == 1) ? cout << "Taken" << endl : cout << "Not taken" << endl;
    }

    // Shift the global history buffer over by 1.
    GHB = GHB << 1;

    // New global history buffer receives the taken / not taken value.
    GHB = GHB | GHB_shift;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "Improper syntax, use: ./SIM <GPB> <RB> <Trace_File>" << endl;
        cout << "Example: ./SIM 5 4 file.t" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        return 1;
    }

    // ./sim <GPB> <RB> <Trace_File>
    // 1) GPB = The number of PC bits used to index the gshare table.
    // 2) RB = The global history register bits used to index the gshare table.
    // 3) Trace_File = The trace file name along with its extension.

    // Switch debug to 1 to check input parameters.
    if (DEBUG == 1)
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "Arguments passed:" << endl;
        cout << "GPB (Number of PC bits to index gshare table) / M: " << argv[1] << endl;
        cout << "RB (Global History Register bits) / N: " << argv[2] << endl;
        cout << "TRACE FILE: " << argv[3] << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }

    // Convert the strings to integers.
    int M = stoi(argv[1]);
    int N = stoi(argv[2]);

    // Make the global buffer table initialized to all WEAKLY TAKEN (int 2).
    int table_size = pow(2, M);
    vector<int> table(table_size, WEAK_TAKE);

    if (DEBUG == 1)
    {
        cout << "table size: " << table_size << endl << endl;
    }

    // Reading from the file.
    if (DEBUG == 1)
    {
        cout << "ADDRESS:\t\t\t\tBRANCH STATUS:" << endl << endl;
    }
    ifstream infile(argv[3]);
    string line;

    // Keeps track of the read and write count.
    int count = 1;

    while (getline(infile, line))
    {
        istringstream iss(line);
        string address, taken;
        if (!(iss >> address >> taken)) { break; }
        
        if (DEBUG == 1)
        {
            cout << "Count: " << count++ << endl;
            cout << address << "\t\t\t\t" << taken << endl;
        }

        // Perform all necessary operations on statistics.
        access_table(address, taken, table, M, N);

        if (DEBUG == 1)
        {
            cout << endl;
        }
    }

    // Close out the file once done reading.
    infile.close();

    if (DEBUG == 1)
    {
        cout << "num_not_taken: " << num_not_taken << endl;
        cout << "num_taken: " << num_taken << endl;
        cout << "num_predict: " << num_predict << endl;
    }

    float misprediction = ((float)mispredict / (float)num_predict);

    cout << M << " " << N << " " << misprediction << endl;

    return 0;
}
