#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "minizen.h"


static struct option long_options[] = {
	{ "data-dir",		required_argument,	NULL, 'd' },
	{ "help",		no_argument,		NULL, 'h' },
	{ NULL, 0, NULL, 0 }
};


static void usage(const char *app) {

	fprintf(stderr, "usage: %s [--data-dir=DIR] TABLE KEY VALUE\n", app);
}


int main(int argc, char **argv) {

	const char *data_dir = ".";

	while (true) {
		int option_index;
		int c = getopt_long(argc, argv, "d:h", long_options,
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
			default:
				usage(argv[0]);
				return EXIT_FAILURE;
		}
	}

	if (argc != optind + 3) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	struct minizen_db *db = minizen_db_open(data_dir);
	if (! db) {
		return EXIT_FAILURE;
	}

	int rc = minizen_search(db, argv[optind], argv[optind + 1],
		argv[optind + 2]);

	minizen_db_close(db);

	return rc;
}

