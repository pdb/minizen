#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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


#define DEFINE_STATIC_HEADER_CB(cb_name, display_title) \
static int cb_name(struct minizen_db *db, json_object *json, int i) { \
	printf(display_title ":\n"); \
	return 0; \
}


DEFINE_STATIC_HEADER_CB(assigned_to_ticket,	"Assigned to ticket");
DEFINE_STATIC_HEADER_CB(assignee,		"Assignee");
DEFINE_STATIC_HEADER_CB(organization,		"Organization");
DEFINE_STATIC_HEADER_CB(submitted_ticket,	"Submitted ticket");
DEFINE_STATIC_HEADER_CB(submitter,		"Submitter");
DEFINE_STATIC_HEADER_CB(ticket,			"Ticket");
DEFINE_STATIC_HEADER_CB(user,			"User");


static void print_kv_pair(const char *key, json_object *value) {

	printf("%20s: %s\n", key, json_object_get_string(value));
}


static int print_object(struct minizen_db *db, json_object *object, int i) {

	json_object_object_foreach(object, key, value) {
		print_kv_pair(key, value);
	}

	return 0;
}


static bool print_kv_if_exists(json_object *object, const char *key) {

	json_object *value;
	if (json_object_object_get_ex(object, key, &value)) {
		print_kv_pair(key, value);
		return true;
	}

	return false;
}


/**
 * Attempts to print a short summary of an object by only printing a few well-
 * known keys (falling back to printing the entire object if none are present).
 */
static int print_object_summary(struct minizen_db *db, json_object *object,
	int i) {

	if (! (print_kv_if_exists(object, "_id") |
		print_kv_if_exists(object, "name") |
		print_kv_if_exists(object, "subject"))) {
		print_object(db, object, i);
	}

	return 0;
}


/**
 * Query one object for related objects, and print the results.
 */
static int minizen_search_secondary(struct minizen_db *db, json_object *json,
	const char *key, const char *join_table, const char *join_key,
	minizen_af_object_cb header) {

	json_object *value;
	if (! json_object_object_get_ex(json, key, &value)) {
		return 1;
	}

	json_object *results = minizen_db_search(db, join_table, join_key,
		json_object_get_string(value));
	if (! results) {
		return 1;
	}

	struct minizen_af_data data;
	data.header = NULL;
	data.object.header = header;
	data.object.body = print_object_summary;

	int rc = minizen_array_formatter(db, results, &data);

	json_object_put(results);

	return rc;
}


static int print_user(struct minizen_db *db, json_object *json, int i) {

	return print_object(db, json, i) ||
		minizen_search_secondary(db, json, "organization_id",
			"organizations", "_id", organization) ||
		minizen_search_secondary(db, json, "_id",
			"tickets", "assignee_id", assigned_to_ticket) ||
		minizen_search_secondary(db, json, "_id",
			"tickets", "submitter_id", submitted_ticket);
}


static int print_organization(struct minizen_db *db, json_object *json, int i) {

	return print_object(db, json, i) ||
		minizen_search_secondary(db, json, "_id",
			"users", "organization_id", user) ||
		minizen_search_secondary(db, json, "_id",
			"tickets", "organization_id", ticket);
}


static int print_ticket(struct minizen_db *db, json_object *json, int i) {

	return print_object(db, json, i) ||
		minizen_search_secondary(db, json, "organization_id",
			"organizations", "_id", organization) ||
		minizen_search_secondary(db, json, "submitter_id",
			"users", "_id", submitter) ||
		minizen_search_secondary(db, json, "assignee_id",
			"users", "_id", assignee);
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

	/*
	 * Use a table-specific object printer, if one exists.
	 */
	if (strcmp(table, "users") == 0) {
		data.object.body = print_user;
	} else if (strcmp(table, "organizations") == 0) {
		data.object.body = print_organization;
	} else if (strcmp(table, "tickets") == 0) {
		data.object.body = print_ticket;
	}

	int rc = minizen_array_formatter(db, results, &data);

	json_object_put(results);

	return rc ? EXIT_FAILURE : EXIT_SUCCESS;
}

