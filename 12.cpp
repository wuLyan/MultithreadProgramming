//通过命令ps -eLF查看各子线程具体运行在哪个核心(查看CPU编号)上
#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

using namespace std;

void show(int id, int count, const string &str);

int main(void)
{
	cout << thread::hardware_concurrency() << endl;

	thread t1(show, 1, 100, "hello");
	thread t2(show, 2, 100, "world"); 
	thread t3(show, 3, 100, "good");
	thread t4(show, 4, 100, "morning"); 

	t1.join();
	t2.join();
	t3.join();
	t4.join();

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
