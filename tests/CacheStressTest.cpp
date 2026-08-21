#include <iostream>
#include <thread>
#include <vector>
#include <random>

#include "KeyValueStore.hpp"


void worker(KeyValueStore& storage , int id){

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int>  distrib(1, 4);

    std::string key = "key" + std::to_string(id);
    
    for(int i = 0 ; i < 100000; i++){


        int randNumb = distrib(gen);

        switch (randNumb)
        {
        case 1:
            storage.set(key , "value" , 100);
            break;
        case 2:
           storage.get(key);
           break;
        case 3:
            storage.del(key);
            break;
        case 4:
            storage.exists(key);
            break;
        }  
    }
}


int main(){

    KeyValueStore storage(100 , "appendonly.aof");

    std::vector<std::thread> threads;

    for(int i = 0 ; i < 10 ; i++){
        threads.emplace_back(worker , std::ref(storage) , i);
    }

    for(auto &thread : threads){
        thread.join();
    }

    std::cout << "Stress test completed!" << std::endl;

    return 0;
}