#include <getopt.h>
#include <minizen/db.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static struct option long_options[] = {
	{ "data-dir",		required_argument,	NULL, 'd' },
	{ "help",		no_argument,		NULL, 'h' },
	{ "lookups",		required_argument,	NULL, 'l' },
	{ NULL, 0, NULL, 0 }
};


static void usage(const char *app) {

	fprintf(stderr, "usage: %s [--data-dir=DIR] --lookups=N "
		"TABLE KEY VALUE\n", app);
}


static int profile(struct minizen_db *db, const char *table, const char *key,
	const char *value, size_t lookups) {

	clock_t start = clock();

	/*
	 * Perform one search to load and cache the table.
	 */
	json_object *results;
	if (! (results = minizen_db_search(db, table, key, value))) {
		return EXIT_FAILURE;
	}
	json_object_put(results);

	clock_t ready = clock();

	/*
	 * Now perform our lookups on the cached data.
	 */
	for (size_t i = 0; i < lookups; i++) {
		if (! (results = minizen_db_search(db, table, key, value))) {
			return EXIT_FAILURE;
		}
		json_object_put(results);
	}

	clock_t end = clock();

	/*
	 * And report on the results.
	 */
	#define CLOCKS_PER_MS ((double) CLOCKS_PER_SEC / 1000)
	printf("%zd lookups in %.3f ms (average = %.3f ms); "
		"first query = %.3f ms\n", lookups,
		(end - ready) / CLOCKS_PER_MS,
		(end - ready) / CLOCKS_PER_MS / lookups,
		(ready - start) / CLOCKS_PER_MS);

	return EXIT_SUCCESS;
}


int main(int argc, char **argv) {

	const char *data_dir = ".";
	size_t lookups = 0;

	while (true) {
		int option_index;
		int c = getopt_long(argc, argv, "d:hl:", long_options,
			&option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 'd':
				data_dir = optarg;
				break;
			case 'h':
				usage(argv[0]);
				return EXIT_SUCCESS;
			case 'l':
				lookups = strtoul(optarg, NULL, 10);
				break;
			default:
				usage(argv[0]);
				return EXIT_FAILURE;
		}
	}

	if (argc != optind + 3 || ! lookups) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	struct minizen_db *db = minizen_db_open(data_dir);
	if (! db) {
		return EXIT_FAILURE;
	}

	int rc = profile(db, argv[optind], argv[optind + 1], argv[optind + 2],
		lookups);

	minizen_db_close(db);

	return rc;
}

