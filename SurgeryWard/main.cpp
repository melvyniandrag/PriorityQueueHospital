#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "templates.h"
#include "classes/doctor.h"
#include "classes/patient.h"
#include "classes/nurse.h"

using namespace std;

sqlite3* db;

using Record = std::vector<std::string>;
using Records = std::vector<Record>;

int select_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names)
{
  Records* records = static_cast<Records*>(p_data);
  try {
    records->emplace_back(p_fields, p_fields + num_fields);
  }
  catch (...) {
    // abort select on failure, don't let exception propogate thru sqlite3 call-stack
    return 1;
  }
  return 0;
}

Records select_stmt(const char* stmt)
{
  Records records;  
  char *errmsg;
  int ret = sqlite3_exec(db, stmt, select_callback, &records, &errmsg);
  if (ret != SQLITE_OK) {
    std::cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
  }
  else {
    std::cerr << records.size() << " records returned.\n";
  }

  return records;
}

void sql_stmt(const char* stmt)
{
  char *errmsg;
  int ret = sqlite3_exec(db, stmt, 0, 0, &errmsg);
  if (ret != SQLITE_OK) {
    std::cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
  }
}


using PatVec = vector<patient>;
using NurVec = vector<nurse>;

patient MakePatient(Record record){
    int id = atoi(record[0].c_str());
    int age = atoi(record[1].c_str());
    int mood = atoi(record[2].c_str());
    int insurance = atoi(record[3].c_str());
    int wealth = atoi(record[4].c_str());
    int bp = atoi(record[5].c_str());
    string illness = record[6];
    return patient(id, age, mood, insurance, wealth, bp, illness);
}

int main(int argc, char ** argv){
    //Open database.
    if (sqlite3_open("SQLite/surgery_ward.db", &db) != SQLITE_OK) {
        std::cerr << "Could not open database.\n";
        return 1;
    }

    Records records = select_stmt("SELECT * FROM PATIENTS");
    sqlite3_close(db);
    // Read patient information into the patient vector.
    PatVec pv;
    for (auto& record : records) {
	    pv.push_back(MakePatient(record));         
    }    
    // Verify that the patients have been loaded correctly
    print_vector(pv); 

    //Read from database of nurse information.
    

     nurse n(0);
     n.WriteData();
     NurVec nv;
     nv.push_back(n);
     print_vector(nv);
}
