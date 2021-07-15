#include <iostream>
#include <thread>
#include <cmath>
#include <vector>
std::vector<int> primes;

void calculate_primes(int how_many){
	if(how_many<2){
		printf("no prime");
		return;
	}
    if(how_many == 2) {
        primes.emplace_back(2);
        return;
    }
    
    int prime_candidate = 3;
    while(prime_candidate < how_many) {
            bool is_prime = true;
            for(int i = 2; i<=std::sqrt(prime_candidate); i++) {
                if(prime_candidate % i == 0) {
                    is_prime = false;
                    break;
                }
            }
            if(is_prime) {
                primes.emplace_back(prime_candidate);
                
            }
            prime_candidate++;
    }
}
int main() {
    int how_many_primes;
    std::cout << "Enter the number of primes you wish to generate\n";
    std::cin >> how_many_primes;
    std::thread t1(calculate_primes,how_many_primes);
    t1.join();
    for(unsigned int i = 0; i < primes.size(); i++) {
        std::cout << primes[i] << ' ';
        if(i % 10 == 0 && i != 0) {
            std::cout << '\n';
        }
    }
    return 0;
}