#include <iostream>
#include <thread>

using namespace std;

void threadFunction(int n);

int main(void)
{
	int i = 0;

	//创建线程时，通通都是按值传递；线程跳转到线程函数时才涉及到传递地址或引用
	//这里如果只写i，则在创建线程时，会将i的值拷贝一份传递给线程的构造函数
	//线程的构造函数会将拷贝的值传递给线程函数，而不是传递i的地址
	thread t1(threadFunction, i);
	t1.join();

	cout << "i = " << i << endl;

	return 0;
}

void threadFunction(int n)
{
	n += 100;
}
