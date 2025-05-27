#include <iostream>
#include <stack>
using namespace std;

int main() {
    stack<int> S;
    
    S.push(10245);  // mangoes
    S.push(20255);  // apple
    S.push(30356);  // C2R
    S.push(12347);  // bukopie
    S.push(77878);  // MagicSarap
    S.push(10255);  // C2G
    S.push(12032);  // BoyBawang
    S.push(11234);  // Banana

    cout << "Id of Sari Sari Store: " << S.size() << endl;

    cout << "Product Id: " << endl;
    while (!S.empty()) {
        cout << S.top() << endl;
        S.pop();
    }

    return 0;
}