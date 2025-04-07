#include <iostream>
#define PI 3.14159

using namespace std;

double squarePerimeter(double side) { return 4 * side; }
double rectanglePerimeter(double length, double width) { return 2 * (length + width); }
double circlePerimeter(double radius) { return 2 * PI * radius; }

int main() {
    double side, length, width, radius;
    int choice;
    
    cout << "1. Square\n2. Rectangle\n3. Circle\nEnter choice: ";
    cin >> choice;
    
    if (choice == 1) {
        cout << "Enter side: "; cin >> side;
        cout << "Perimeter: " << squarePerimeter(side) << endl;
    } else if (choice == 2) {
        cout << "Enter length and width: "; cin >> length >> width;
        cout << "Perimeter: " << rectanglePerimeter(length, width) << endl;
    } else if (choice == 3) {
        cout << "Enter radius: "; cin >> radius;
        cout << "Perimeter: " << circlePerimeter(radius) << endl;
    } else {
        cout << "Invalid choice!" << endl;
    }
    
    return 0;
}
