#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "filter.h"


struct minizen_db {
	char *dir;
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
 * Tests whether a given table name is legal (and indirectly whether it can
 * be used in a path component).
 */
static bool minizen_db_valid_table(const char *table) {

	if (! table) {
		return false;
	}

	return (strcmp(table, "users") == 0 ||
		strcmp(table, "organizations") == 0 ||
		strcmp(table, "tickets") == 0);
}


/**
 * Loads the JSON data file that corresponds to the given table name and
 * performs basic sanity checks on its content. Returns NULL on error.
 */
static json_object * minizen_db_load_table(struct minizen_db *db,
	const char *table) {

	if (! minizen_db_valid_table(table)) {
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


json_object * minizen_db_search(struct minizen_db *db, const char *table,
	const char *key, const char *value) {

	json_object *root = minizen_db_load_table(db, table);
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

	free(db->dir);
	free(db);
}

