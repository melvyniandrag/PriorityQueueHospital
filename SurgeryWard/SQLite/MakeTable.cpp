#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   char const *sql_create;
   char const *sql_insert;

   /* Open database */
   rc = sqlite3_open("surgery_ward.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql_create = "CREATE TABLE PATIENTS("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "AGE            INT     NOT NULL," \
         "MOOD		 INT	 NOT NULL," \
	 "INSURANCE      INT     NOT NULL," \
	 "WEALTH	 INT     NOT NULL," \
	 "BP             INT     NOT NULL,"\
	 "ILLNESS        CHAR(20)    NOT NULL);";


   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql_create, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   
   /* Insert SQL statement */
   sql_insert = "INSERT INTO PATIENTS(ID,AGE,MOOD,INSURANCE,WEALTH,BP,ILLNESS) " \
                "VALUES(1, 12, 1, 1, 100000, 127, 'CIRRHOSIS'); "\
	        "INSERT INTO PATIENTS(ID,AGE,MOOD,INSURANCE,WEALTH,BP,ILLNESS) " \
                "VALUES(2, 72, 2, 0, 1000, 115, 'WARTS'); "\
	        "INSERT INTO PATIENTS(ID,AGE,MOOD,INSURANCE,WEALTH,BP,ILLNESS) " \
                "VALUES(3, 17, 1,2, 80000, 145, 'HEART DISEASE'); "\
	        "INSERT INTO PATIENTS(ID,AGE,MOOD,INSURANCE,WEALTH,BP,ILLNESS) " \
                "VALUES(4, 55, 1, 3,80500, 162, 'HEART DISEASE'); "\
		"INSERT INTO PATIENTS(ID,AGE,MOOD,INSURANCE,WEALTH,BP,ILLNESS) " \
                "VALUES(5, 45, 1, 2,500, 122, 'CIRRHOSIS');";

     /* Execute SQL statement */
   rc = sqlite3_exec(db, sql_insert, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Records created successfully\n");
   }
 	
   sqlite3_close(db);
   return 0;
}
