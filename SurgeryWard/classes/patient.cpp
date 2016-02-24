#include "patient.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const patient& pat){
    os << "Hello, I am patient no. " << pat.id;
    return os;
}
