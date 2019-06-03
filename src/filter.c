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

