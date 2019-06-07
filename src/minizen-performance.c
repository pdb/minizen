#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "minizen.h"


static int minizen_performance_run(struct minizen_db *db, const char *table,
	const char *key, const char *value, size_t lookups) {

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


static struct option long_options[] = {
	{ "lookups",		required_argument,	NULL, 'l' },
	{ NULL, 0, NULL, 0 }
};


static void minizen_performance_usage() {

	usage("performance --lookups=N TABLE KEY VALUE");
}


int minizen_performance(struct minizen_db *db, int argc, char **argv) {

	size_t lookups = 0;

	optind = 1;	/* Reset getopt for this new argc/argv */
	while (true) {
		int option_index = 0;
		int c = getopt_long(argc, argv, "l:", long_options,
			&option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 'l':
				lookups = strtoul(optarg, NULL, 10);
				break;
			default:
				minizen_performance_usage();
				return EXIT_FAILURE;
		}
	}

	if (argc != optind + 3 || ! lookups) {
		minizen_performance_usage();
		return EXIT_FAILURE;
	}

	argv += optind;

	return minizen_performance_run(db, argv[0], argv[1], argv[2], lookups);
}

