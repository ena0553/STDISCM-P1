#include <iostream>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <memory>

using namespace std;

struct Job {
    int n;
    atomic<int> remainingTasks;
    atomic<bool> composite{false};
    Job(int n, int count) : n(n), remainingTasks(count) {}
};

thread_local int workerId = -1;
class ThreadPool {

    public:
        ThreadPool(size_t num_threads = thread::hardware_concurrency())
        {
            for(size_t i = 0 ; i < num_threads ; ++i){
                threads.emplace_back([this, i] {
                    workerId = (int)i;
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

string timestamp(){
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::tm* local_tm = std::localtime(&now_c);
    ostringstream oss;
    oss << std::put_time(local_tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
    string timestamp = oss.str();

    return timestamp;
}

void checkPrime(int checking, ThreadPool& pool, vector<int>& results){
    int countDivs = 0;
    for(long long j = 3 ; j*j <= checking ; j+=2){
        countDivs++;
    }

    if(countDivs == 0){
        results[checking] = 1;
        return;
    }

    auto job = make_shared<Job>(checking, countDivs);

    for(long long j = 3 ; j*j <= checking ; j += 2){
        pool.enqueue([job, j, &results] {
            if(job->n % j == 0){
                job->composite = true;
            }

            if(--job->remainingTasks == 0 && !job->composite){
                results[job->n] = 1;
            }
        });
    }
}

int main() {

    int nThreads;
    int number;

    std::ifstream file("config.txt");
    string line;
    while(file >> line)
    {
        if(line == "num-threads"){
            file >> nThreads;
        }
        else if(line == "y-number"){
            file >> number;
        }
    }

    string startTime = "Start time: " + timestamp() ;
    cout << "Start time: " << timestamp() << endl;
    auto t0 = chrono::steady_clock::now();

    vector<int> results(number + 1, 0);
    {
        ThreadPool pool(nThreads);
        for (int i = 2 ; i < number ; i++){
            if (!(i % 2 == 0) || (i == 2)){
                checkPrime(i, pool, results);
            }
        }
        
    }

    for (int i = 2 ; i <= number ; i++){
        if(results[i]){
            cout << "Prime:" << i << endl;
        }
    }

    cout << endl;
    cout << startTime << endl;
    cout << "End time: " << timestamp() << endl;
    auto t1 = chrono::steady_clock::now();     
    
    auto ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    cout << "Elapsed: " << ms << " ms" << endl;

    return 0;
}