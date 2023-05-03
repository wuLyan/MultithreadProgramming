//分离主线程与子线程
#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

using namespace std;

void funct(void);

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
};

int main(void)
{
	funct();

	for(int i = 0; i < 100; ++i)
	{
		cout << "Main thread is running!" << endl;
		sleep(1);
	}

	cout << "Bye!" << endl;

	return 0;
}

void funct(void)
{
	int id = 1;
	int count = 10;
	string str = "Hello world";

	Show s1(id, count, str); //采用拷贝的方式将局部变量用于类的成员变量初始化
	thread t1(s1); //通过函数符创建线程

	t1.detach(); //分离线程，主线程不再等待子线程结束，各自执行各自的任务
}
