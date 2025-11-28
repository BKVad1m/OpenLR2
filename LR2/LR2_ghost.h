#include "structure.h"
#include "Engine.h"

int WriteGhostInDatabase(sqlite3 * sql, CSTR songMD5, PLAYSCORE * score);
int ReadGhostToScore(sqlite3 * sql, CSTR songMD5, PLAYSCORE * score);
CSTR ReadGhost(sqlite3 * sql, CSTR songMD5);
