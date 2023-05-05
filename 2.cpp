//通过函数指针创建线程，并向线程函数传递参数
#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

using namespace std;

void show(int id, int count, const string &str);

int main(void)
{
	thread t1(show, 1, 10, "hello"); //盲猜这个构造函数使用了可变参数模板
	thread t2(show, 2, 20, "world"); //本质上是按指针传递给线程函数

	t1.join(); //阻塞等待子线程1运行结束
	t2.join(); //阻塞等待子线程2运行结束

	cout << "Bye!" << endl;

	return 0;
}

//实参是字符串字面值常量(char const *)，因此可以被隐式转换为string类型(妙啊)
//形参列表中必须是const string &str，因为实参是字符串字面值常量不可被修改
void show(int id, int count, const string &str)
{
	for(int i = 0; i < count; ++i)
	{
		cout << "Thread " << id << ": " << str << endl;
		sleep(1);
	}
}
