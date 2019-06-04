#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"


struct minizen_db {
	char *dir;
};


struct minizen_db * minizen_db_open(const char *dir) {

	if (! dir) {
		fprintf(stderr, "minizen_db_open: directory not specified");
		return NULL;
	}

	struct minizen_db *db = malloc(sizeof(struct minizen_db));
	if (! db) {
		perror("minizen_db_open: malloc");
		return NULL;
	}

	db->dir = strdup(dir);
	if (! db->dir) {
		perror("minizen_db_open: strdup");
		free(db);
		return NULL;
	}

	return db;
}


/**
 * Qualifies a filename that is relative to the database's data directory. This
 * is probably an overkill (given PATH_MAX will *likely* always be long enough)
 * but we also take the opportunity to support varargs in the filename as a
 * convenience for callers.
 */
static char * minizen_db_get_path(struct minizen_db *db,
	const char *filename, ...) {

	int dir_len = strlen(db->dir);

	va_list ap;
	va_start(ap, filename);
	int bytes = dir_len + 1 + vsnprintf(NULL, 0, filename, ap) + 1;
	va_end(ap);

	char *path = malloc(bytes);
	if (! path) {
		return NULL;
	}

	strcpy(path, db->dir);
	path[dir_len] = '/';

	va_start(ap, filename);
	vsnprintf(path + dir_len + 1, bytes - dir_len - 1, filename, ap);
	va_end(ap);

	return path;
}


void minizen_db_close(struct minizen_db *db) {

	if (! db) {
		return;
	}

	free(db->dir);
	free(db);
}

