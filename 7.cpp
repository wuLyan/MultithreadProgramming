//线程本地存储
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void threadFunction(int id);

int i = 0;
thread_local int j = 0; //当子线程使用该变量时，会拷贝至本线程之内，不会影响其他线程与主线程

int main(void)
{
	thread t1(threadFunction, 1);
	t1.join();

	thread t2(threadFunction, 2);
	t2.join();

	cout << "i = " << i << ", j = " << j << endl;
	//主线程与各子线程之间共享内存空间中的i，但是各线程之间的j是独立的

	return 0;
}

void threadFunction(int id)
{
	cout << "Thread " << id << ", i = " << i << ", j = " << j << endl;
	++i;
	++j;
}
