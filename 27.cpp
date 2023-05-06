//async()创建异步任务：同步任务选项
#include <iostream>
#include <future>
#include <unistd.h>

using namespace std;

bool is_prime(int x);

//async的其他参数： 1) launch::async(异步)   2) launch::deferred(推迟)
int main(void)
{
//	future<bool> fut = async(launch::async, is_prime, 313222313); //创建异步任务，会创建新线程
//	future<bool> fut = async(is_prime, 313222313); //默认由系统自动推断同步任务或异步任务
//	future<bool> fut = async(launch::deferred, is_prime, 313222313); //创建同步任务，不会创建新线程
	
	//显式地指明由系统自动推断同步任务或异步任务
	future<bool> fut = async(launch::async|launch::deferred, is_prime, 313222313);	

	cout << "Checking whether 313222313 is prime." << endl;
	cout << "main() thread id = " << this_thread::get_id() << endl;
	sleep(5);

	bool ret = fut.get();

	if(ret)
		cout << "It is prime" << endl;
	else
		cout << "It is not prime" << endl;
}

bool is_prime(int x)
{
	cout << "Calculating. Please wait....." << endl;
	cout << "is_prime() thread id = " << this_thread::get_id() << endl;
	sleep(5);

	for(int i = 2; i < x; i++)
	{
		if(x % i == 0)
			return false;
	}
	return true;
}
