#include "strclass.h"

extern "C" {
#include "sqlite/sqlite3.h"
}

#include <mutex>

extern int EnabledInsane;
inline std::mutex g_db_lock;

bool ANSItoUTF8(LPCSTR str, char * oBuf, size_t * oSize);
bool UTF8toANSI(LPCSTR str, char * oBuf, size_t * oSize);

int SQL_Run(CSTR queryStr, sqlite3 * sql);
int SQL_prepare(CSTR queryStr, sqlite3 * sql, sqlite3_stmt ** ppStmt);
CSTR SQL_GetColumn(int i, sqlite3_stmt * pStmt);