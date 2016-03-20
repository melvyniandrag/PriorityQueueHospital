#pragma once
#include<vector>
#include<iostream>
#include <string>
#include <sstream>
#include <iomanip>
template<typename T>
void print_vector(std::vector<T> V){
    for(auto t : V){
        std::cout << t << std::endl;
    }
}

unsigned int StringToInt(std::string colorString){
    unsigned int color;
    std::stringstream stream(colorString);
    stream >> std::hex >> color;
    return color;
}


class SDL_Colors{
public:
    SDL_Colors(std::string p_alive, std::string p_dead, std::string p_curr, std::string n_pres, std::string n_abs);
    unsigned int p_alive_int, p_dead_int, p_curr_int, n_pres_int, n_abs_int;
};

SDL_Colors::SDL_Colors(std::string p_alive, std::string p_dead, std::string p_curr, std::string n_pres, std::string n_abs){
    p_alive_int = StringToInt(p_alive);
    p_dead_int = StringToInt(p_dead);
    p_curr_int = StringToInt(p_curr);
    n_pres_int = StringToInt(n_pres);
    n_abs_int = StringToInt(n_abs);
}

