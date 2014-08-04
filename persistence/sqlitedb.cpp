
#include "sqlitedb.h"

//Creates the database or logs a message if it's already there/does not work.
SqliteDB::SqliteDB(std::string dbName)
{
#ifndef STUB_SQL
   char *zErrMsg = 0;
   int rc;
   char *sql;

   //Open the sqlite database
   rc = sqlite3_open(dbName.c_str(), &db);

   if( rc ){
       std::cout<<("Can't open database")<<std::endl;
   }

   //Statement to create the necessary table
   sql =    "CREATE TABLE RESULTS ("  \
			"CURRENT_TIME      TEXT  PRIMARY KEY  NOT NULL    ," \
            "ID            INT    NOT NULL    ," \
            "AGE           INT                                              ," \
            "GENDER        TEXT                                             );";

  //Execute the statement
  rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
  counter=0;
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
//Write demographic information regarding each face which has been detected

void SqliteDB::appendINSERTION(AGPacket info, string* lastStr){

	std::string sql1,sql3,sql2="";

	std::stringstream ss;
	ss << info.ageCatg;
    std::string age = ss.str();

	std::stringstream ss2;
	ss2 << info.ageCatg;
    std::string ID = ss2.str();

	std::stringstream ss3;
	ss3 << counter;
    std::string counters = ss3.str();


    //Get the gender in a string format
    std::string gender;
	if (info.gender)
        gender = "M";
    else
        gender = "F"; 

    //Create SQL statement. This is not a pretty way of crafting
    //one, since it's vulnerable to injection.
    //If this code is ever put in production, this should probably
    //be a prepared statement or at least escaped, even
    //if the risk of attack is minimal.
	
    sql1 = "INSERT INTO RESULTS (CURRENT_TIME,ID,AGE,GENDER)" \
            "VALUES ('";
	sql1.append(info.currentTime);
	sql3="-";
	sql3.append(counters);
	sql2.append("',");
	sql2.append(ID);
	sql2.append(",");
    sql2.append(age);
    sql2.append(",'");
    sql2.append(gender);
    sql2.append("');");
	
	if((*lastStr).size()!=0){
		if(sql2.compare((*lastStr).substr((*lastStr).size()-11,(*lastStr).size()))==0){
			if(sql1.compare((*lastStr).substr(0,76))==0)
				return;
		}
	}
	sql1.append(sql3);
	sql1.append(sql2);
	(*lastStr)=sql1;
	CommandStr.append(sql1);
	counter++;
	
}

void SqliteDB::writeINdb(){
#ifndef STUB_SQL
    char *zErrMsg = 0;
    int rc;

    /* Execute SQL statement */
	rc = sqlite3_exec(db, CommandStr.c_str(), 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){	
       sqlite3_free(zErrMsg);
    }
	CommandStr="";
	
	counter=0;
#endif
}


void SqliteDB::writeDetection(AGPacket info)
{
#ifndef STUB_SQL
    char *zErrMsg = 0;
    std::string sql;
    int rc;
	
    //Get the age in a string format for easy writing
	
    std::stringstream ss;
	ss << info.ageCatg;
    std::string age = ss.str();

	std::stringstream ss2;
	ss2 << info.ageCatg;
    std::string ID = ss2.str();

	std::stringstream ss3;
	ss3 << counter;
    std::string counters = ss3.str();


    //Get the gender in a string format
    std::string gender;
	if (info.gender)
        gender = "M";
    else
        gender = "F"; 

    //Create SQL statement. This is not a pretty way of crafting
    //one, since it's vulnerable to injection.
    //If this code is ever put in production, this should probably
    //be a prepared statement or at least escaped, even
    //if the risk of attack is minimal.
	
    sql = "INSERT INTO RESULTS (CURRENT_TIME,ID,AGE,GENDER)" \
            "VALUES ('";
	sql.append(info.currentTime);
	sql.append("-");
	sql.append(counters);
	sql.append("',");
	sql.append(ID);
	sql.append(",");
    sql.append(age);
    sql.append(",'");
    sql.append(gender);
    sql.append("');");


    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
	counter++;
    if( rc != SQLITE_OK ){	
       sqlite3_free(zErrMsg);
    }
#endif
}
