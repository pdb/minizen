#ifndef MINIZEN_DB_H_
#define MINIZEN_DB_H_

#include <json-c/json.h>

struct minizen_db;


/**
 * Instantiates a new minizen database instance that expects to find JSON data
 * files in the specified directory. Returns a reference to the database on
 * success (which should be closed with minizen_db_close when no longer
 * required), or NULL on error.
 *
 * In the future this call could be adapted to support different database
 * backends by taking a URI and switching on scheme; e.g.:
 * 	file:///var/db/minizen
 * 	sqlite:///var/db/minizen/db.sqlite
 *	mysql://localhost:3306/minizen
 */
struct minizen_db * minizen_db_open(const char *dir);


/**
 * Queries a given table for all objects that have a key/value pair matching
 * the given criteria. Returns an array containing the matched objects on
 * success, or NULL on error.
 */
json_object * minizen_db_search(struct minizen_db *db, const char *table,
	const char *key, const char *value);


/**
 * Closes the specified minizen database instance and frees all associated
 * resources.
 */
void minizen_db_close(struct minizen_db *db);


#endif
