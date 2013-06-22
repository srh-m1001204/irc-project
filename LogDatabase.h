#ifndef LOGDATABASE_H
#define LOGDATABASE_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <time.h>

#include <sqlite3.h>

using namespace std;

class LogDatabase {
public:
    static sqlite3      *database;
    static const char   *path;

    static void     InitDatabase(const char *path);

    static void     OpenLog();
    static void     CloseLog();
    static void     CreateLogTable();
    static void     DropLogTable();
    static void     ClearLogTable();

    static void     InsertLog(string date, string name, string channel, string log);
    static string   LastSeen(string name);
    static string   GetLog();

private:

};

#endif
