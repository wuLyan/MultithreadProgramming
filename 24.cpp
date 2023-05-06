//条件变量condition_variable的成员函数wait的第二种用法
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

using namespace std;

mutex mtx;
condition_variable flag;
int count = -1;

void signal_(void);
void wait(int id);

int main(void)
{
	thread t1(signal_);
	thread t2(wait, 2);
	thread t3(wait, 3);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}

void signal_(void)
{
	sleep(5);
	unique_lock<mutex> lck(mtx);
	cout << "notify all threads" << endl;
	lck.unlock(); //必须在其他线程被唤醒之前手动解锁互斥量
	flag.notify_all();

	sleep(5);
	lck.lock(); //重新上锁互斥量，因为下面要更改共享数据
	count = 1;
	cout << "notify all threads again" << endl;
	lck.unlock();
	flag.notify_all();
}

void wait(int id)
{
	unique_lock<mutex> lck(mtx);
	cout << "Thread " << id << " waits for count: " << endl;
	//这里有一个问题：为什么在wait函数中的lambda表达式返回值为false时，子线程t2、t3可以同时打印上面这条语句
	//按照之前的理解，只能有一个线程打印这条语句，另外一个线程由于互斥量mtx被加锁不会运行
	//奥理解了，当运行下面的wait语句时，wait会暂时解锁互斥量mtx，这时另一个子线程可以对其加锁
	flag.wait(lck, [](){return count == 1;});
	//成员函数wait的第二个参数是一个lambda表达式，用于判断条件是否满足
	//当互斥锁mtx收到信号且lambda表达式返回true时，wait才通过阻塞，继续执行下面的语句
	//当lambda表达式返回true时，即使互斥锁mtx没有收到信号，wait也会通过阻塞，但此时不会暂时解锁互斥锁mtx
	//即此时子线程t2、t3某一时刻只能有一个线程在执行，另外一个线程由于互斥量mtx被加锁不会运行，卡在了第一步实例化对象lck处
	cout << "Thread " << id << ": " << count << endl;
	//sleep(1);
}
