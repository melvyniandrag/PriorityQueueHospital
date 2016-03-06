#include "patient.h"
#include <iostream>
#include <map>
#include <string>

std::ostream& operator<<(std::ostream& os, const patient& pat){
    os << "Hello, I am patient no. " <<  pat.id << " and I have " << pat.illness;
    return os;
}

void patient::SetTimeTilDeath(std::map<std::string, int> dm){
	this->time_to_die = dm[this->illness];
}