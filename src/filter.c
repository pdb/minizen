#include <string.h>

#include "filter.h"


/**
 * Tests whether a given JSON value is, or contains, a given search value.
 *
 * In general this returns true if the JSON value stringifies to the given
 * search value, except for:
 * 	- nulls, which only match an empty string (not a literal "null");
 * 	- arrays, which are recursively searched; and
 * 	- objects, which never match.
 */
bool json_value_match(json_object *json_value, const char *search_value) {

	if (json_object_is_type(json_value, json_type_array)) {
		int len = json_object_array_length(json_value);
		for (int i = 0; i < len; i++) {
			json_object *element =
				json_object_array_get_idx(json_value, i);
			if (json_value_match(element, search_value)) {
				return true;
			}
		}
		return false;
	} else if (json_object_get_type(json_value) == json_type_object) {
		return false;
	} else if (json_object_get_type(json_value) == json_type_null) {
		return search_value[0] == '\0';
	} else {
		return strcmp(json_object_get_string(json_value),
			search_value) == 0;
	}
}


/**
 * Tests whether a given JSON object matches the given search criteria; i.e.,
 * whether it contains a property matching the given key name and value.
 */
bool json_object_match(json_object *object, const char *key,
	const char *value) {

	json_object *json_value;
	if (json_object_object_get_ex(object, key, &json_value)) {
		return json_value_match(json_value, value);
#ifdef MISSING_KEY_CONSIDERED_EMPTY
	} else if (value[0] == '\0') {
		return true;
#endif
	} else {
		return false;
	}
}

