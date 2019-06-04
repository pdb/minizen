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


void minizen_db_close(struct minizen_db *db) {

	if (! db) {
		return;
	}

	free(db->dir);
	free(db);
}

