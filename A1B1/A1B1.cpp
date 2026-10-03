#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

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

void checkPrime(int start, int end){

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
            cout << i << ", ";
        }
    }

    /*
    int prime = 1;  

    if(num%2 == 0 && num != 2){
        prime = 0;
    }
    else if(floor(sqrt(num)) == sqrt(num)){
        prime = 0;
    }
    else{
        for(int i = 2 ; i < sqrt(num) ; i++){
            if(num%i == 0){
                prime = 0;
                break;
            }
        }
    }
    return prime;
    */

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
    int number = 1000;

    int division = number/nThreads;
    
    //for (int i = 0 ; i < nThreads ; i++){}

    thread t1(checkPrime, 2, division);
    thread t2(checkPrime, division + 1, division*2);
    thread t3(checkPrime, (division*2)+1, division *3);
    thread t4(checkPrime, (division*3) + 1, number);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "\nTime: " << duration.count() << " microseconds";


    return 0;
}