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

void checkPrime(int checking){
    int prime = 1;

    if(checking == 2){
        prime = 1;
    }

    else{
        for(int j = 3 ; j*j <= (checking) ; j+=2){
            if(checking % j == 0){
                prime = 0;
                break;
            }
        }
    }

    if(prime){
        cout << checking << ", ";
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

    cout << "Start time: " << timestamp() << endl;

    for(int i = 2 ; i < number ; i++){
        if (!(i % 2 == 0) || (i == 2)){
            checkPrime(i);
        }
    }

    cout << endl;
    cout << "End time: " << timestamp() << endl;


    return 0;
}