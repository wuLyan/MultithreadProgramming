#include <iostream>

using namespace std;

class A
{
    public:
        A()
        {
            cout << "A()" << endl;
        }
        ~A()
        {
            cout << "~A()" << endl;
        }
        void print()
        {
            cout << "print()" << endl;
        }
};

int main()
{
    int n = 3;
    while(n --)
    {
        A a;
        //cout << "Hello World!" << endl;
        cout << "------------" << endl;
    }
    //输出结果说明每轮循环结束后，对象a都被析构了
    return 0;
}