#include "unit-tests.h"

#include "filter.h"


START_TEST(json_value_match__string) {

	json_object *string = json_object_new_string("Foo");

	ck_assert(json_value_match(string, "Foo"));
	ck_assert(! json_value_match(string, "Foo Bar"));
	ck_assert(! json_value_match(string, "F"));
	ck_assert(! json_value_match(string, "foo"));
	ck_assert(! json_value_match(string, ""));

	json_object_put(string);
}
END_TEST


START_TEST(json_value_match__empty_string) {

	json_object *string = json_object_new_string("");

	ck_assert(json_value_match(string, ""));

	json_object_put(string);
}
END_TEST


START_TEST(json_value_match__null) {

	ck_assert(json_value_match(NULL, ""));
	ck_assert(! json_value_match(NULL, "null"));
}
END_TEST


START_TEST(json_value_match__integer) {

	json_object *integer = json_object_new_int(1);

	ck_assert(json_value_match(integer, "1"));

	json_object_put(integer);
}
END_TEST


START_TEST(json_value_match__array) {

	json_object *array = json_object_new_array();
	json_object_array_add(array, json_object_new_string("Foo"));
	json_object_array_add(array, json_object_new_string("Bar"));
	json_object_array_add(array, json_object_new_int(1));

	ck_assert(json_value_match(array, "Foo"));
	ck_assert(json_value_match(array, "Bar"));
	ck_assert(json_value_match(array, "1"));

	ck_assert(! json_value_match(array, "Baz"));
	ck_assert(! json_value_match(array, ""));

	json_object_put(array);
}
END_TEST


START_TEST(json_value_match__object) {

	json_object *object = json_object_new_object();

	ck_assert(! json_value_match(object, ""));
	ck_assert(! json_value_match(object, "{}"));

	json_object_put(object);
}
END_TEST


START_TEST(json_object_match__missing) {

	json_object *object = json_object_new_object();

#ifdef MISSING_KEY_CONSIDERED_EMPTY
	ck_assert(json_object_match(object, "missing", ""));
#else
	ck_assert(! json_object_match(object, "missing", ""));
#endif

	json_object_put(object);
}
END_TEST


START_TEST(json_object_match__present) {

	json_object *object = json_object_new_object();
	json_object_object_add(object, "foo", json_object_new_string("bar"));

	ck_assert(json_object_match(object, "foo", "bar"));
	ck_assert(! json_object_match(object, "foo", "baz"));

	json_object_put(object);
}
END_TEST


Suite * create_filter_suite() {

	TCase *tc1 = tcase_create("json_value_match");
	tcase_add_test(tc1, json_value_match__string);
	tcase_add_test(tc1, json_value_match__empty_string);
	tcase_add_test(tc1, json_value_match__null);
	tcase_add_test(tc1, json_value_match__integer);
	tcase_add_test(tc1, json_value_match__array);
	tcase_add_test(tc1, json_value_match__object);

	TCase *tc2 = tcase_create("json_object_match");
	tcase_add_test(tc2, json_object_match__missing);
	tcase_add_test(tc2, json_object_match__present);

	Suite *suite = suite_create("filter");
	suite_add_tcase(suite, tc1);
	suite_add_tcase(suite, tc2);

	return suite;
}

