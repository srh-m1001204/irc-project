#include "LogDatabase.h"

sqlite3     *LogDatabase::database = NULL;
const char  *LogDatabase::path = NULL;

void LogDatabase::InitDatabase(const char *path) {
    LogDatabase::path = path;
    OpenLog();
    CreateLogTable();
    CloseLog();
}
void LogDatabase::OpenLog() {
    if(sqlite3_open(path, &database) != 0)
        perror("Failed to open database");
}
void LogDatabase::CloseLog() {
    sqlite3_close(database);
}
void LogDatabase::CreateLogTable() {
    OpenLog();
    if (sqlite3_exec(database, "CREATE TABLE CHATLOG (DATE VARCHAR(32), NICKNAME VARCHAR(32), CHANNEL VARCHAR(32), LOG TEXT)", NULL, NULL, NULL))
        perror("Database already exists...");
    CloseLog();
}
void LogDatabase::DropLogTable() {
    OpenLog();
    sqlite3_exec(database, "DROP TABLE CHATLOG", NULL, NULL, NULL);
    CloseLog();
}
void LogDatabase::ClearLogTable() {
    OpenLog();
    sqlite3_exec(database, "DELETE FROM CHATLOG", NULL, NULL, NULL);
    CloseLog();
}

void LogDatabase::InsertLog(string name, string channel, string log) {
    OpenLog();
    int date = time(0);
    stringstream sstr;
    sstr << "INSERT INTO CHATLOG (DATE, NICKNAME, CHANNEL, LOG) VALUES ('"<< date <<"','"<< name <<"','"<< channel <<"','"<< log <<"')";

    if (sqlite3_exec(database, sstr.str().c_str(), NULL, NULL, NULL))
        perror("Failed to insert...");
	CloseLog();
}
string LogDatabase::LastSeen(string name) {
    OpenLog();
    sqlite3_stmt *stmt;

	stringstream sstr;
	sstr << "SELECT * FROM CHATLOG WHERE NICKNAME = '" << name << "' ORDER BY DATE DESC LIMIT 1";
	sqlite3_prepare(database, sstr.str().c_str(), -1, &stmt, NULL);

    struct  tm   *timeinfo;
    time_t  t;

	stringstream sstr2;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
        t = sqlite3_column_int(stmt, 0);
	    timeinfo = localtime(&t);
		sstr2 << asctime(timeinfo) << (char*)sqlite3_column_text(stmt, 1) << " " << endl;
	} sqlite3_finalize(stmt);

    CloseLog();
	return sstr2.str();
}
string LogDatabase::GetLog() {
    OpenLog();
    sqlite3_stmt *stmt;
    const char *tail;

    struct  tm   *timeinfo;
    time_t  t;

    if (sqlite3_prepare_v2(database, "SELECT * FROM CHATLOG ORDER BY DATE DESC LIMIT 20", 1000, &stmt, &tail))
        CreateLogTable();
	stringstream sstr;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
        t = sqlite3_column_int(stmt, 0);
        timeinfo = localtime(&t);
        string strtime = asctime(timeinfo);
        strtime = strtime.erase(strtime.length()-1);

        sstr << strtime << " "
            << sqlite3_column_text(stmt, 1) << " "
			<< sqlite3_column_text(stmt, 2) << " "
			<< sqlite3_column_text(stmt, 3) << "\n";
	} sqlite3_finalize(stmt);
	CloseLog();
	return sstr.str();
}
