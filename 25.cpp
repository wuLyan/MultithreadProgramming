//async()函数的使用：使用函数创建任务
//子线程的线程函数的返回值：全局变量 + 互斥锁 或 future + async
#include <iostream>
#include <future> //async()函数所在头文件，future是类模板
#include <unistd.h>

using namespace std;

bool is_prime(int x);

int main(void)
{
	future<bool> fut = async(is_prime, 313222313);
	//创建任务对象，is_prime是任务函数，313222313是任务函数的参数
	cout << "Checking whether 313222313 is prime." << endl;

	bool ret = fut.get(); //获取任务函数的返回值，若任务函数未完成，则阻塞等待

	if(ret)
		cout << "It is prime" << endl;
	else
		cout << "It is not prime" << endl;
}

bool is_prime(int x)
{
	cout << "Calculating. Please wait....." << endl;
	sleep(5);

	for(int i = 2; i < x; i++)
	{
		if(x % i == 0)
			return false;
	}
	return true;
}
