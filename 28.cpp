//主线程与子线程间的数据传输：promise + future
#include <iostream>
#include <future>

using namespace std;

void add_func(promise<int> &my_promise, int x, int y);

int main(void)
{
	promise<int> pm; //实例化模板类的对象，线程的发送端
	future<int> future; //实例化模板类的对象，线程的接收端
	future = pm.get_future();  //make future<->promise get relationship

	thread t(add_func, ref(pm), 10, 20); //向子线程传递引用

	int sum = future.get(); //wait for child thread calculate result, blocked function
	cout << "sum = " << sum << endl;

	t.join();

	return 0;
}

void add_func(promise<int> &my_promise, int x, int y)
{
	cout << "x = " << x << ", y = " << y << endl;
	int sum = 0;
	sum = x + y;
	this_thread::sleep_for(chrono::seconds(5));

	my_promise.set_value(sum); //send result to main thread
}
