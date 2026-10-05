#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

class ThreadPool {

    public:
        ThreadPool(size_t num_threads = thread::hardware_concurrency())
        {
            for(size_t i = 0 ; i < num_threads ; ++i){
                threads.emplace_back([this] {
                    while(true) {
                        function<void()> task;

                        {
                            unique_lock<mutex> lock(queue_mutex);

                            cv.wait(lock, [this]{
                                return !tasks.empty() || stop;
                            });

                            if (stop && tasks.empty()){
                                return;
                            }
                            
                            task = move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        ~ThreadPool()
        {
            {
                unique_lock<mutex> lock(queue_mutex);
                stop = true;
            }

            cv.notify_all();

            for (auto& thread : threads){
                thread.join();
            }
        }

        void enqueue(function<void()> task)
        {
            {
                unique_lock<std::mutex> lock(queue_mutex);
                tasks.emplace(move(task));
            }
            cv.notify_one();
        }
    
    private:
        vector<thread> threads;

        queue<function<void()>> tasks;

        mutex queue_mutex;

        condition_variable cv;

        bool stop = false;
};

int main(){

    ThreadPool pool(6);

    for (int i = 0 ; i < 6 ; i++){
        pool.enqueue([i] {
            cout << "Task " << i << " is running on thread " << this_thread::get_id() << endl;
            this_thread::sleep_for(
            chrono::milliseconds(100));
        });
    }

}