#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>


using namespace std;

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

void checkPrime(int start, int end, int threadID){

    for(int i = start;  i < end + 1; i++){
        int prime = 1;

        if(i %2 == 0 && i !=2){
            prime = 0;
        }
        else{
            for(int j = 3 ; j*j <= (i) ; j+=2){
                if(i % j == 0){
                    prime = 0;
                    break;
                }
            }
        }

        if(prime){
            cout << endl << "Thread ID: " << threadID << " Prime: " << i << " " << timestamp() << endl;
        }
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

    cout << timestamp();

    int division = number/nThreads;

    vector<thread> workers;

    for(int i = 0 ; i < nThreads ; i++){

        int startInterval = 2 + i * division;
        
        int endInterval;

        if(i == nThreads-1){
            endInterval = number;
        }
        else{
            endInterval = startInterval + division - 1;
        }

        workers.emplace_back(checkPrime, startInterval, endInterval, i);
    }

    for (auto& w : workers) w.join();

    cout << endl;
    cout << timestamp();

    return 0;
}