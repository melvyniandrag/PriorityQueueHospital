#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "templates.h"
#include "classes/doctor.h"
#include "classes/patient.h"
#include "classes/nurse.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <cmath>

#include <map>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}
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
           // if (which_nurse==1)
           //     nv.push_back(new nurse<IncrAgeSort>(id, ias));
           // else if (which_nurse == 2)
           //     nv.push_back(new nurse<DecrAgeSort>(id, das));
           // else if (which_nurse ==3)
           //     nv.push_back(new nurse<IncrBloodPressureSort>(id, ibps));
           // else if (which_nurse == 4)
           //     nv.push_back(new nurse<DecrBloodPressureSort>(id, dbps));
           // else if (which_nurse == 5)
           //     nv.push_back(new nurse<WealthSort>(id, ws));
           // else if (which_nurse == 6)
           //     nv.push_back(new nurse<InsuranceSort>(id, is));
           // else
           //     nv.push_back(new nurse<MoodSort>(id, ms));
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

    return 0;
}
