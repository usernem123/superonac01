#include <iostream>
#include <iomanip>

using namespace std;
// Function to calculate retail price
double calculateRetailPrice(double wholesaleCost, double markupPercentage) {
    return wholesaleCost + (wholesaleCost * (markupPercentage / 100));
}
int main() {
    double wholesaleCost;
    double markupPercentage;
    // Ask the user for the wholesale cost
    cout << "Enter the item's wholesale cost: ";
    cin >> wholesaleCost;
    // Ask the user for the markup percentage
    cout << "Enter the item's markup percentage: ";
    cin >> markupPercentage;

    // Calculate the retail price
    double retailPrice = calculateRetailPrice(wholesaleCost, markupPercentage);

    // Display the retail price
    cout << fixed << setprecision(2); // Set precision for output
    cout << "The item's retail price is: $" << retailPrice << endl;

    return 0;
}