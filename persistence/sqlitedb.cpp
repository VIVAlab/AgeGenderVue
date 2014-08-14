
#include "sqlitedb.h"

//Creates the database or logs a message if it's already there/does not work.
SqliteDB::SqliteDB(string date)
{
#ifndef STUB_SQL
   char *zErrMsg = 0;
   int rc;
   char *sql;
   
   //Open the sqlite database
   rc = sqlite3_open(("Database//"+date.substr(0,10)+".db").c_str(), &db);

   if( rc ){
       std::cout<<("Can't open database")<<std::endl;
   }
   currentdate=date.substr(0,10);
   //Statement to create the necessary table
   sql =    "CREATE TABLE RESULTS ("  \
			"TIME_ID      INT  PRIMARY KEY  NOT NULL    ," \
			"MALE_TEEN    INT ," \
			"MALE_ADULT INT ," \
			"MALE_SENIOR INT ," \
			"FEMALE_TEEN INT ," \
			"FEMALE_ADULT INT ," \
			"FEMALE_SENIOR INT" \
            ");";

  //Execute the statement
  rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
  //Check for errors
  if( rc != SQLITE_OK ){
    sqlite3_free(zErrMsg);
  }

#endif
}



SqliteDB::~SqliteDB()
{
#ifndef STUB_SQL
    sqlite3_close(db);
#endif
}

void SqliteDB::Generatedb(string date){
	char *zErrMsg = 0;
    int rc;
    char *sql;
	currentdate=date.substr(0,10);
	rc = sqlite3_open(("Database//"+currentdate+".db").c_str(), &db);

    if( rc ){
       std::cout<<("Can't open database")<<std::endl;
    }

   //Statement to create the necessary table
   sql =    "CREATE TABLE RESULTS ("  \
			"TIME_ID      INT  PRIMARY KEY  NOT NULL    ," \
			"MALE_TEEN    INT ," \
			"MALE_ADULT INT ," \
			"MALE_SENIOR INT ," \
			"FEMALE_TEEN INT ," \
			"FEMALE_ADULT INT ," \
			"FEMALE_SENIOR INT" \
            ");";

  //Execute the statement
  rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
  //Check for errors
  if( rc != SQLITE_OK ){
    sqlite3_free(zErrMsg);
  }

}

//Write demographic information regarding each face which has been detected

void SqliteDB::writeINdb(string date,int ft,int fa,int fs,int mt,int ma,int ms){
#ifndef STUB_SQL
    char *zErrMsg = 0;
    int rc;
	
	if(date.substr(0,10).compare(currentdate)!=0)
	{
		Generatedb(date);
	}

	char ttt[80];
	sprintf(ttt,"%d,%d,%d,%d,%d,%d",mt,ma,ms,ft,fa,fs);
	string sql;
	sql = "INSERT INTO RESULTS (TIME_ID, MALE_TEEN ,MALE_ADULT, MALE_SENIOR, FEMALE_TEEN, FEMALE_ADULT, FEMALE_SENIOR)" \
            "VALUES (";
	cout<<date.substr(11,date.length()).c_str()<<endl;
	sql.append(date.substr(11,date.length()));
	sql.append(",");
	sql.append(ttt);
    sql.append(");");

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){	
       sqlite3_free(zErrMsg);
    }
#endif
}
