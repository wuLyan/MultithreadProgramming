//异常情况下的RAII等待方式
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

//RAII(资源获取即初始化))要使用到的类
class thread_guard
{
	private:
		thread &g_thread; //线程不支持拷贝构造，所以只能用引用
	public:
		explicit thread_guard(thread &my_thread) : g_thread(my_thread){} //禁止隐式类型转换
		~thread_guard()
		{
			if(g_thread.joinable()) //判断所引用的线程是否可join
				g_thread.join();
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

//RAII
void funct(void)
{
	int id = 1;
	int count = 10;
	string str = "Hello world";

	Show s1(id, count, str);
	thread t1(s1);
	thread_guard gt(t1); //创建RAII对象，使其绑定到线程t1上
	//在销毁gt对象时，会调用析构函数，析构函数中会调用join()函数等待线程t1结束
	//局部对象gt的生命周期结束时会自动调用析构函数，线程t1也就结束了

	int n1, n2;
	cout << "Please enter two numbers: " << endl;
	cin >> n1 >> n2;
	
	try
	{
		if(n2 == 0)
			throw runtime_error("n2 can't be 0");
		cout << "n1 / n2 = " << n1 / n2 << endl;
	}
	catch(runtime_error err)
	{
		cout << "Quit with exception" << endl;
//		t1.join();
//		return;
	}

//	t1.join();
}
