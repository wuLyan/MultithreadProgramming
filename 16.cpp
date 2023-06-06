//死锁的产生原因一：某一线程一直不释放资源，导致其他线程无法访问该资源
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;

void thread1(void);
void thread2(void);

mutex mt;
int num = 100;

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
	lock_guard<mutex> guard(mt);
	cout << "Thread1: Shared data ---> num = " << num << endl;
	while(1); //陷入死循环，函数一直不结束进入死锁状态
}

void thread2(void)
{
	cout << "Thread2 is running: " << endl;
	sleep(1);  //C++线程库中的延时函数：this_thread::sleep_for(chrono::seconds(1));
	lock_guard<mutex> guard(mt); //已加锁的互斥量无法再次加锁(相当于资源计数量变为0)，陷入死锁状态
	cout << "Thread2: Shared data ---> num = " << num << endl;
}
