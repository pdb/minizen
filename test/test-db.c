#include "unit-tests.h"

#include "db.h"


START_TEST(minizen_db_open__success) {

	struct minizen_db *db = minizen_db_open(".");
	ck_assert(db != NULL);

	minizen_db_close(db);
}
END_TEST


START_TEST(minizen_db_open__no_dir) {

	ck_assert(minizen_db_open(NULL) == NULL);
}
END_TEST


START_TEST(minizen_db_close__no_db) {

	minizen_db_close(NULL);
}
END_TEST


START_TEST(minizen_db_search__missing_table) {

	struct minizen_db *db =
		minizen_db_open(TOP_SRCDIR "/test/data/partial");

	ck_assert(minizen_db_search(db, "tickets", "foo", "bar") == NULL);

	minizen_db_close(db);
}
END_TEST


START_TEST(minizen_db_search__invalid_table_name) {

	struct minizen_db *db =
		minizen_db_open(TOP_SRCDIR "/test/data/partial");

	ck_assert(minizen_db_search(db, NULL, "name", "Alice") == NULL);
	ck_assert(minizen_db_search(db, "invalid", "name", "Alice") == NULL);

	minizen_db_close(db);
}
END_TEST


START_TEST(minizen_db_search__invalid_table_data) {

	struct minizen_db *db =
		minizen_db_open(TOP_SRCDIR "/test/data/invalid");

	ck_assert(minizen_db_search(db, "users", "name", "Alice") == NULL);
	ck_assert(minizen_db_search(db, "tickets", "_id", "1") == NULL);

	minizen_db_close(db);
}
END_TEST


START_TEST(minizen_db_search__matching) {

	struct minizen_db *db =
		minizen_db_open(TOP_SRCDIR "/test/data/partial");

	json_object *result;

	result = minizen_db_search(db, "users", "name", "Alice");
	ck_assert(json_object_is_type(result, json_type_array) &&
		json_object_array_length(result) == 1);
	json_object_put(result);

	result = minizen_db_search(db, "users", "name", "Anonymous");
	ck_assert(json_object_is_type(result, json_type_array) &&
		json_object_array_length(result) == 0);
	json_object_put(result);

	minizen_db_close(db);
}
END_TEST


Suite * create_db_suite() {

	TCase *tc1 = tcase_create("minizen_db_open");
	tcase_add_test(tc1, minizen_db_open__success);
	tcase_add_test(tc1, minizen_db_open__no_dir);

	TCase *tc2 = tcase_create("minizen_db_close");
	tcase_add_test(tc1, minizen_db_close__no_db);

	/*
	 * This is more an integration test than a unit test; but we include it
	 * in this harness/application for convenience.
	 */
	TCase *tc3 = tcase_create("minizen_db_search");
	tcase_add_test(tc3, minizen_db_search__missing_table);
	tcase_add_test(tc3, minizen_db_search__invalid_table_name);
	tcase_add_test(tc3, minizen_db_search__invalid_table_data);
	tcase_add_test(tc3, minizen_db_search__matching);

	Suite *suite = suite_create("db");
	suite_add_tcase(suite, tc1);
	suite_add_tcase(suite, tc2);
	suite_add_tcase(suite, tc3);

	return suite;
}

