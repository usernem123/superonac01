#include<iostream>
using namespace std;
int main()
{
    int FArray[10], SArray[10], AMerge[20];
	int n, m, x,y;
    cout<<"Enter the Size of First Array: ";
    cin>>n;
    if (n > 10) {
        cout << "Size of First Array should not exceed 10." << endl;
        return 1;
    }
    cout<<"Enter "<<n<<" Elements for First Array: "<<endl;
    for(x=0; x<n; x++)
    {
        cin>>FArray[x];
        AMerge[x] = FArray[x];
    }
    y = x;
    cout<<endl<<"Enter the Size of Second Array: ";
    cin>>m;
    if (m > 10) {
        cout << "Size of Second Array should not exceed 10." << endl;
        return 1;
    }
    cout<<"Enter "<<m<<" Elements for Second Array: "<<endl;
    for(x=0; x<m; x++)
    {
        cin>>SArray[x];
        AMerge[y] = SArray[x];
        y++;
    }
    cout<<"\nMerged Array :"<<endl;
    for(x=0; x<y; x++)
        cout<<AMerge[x]<<" ";
    cout<<endl; 
}
