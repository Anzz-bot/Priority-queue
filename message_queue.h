#include <queue>
#include <mutex>
#include <condition_variable>

class MessageQueue
{
public:
    void push(std::string value, int priority)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(std::make_pair(priority, value));
        lock.unlock();
        m_condition.notify_one();
    }

    std::pair<int,std::string> pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this]() { return !m_queue.empty(); });
        std::string value = m_queue.top().second;
        int priority=m_queue.top().first;
        m_queue.pop();
        return std::make_pair(priority,value);
    }
    bool empty(){
        std::lock_guard<std::mutex> lock{m_mutex};
        return m_queue.empty();
    }

private:
    std::priority_queue<std::pair< int,std::string >, std::vector<std::pair< int,std::string >>, std::greater<std::pair< int,std::string >>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};


