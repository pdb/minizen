#ifndef MINIZEN_FORMATTER_H_
#define MINIZEN_FORMATTER_H_

#include <minizen/db.h>


typedef int (*minizen_af_cb)(struct minizen_db *db, int elements);
typedef int (*minizen_af_object_cb)(struct minizen_db *db, json_object *json,
	int i);


struct minizen_af_data {
	minizen_af_cb header;
	struct {
		minizen_af_object_cb header;
		minizen_af_object_cb body;
	} object;
};


/**
 * A general purpose function for iterating, and likely printing, a JSON array.
 *
 * Assuming n elements in the array (E1, E2, .. En) the callbacks invoked are:
 *
 * 	data->header(db, n)
 *
 * 	data->object.header(db, E1, 1)
 * 	data->object.body(db, E1, 1)
 *
 * 	data->object.header(db, E2, 2)
 * 	data->object.body(db, E2, 2)
 * 	...
 * 	data->object.header(db, En, n)
 * 	data->object.body(db, En, n)
 *
 * Any callback may be specified as NULL in which case it is skipped. This
 * function returns zero unless one of the callbacks return a non-zero value,
 * in which case processing stops and that value returned.
 */
int minizen_array_formatter(struct minizen_db *db, json_object *array,
	struct minizen_af_data *data);


#endif
