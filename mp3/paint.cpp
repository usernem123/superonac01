#include <iostream>
#include <cmath> // For ceil function

using namespace std;

// Constants
const int SQUARE_FEET_PER_GALLON = 115;
const double LABOR_HOURS_PER_GALLON = 8.0;
const double LABOR_COST_PER_HOUR = 120.00;

// Function to calculate the number of gallons of paint required
double calculateGallonsOfPaint(double totalSquareFeet) {
    return ceil(totalSquareFeet / SQUARE_FEET_PER_GALLON);
}

// Function to calculate the hours of labor required
double calculateLaborHours(double gallons) {
    return gallons * LABOR_HOURS_PER_GALLON;
}

// Function to calculate the cost of the paint
double calculateCostOfPaint(double gallons, double pricePerGallon) {
    return gallons * pricePerGallon;
}

// Function to calculate the labor charges
double calculateLaborCharges(double hours) {
    return hours * LABOR_COST_PER_HOUR;
}

// Function to calculate the total cost of the paint job
double calculateTotalCost(double paintCost, double laborCost) {
    return paintCost + laborCost;
}

int main() {
    int numberOfRooms;
    double pricePerGallon;

    // Ask the user for the number of rooms and price of paint per gallon
    cout << "Enter the number of rooms to be painted: ";
    cin >> numberOfRooms;
    cout << "Enter the price of the paint per gallon: ";
    cin >> pricePerGallon;

    double totalSquareFeet = 0.0;

    // Get the square feet of wall space for each room
    for (int i = 1; i <= numberOfRooms; ++i) {
        double squareFeet;
        cout << "Enter the square feet of wall space for room " << i << ": ";
        cin >> squareFeet;
        totalSquareFeet += squareFeet;
    }

    // Calculate required data
    double gallonsRequired = calculateGallonsOfPaint(totalSquareFeet);
    double laborHoursRequired = calculateLaborHours(gallonsRequired);
    double costOfPaint = calculateCostOfPaint(gallonsRequired, pricePerGallon);
    double laborCharges = calculateLaborCharges(laborHoursRequired);
    double totalCost = calculateTotalCost(costOfPaint, laborCharges);

    // Display the results
    cout << fixed; // Set fixed-point notation
    cout.precision(2); // Set precision to 2 decimal places
    cout << "\n--- Painting Job Summary ---" << endl;
    cout << "Gallons of paint required: " << gallonsRequired << endl;
    cout << "Hours of labor required: " << laborHoursRequired << endl;
    cout << "Cost of the paint: P" << costOfPaint << endl;
    cout << "Labor charges: P" << laborCharges << endl;
    cout << "Total cost of the paint job: P" << totalCost << endl;

    return 0;
}