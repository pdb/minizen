#include <minizen/db.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filter.h"


typedef enum minizen_table {
	MINIZEN_TABLE_ORGANIZATIONS,
	MINIZEN_TABLE_TICKETS,
	MINIZEN_TABLE_USERS,
	MAX_MINIZEN_TABLE
} minizen_table_t;


static minizen_table_t table_name_to_slot(const char *table) {

	if (strcmp(table, "organizations") == 0) {
		return MINIZEN_TABLE_ORGANIZATIONS;
	} else if (strcmp(table, "tickets") == 0) {
		return MINIZEN_TABLE_TICKETS;
	} else if (strcmp(table, "users") == 0) {
		return MINIZEN_TABLE_USERS;
	}

	return -1;
}


struct minizen_db {
	char *dir;
	json_object *table_cache[MAX_MINIZEN_TABLE];
};


struct minizen_db * minizen_db_open(const char *dir) {

	if (! dir) {
		fprintf(stderr, "minizen_db_open: directory not specified\n");
		return NULL;
	}

	struct minizen_db *db = malloc(sizeof(struct minizen_db));
	if (! db) {
		perror("minizen_db_open: malloc");
		return NULL;
	}

	memset(db, 0, sizeof(struct minizen_db));

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


/**
 * Loads the JSON data file that corresponds to the given table name and
 * performs basic sanity checks on its content. Returns NULL on error.
 */
static json_object * minizen_db_load_table(struct minizen_db *db,
	const char *table) {

	if (table_name_to_slot(table) == -1) {
		fprintf(stderr, "invalid table: %s\n", table);
		return NULL;
	}

	char *path = minizen_db_get_path(db, "%s.json", table);
	if (! path) {
		fprintf(stderr, "minizen_db_get_path failed\n");
		return NULL;
	}

	json_object *root = json_object_from_file(path);
	free(path);

	if (! root) {
		/*
		 * Note that json_util_get_last_err() unexpectedly returns NULL
		 * if json_object_from_file() fails because the JSON is
		 * unpareseable. This bug/inconsistency in json-c was exposed
		 * by one of the unit tests in this project.
		 */
		const char *msg = json_util_get_last_err();
		if (msg) {
			fprintf(stderr, "%s", msg);
		} else {
			fprintf(stderr, "%s: invalid JSON file\n", table);
		}
		return NULL;
	}

	if (! json_object_is_type(root, json_type_array)) {
		fprintf(stderr, "%s: root object is not an array\n", table);
		json_object_put(root);
		return NULL;
	}

	return root;
}


/**
 * Gets the JSON data that corresponds to the given table name (possibly from
 * disk, possibly from cache). Returns NULL on error.
 *
 * This function increments the reference count of the returned object so that
 * it is semantically equivalent to minizen_db_load_table; i.e., the caller is
 * responsible for decrementing the reference count when done with the object.
 */
static json_object * minizen_db_get_table(struct minizen_db *db,
	const char *table) {

	minizen_table_t i = table_name_to_slot(table);
	if (i == -1) {
		fprintf(stderr, "unknown table: %s\n", table);
		return NULL;
	}

	if (! db->table_cache[i]) {
		db->table_cache[i] = minizen_db_load_table(db, table);
	}

	return json_object_get(db->table_cache[i]);
}


json_object * minizen_db_search(struct minizen_db *db, const char *table,
	const char *key, const char *value) {

	if (! table) {
		fprintf(stderr, "minizen_db_search: no table specified\n");
		return NULL;
	}

	json_object *root = minizen_db_get_table(db, table);
	if (! root) {
		return NULL;
	}

	json_object *results = json_filter(root, key, value);

	json_object_put(root);

	return results;
}


void minizen_db_close(struct minizen_db *db) {

	if (! db) {
		return;
	}

	for (int i = 0; i < MAX_MINIZEN_TABLE; i++) {
		json_object_put(db->table_cache[i]);
	}

	free(db->dir);
	free(db);
}

