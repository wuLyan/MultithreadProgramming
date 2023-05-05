//在多线程中使用互斥(互斥锁)
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <unistd.h>

using namespace std;

vector<int> ivec; //待写入数据的容器
mutex guard_mutex; //定义互斥锁

void write_to_vector(int id, int value);

int main(void)
{
	thread t1(write_to_vector, 1, 100);
	thread t2(write_to_vector, 2, 200);
	thread t3(write_to_vector, 3, 300);
	thread t4(write_to_vector, 4, 400);
	thread t5(write_to_vector, 5, 500);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	return 0;
}

void write_to_vector(int id, int value)
{
	cout << "Thread " << id << " is running!" << endl;

	lock_guard<mutex> guard(guard_mutex); //用互斥锁初始化lock_guard对象
	cout << "Thread " << id << " lock to the resource!" << endl;
	ivec.push_back(value);
	sleep(5);
	//当线程函数结束时，lock_guard类的对象guard析构，自动释放互斥锁
}
