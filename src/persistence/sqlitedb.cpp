
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
       print_err("Can't open database");
       print_err(sqlite3_errmsg(db));
   }

   //Statement to create the necessary table
   sql =    "CREATE TABLE RESULTS("  \
            "ID            INTEGER PRIMARY KEY AUTOINCREMENT    NOT NULL    ," \
            "START_TIME    TEXT                                 NOT NULL    ," \
            "END_TIME      TEXT                                 NOT NULL    ," \
            "AGE           INT                                              ," \
            "GENDER        TEXT                                             );";

  //Execute the statement
  rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

  //Check for errors
  if( rc != SQLITE_OK ){
    print_err(zErrMsg);
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

void SqliteDB::writeDetection(DetectionInformation info)
{
#ifndef STUB_SQL
    char *zErrMsg = 0;
    std::string sql;
    int rc;

    //Get the age in a string format for easy writing
    std::stringstream ss;
    ss << info.getAge();
    std::string age = ss.str();

    //Get the gender in a string format
    std::string gender;
    if (info.getGender())
        gender = "M";
    else
        gender = "F";

    //Create SQL statement. This is not a pretty way of crafting
    //one, since it's vulnerable to injection.
    //If this code is ever put in production, this should probably
    //be a prepared statement or at least escaped, even
    //if the risk of attack is minimal.

    sql = "INSERT INTO RESULTS (START_TIME,END_TIME,AGE,GENDER)" \
            "VALUES ('";
    sql.append(info.getStartTime());
    sql.append("','");
    sql.append(info.getStopTime());
    sql.append("',");
    sql.append(age);
    sql.append(",'");
    sql.append(gender);
    sql.append("');");




    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
       print_err(zErrMsg);
       sqlite3_free(zErrMsg);
    }
#endif
}
