#include <iostream>
using namespace std;

int main() {
    int height = 19; // Height of each roof
    int baseHeight = 29; // Height of the base (walls)
    int width = 29;  // Width of the base (should be odd for symmetry)
    int doorHeight = 3; // Height of the door
    int doorWidth = 5;  // Width of the door
    int windowHeight = 2; // Height of the windows
    int windowWidth = 3;  // Width of the windows
    // Draw the roof
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (j >= (width / 2 - i) && j <= (width / 2 + i)) {
                cout << "^";
            } else {
                cout << " ";
            }
        }
    cout << endl;
}

    // Draw the base of the mansion
    for (int i = 1; i <= baseHeight; ++i) {
        for (int j = 1; j <= width; ++j) {
            // Draw the door
            if (i >= baseHeight - doorHeight + 1 && j >= (width - doorWidth) / 2 && j < (width + doorWidth) / 2) {
                if (i == baseHeight) {
                    cout << "="; // Bottom of the door
                } else {
                    cout << " "; // Leave space for the door
                }
            } else if (i >= 2 && i < 2 + windowHeight && 
                       ((j >= (width / 4) + 1 && j < (width / 4) + 1 + windowWidth) || 
                        (j >= (3 * width / 4) + 1 && j < (3 * width / 4) + 1 + windowWidth))) {
                cout << "o"; // Windows
            } else if (i >= baseHeight - doorHeight + 1 && j == width / 2) {
                cout << "|"; // Elevator shaft
            } else {
                cout << "*"; // Print the walls of the mansion
            }
        }
        cout << endl;
    }
    for (int i = 1; i <= baseHeight; ++i) {
        for (int j = 1; j <= width; ++j) {
            // Draw the door
            if (i >= baseHeight - doorHeight + 1 && j >= (width - doorWidth) / 2 && j < (width + doorWidth) / 2) {
                if (i == baseHeight) {
                    cout << "="; // Bottom of the door
                } else {
                    cout << " "; // Leave space for the door
                }
            } else if (i >= 2 && i < 2 + windowHeight && 
                       ((j >= (width / 4) + 1 && j < (width / 4) + 1 + windowWidth) || 
                        (j >= (3 * width / 4) + 1 && j < (3 * width / 4) + 1 + windowWidth))) {
                cout << "o"; // Windows
            } else {
                cout << "*"; // Print the walls of the mansion
            }
    }
    cout << endl;
}
    // Draw the family
    cout << "   O   O   O   O   " << endl; // Heads of the family members
    cout << "  /|\\ /|\\ /|\\ /|\\ " << endl; // Bodies of the family members
    cout << "  / \\ / \\ / \\ / \\ " << endl; // Legs of the family members
    // Draw the ground
    for (int j = 1; j <= width; ++j) {
        cout << "="; // Ground line
    }
    cout << endl;

    // Draw the tree
    cout << "        ^  " << endl; // Top of the tree
    cout << "       ^^^ " << endl; // Leaves of the tree
    cout << "      ^^^^^" << endl; // Leaves of the tree
    cout << "     ^^^^^^^" << endl; // Leaves of the tree
    cout << "    ^^^^^^^^^" << endl; // Leaves of the tree
    cout << "    ^^^^^^^^^^" << endl; // Leaves of the tree
    cout << "   ^^^^^^^^^^^^" << endl; // Leaves of the tree
    cout << "  ^^^^^^^^^^^^^^" << endl; // Leaves of the tree
    cout << " ^^^^^^^^^^^^^^^^" << endl; // Leaves of the tree
    cout << "^^^^^^^^^^^^^^^^^^" << endl; // Leaves of the tree
    cout << "       |||  " << endl; // Trunk of the tree
    cout << "       |||  " << endl; // Trunk of the tree
    cout << "       |||  " << endl; // Trunk of the tree
    cout << "       |||  " << endl; // Trunk of the tree

    // Draw plants next to the mansion
    cout << "   *  *  *  *  *  " << endl; // Plants
    cout << "  *  *  *  *  * * " << endl; // Plants
    cout << " *  *  *  *  *  * " << endl; // Plants

    return 0;
}
