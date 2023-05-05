//线程访问局部变量的隐患
#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

using namespace std;

void funct(void);

class Show
{
	private:
		int &id;
		int &count;
		string &str;
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

	Show s1(id, count, str); //类的成员变量是引用类型，绑定了局部变量
	thread t1(s1);

	t1.detach();
	//一旦这个函数结束，类的成员变量所绑定的局部变量就会被销毁
	//尽量不要在函数中使用指针或引用变量创建线程
	//若不得不使用，要保证在函数结束之前，线程已经结束
}
