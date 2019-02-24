/// @author Rocky Gonzalez
/// @note da1b_arrays
/// @file da1b_arrays.cpp
/// @version 2019-02-23
/// @brief The program is used to verify values for DA1B.

#include <iostream>

using namespace std;

int main() {

    unsigned int array_x[100];  // Create an array to store X-Pointer Values
    unsigned int array_y[100];  // Create an array to store Y-pointer Values
    unsigned int array_z[100];  // Create an array to store Z-Pointer Values

    unsigned char store = 0;    // Values that range from "0-255" (Byte)
    unsigned char i;            // Iteration for Array X
    int total_y = 0;			// Total Value of Y Array
    int total_z = 0;			// Total Value of Z Array

    cout << "Array X" << endl;  // Display "Array X" into Monitor
    for (i = 0; i < 99; i++) {  // Loop and Store Values of "7" greater than 10
        if (store <= 10) {
            store = store + 7;  // If not greater than 10, add 7 more
        }
        if (store <= 10) {
            store = store + 7;  // If still not greater than 10, add 7 more
        }
        array_x[i] = store;         // Store Values into Array X
        cout << array_x[i] << " ";  // Display into Terminal
        store = store + 7;          // Increment "7"
    }
    cout << endl << endl;			// Line Break

    unsigned char j = 0;            // Iteration for Array Y
    unsigned char k = 0;            // Iteration for Array Z

    cout << "Array Y" << endl;      // Display "Array Y" into Monitor
    for (i = 0; i < 99; i++) {		// Loop X Data
        if(array_x[i]%3 == 0) {     // If divisible by 3, store it into Array Y and Display
            array_y[j] = array_x[i];		// Store into Y Data
            total_y = total_y + array_x[i];	// Add Value (total)
            j = j++;						// Increment Y Array
            cout << array_y[j] << " ";		// Create List of Y Values
        }
    }
    cout << endl << endl;

    cout << "Array Z" << endl;      // Display "Array Z" into Monitor
    for (i = 0; i < 99; i++) {		// Loop X Data
        if(array_x[i]%3 != 0) {     // If not divisible by 3, Store in Array Z and Display
            array_z[k] = array_x[i];		// Store into Z Data
            total_z = total_z + array_x[i];	// Add Value (total)
            k = k++;						// Increment Z Array
            cout << array_z[k] << " ";		// Create List of Z Values
        }
    }
    cout << endl << endl;							// Line Break
    cout << "Total Y: " << total_y << endl << endl;	// Totality of Y Values
    cout << "Total Z: " << total_z << endl << endl;	// Totality of Z Values
}
