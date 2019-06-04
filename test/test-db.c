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


Suite * create_db_suite() {

	TCase *tc1 = tcase_create("minizen_db_open");
	tcase_add_test(tc1, minizen_db_open__success);
	tcase_add_test(tc1, minizen_db_open__no_dir);

	TCase *tc2 = tcase_create("minizen_db_close");
	tcase_add_test(tc1, minizen_db_close__no_db);

	Suite *suite = suite_create("db");
	suite_add_tcase(suite, tc1);
	suite_add_tcase(suite, tc2);

	return suite;
}

