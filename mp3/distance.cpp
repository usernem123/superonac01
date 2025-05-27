#include <iostream>
using namespace std;

// Function to convert meters to kilometers and display the result
void showKilometers(double meters) {
    double kilometers = meters * 0.001;
    cout << meters << " meters is " << kilometers << " kilometers." << endl;
}

// Function to convert meters to inches and display the result
void showInches(double meters) {
    double inches = meters * 39.37;
    cout << meters << " meters is " << inches << " inches." << endl;
}

// Function to convert meters to feet and display the result
void showFeet(double meters) {
    double feet = meters * 3.281;
    cout << meters << " meters is " << feet << " feet." << endl;
}

// Function to display the menu
void showMenu() {
    cout << "\nSelect a conversion option:\n";
    cout << "1. Convert to kilometers\n";
    cout << "2. Convert to inches\n";
    cout << "3. Convert to feet\n";
    cout << "4. Quit the program\n";
}

int main() {
    double meters;
    char choice;

    // Ask the user to enter a distance in meters
    cout << "Enter a distance in meters: ";
    cin >> meters;

    // Validate the input for negative numbers
    while (meters < 0) {
        cout << "Distance cannot be negative. Please enter a valid distance in meters: ";
        cin >> meters;
    }

    do {
        // Display the menu
        showMenu();
        cout << "Enter your choice (1-4): ";
        cin >> choice;

        switch (choice) {
            case '1':
                showKilometers(meters);
                break;
            case '2':
                showInches(meters);
                break;
            case '3':
                showFeet(meters);
                break;
            case '4':
                cout << "Quitting the program." << endl;
                break;
            default:
                cout << "Invalid choice. Please select a valid option." << endl;
        }
    } while (choice != '4');

    return 0;
}