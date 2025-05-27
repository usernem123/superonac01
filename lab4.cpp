
#include<iostream>
#include<stack>
using namespace std;
int main(){
stack <int> A;
A.push(5);
A.push(3);
A.push(2);
A.push(4);
A.push(7);
cout<<"Stack size: "<<A.size() <<endl;
cout<<"Contents of stack: "<<endl;
while(!A.empty()){
cout<<A.top()<<"\t";
A.pop();
}
cout<<endl;
cout<<system("pause");
}