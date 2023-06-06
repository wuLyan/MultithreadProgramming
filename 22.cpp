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
		从而解锁互斥量，以便线程t2可以为互斥量加锁继续执行
		{
			lock_guard<mutex> lck(mtx);
			count = i++;
		}
*/
		unique_lock<mutex> lck(mtx);
		count = i++;
		lck.unlock(); //手动解锁互斥量mtx

		// flag.notify_one(); //当使用notify_one时只能通知一个线程，另外两个线程只能有一个线程接收到信号
		flag.notify_all(); //当使用notify_all时可以通知所有线程，所有线程都会接收到信号
		// 当发出信号之后没有其他线程接收也没关系，也就是没有线程从等待状态变为阻塞状态
		// 当线程接收到信号之后，由等待状态变为阻塞状态，只有当互斥锁mtx被解锁之后才能被唤醒，要注意区分概念
		sleep(1);
	}
}

void thread2(void)
{
	while(1)
	{
		// 因为lock_guard模板类的对象无法被暂时解锁，所以改用unique_lock模板类
		unique_lock<mutex> lck(mtx);
		cout << "Thread2 waits for count: " << endl;
		flag.wait(lck); //进入等待状态(running -> waiting)，等待时会暂时性地解锁mtx用于接收信号，因此不会陷入死锁
		// 当收到条件变量发出的信号后线程由等待状态变为阻塞状态(waiting -> blocking)，当互斥锁mtx被解锁后线程才会被唤醒(阻塞状态变为就绪状态)
		// 虽然阻塞状态和等待状态都是进入系统的阻塞队列，但是两者的含义不能混用：阻塞表示由于互斥锁被占用而无法继续执行，等待表示由于条件不满足而无法继续执行
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
