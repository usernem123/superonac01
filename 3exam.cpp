#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class SariSari {
public:
    string name;
    string id;
    double price;
    double shelfLife; 
    double pcs;

    void setData(string n, string i, double p1, double s, double p2) {
        name = n;
        id = i;
        price = p1;
        shelfLife = s;
        pcs = p2;
    }

    void displayInfo() const { // Marked as const since it doesn't modify the object
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl; 
        cout << "Price: " << price << endl; 
        cout << "Shelf life: " << shelfLife << endl; 
        cout << "Pcs: " << pcs << endl; 
    }
};

void selectionSort(vector<SariSari>& products) {
    int n = products.size();
    for (int startIndex = 0; startIndex < n - 1; startIndex++) {
        int minIndex = startIndex;
        for (int position = startIndex + 1; position < n; position++) {
            if (products[position].shelfLife < products[minIndex].shelfLife || 
                (products[position].shelfLife == products[minIndex].shelfLife && 
                 products[position].name < products[minIndex].name)) {
                minIndex = position;
            }
        }
        swap(products[minIndex], products[startIndex]);
    }
}

void displayProducts(const vector<SariSari>& products) {
    for (const auto& product : products) {
        product.displayInfo();
        cout << "------------------------" << endl;
    }
}
int main() {
    vector<SariSari> products; 

    SariSari product;

    product.setData("mangoes", "10245", 20, 60, 100);
    products.push_back(product);

    product.setData("apple", "20255", 15, 38, 50);
    products.push_back(product);

    product.setData("C2red", "30356", 30, 360, 200);
    products.push_back(product);

    product.setData("bukopie", "12347", 40, 120, 150);
    products.push_back(product);

    product.setData("magic_sarap", "77878", 10, 520, 1000);
    products.push_back(product);

    product.setData("C2green", "10255", 25, 360, 300);
    products.push_back(product);

    product.setData("boy_bawang", "12032", 18, 400, 140);
    products.push_back(product);

    product.setData("banana", "11234", 7, 24, 30);
    products.push_back(product);

    string inputName;
    int quantity;
    cout << "Enter the product name: ";
    cin >> inputName;
    cout << "Enter the quantity: ";
    cin >> quantity;

    double totalPrice = 0;
    bool found = false;

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

    selectionSort(products);
    cout << "\nProducts sorted by shelf life and name:\n";
    displayProducts(products);

    return 0;   
}