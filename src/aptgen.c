#include <stdio.h>

int main() {
	FILE *file;
	file = fopen("test5.txt", "w");
	int i;
	for (i = 220; i < 321; i ++) {
		fprintf(file, "%d\n", i);
		fprintf(file, "01 01 1.00 1.00\n-1\n");
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
	fclose(file);
	return 0;
}
