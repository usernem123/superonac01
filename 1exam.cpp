#include <iostream>
#include <string>
#include <vector> // Include vector to store the products
using namespace std;

class SariSari {
public:
    string name;
    double price;
    double shelf_life;
    double pcs;
    string id;

    void setData(string n, double p, double s, double ps, string i) {
        name = n;
        id = i;
        price = p;
        shelf_life = s;
        pcs = ps;
    }

    void displayInfo() {
        cout << "Name: " << name << endl;
        cout << " PRICE: " << price << endl;
        cout << " SHELF LIFE: " << shelf_life << endl;
        cout << " PIECES: " << pcs << endl;
        cout << " ID: " << id << endl;
    }
};

int main() {
    // Store products in a vector
    vector<SariSari> products(8);

    products[0].setData("mangoes", 20, 60, 100, "10245");
    products[1].setData("apple", 15, 38, 50, "20255");
    products[2].setData("c2_red", 30, 360, 200, "30356");
    products[3].setData("bukopie", 40, 120, 150, "12347");
    products[4].setData("magic_sarap", 10, 520, 1000, "77878");
    products[5].setData("c2_green", 25, 360, 300, "10255");
    products[6].setData("boy_bawang", 18, 400, 140, "12032");
    products[7].setData("banana", 7, 24, 30, "11234");

    string inputName;
    int quantity;
    cout << "Enter the product name: ";
    cin >> inputName;
    cout << "Enter the quantity: ";
    cin >> quantity;

    double totalPrice = 0;
    bool found = false;

    // Loop through the vector of products
    for (const auto& p : products) {
        if (p.name == inputName) {
            totalPrice = p.price * quantity;
            found = true;
            break;
        }
    }

    if (found) {
        cout << "Total price for " << quantity << " " << inputName << "(s): " << totalPrice << endl;
    } else {
        cout << "Product not found." << endl;
    }

    return 0;
}
