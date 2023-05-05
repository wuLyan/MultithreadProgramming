//线程并发同步：条件变量condition_variable(mutex)、condition_variable_any(任何互斥量)、future、promise
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

using namespace std;

mutex mtx;
condition_variable flag; //条件变量本质上是类的对象，用于线程间的条件同步
//用于下面调用成员函数notify_one、notify_all、wait

int count = -1;

void thread1(void);
void thread2(void);
void thread3(void);

int main(void)
{
	thread t1(thread1);
	thread t2(thread2);
	thread t3(thread3);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}

void thread1(void)
{
	int i = 0;
	sleep(5);

	while(1)
	{
/*      //专门新开一个作用域，保证lock_guard模板类的对象在发出信号前都会被析构
		从而解锁互斥量，以便线程t2可以为互斥量解锁继续执行
		{
			lock_guard<mutex> lck(mtx);
			count = i++;
		}
*/
		unique_lock<mutex> lck(mtx);
		count = i++;
		lck.unlock(); //手动进行解锁mtx

		//flag.notify_one(); //当使用notify_one时只能通知一个线程，另外两个线程只能有一个线程被唤醒
		flag.notify_all(); //当使用notify_all时可以通知所有线程，所有线程都会被唤醒
		sleep(1);
	}
}

void thread2(void)
{
	while(1)
	{
		//因为lock_guard模板类的对象无法被暂时解锁，所以改用unique_lock模板类
		unique_lock<mutex> lck(mtx);
		cout << "Thread2 waits for count: " << endl;
		flag.wait(lck); //阻塞等待时会暂时性地解锁mtx用于接收信号，因此不会陷入死锁
		cout << count << endl;
	}
}

void thread3(void)
{
	while(1)
	{
		unique_lock<mutex> lck(mtx);
		cout << "Thread3 waits for count: " << endl;
		flag.wait(lck);
		cout << count << endl;
	}
}
