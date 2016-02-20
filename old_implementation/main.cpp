/*
I don't quite understand this stuff! You have to compile kind of like this:
gcc  -pthread test1.c /usr/local/lib/libsqlite3.a -ldl
*/

#include <iostream>
#include <queue>
#include <string>
#include <functional>
#include "sqlite3.h"

using namespace std;

/* Global variables for storing data from SQL queries */
string disease;
int db_key;
double db_time; 

/* The patient class */
class patient{
public:
	string disease;
	double waiting_time, death_time, heal_time;
	patient(string d, double d_t, double h_t): disease(d), death_time(d_t), heal_time(h_t), waiting_time(0){};
	void time_elapse(double t){waiting_time += t;};
};

/* Overloaded the less than operator for ordering the urgency of treating patients. */

bool operator<(const patient& lhs, const patient& rhs)
{
  return lhs.death_time < rhs.death_time;
}

/*
The SQL commands need to :
get the disease name for a patient. 
SELECT heal_key FROM diseases
WHERE name == patient_disease_name.

Then, SELECT time FROM heal_times
WHERE heal_key == my_heal_key.

And, alternatively, 
SELECT death_key FROM diseases
WHERE name == patient_disease_name
*/


int main(){
	/*
	Initialize a priority queue of patients in the waiting room. 
	Get the list of diseases from the db, store in a vector. 
	Grab random elements from the vector to initialize the patients.
	Draw and sdl window. 
	Interact with the db.
	*/

	int sql_command;
	sqlite3 * db;
	sql_command = sqlite3_open("~/Desktop/HospitalProject/test.db", &db);
	if(sql_command){ cout << "opened db!"<<endl;}
	else {cout << "didn't open" << endl;}

	// Make patients
	int N = 10;
	for(int i = 0; i < N; i++){

	}


	/*
	Use a map to store the key/value pairs mapping integers to names of diseases. The database will not know disease name. 
	i.e.
	diseases.db 
	-------------------------

	Table: diseases
	id | death_key | heal_key
	--------------------------
	1      1           1
	2      1           1
	3      1           2
	4      2           1
	5      2           2


	Table: death
	death_key | death_time
	--------------------------
	1             10
	2             20


	Table: heal
	heal_key | heal_time
	--------------------------
	1            5
	2            10
	


	And then we have an std:map that will give us a dictionary like (1: "cancer", 2: "oral thrush", 3: "bronchitis", 4: "appendicitis", 5: "sniffles")
	Every randomly created person has a number assigned to them. This will allow us to give them a disease name, and extract the necessary times from the database.
	We could just put another table in the db to serve as our map from integers to names, but in the interest in exploring the stl, I'll use a map.


	*/


	//string extract_times = "SELECT * FROM diseases WHERE "

	patient a("cholera", 10.0, 10.0);
	patient b("cancer", 20.0, 20.0);
	priority_queue<patient, vector<patient>, less<patient> > patients;
	patients.push(a);
	patients.push(b);
	cout << patients.top().disease << endl;



	/* Loop. At the beginning of a loop, a person is selected to be healed. they are popper from the top of the priority queue. 
	Then some time will elapse. The time is taken as the min(heal_time, death_time - waiting_time).  Then, either they are cured or they die, and the appropriate action is performed.
	Then the clocks for all of the patients in the priority queue must be updated. 
	We make a temporary queue, and pop the patients off the old queue and one by one update their watiting times. If a patient has been waiting too long, he is not pushed onto the new queue.
	If he still has some life in him, he is pushed onto the queue.
	
	If a persons wait time - death time is non positive, they die, and the appropriate action is taken.
	When all of the patients are updated, we begin a new iteration.


	*/


}
