#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

int checkPrime(int num){
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

}

int main() {
    int number;
    int threads;

    std::ifstream file("config.txt");
    string line;
    while(file >> line)
    {
        if(line == "num-threads"){
            file >> threads;
        }
        else if(line == "y-number"){
            file >> number;
        }
    }

    for (int i = 2 ; i < number+1 ; i++){
        if(checkPrime(i) == 1){
            cout << i << "\n";
        }
        
    }

    return 0;
}