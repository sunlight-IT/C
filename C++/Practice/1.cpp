#include <iostream>
using namespace std;

#define MA 220
int main(void)
{
    int long_dis;
    cout << "Please Enter a distance (long):";
    cin >> long_dis;
    cout << "long is " << long_dis * MA << " ma";
    return 0;
}