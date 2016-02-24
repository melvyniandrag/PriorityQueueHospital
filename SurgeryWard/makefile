COMP = g++ -std=c++11
LIBS = -lsqlite3 -lpthread -ldl

all: SurgeryWard 

SurgeryWard: main.o patient.o nurse.o
	$(COMP) main.o patient.o nurse.o -o SurgeryWard $(LIBS)

main.o: main.cpp classes/nurse.h classes/patient.h classes/doctor.h classes/person.h templates.h 
	$(COMP) -c main.cpp $(LIBS)

patient.o: classes/patient.cpp classes/patient.h
	$(COMP) -c classes/patient.cpp

nurse.o: classes/nurse.cpp classes/nurse.h
	$(COMP) -c classes/nurse.cpp

clean:
	rm *.o classes/*.o sorts/*.o

