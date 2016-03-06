#pragma once

#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include "person.h"

using String = std::string;

class patient : public person{
public:
    patient(int id,
            int age,
	        int mood,
            int insurance,
            int wealth,
            int bp, 
	        String illness) :
            id(id),
            age(age), 
            mood(mood),
            insurance(insurance),
            wealth(wealth),
            bp(bp),
            illness(illness), 
            alive(1) 
            {};
    void SetTimeTilDeath(std::map<std::string, int> dm);
    void WriteData() { std::cout << "This is a placeholder." << std::endl;} 
    void Die() {this->alive = 0;}
    int IsAlive() {return alive;}
    friend std::ostream& operator<< (std::ostream& os, const patient& pat);
    int id;
    String illness;
    int alive;
    int time_to_die;
    int age;
    int mood;
    int insurance;
    int wealth;
    int bp;
};
