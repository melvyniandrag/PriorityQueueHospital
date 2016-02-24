#include <vector>
#include <iostream>
#include <functional>
#include <queue>
#include "patient.h"
#include "person.h"
#include "nurse.h"


std::ostream& operator<<(std::ostream& os, const nurse& n){
    os << "Hello I am nurse no. " << n.id;
    return os;
}

void nurse1::ResetQueue(std::vector<patient> pv){
	for(auto p : pv){
		this->pq.push(p);
	}
}

void nurse2::ResetQueue(std::vector<patient> pv){
	for(auto p : pv){
		this->pq.push(p);
	}
}

void nurse3::ResetQueue(std::vector<patient> pv){
	for(auto p : pv){
		this->pq.push(p);
	}
}

void nurse4::ResetQueue(std::vector<patient> pv){
	for(auto p : pv){
		this->pq.push(p);
	}
}

void nurse5::ResetQueue(std::vector<patient> pv){
	for(auto p : pv){
		this->pq.push(p);
	}
}

void nurse6::ResetQueue(std::vector<patient> pv){
	for(auto p : pv){
		this->pq.push(p);
	}
}

void nurse7::ResetQueue(std::vector<patient> pv){
	for(auto p : pv){
		this->pq.push(p);
	}
}
