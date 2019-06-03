#include <check.h>
#include <stdlib.h>


int main() {

	SRunner *runner = srunner_create(suite_create("minizen"));
	srunner_run_all(runner, CK_NORMAL);
	int failures = srunner_ntests_failed(runner);
	srunner_free(runner);

	return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}

