#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

void grep(const char *pregex, const char *path)
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	regex_t regex;
	int rc;
	int matches = 0;

	fp = fopen(path, "r");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", path);
		return;
	}

	rc = regcomp(&regex, pregex, 0);
	if (rc != 0) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		rc = regexec(&regex, line, 0, NULL, 0);
		if (rc == 0)
			matches++;
	}
	printf("%s: %d\n", path, matches);

	fclose(fp);
	if (line)
		free(line);

	regfree(&regex);
}
