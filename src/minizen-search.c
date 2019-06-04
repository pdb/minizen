#include <stdio.h>

#include "formatter.h"
#include "minizen.h"


static int print_primary_header(struct minizen_db *db, int results) {

	/*
	 * Don't leave the user without any output if there are no results (and
	 * so print_primary_object_header isn't called).
	 */
	if (! results) {
		fprintf(stderr, "No results found\n");
	}

	return 0;
}


static int print_primary_object_header(struct minizen_db *db,
	json_object *json, int i) {

	if (i > 1) {
		printf("--\n");
	}

	printf("Result %d:\n", i);

	return 0;
}


static void print_kv_pair(const char *key, json_object *value) {

	printf("%20s: %s\n", key, json_object_get_string(value));
}


static int print_object(struct minizen_db *db, json_object *object, int i) {

	json_object_object_foreach(object, key, value) {
		print_kv_pair(key, value);
	}

	return 0;
}


int minizen_search(struct minizen_db *db, const char *table, const char *key,
	const char *value) {

	json_object *results = minizen_db_search(db, table, key, value);
	if (! results) {
		return EXIT_FAILURE;
	}

	struct minizen_af_data data;
	data.header = print_primary_header;
	data.object.header = print_primary_object_header;
	data.object.body = print_object;

	int rc = minizen_array_formatter(db, results, &data);

	json_object_put(results);

	return rc ? EXIT_FAILURE : EXIT_SUCCESS;
}

