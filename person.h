#include <string>

using String = std::string;

class patient;
class doctor;

class disease{
public:
    String name;
    double t_cure;
    double t_lethal;
    disease(String name, double t_cure, double t_lethal) : name(name), t_cure(t_cure), t_lethal(t_lethal) {}
};

class person{
public:
    explicit
    person(String name, String color, int status) : name(name), color(color), status(status) {}
    String name;
    String color;
    int status; // enum?
};	

class doctor : public person{
public:
    doctor(String name, String color, int status) : person(name, color, status){}
    String greet(person pat){
        return "Hello " + pat.name + " my name is " + this->name + "."; 
    }
    String  bye(person pat){
        return "Good bye " + pat.name + ".";
    }
};

class patient : public person{
public:
    patient(String name, String color, int Status, disease d): person(name, color, status), d(d)  {}
    String greet(person doc){
        return "Hi Dr. " + doc.name + ". I have " + this-> d.name + ".";
    }
    String bye (person doc){
        return "Thanks Dr. " + doc.name + "!";
    }
    disease d;
};


