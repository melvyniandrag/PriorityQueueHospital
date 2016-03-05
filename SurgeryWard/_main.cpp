#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "templates.h"
#include "classes/doctor.h"
#include "classes/patient.h"
#include "classes/nurse.h"
#include "SDL/sdl.h"
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

nurse MakeNurse(Record record){
    int id = atoi(record[0].c_str());
    int which_nurse = atoi(record[1].c_str());
    switch (which_nurse){
        case 1:
            return nurse1(id);
        case 2:
            return nurse2(id);
        case 3:
            return nurse3(id);
        case 4:
            return nurse4(id);
        case 5:
            return nurse5(id);
        case 6:
            return nurse6(id);
        case 7:
            return nurse7(id);
    }
}

int main(int argc, char ** argv){
    //Open database.
    if (sqlite3_open("SQLite/surgery_ward.db", &db) != SQLITE_OK) {
        std::cerr << "Could not open database.\n";
        return 1;
    }

    // Read patient information into the patient vector.
    Records patient_records = select_stmt("SELECT * FROM PATIENTS");
    PatVec pv;
    for (auto& patient_record : patient_records) {
	    pv.push_back(MakePatient(patient_record));         
    }    
    // Verify that the patients have been loaded correctly.
    print_vector(pv); 

    //Read from database of nurse information.
    Records nurse_records = select_stmt("SELECT * FROM NURSES"); 
    NurVec nv;
    for(auto& nurse_record : nurse_records){
        nv.push_back(MakeNurse(nurse_record));
    }
    //Verify that the nurses have been loaded correctly.
    print_vector(nv);



    //close database
    sqlite3_close(db);



    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;
    
    //The window renderer
    SDL_Renderer* gRenderer = NULL;


	if( !init(gWindow, gRenderer) )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render red filled quad
				SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );		
				SDL_RenderFillRect( gRenderer, &fillRect );

				//Render green outlined quad
				SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );		
				SDL_RenderDrawRect( gRenderer, &outlineRect );
				
				//Draw blue horizontal line
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
				SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

				//Draw vertical line of yellow dots
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
				for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
				{
					SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close(gWindow, gRenderer);





}
