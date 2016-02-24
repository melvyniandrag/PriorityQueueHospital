/* 
* The nurse classes should be inherited from a nurse super class. The only defining feature of the nurses is the manner of sorting the members in their priority queues.

I'm not really sure how to pass the comparators to the nurse constructor.
*/
#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include "patient.h"
#include "nurse.h"


class nurse : public person{
public:
    nurse(int id) : id(id){};
    void WriteData(){std::cout << "This is a placeholder." << std::endl;}
    friend std::ostream& operator<< (std::ostream& os, const nurse& n);
    int id;
};
/*****************************te<typename T>
void f(T s)
{
    std::cout << s << '\n';
}*********************************************************/
struct IncrAgeSort{
    bool operator()(patient p1, patient p2){
        return p1.age > p2.age;
    }
};
class nurse1 : public nurse{
public:
    nurse1(int id) : nurse(id){};
    int GetCurrentPatient();
    void ResetQueue(std::vector<patient> pv);

private:
    std::priority_queue<patient, std::vector<patient>, IncrAgeSort> pq; 
};
/**************************************************************************************/
struct DecrAgeSort{
    bool operator()(patient p1, patient p2){
        return p1.age < p2.age;
    }
}; 
class nurse2 : public nurse{
public:
    nurse2(int id) : nurse(id) { };
    void ResetQueue(std::vector<patient> pv);
   int GetCurrentPatient();
private:
    std::priority_queue<patient, std::vector<patient>, DecrAgeSort> pq; 
};
/**************************************************************************************/
struct IncrBloodPressureSort{
    bool operator()(patient p1, patient p2){
        return p1.bp < p2.bp;
    }
};

class nurse3 : public nurse{
public:
    nurse3(int id) : nurse(id){ };
    void ResetQueue(std::vector<patient> pv);
   int GetCurrentPatient();
private:
    std::priority_queue<patient, std::vector<patient>, IncrBloodPressureSort> pq; 
};
/**************************************************************************************/
struct DecrBloodPressureSort{
    bool operator()(patient p1, patient p2){
        return p1.bp > p2.bp;
    }
};
class nurse4 : public nurse{
public:
    nurse4(int id) : nurse(id){ };
    void ResetQueue(std::vector<patient> pv);
   int GetCurrentPatient();
private:
    std::priority_queue<patient, std::vector<patient>, DecrBloodPressureSort> pq; 
};
/**************************************************************************************/
struct WealthSort{
    bool operator()(patient p1, patient p2){
        return p1.wealth > p2.wealth;
    }
};
class nurse5 : public nurse{
public:
    nurse5(int id) : nurse(id) { };
    void ResetQueue(std::vector<patient> pv);
   int GetCurrentPatient();
private:
    std::priority_queue<patient, std::vector<patient>, WealthSort> pq; 
};
/**************************************************************************************/
struct InsuranceSort{
    bool operator()(patient p1, patient p2){
        return p1.insurance > p2.insurance;
    }
};
class nurse6 : public nurse{
public:
    nurse6(int id) : nurse(id){ };
    void ResetQueue(std::vector<patient> pv);
   int GetCurrentPatient();
private:
    std::priority_queue<patient, std::vector<patient>, InsuranceSort> pq; 
};
/**************************************************************************************/
struct MoodSort{
    bool operator()(patient p1, patient p2){
        return p1.mood > p2.mood;
    }
};
class nurse7 : public nurse{
public:
    nurse7(int id) : nurse(id) { };
    void ResetQueue(std::vector<patient> pv);
   int GetCurrentPatient();
private:
    std::priority_queue<patient, std::vector<patient>, MoodSort> pq; 
};
