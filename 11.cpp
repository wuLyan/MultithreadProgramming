#include <iostream>
#include <thread>

using namespace std;

void threadFunction(int n);

int main(void)
{
	int i = 0;

	//创建线程时，通通都是按值传递；线程跳转到线程函数时才涉及到传递地址或引用
	thread t1(threadFunction, i);
	t1.join();

	cout << "i = " << i << endl;

	return 0;
}

void threadFunction(int n)
{
	n += 100;
}
