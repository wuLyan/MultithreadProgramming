//避免死锁的方法三：使用scoped_lock模板类(C++17标准)
//编译方法：g++ 21.cpp -std=c++17
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;

void thread1(void);
void thread2(void);

mutex mt1;
mutex mt2;

int a = 100;
int b = 200;

int main(void)
{
	thread t1(thread1);
	thread t2(thread2);

	t1.join();
	t2.join();

	cout << "All threads end." << endl;
}

void thread1(void)
{
	cout << "Thread1 is running: " << endl;
	//scoped_lock<mutex, mutex> guard(mt1, mt2);
	scoped_lock guard(mt1, mt2); //C++17新特性，可以省略模板参数，由编译器自动推断参数种类及个数
	cout << "Thread1: Shared data ---> a = " << a << endl;
	sleep(1);
	cout << "Thread1: Shared data ---> b = " << b << endl;

	cout << "Thread1: Get shared data: a + b = " << a + b << endl;
}

void thread2(void)
{
	cout << "Thread2 is running: " << endl;
	//scoped_lock<mutex, mutex> guard(mt1, mt2);
	scoped_lock guard(mt1, mt2);
	cout << "Thread2: Shared data ---> b = " << b << endl;
	sleep(1);
	cout << "Thread2: Shared data ---> a = " << a << endl;

	cout << "Thread2: Get shared data: a + b = " << a + b << endl;
}
