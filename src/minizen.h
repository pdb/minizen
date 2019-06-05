#ifndef MINIZEN_H_
#define MINIZEN_H_

#include <minizen/db.h>


int minizen_search(struct minizen_db *db, const char *table, const char *key,
	const char *value);


#endif
