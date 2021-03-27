#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <list>
#include <condition_variable>

struct CTask;
std::mutex gMutex;
std::list <std::shared_ptr<CTask>> tasklist;
std::condition_variable cond;
thread_local int pc = 0;

struct CTask
{
	CTask(int data,std::string &src) :m_data(data),m_src(src) {}
	~CTask()
	{
		//printf("task=%d dctor\n", m_data);
	}
	int	m_data;
	std::string m_src;
}; 

class CProducer
{
public:
	explicit CProducer(const std::string desc) :m_desc(desc)
	{
		std::cout << "starting producer thread:" << m_desc << std::endl;
	}
	void produce()
	{
		while (1)
		{
			std::lock_guard<std::mutex> lock(gMutex);
			std::shared_ptr<CTask> task(new CTask(++pc, this->m_desc));
			tasklist.push_back(task);
			cond.notify_one();
			//printf("produce task,data=%d,produce thread=%s\n", pc, m_desc.c_str());
			//std::this_thread::sleep_for(std::chrono::microseconds(1000));
		}
	}
	~CProducer() {
		std::cout << "ending producer thread:" << m_desc << std::endl;
		std::fflush(stdout);
	}
private:
	std::string m_desc;	//描述
};
class CConsumer 
{
public:
	explicit CConsumer(const std::string &desc) :m_desc(desc)
	{
		std::cout << "starting consumer thread:" << m_desc << std::endl;
	}
	~CConsumer()
	{
		std::cout << "ending consumer thread:" << m_desc << std::endl;
		std::fflush(stdout);
	}
	void consume()
	{
		while (1)
		{
			std::unique_lock<std::mutex> lock(gMutex);
			whilie (tasklist.empty())
			{
				cond.wait_for(lock, std::chrono::microseconds(100));
				//std::this_thread::sleep_for(std::chrono::microseconds(1000));
				continue;
			}
			//printf("queue size=%d\n", tasklist.size());
			std::shared_ptr<CTask> task = tasklist.front();
			//printf("consume task,data=%d src_thread=%s\n", task->m_data, task->m_src.c_str());
			tasklist.pop_front();
			//std::this_thread::sleep_for(std::chrono::microseconds(1000));
		}
	}
private:
	std::string m_desc;
};

int main()
{
	int produce_cnt = 2;
	int consume_cnt = 3;
	for (auto idx = 0; idx < produce_cnt; ++idx)
	{
		std::shared_ptr<CProducer> produce(new CProducer(std::to_string(idx)));
		auto produce_th = std::thread((std::bind(&CProducer::produce, produce)));
		produce_th.detach();
	}
	for (auto idx = 0; idx < consume_cnt; ++idx)
	{
		std::shared_ptr<CConsumer> consumer(new CConsumer(std::to_string(idx)));
		std::thread consume_th(std::bind(&CConsumer::consume, consumer));
		consume_th.detach();
	}
	getchar();
	return 0;
}
