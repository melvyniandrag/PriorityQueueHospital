/* 
* The nurse classes should be inherited from a nurse super class. The only defining feature of the nurses is the manner of sorting the members in their priority queues.
*/
#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include "patient.h"
#include "person.h"
struct IncrAgeSort{
    bool operator()(patient p1, patient p2){
        return p1.age > p2.age;
    }
};
struct DecrAgeSort{
    bool operator()(patient p1, patient p2){
        return p1.age < p2.age;
    }
}; 
struct IncrBloodPressureSort{
    bool operator()(patient p1, patient p2){
        return p1.bp < p2.bp;
    }
};
struct DecrBloodPressureSort{
    bool operator()(patient p1, patient p2){
        return p1.bp > p2.bp;
    }
};
struct WealthSort{
    bool operator()(patient p1, patient p2){
        return p1.wealth > p2.wealth;
    }
};
struct InsuranceSort{
    bool operator()(patient p1, patient p2){
        return p1.insurance > p2.insurance;
    }
};
struct MoodSort{
    bool operator()(patient p1, patient p2){
        return p1.mood > p2.mood;
    }
};

class nurse1 : public person{
public:
    //nurse1(sort) {MySort = sort; };
    nurse1(){};
    void WriteData(){std::cout << "This is a placeholder." << std::endl;}
    void ResetQueue(std::vector<patient> pv);
    int GetCurrentPatient();
private:
    std::priority_queue<patient, std::vector<patient>, IncrAgeSort> pq; 
};
//class nurse2 : public person{
//public:
//    nurse1() { };
//    void WriteData(){std::cout << "This is a placeholder." << std::endl;}
//    void ResetQueue(std::vector<patient> pv);
//   int GetCurrentPatient();
//private:
//    std::priority_queue<patient, std::vector<patient>, IncrAgeSort> pq; 
//};
//class nurse1 : public person{
//public:
//    nurse1() { };
//    void WriteData(){std::cout << "This is a placeholder." << std::endl;}
//    void ResetQueue(std::vector<patient> pv);
//   int GetCurrentPatient();
//private:
//    std::priority_queue<patient, std::vector<patient>, IncrAgeSort> pq; 
//};
//class nurse1 : public person{
//public:
//    nurse1() { };
//    void WriteData(){std::cout << "This is a placeholder." << std::endl;}
//    void ResetQueue(std::vector<patient> pv);
//   int GetCurrentPatient();
//private:
//    std::priority_queue<patient, std::vector<patient>, IncrAgeSort> pq; 
//};
//
//class nurse1 : public person{
//public:
//    nurse1() { };
//    void WriteData(){std::cout << "This is a placeholder." << std::endl;}
//    void ResetQueue(std::vector<patient> pv);
//   int GetCurrentPatient();
//private:
//    std::priority_queue<patient, std::vector<patient>, IncrAgeSort> pq; 
//};
//
//class nurse1 : public person{
//public:
//    nurse1() { };
//    void WriteData(){std::cout << "This is a placeholder." << std::endl;}
//    void ResetQueue(std::vector<patient> pv);
//   int GetCurrentPatient();
//private:
//    std::priority_queue<patient, std::vector<patient>, IncrAgeSort> pq; 
//};
//
//class nurse1 : public person{
//public:
//    nurse1() { };
//    void WriteData(){std::cout << "This is a placeholder." << std::endl;}
//    void ResetQueue(std::vector<patient> pv);
//   int GetCurrentPatient();
//private:
//    std::priority_queue<patient, std::vector<patient>, IncrAgeSort> pq; 
//};
