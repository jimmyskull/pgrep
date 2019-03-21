#include <stdio.h>
#include <stdlib.h>

#include "dir.h"
#include "grep.h"

void process_dir(const char *regex, const struct dir *dir)
{
	struct file *item;

	for (item = dir->head->next; item != NULL; item = item->next)
		grep(regex, item->path);
}

int main(int argc, char **argv)
{
	const int max_threads = atoi(argv[1]);
	const char *input_regex = argv[2];
	const char *root = argv[3];
	struct dir *dir;

	dir = init_dir(root);
	print_dir(dir);
	process_dir(input_regex, dir);
	free_dir(dir);
	return EXIT_SUCCESS;
}
