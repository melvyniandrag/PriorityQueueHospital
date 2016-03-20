#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include "patient.h"

class basenurse{
public:
    virtual patient GetPatient() = 0;
    virtual void ResetQueue(std::vector<patient> pv) = 0;
    virtual void WriteData() = 0;
};


template<typename COMPARATOR>
class nurse : public basenurse{
private:
    COMPARATOR comparator;
    std::priority_queue<patient, std::vector<patient>, COMPARATOR> pq;
public:
    nurse(int id, COMPARATOR c) : id(id), comparator(c){}
    void ResetQueue(std::vector<patient> pv){
        while(!pq.empty()){
            pq.pop();
        }
        for (auto p : pv){
            pq.push(p);
        }
    };
    patient GetPatient(){
        return pq.top();
    };
    void WriteData(){ std::cout << "Hi, I am nurse no." << this->id << std::endl;}
    template<typename tn>
    friend std::ostream& operator<< (std::ostream& os, const nurse<tn>& n);
    int id;
};

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
