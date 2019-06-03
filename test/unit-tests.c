#include <check.h>
#include <stdlib.h>

#include "unit-tests.h"


int main() {

	SRunner *runner = srunner_create(create_filter_suite());
	srunner_run_all(runner, CK_NORMAL);
	int failures = srunner_ntests_failed(runner);
	srunner_free(runner);

	return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}

