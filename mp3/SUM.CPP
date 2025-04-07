#include <iostream>
#include <cmath> // For pow function
using namespace std;
// Function to calculate the average of two numbers
double calculateAverage(double num1, double num2) {
    return (num1 + num2) / 2.0;
}
// Function to calculate the sum of the squares of two numbers
double calculateSumOfSquares(double num1, double num2) {
    return pow(num1, 2) + pow(num2, 2);
}
// Function to calculate the square of the sums of two numbers
double calculateSquareOfSums(double num1, double num2) {
    return pow((num1 + num2), 2);
}
int main() {
    double number1, number2;
    // Ask the user for two numbers
    cout << "Enter the first number: ";
    cin >> number1;
    cout << "Enter the second number: ";
    cin >> number2;
    // Calculate average, sum of squares, and square of sums
    double average = calculateAverage(number1, number2);
    double sumOfSquares = calculateSumOfSquares(number1, number2);
    double squareOfSums = calculateSquareOfSums(number1, number2);
    // Display the results
    cout << "Average: " << average << endl;
    cout << "Sum of the squares: " << sumOfSquares << endl;
    cout << "Square of the sums: " << squareOfSums << endl;
    return 0;
}