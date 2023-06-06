// 在程序运行时控制线程数量
// 应用：多核并发计算
#include <iostream>
#include <thread>
#include <vector>
#include <numeric>

using namespace std;

int parallel_accumulate(vector<int>::iterator first, vector<int>::iterator last, int init);
void accumulate_block(vector<int>::iterator first, vector<int>::iterator last, int &sum);

int main(void)
{
	vector<int> ivec(100); //ivec中有100个元素，初始化容器时执行值初始化，即每个元素都为0

	for(int i = 1; i <= 100; ++i)
		ivec.push_back(i);

	cout << parallel_accumulate(ivec.begin(), ivec.end(), 0) << endl;

	return 0;
}

int parallel_accumulate(vector<int>::iterator first, vector<int>::iterator last, int init)
{
	const unsigned int length = distance(first, last); //计算迭代器范围内的元素数量
	if(!length)
		return init;
	
	const unsigned int max_per_thread = 25; //预定义每个线程处理的最大元素数量
	const unsigned int max_threads = (length + max_per_thread - 1) / max_per_thread; //计算所需线程数量
	const unsigned int hardware_threads = thread::hardware_concurrency(); //计算硬件支持的并发线程数
	//由于在某些操作系统下hardware_concurrency()返回0，所以需要对其进行判断，至少是双核处理器
	const unsigned int num_threads = min(hardware_threads != 0 ? hardware_threads : 2, max_threads); //根据实际情况计算最终所需线程数量
	const unsigned int block_size = length / num_threads; //计算每个线程实际处理的元素数量，思想是将所有待计算的数均摊到各线程上

	vector<thread> threads(num_threads - 1); //创建线程容器，注意这里的线程数量比实际所需线程数量少1，因为主线程也可以参与计算
	vector<int> results(num_threads); //创建结果容器，用于存放各线程的计算结果

	vector<int>::iterator block_start = first;
	//创建所需的子线程，注意这里的线程数量比实际所需线程数量少1，因为主线程也可以参与计算
	for(int i = 0; i < (num_threads - 1); ++i)
	{
		vector<int>::iterator block_end = block_start; //每一个子线程都计算左闭右开区间内元素之和
		advance(block_end, block_size); //将迭代器block_end向右移动block_size个位置
		threads[i] = thread(accumulate_block, block_start, block_end, ref(results[i])); //std::accumulate(起始范围, 终止范围, 初始值);
		block_start = block_end;
	}
	accumulate_block(block_start, last, results[num_threads-1]); //主线程一般要比子线程计算的元素数量多

	for(auto &r : threads)
		r.join(); //阻塞等待各子线程结束

	return accumulate(results.begin(), results.end(), init);
}

//子线程要修改主线程中的变量，所以sum需要传引用
void accumulate_block(vector<int>::iterator first, vector<int>::iterator last, int &sum)
{
	sum = accumulate(first, last, sum);
}
