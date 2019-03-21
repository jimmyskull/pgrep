#include "dir.h"

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct file *new_file(const char *path)
{
	struct file *item = (struct file *)malloc(sizeof(struct file));
	if (item == NULL) {
		fprintf(stderr, "Failed to allocate file struct.\n");
		exit(1);
	}
	strcpy(item->path, path);
	item->next = NULL;
	return item;
}

struct file *listdir(const char *name, struct file *tail)
{
	DIR *dir;
	struct dirent *entry;
	struct file *next;
	char path[1024];

	if (!(dir = opendir(name)))
		return NULL;

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0)
			continue;
		if (strcmp(entry->d_name, "..") == 0)
			continue;
		snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
		if (entry->d_type == DT_DIR) {
			next = listdir(path, tail);
			if (next != NULL)
				tail = next;
		} else {
			tail->next = new_file(path);
			tail = tail->next;
		}
	}
	closedir(dir);
	return tail;
}

struct dir *init_dir(const char *root)
{
	struct dir *dir = (struct dir *)malloc(sizeof(struct dir));
	if (dir == NULL) {
		fprintf(stderr, "Failed to allocate directory struct.\n");
		exit(1);
	}
	strcpy(dir->root, root);
	dir->head = new_file("");
	(void)listdir(root, dir->head);
	update_nfiles(dir);
	return dir;
}

void free_dir(struct dir *dir)
{
	struct file *path = dir->head;
	struct file *next;
	while (path != NULL) {
		next = path->next;
		free(path);
		path = next;
	}
	free(dir);
}

void update_nfiles(struct dir *dir)
{
	struct file *item = dir->head;
	int nfiles = 0;
	while (item != NULL) {
		nfiles++;
		item = item->next;
	}
	dir->nfiles = nfiles;
}

void print_dir(const struct dir *dir)
{
	printf("root: %s\n", dir->root);
	printf("nfiles: %d\n", dir->nfiles);
}
