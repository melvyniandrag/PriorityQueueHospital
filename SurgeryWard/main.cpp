#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "templates.h"
#include "classes/doctor.h"
#include "classes/patient.h"
#include "classes/nurse.h"
#include "graphics/sdl.h"

#include <SDL/SDL.h>
#include <stdio.h>
#include <cmath>

#include <map>

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
using DiseaseMap = map<string, int>;

void AddDisease(DiseaseMap& dm, Record record){
    string illness = record[0];
    int time_to_die = atoi(record[1].c_str());
     dm.insert(std::pair<string, int>(illness, time_to_die));
}

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

const int DB_ERROR = 1;
const int SDL_ERROR = 2;

int main(int argc, char ** argv){
    //Open database.
    if (sqlite3_open("SQLite/surgery_ward.db", &db) != SQLITE_OK) {
        std::cerr << "Could not open database.\n";
        return 1;
    }
    PatVec pv;
    DiseaseMap dm;

    // Read patient information into the patient vector.
    try{
        Records patient_records = select_stmt("SELECT * FROM PATIENTS");
        for (auto& patient_record : patient_records) {
	        pv.push_back(MakePatient(patient_record));         
        }    
            }
    catch(...){
        cout << "ERROR No. " << DB_ERROR << "! Unable to read in patient information." << endl;
    }
// Verify that the patients have been loaded correctly.
    std::cout << "printing the contents of pv" << std::endl;
    //print_vector(pv);
        vector<basenurse*> nv;

    //Read from database of nurse information.
    try{
        std::cout << "There are " << nv.size() << "nurses in the nurse vector" << std::endl;
        Records nurse_records = select_stmt("SELECT * FROM NURSES"); 
        IncrAgeSort ias;
        DecrAgeSort das;
        IncrBloodPressureSort ibps;
        DecrBloodPressureSort dbps;
        WealthSort ws;
        InsuranceSort is;
        MoodSort ms;
        for(auto nurse_record : nurse_records){
            std::cout << "There are " << nv.size() << "nurses in the nurse vector" << std::endl;
            int id = atoi(nurse_record[0].c_str());
            int which_nurse = atoi(nurse_record[1].c_str());
            cout << "LOADING NURSE " <<  id << " of type " << which_nurse << endl;
            switch (which_nurse){
                case 1:
                    nv.push_back(new nurse<IncrAgeSort>(id, ias));break;
                case 2:
                    nv.push_back(new nurse<DecrAgeSort>(id, das)); break;
                case 3:
                    nv.push_back(new nurse<IncrBloodPressureSort>(id, ibps)); break;
                case 4:
                    nv.push_back(new nurse<DecrBloodPressureSort>(id, dbps)); break;
                case 5:
                    nv.push_back(new nurse<WealthSort>(id, ws)); break;
                case 6:
                    nv.push_back(new nurse<InsuranceSort>(id, is)); break;
                case 7:
                    nv.push_back(new nurse<MoodSort>(id, ms));break;
            }
        }
    }
    catch(...){
        cout << "Error No. " << DB_ERROR << "! Unable to read in nurse information." << endl;
    }
    std::cout << "There are " << nv.size() << "nurses in the nurse vector" << std::endl;
    int dumy;
    cin >> dumy;
    // Read from db of disease information.
    try{
        Records disease_records = select_stmt("SELECT * FROM DISEASES");
        for (auto& disease_record : disease_records){
            AddDisease(dm, disease_record);
        }
        //Verify that the diseases have been loaded correctly.
        for (auto const &m : dm){
            std::cout << m.first << " " << m.second << std::endl;
        }
    }
    catch(...){
        cout << "Error No. " << DB_ERROR << "! Unable to read in disease information." << endl;
    }

    // Now set each patient's time to die based on the values in the disease map.
    for (auto& p : pv){
        p.SetTimeTilDeath(dm);
    }
    
    //close database
    sqlite3_close(db);
    std::cout << " Just closed the db connection!!!!" << std::endl; 
    int n_pats = pv.size();
    for(auto n : nv){
            n->ResetQueue(pv);
    }
    while(n_pats != 0){
        std::cout << n_pats << " elements remaining in pv" << std::endl;
        // Add the pv elements to the nurses.
        
        for(auto& n : nv){
            cout << n->GetPatient().id << endl;
        }
        int dum;
        cin >> dum;
        n_pats -= 1;
    }

static const int width = 640;
    static const int height = 480;
    static const int max_radius = 64;
 
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
      return 1;
 
    atexit(SDL_Quit);
 
    SDL_Surface *screen = SDL_SetVideoMode(width, height, 0, SDL_DOUBLEBUF);
 
    if (screen == NULL)
        return 2;
 
    while(true)
    {
        SDL_Event event; 
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                return 0;
        }

 
        int x = 60;
        int y = 14;
        int r = 30;


        if (r >= 4)
        {
            if (x < r + 2)
                x = r + 2;
            else if (x > width - r - 2)
                x = width - r - 2;
 
            if (y < r + 2)
                y = r + 2;
            else if (y > height - r - 2)
                y = height - r - 2;
        }
 
        SDL_LockSurface(screen);
 
        fill_circle(screen, x, y, r, 0xcc0000);
        draw_circle(screen, x, y, r, 0xffffff);

        int x2 = 1;
        int y2 = 4;
        int r2 = 10;

        if (r2 >= 4)
        {
            if (x2 < r2 + 2)
                x2 = r2 + 2;
            else if (x2 > width - r2 - 2)
                x2 = width - r2 - 2;
 
            if (y2 < r2 + 2)
                y2 = r2 + 2;
            else if (y2 > height - r2 - 2)
                y2 = height - r2 - 2;
        }
 
        SDL_LockSurface(screen);
 
        fill_circle(screen, x2, y2, r2, 0xcc0000);
        draw_circle(screen, x2, y2, r2, 0xffffff);
 
        SDL_FreeSurface(screen);
 
        SDL_Flip(screen);
    }
 
    return 0;
    return 0;
}
