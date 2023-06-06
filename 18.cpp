//避免死锁的方法一：同时对多个互斥锁进行加锁操作，需要手动解锁或lock_guard类的析构函数自动解锁
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
	lock(mt1, mt2); //调用库函数同时对两个互斥锁进行加锁操作，需要手动解锁
	//参数adopt_lock：对未上锁的互斥锁加锁，对已上锁的互斥锁不加锁
	lock_guard<mutex> guard1(mt1, adopt_lock);  //std::adopt_lock
	lock_guard<mutex> guard2(mt2, adopt_lock);  //利用类的析构函数中的解锁操作，自动解锁
	cout << "Thread1: Shared data ---> a = " << a << endl;
	sleep(1);
	cout << "Thread1: Shared data ---> b = " << b << endl;
	cout << "Thread1: Get shared data: a + b = " << a + b << endl;
}

void thread2(void)
{
	cout << "Thread2 is running: " << endl;
	lock(mt1, mt2);
	lock_guard<mutex> guard1(mt1, adopt_lock);
	lock_guard<mutex> guard2(mt2, adopt_lock);
	cout << "Thread2: Shared data ---> b = " << b << endl;
	sleep(1);
	cout << "Thread1: Shared data ---> a = " << a << endl;
	cout << "Thread2: Get shared data: b - a = " << b - a << endl;
}
