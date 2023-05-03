//通过Lambda表达式(理解为匿名的内联函数)和成员函数创建线程
#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

using namespace std;

class Show
{
	private:
		int id;
		int count;
		string str;
	public:
		Show(int i, int c, string s) : id(i), count(c), str(s){};
		void operator()() const
		{
			for(int i = 0; i < count; ++i)
			{
				cout << "Thread " << id << ": " << str << endl;
				sleep(1);
			}
		}

		void display(void)
		{
			for(int i = 0; i < count; ++i)
			{
				cout << "Thread " << id << ": " << str << endl;
				sleep(1);
			}
		}
};

int main(void)
{
//	Show s1(1, 10, "Hello");
	Show s2(2, 20, "world");

//	thread t1(s1);
	thread t1(Show(1, 10, "Hello"));
	thread t2(s2);

	//lambda create thread，用Lambda表达式替代函数指针即可
	thread t3([](int id, int count, string str){
		for(int i = 0; i < count; ++i)
		{
			cout << "Thread " << id << ": " << str << endl;
			sleep(1);
		}
	}, 3, 30, "I love you, Rick!");

	//function member
	Show s4(4, 40, "Good morning");
	thread t4(&Show::display, s4); //一定要指明类作用域，且传递地址
	//本质上还是函数指针，只不过是类的成员函数指针

	//以上四个线程同步并发执行

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	cout << "Bye!" << endl;

	return 0;
}

