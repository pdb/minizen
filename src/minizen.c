#include <getopt.h>
#include <minizen/db.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minizen.h"


static struct option long_options[] = {
	{ "data-dir",		required_argument,	NULL, 'd' },
	{ "help",		no_argument,		NULL, 'h' },
	{ NULL, 0, NULL, 0 }
};


static const char *app;


void usage(const char *command) {

	fprintf(stderr, "usage: minizen [--data-dir=DIR] %s\n",
		command ? command : "<command> [<args>]");
}


int main(int argc, char **argv) {

	const char *data_dir = ".";

	app = argv[0];

	while (true) {
		int option_index;
		int c = getopt_long(argc, argv, "+d:h", long_options,
			&option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 'd':
				data_dir = optarg;
				break;
			case 'h':
				usage(NULL);
				return EXIT_SUCCESS;
			default:
				usage(NULL);
				return EXIT_FAILURE;
		}
	}

	if (argc == optind) {
		usage(NULL);
		return EXIT_FAILURE;
	}

	struct minizen_db *db = minizen_db_open(data_dir);
	if (! db) {
		return EXIT_FAILURE;
	}

	const char *command = argv[optind];
	argc -= optind, argv += optind;

	int rc;
	if (strcmp(command, "search") == 0) {
		rc = minizen_search(db, argc, argv);
	} else {
		fprintf(stderr, "minizen: '%s' is not a valid command\n",
			command);
		rc = EXIT_FAILURE;
	}

	minizen_db_close(db);

	return rc;
}

