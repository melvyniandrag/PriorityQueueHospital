#pragma once

#include <iostream>
#include "person.h"

class doctor : public person{
public:
    doctor(int curr_pat) : curr_pat(curr_pat){};
    int curr_pat;
    void WriteData(){ std::cout << "This is a placeholder for now. This function will write to the SQL database." << std::endl;}
private:
};
