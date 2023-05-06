//async()函数的使用：使用类的成员函数创建异步任务
#include <iostream>
#include <future>
#include <unistd.h>

using namespace std;

class Prime
{
	public:
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
};

int main(void)
{
	Prime p;

	//future<bool> fut = async(is_prime, 313222313);
	//auto fut = async(&Prime::is_prime, p, 313222313);
	future<bool> fut = async(&Prime::is_prime, p, 313222313); //类似于用类的成员函数指针创建线程的方式
	cout << "Checking whether 313222313 is prime." << endl;

	bool ret = fut.get();

	if(ret)
		cout << "It is prime" << endl;
	else
		cout << "It is not prime" << endl;
}


