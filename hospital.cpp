#include "person.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv){
   disease flu("flu", 10, 20);
   doctor doc("Melvyn", "white", 1);
   patient pat("Susan", "white", 0, flu);
   cout << doc.greet(pat) << endl;
   cout << pat.greet(doc) << endl;
   cout << doc.bye(pat) << endl;
   cout << pat.bye(doc) << endl;
} 
