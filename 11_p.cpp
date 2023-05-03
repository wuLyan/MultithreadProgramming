//向线程传递地址
#include <iostream>
#include <thread>

using namespace std;

void threadFunction(int *pt);

int main(void)
{
	int i = 0; //只存在于主线程作用域的局部变量

	//变量i的地址首先被传递给线程构造函数创建线程
	//然后线程函数通过指针间接修改变量i的值
	thread t1(threadFunction, &i); //通过函数指针创建线程
	t1.join();

	cout << "i = " << i << endl;

	return 0;
}

void threadFunction(int *pt)
{
	*pt += 100;
}
