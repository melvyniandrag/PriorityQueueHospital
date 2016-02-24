#pragma once
#include<vector>
#include<iostream>

template<typename T>
void print_vector(std::vector<T> V){
    for(auto t : V){
        std::cout << t << std::endl;
    }
}
