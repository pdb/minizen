#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "db.h"


static struct option long_options[] = {
	{ "data-dir",		required_argument,	NULL, 'd' },
	{ NULL, 0, NULL, 0 }
};


static void usage(const char *app) {

	fprintf(stderr, "usage: %s [--data-dir=DIR]\n", app);
}


int main(int argc, char **argv) {

	const char *data_dir = ".";

	while (true) {
		int option_index;
		int c = getopt_long(argc, argv, "d:", long_options,
			&option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 'd':
				data_dir = optarg;
				break;
			default:
				usage(argv[0]);
				return EXIT_FAILURE;
		}
	}

	if (argc != optind) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	struct minizen_db *db = minizen_db_open(data_dir);
	if (! db) {
		return EXIT_FAILURE;
	}

	minizen_db_close(db);

	return EXIT_SUCCESS;
}

