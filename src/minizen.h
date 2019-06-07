#ifndef MINIZEN_H_
#define MINIZEN_H_

#include <minizen/db.h>


/*
 * Display the standard application usage, but with command-specific arguments.
 */
void usage(const char *command);

/*
 * Command handlers.
 *
 * Note that argv[0] will contain the command name, in the same way it contains
 * the application name for main().
 */
int minizen_performance(struct minizen_db *db, int argc, char **argv);
int minizen_search(struct minizen_db *db, int argc, char **argv);


#endif
