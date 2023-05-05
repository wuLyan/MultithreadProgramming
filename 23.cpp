//条件变量的用法：生产者与消费者问题
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

using namespace std;

mutex mtx;
condition_variable produce, consume;

int cargo = -1; //表示库存货物的数量

void consumer(void);
void producer(int n);

int main(void)
{
	thread consumers[10], producers[10]; //存放子线程对象的数组

	for(int i = 0; i < 10; i++)
	{
		consumers[i] = thread(consumer);
		producers[i] = thread(producer, i + 1);
	}

	for(int i = 0; i < 10; i++)
	{
		consumers[i].join();
		producers[i].join();
	}

	return 0;
}

void consumer(void)
{
	unique_lock<mutex> lck(mtx);
	if(cargo == -1)
	{
		//库存货物不足，需要等待生产者发出的信号
		cout << "Consumer waits for cargo!" << endl;
		consume.wait(lck);
	}
	cout << cargo << endl;
	cargo = -1; //消费完成再次将库存货物置为-1
	//各消费者的消费能力不同，即不管有多少库存货物都全部消费完毕
	produce.notify_one(); //通知生产者之一生产货物(按需生产)
}

void producer(int n)
{
	sleep(5);
	unique_lock<mutex> lck(mtx);
	if(cargo != -1)
		produce.wait(lck); //还有库存货物，不再继续生产，等待消费者消费完毕发出信号
	cargo = n; //各生产者的生产能力不同
	consume.notify_one(); //通知消费者之一消费货物(按需生产)
}
