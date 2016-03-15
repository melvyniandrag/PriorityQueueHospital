COMP = g++ -std=c++11
SQLLIBS = -lsqlite3 -lpthread -ldl
SDLLIBS = -lSDL -lSDL_ttf

all: SurgeryWard 

SurgeryWard: main.o patient.o nurse.o sdl.o
	$(COMP) main.o patient.o nurse.o sdl.o -o SurgeryWard $(SDLLIBS) $(SQLLIBS)

main.o: main.cpp classes/nurse.h classes/patient.h classes/doctor.h classes/person.h templates.h graphics/sdl.h
	$(COMP) -c main.cpp 

patient.o: classes/patient.cpp classes/patient.h
	$(COMP) -c classes/patient.cpp

nurse.o: classes/nurse.cpp classes/nurse.h
	$(COMP) -c classes/nurse.cpp

sdl.o: graphics/sdl.cpp graphics/sdl.h
	$(COMP) -c graphics/sdl.cpp

clean:
	rm *.o
