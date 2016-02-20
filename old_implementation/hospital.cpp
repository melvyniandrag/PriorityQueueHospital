#include "person.h"
#include <iostream>

using namespace std;

int doctor::doctor_counter = 0;
int patient::patient_counter = 0;

int main(int argc, char** argv){
	/* Make a priority queue of patients and another prioirity queue of doctors.*/
	


   disease flu("flu", 10, 20);
   doctor doc("Melvyn", "white", 1);
   patient pat("Susan", "white", 0, flu);
   cout << doc.greet(pat) << endl;
   cout << pat.greet(doc) << endl;
   cout << doc.bye(pat) << endl;
   cout << pat.bye(doc) << endl;
} 
