#include <iostream>
#include <stdlib.h>
using namespace std;
char x[100];
char y;
int z;

int main()
{ 
cout<<"Enter a name: ";
cin>>x;
cout<<"You entered "<<x<<endl;
cout<<endl<<"Enter letter to search: ";
cin>>y;
for(z=0; x[z] != '\0'; z++)
{
    if (y==x[z])
        cout<<y<<" is located at "<<"array["<<z<<"]"<<endl;
}
return 0;
}

