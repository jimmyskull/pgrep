#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include "dir.h"
#include "grep.h"

struct params {
	const char *regex;
	struct file *file;
	int size;
	int *result;
};

void *grep_job(void *arg) {
	struct params *params = (struct params *)arg;
	struct file *item = params->file;
	int offset = 0;
	int size = params->size;

	while (size-- && item != NULL) {
		// printf("%s\n", item->path);
		params->result[offset++] = grep(params->regex, item->path);
		item = item->next;
	}
	return NULL;
}

void process_dir(const char *regex, const struct dir *dir, int njobs)
{
	struct file *item;
	int n = dir->nfiles;
	int job_size = ceil(n / njobs) + 1;
	int *result = (int *)malloc(sizeof(int) * n);
	int idx = 0;
	int jobid;
	struct params p[njobs];
	pthread_t jobs[njobs];

	if (result == NULL) {
		fprintf(stderr, "Failed to allocate result space.\n");
		exit(1);
	}

	for (item = dir->head->next; item != NULL; item = item->next, idx++) {
		if (idx % job_size != 0)
			continue;
		jobid = idx / job_size;
		printf("JOBID %d\n", jobid);
		p[jobid].regex = regex;
		p[jobid].file = item;
		p[jobid].size = job_size;
		p[jobid].result = result + idx;
		grep_job(&p[jobid]);

		if (pthread_create(&jobs[jobid], NULL, grep_job, &p[jobid])) {
			fprintf(stderr, "Error creating thread\n");
			exit(1);
		}
	}

	for (jobid = 0; jobid < njobs; jobid++) {
		fprintf(stderr, "Waiting job %d\n", jobid);
		if (pthread_join(jobs[jobid], NULL) != 0) {
			fprintf(stderr, "Error joining thread\n");
			exit(1);
		}
	}

	for (item = dir->head->next, idx = 0;
		item != NULL;
		item = item->next, idx++) {

		printf("%s: %d\n", item->path, result[idx]);
	}

	free(result);
}

int main(int argc, char **argv)
{
	const int njobs = atoi(argv[1]);
	const char *input_regex = argv[2];
	const char *root = argv[3];
	struct dir *dir;

	dir = init_dir(root);
	print_dir(dir);
	process_dir(input_regex, dir, njobs);
	free_dir(dir);
	return EXIT_SUCCESS;
}
