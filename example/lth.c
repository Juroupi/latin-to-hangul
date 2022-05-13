#include <stdio.h>

#include "../latin-to-hangul.h"

int main(int argc, char* argv[]) {

	for (int i = 1; i < argc; i++) {

		long code = latin_to_hangul(argv[i]);

		if (code == 0) {
			printf("?\n");
		}

		else {
			printf("U+%lX\n", code);
		}
	}

	return 0;
}