#ifndef _DIR_H_
#define _DIR_H_

#include <linux/limits.h>

struct file {
	char path[PATH_MAX];
	struct file *next;
};

struct dir {
	struct file *head;
	char root[PATH_MAX];
	int nfiles;
};

extern struct dir *init_dir(const char *);

extern void free_dir(struct dir *);

extern void update_nfiles(struct dir *);

extern void print_dir(const struct dir *);

#endif  /* _DIR_H_ */
