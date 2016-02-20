#include <iostream>
#include "classes/doctor.h"
#include "classes/patient.h"
#include "classes/nurse.h"
using namespace std;

int main(int argc, char ** argv){
    //Read from database of patient information.
    //Read from database of nurse information.
    //


    nurse1 n;
    n.WriteData();
    patient* p1 = new patient(10, "measles");
    p1->WriteData();    

}
