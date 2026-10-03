#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <ctime>
#include <vector>

using namespace std;

void function1(){
    for (int i = 0 ; i < 200 ; i++){
        std::cout << "+";
    }
}

void function2(){
    for (int i = 0 ; i < 200 ; i++){
        std::cout << "-";
    }
}

void checkPrime(int start, int end, std::vector<int>& primes){

    for(int i = start;  i < end + 1; i++){
        int prime = 1;

        if(i %2 == 0 && i !=2){
            prime = 0;
        }

        else if (floor(sqrt(i)) == sqrt(i)){
            prime = 0;
        }
        else{
            for(int j = 2 ; j*j < (i) ; j++){
                if(i % j == 0){
                    prime = 0;
                    break;
                }
            }
        }

        if(prime){
            primes.push_back(i);
        }
    }

}

int main() {

    auto start = chrono::high_resolution_clock::now();

    /*
    thread worker1(function1);
    thread worker2(function2);

    cout << "\n";

    worker1.join();
    worker2.join();

    */

    int nThreads = 4;
    int number = 10000;

    int division = number/nThreads;
    
    std::vector<int> primes;
    std::vector<int> d1, d2, d3, d4;

    thread t1(checkPrime, 2, division, ref(d1));
    thread t2(checkPrime, division + 1, division*2, ref(d2));
    thread t3(checkPrime, (division*2)+1, division *3, ref(d3));
    thread t4(checkPrime, (division*3) + 1, number, ref(d4));
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    primes.reserve(d1.size() + d2.size() + d3.size() + d4.size());
    primes.insert(primes.end(), d1.begin(), d1.end());
    primes.insert(primes.end(), d2.begin(), d2.end());
    primes.insert(primes.end(), d3.begin(), d3.end());
    primes.insert(primes.end(), d4.begin(), d4.end());

    for (int i = 0 ; i < primes.size() ; i++){
        cout << primes[i] << ", ";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "\nTime: " << duration.count() << " microseconds";


    return 0;
}