#include <unistd.h>

#include "minizen.h"


int minizen_search(struct minizen_db *db, const char *table, const char *key,
	const char *value) {

	json_object *results = minizen_db_search(db, table, key, value);
	if (! results) {
		return EXIT_FAILURE;
	}

	json_object_to_fd(STDOUT_FILENO, results, JSON_C_TO_STRING_PRETTY);

	json_object_put(results);

	return EXIT_SUCCESS;
}

