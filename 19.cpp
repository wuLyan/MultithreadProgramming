//避免死锁的方法二：使用unique_lock模板类
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;

mutex mt; //定义互斥锁

void thread1(int id, const string &str);

int main(void)
{
	thread t1(thread1, 1, "Hello world"); //通过函数指针创建线程，并向线程函数传递参数
	t1.join();

	return 0;
}

/*
void thread1(int id, const string &str)
{
	lock_guard<mutex> guard(mt);
	cout << "Thread" << id << ": " << str << endl;
}
*/

void thread1(int id, const string &str)
{
	unique_lock<mutex> guard(mt, defer_lock); //defer_lock表示延迟加锁
	/*
		do_something()
	*/
	guard.lock(); //调用类unique_lock的成员函数lock()，手动加锁
	cout << "Thread" << id << ": " << str << endl;
	guard.unlock();

	int sum = 0;
	for(int i = 1; i <= 100; i++)
		sum += i;

	guard.lock();
	cout << "Thread" << id << ": " << sum << endl;
	//没有手动解锁，类对象guard析构时会自动解锁
}
