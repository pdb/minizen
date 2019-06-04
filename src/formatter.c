#include "formatter.h"


int minizen_array_formatter(struct minizen_db *db, json_object *array,
	struct minizen_af_data *data) {

	int rc = 0;
	int len = json_object_array_length(array);
	if (data->header) {
		rc = (*data->header)(db, len);
	}

	for (int i = 0; i < len && ! rc; i++) {
		json_object *element = json_object_array_get_idx(array, i);
		if (data->object.header) {
			rc = (*data->object.header)(db, element, i + 1);
			if (rc) break;
		}
		if (data->object.body) {
			rc = (*data->object.body)(db, element, i + 1);
		}
	}

	return rc;
}

