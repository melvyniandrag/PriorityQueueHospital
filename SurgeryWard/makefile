COMP = g++ -std=c++11

all: SurgeryWard 

SurgeryWard: main.o nurse.o
	$(COMP) main.o nurse.o -o SurgeryWard

main.o: main.cpp classes/nurse.h classes/patient.h classes/doctor.h classes/person.h 
	$(COMP) -c main.cpp

nurse.o: classes/nurse.cpp classes/nurse.h
	$(COMP) -c classes/nurse.cpp
clean:
	rm *.o classes/*.o sorts/*.o

