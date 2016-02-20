#include "classes/patient.h"
struct IncrAgeSort{
    bool operator()(patient p1, patient p2){
        return p1.age > p2.age;
    }
};
