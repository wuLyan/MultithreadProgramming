//通过函数指针创建线程
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void hello(void);

//主线程
int main(void)
{
	thread t1(hello); //通过传递函数指针实例化线程对象t1，hello为线程函数
	//函数名就是函数指针，表示函数在内存空间中的首地址

	t1.join(); //阻塞等待子线程运行结束

	cout << "Bye!" << endl;

	return 0; //主线程结束时会回收系统资源，回收之后子线程也无法运行
}

//子线程
void hello(void)
{
	for(int i = 0; i < 10; ++i)
	{
		cout << "Hello world!" << endl;
		sleep(1);
	}
}
