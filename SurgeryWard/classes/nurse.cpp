#include "nurse.h"

template<typename COMPARATOR>
std::ostream& operator<<(std::ostream& os, const nurse<COMPARATOR>& n){
    os << "Hello I am nurse no. " << n.id;
    return os;
}
