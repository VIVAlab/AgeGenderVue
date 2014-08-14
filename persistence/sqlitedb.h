#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <iostream>
#include <sstream>

//#define STUB_SQL


#ifndef STUB_SQL
#include "sqlite3.h"
#endif

#include "sqlitedb.h"
#include "../AGPacket.h"

#include "../utils.h"

//Example implementation of the persistence unit interface
class SqliteDB
{
    public:
        //Create the database or load the database with the given name
        SqliteDB(std::string date);
        ~SqliteDB();

        //Implements the interface
		void writeINdb(string date,int ft,int fa,int fs,int mt,int ma,int ms);
	    
    private:
        #ifndef STUB_SQL
        sqlite3 *db;
		string CommandStr;
		string currentdate;
		void Generatedb(string dbName);
#endif

};

#endif // SQLITEDB_H
