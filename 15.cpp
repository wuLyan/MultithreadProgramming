//避免传递地址或引用传递到互斥锁作用范围之外以保护共享数据
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;

class user_data
{
	public:
		int a;
		char c;

		void change_data(void){a *= 10; c += 1;}
};

user_data *unprotected_pt; //全局变量，指向类对象的指针

void function(user_data &my_data)
{
	unprotected_pt = &my_data; //让游离指针指向了受保护的共享数据
}

class protect_data
{
	private:
		user_data data; //protect shared data，其他类的对象
		mutex guard_mutex; //定义互斥锁
	public:
		protect_data(){data.a = 1; data.c = 'A';}
		void process_data(void) //私有成员变量只能通过成员函数访问
		{   //某一时刻只能有一个线程修改共享数据
			lock_guard<mutex> guard(guard_mutex);
			data.a += 10;
			data.c += 1;
			function(data);
			sleep(5);
		}
		void print(void)
		{
			cout << "data.a = " << data.a << ", data.c = " << data.c << endl;
		}
};

void thread_function(protect_data &pd)
{
	pd.process_data(); //调用成员函数为共享数据加锁
}

int main(void)
{
	protect_data pd;
	pd.print(); //data.a = 1, data.c = A

	//向线程传递引用参数
	thread t1(thread_function, ref(pd));
	thread t2(thread_function, ref(pd));

	//阻塞等待子线程结束
	t1.join();
	t2.join();

	pd.print(); //data.a = 21, data.c = C

	unprotected_pt->change_data(); 
	//通过游离指针调用类的成员函数改变了共享数据，对线程加锁保护数据的方法形同虚设
	pd.print(); //data.a = 210, data.c = D

	return 0;
}

