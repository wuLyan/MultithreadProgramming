//线程移动，joinable()函数的使用
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void show(int count, const string &str);

int main(void)
{
	thread t1(show, 10, "Hello"); //通过函数指针创建线程
	cout << "Thread1 id = " << t1.get_id() << endl; //调用thread类中的成员函数获得线程的id号
	sleep(5);

	thread t2 = move(t1); //线程t1的所有权转移给t2，t1被销毁
	cout << "Thread2 id = " << t2.get_id() << endl; //与原t1线程的id号相同，这可说明线程t2是由线程t1移动而来的

	cout << "t1.joinable = " << t1.joinable() << endl;
	cout << "t2.joinable = " << t2.joinable() << endl;
 
	if(t1.joinable())
		t1.join(); //不存在的线程无法join(汇合)，会抛出异常
	if(t2.joinable()) //线程安全的写法，在join之前判断线程是否可join
		t2.join();
//	cout << "t2.joinable = " << t2.joinable() << endl;

	thread t3;
//	cout << "t3.joinable = " << t3.joinable() << endl;
	if(t3.joinable()) //使用默认构造函数创建的线程也无法被join
		t3.join();

	cout << "Bye!" << endl;

	return 0;
}

void show(int count, const string &str)
{
	for(int i = 0; i < count; i++)
	{
		cout << str << endl;
		sleep(1);
	}
}
