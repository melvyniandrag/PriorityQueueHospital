#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "templates.h"
#include "classes/doctor.h"
#include "classes/patient.h"
#include "classes/nurse.h"
#include "graphics/sdl.h"
#include<cassert>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
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

static const int width = 1200;
static const int height = 600;
static const int max_radius = 64;

//Color codes were taken from://en.wikipedia.org/wiki/Web_colors  
string DEAD_STRING = "0xff0000";
string ALIVE_STRING = "0x00ff00";
string CURRENT_STRING = "0xffffff";

string PRESENT_NURSE_STRING = "0xdda0dd";
string ABSENT_NURSE_STRING = "0x008080";

const int DEAD = 0;
const int ALIVE = 1;
const int CURR = 2;

SDL_Colors colors(ALIVE_STRING, DEAD_STRING, CURRENT_STRING, PRESENT_NURSE_STRING, ABSENT_NURSE_STRING);

void make_patient(int xpatient, int ypatient, int rpatient, const int height, const int width, SDL_Surface *screen, int pat_status, int num_nurses){
        int x = xpatient;
        int y = ypatient;
        int r = rpatient;
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
        if (pat_status == DEAD){
            fill_circle(screen, x, y, r, colors.p_dead_int);
            draw_circle(screen, x, y, r, colors.p_dead_int);
        }else if(pat_status == ALIVE){
            fill_circle(screen, x, y, r, colors.p_alive_int);
            draw_circle(screen, x, y, r, colors.p_alive_int);
        }
        else if(pat_status == CURR){
            fill_circle(screen, x, y, r, colors.p_curr_int);
            draw_circle(screen, x, y, r, colors.p_curr_int);
        }
        else{
            cout << "ERROR IN MAKE PATIENT. INVALID pat_status." << endl;
        }
        /*********************** Nurse circles*******************************/
        float rnurse = 0.15*rpatient; 
        int xnurse = xpatient - 1.5*(rnurse + rpatient); 
        float y2 = (ypatient - rpatient) + rpatient * 0.25;
        float y3 = (ypatient - rpatient) + rpatient * 0.75;
        float y4 = (ypatient - rpatient) + rpatient * 1.25;
        float y5 = (ypatient - rpatient) + rpatient * 1.75;
  
       vector<float> nurseY{y2, y3, y4, y5};
        const int max_nurses = 4;
        if (num_nurses > 4){
            num_nurses = 4;
        }
        for(int i = 0; i < num_nurses; i++){
            fill_circle(screen, xnurse, nurseY.at(i), rnurse, colors.n_pres_int);
            draw_circle(screen, xnurse, nurseY.at(i), rnurse, colors.n_pres_int);
        }
        for(int i = num_nurses; i < max_nurses; i++){
            fill_circle(screen, xnurse, nurseY.at(i), rnurse, colors.n_abs_int);
            draw_circle(screen, xnurse, nurseY.at(i), rnurse, colors.n_abs_int);
        }
        SDL_UnlockSurface(screen);

}

int main(int argc, char ** argv){
    
    cout << colors.p_curr_int <<endl;
    cout << colors.p_alive_int << endl;
    cout << colors.p_dead_int << endl;
    cout << colors.n_pres_int << endl;
    cout << colors.n_abs_int << endl;

    //Open database.
    if (sqlite3_open("SQLite/surgery_ward.db", &db) != SQLITE_OK) {
        std::cerr << "Could not open database.\n";
        return 1;
    }
    PatVec pv;
    DiseaseMap dm;

    // Read patient information into the patient vector.
    try{
        Records rpatientecords = select_stmt("SELECT * FROM PATIENTS");
        for (auto& rpatientecord : rpatientecords) {
	        pv.push_back(MakePatient(rpatientecord));         
        }    
            }
    catch(...){
        cout << "ERROR No. " << DB_ERROR << "! Unable to read in patient information." << endl;
    }
    
    cout << "Loaded " << pv.size() << "patients." << endl;
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
   
    // Load all patients into nurses. 
    for(auto n : nv){
            n->ResetQueue(pv);
    }

       if (SDL_Init(SDL_INIT_VIDEO) != 0)
       return 1;
 
    atexit(SDL_Quit);
 
    SDL_Surface *screen = SDL_SetVideoMode(width, height, 0, SDL_DOUBLEBUF);
 
   if (screen == NULL)
        return 2;
 
   SDL_WM_SetCaption("SDL Tutorial", "SDL Tutorial");

   // Initialize SDL_ttf library
   if (TTF_Init() != 0)
   {
      cerr << "TTF_Init() Failed: " << TTF_GetError() << endl;
      SDL_Quit();
      exit(1);
   }

   // Load a font
   TTF_Font *font;
   font = TTF_OpenFont("lazy.ttf", 24);
   if (font == NULL)
   {
      cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << endl;
      TTF_Quit();
      SDL_Quit();
      exit(1);
   }

    SDL_Surface *text;
    SDL_Color text_color = {255, 255, 255};
    text = TTF_RenderText_Solid(font,"               Surgery Room Simulator",text_color);
   
    // Begin main loop.
    int n_pats = pv.size();
    int curr_pat_idx = 0;
    /*
    Logic for the SDL loop:
    We have a vector of patients {p1, ... p8}
    p1 is current. Once done it should be gone.

    Loop while there are still patients to consider.
        set time counter to zero
        set the current patient to th current number. All patients numbers below this will be colored alive / dead, and the nurses won't be able to vist them.
        
        Inside, we must loop over the time steps until the patient dies.
            During every iteration, 
            
            incrementthe time counter by one.

    */
    vector<int> patientX{200, 500, 800, 1100, 200, 500, 800, 1100};
    vector<int> patientY{200, 200, 200, 200, 500, 500, 500, 500};
    const int patientR = 80;    
    vector<int> patientStat{ALIVE, ALIVE, ALIVE, ALIVE, ALIVE, ALIVE, ALIVE, ALIVE};
    vector<int> nnurses {0,0,0,0,0,0,0,0};
    int num_of_nurses = nv.size();
    
    vector<patient> remaining_patients = pv;
    while(remaining_patients.size() != 0)
    {
        fill(nnurses.begin(), nnurses.end(),0);
        cout << "Current Patient Index " << curr_pat_idx << endl; 
        cout << "nnurses:" << endl;
        for (auto n : nnurses){
            cout << n << " ";
        }
        cout << endl;
        SDL_Event event; 
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                return 0;
        }
        
        // Load the nurses with remaining patients.
        for(auto& n : nv){
            n->ResetQueue(remaining_patients);
        }

        for(int pat_idx = 0; pat_idx < curr_pat_idx; pat_idx++){
            make_patient(patientX.at(pat_idx), patientY.at(pat_idx), patientR, height, width, screen, patientStat.at(pat_idx), nnurses.at(pat_idx));
        }
        
        int time_to_die = remaining_patients.at(0).time_to_die;
        int timer = 0;
        vector<int> old_nurse_positions;
        for(int curr_nurse = 0; curr_nurse < num_of_nurses; curr_nurse++){
            old_nurse_positions.push_back(-1);
        }
        while(timer <= remaining_patients.size()){
            patientStat.at(curr_pat_idx) = CURR;
            //count the nurses.
            int curr_nurse_idx = 0;
            for(auto& n : nv){
                if(old_nurse_positions.at(curr_nurse_idx) != curr_pat_idx){
                    if(old_nurse_positions.at(curr_nurse_idx) != -1){
                        nnurses.at(old_nurse_positions.at(curr_nurse_idx)) -= 1;
                    }
                    patient current_patient = n->GetPatient();
                    old_nurse_positions.at(curr_nurse_idx) = current_patient.id;
                    nnurses.at(current_patient.id) += 1;
                }
                curr_nurse_idx += 1;
            }
            cout << "nnurses:" << endl;
            for (auto n : nnurses){
                cout << n << " ";
            }
            cout << endl;
            make_patient(patientX.at(curr_pat_idx), patientY.at(curr_pat_idx),\
                         patientR, height, width, screen, patientStat.at(curr_pat_idx), nnurses.at(curr_pat_idx));
            for(int pat_idx = curr_pat_idx + 1; pat_idx < 8; pat_idx++){
                make_patient(patientX.at(pat_idx), patientY.at(pat_idx), patientR, height,\
                             width, screen, patientStat.at(pat_idx), nnurses.at(pat_idx));
            }    
            if ((timer == time_to_die)or(timer==remaining_patients.size())){
                cout << "PATIENT HAS DIED!" << endl;
                patientStat.at(curr_pat_idx) = DEAD;
                make_patient(patientX.at(curr_pat_idx), patientY.at(curr_pat_idx),\
                             patientR, height, width, screen, patientStat.at(curr_pat_idx), nnurses.at(curr_pat_idx));
                break;
            }
            else if(nnurses.at(curr_pat_idx) >= 4){
                patientStat.at(curr_pat_idx) = ALIVE;        
                make_patient(patientX.at(curr_pat_idx), patientY.at(curr_pat_idx),\
                             patientR, height, width, screen, patientStat.at(curr_pat_idx), nnurses.at(curr_pat_idx));
                break;
            }
            timer += 1;
            if (SDL_BlitSurface(text, NULL, screen, NULL) != 0)
            {
                cerr << "SDL_BlitSurface() Failed: " << SDL_GetError() << endl;
                break;
            }
            for(int wait = 0; wait < 1000000; wait++){
                for(int wait2 = 0; wait2 < 100; wait2++){
                    int dummy = 0;
                }
            }
        }
        curr_pat_idx += 1;        
        remaining_patients.erase(remaining_patients.begin());
        //SDL_UnlockSurface(screen);
        if (SDL_BlitSurface(text, NULL, screen, NULL) != 0)
        {
             cerr << "SDL_BlitSurface() Failed: " << SDL_GetError() << endl;
            break;
        }
        SDL_FreeSurface(screen);
        SDL_Flip(screen);
        if(curr_pat_idx == n_pats){
            int make_wait;
            cin >> make_wait;
        }    
    }
    
    return 0;
}
