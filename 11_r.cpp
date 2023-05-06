//向线程传递引用
#include <iostream>
#include <thread>

using namespace std;

void threadFunction(int &r);

int main(void)
{
	int i = 0;

	//传递引用时，必须保证引用所指向的对象在子线程结束前一直存在
	thread t1(threadFunction, ref(i));  //std::ref()：将传入的参数转换为引用
	t1.join();

	cout << "i = " << i << endl;

	return 0;
}

void threadFunction(int &r)
{
	r += 100;
	cout << "r = " << r << endl;
}
