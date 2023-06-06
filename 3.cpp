//通过函数对象(函数符/仿函数)创建线程
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
		// 必须重载()运算符，使得Show类的对象可以像函数一样调用，只不过这个重载函数符的形参列表为空
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
//	Show s1(1, 10, "Hello");
	Show s2(2, 20, "world");

//	thread t1(s1);
	thread t1(Show(1, 10, "Hello")); //通过匿名对象(函数符)创建线程
	thread t2(s2); //通过已实例化对象(函数符)创建线程

	t1.join();
	t2.join();

	cout << "Bye!" << endl;

	return 0;
}

