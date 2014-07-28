#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <iostream>
#include <sstream>

//#define STUB_SQL


#ifndef STUB_SQL
#include "sqlite3.h"
#endif

#include "sqlitedb.h"

#include "dbinterface.h"
#include "../AGPacket.h"

#include "../utils.h"

//Example implementation of the persistence unit interface
class SqliteDB : public DBInterface
{
    public:
        //Create the database or load the database with the given name
        SqliteDB(std::string dbName);
        ~SqliteDB();

        //Implements the interface
		void writeDetection(AGPacket info);
		void appendINSERTION(AGPacket info);	
		void writeINdb();
	   int counter;
	   
    private:
        #ifndef STUB_SQL
        sqlite3 *db;
		string CommandStr;

#endif

};

#endif // SQLITEDB_H
