//避免死锁的方法二：使用unique_lock模板类
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
	//lock(mt1, mt2);
	//lock_guard<mutex> guard1(mt1, adopt_lock);  //std::adopt_lock
	//lock_guard<mutex> guard2(mt2, adopt_lock);
	unique_lock<mutex> guard1(mt1, defer_lock);
	unique_lock<mutex> guard2(mt2, defer_lock);
	//guard1.lock();
	//guard2.lock(); //当两个线程不是同时对两个互斥锁进行加锁操作时，又会造成死锁现象
	//调用库函数lock()，而不是成员函数lock()
	lock(guard1, guard2); //同时对unique_lock类的对象guard1、guard2进行加锁操作，无需手动解锁
	//lock(mt1, mt2); //错误，如果单独对互斥量进行加锁，最后还需要手动解锁，否则又会造成死锁现象
	cout << "Thread1: Shared data ---> a = " << a << endl;
	sleep(1);
	cout << "Thread1: Shared data ---> b = " << b << endl;
	cout << "Thread1: Get shared data: a + b = " << a + b << endl;
}

void thread2(void)
{
	cout << "Thread2 is running: " << endl;
	//lock(mt1, mt2);
	//lock_guard<mutex> guard1(mt1, adopt_lock);
	//lock_guard<mutex> guard2(mt2, adopt_lock);
	unique_lock<mutex> guard1(mt1, defer_lock);
	unique_lock<mutex> guard2(mt2, defer_lock);
	lock(guard1, guard2);
	cout << "Thread2: Shared data ---> b = " << b << endl;
	sleep(1);
	cout << "Thread2: Shared data ---> a = " << a << endl;
	cout << "Thread2: Get shared data: b - a = " << b - a << endl;
}
