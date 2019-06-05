#ifndef MINIZEN_FILTER_H_
#define MINIZEN_FILTER_H_

#include <json-c/json.h>
#include <stdbool.h>


bool json_value_match(json_object *json_value, const char *search_value);
bool json_object_match(json_object *object, const char *key,
	const char *value);
json_object * json_filter(json_object *root, const char *key,
	const char *value);


#endif
