#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sepol/policydb/policydb.h>
#include <sepol/policydb/services.h>

#include "HelloWorld.h"

int load_policy(char *filename, policydb_t *policydb, struct policy_file *pf) {
	int fd;
	struct stat sb;
	void *map;
	int ret;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Can't open '%s':  %s\n",
		        filename, strerror(errno));
		return 1;
	}
	if (fstat(fd, &sb) < 0) {
		fprintf(stderr, "Can't stat '%s':  %s\n",
		        filename, strerror(errno));
		return 1;
	}
	map = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
	           fd, 0);
	if (map == MAP_FAILED) {
		fprintf(stderr, "Can't mmap '%s':  %s\n",
		        filename, strerror(errno));
		close(fd);
		return 1;
	}

	policy_file_init(pf);
	pf->type = PF_USE_MEMORY;
	pf->data = map;
	pf->len = sb.st_size;
	if (policydb_init(policydb)) {
		fprintf(stderr, "policydb_init: Out of memory!\n");
		munmap(map, sb.st_size);
		close(fd);
		return 1;
	}
	ret = policydb_read(policydb, pf, 1);
	if (ret) {
		fprintf(stderr, "error(s) encountered while parsing configuration\n");
		munmap(map, sb.st_size);
		close(fd);
		return 1;
	}

	munmap(map, sb.st_size);
	close(fd);
	return 0;
}

__attribute__ ((visibility ("default"))) 
JNIEXPORT void JNICALL Java_HelloWorld_hacksepol(JNIEnv *env, jobject obj)
{
	policydb_t policydb;
	struct policy_file pf, outpf;
	sidtab_t sidtab;
	char *policy = "/data/security/current/sepolicy_orig";
	char *outfile = "/data/security/current/sepolicy";
	FILE *fp;
	char *permissive = "system_server";

	if (load_policy(policy, &policydb, &pf)) {
		fprintf(stderr, "Could not load policy\n");
		return 1;
	}

	type_datum_t *type;
	type = hashtab_search(policydb.p_types.table, permissive);
	if (type == NULL) {
		fprintf(stderr, "type %s does not exist\n", permissive);
		return 2;
	}
	if (ebitmap_set_bit(&policydb.permissive_map, type->s.value, 1)) {
		fprintf(stderr, "Could not set bit in permissive map\n");
		return 1;
	}

	fp = fopen(outfile, "w");
	if (!fp) {
		fprintf(stderr, "Could not open outfile\n");
		return 1;
	}

	policy_file_init(&outpf);
	outpf.type = PF_USE_STDIO;
	outpf.fp = fp;

	if (policydb_write(&policydb, &outpf)) {
		fprintf(stderr, "Could not write policy\n");
		return 1;
	}

	fclose(fp);

	policydb_destroy(&policydb);

	return 0;

}
#if 0
int __attribute__ ((visibility ("default"))) 
hack_sepol(char *s)
{
	policydb_t policydb;
	struct policy_file pf, outpf;
	sidtab_t sidtab;
	const char *policy = "/data/security/current/sepolicy_orig";
	const char *outfile = "/data/security/current/sepolicy";
	FILE *fp;
	char *permissive = s;

	if (load_policy(policy, &policydb, &pf)) {
		fprintf(stderr, "Could not load policy\n");
		return 1;
	}

	type_datum_t *type;
	type = hashtab_search(policydb.p_types.table, permissive);
	if (type == NULL) {
		fprintf(stderr, "type %s does not exist\n", permissive);
		return 2;
	}
	if (ebitmap_set_bit(&policydb.permissive_map, type->s.value, 1)) {
		fprintf(stderr, "Could not set bit in permissive map\n");
		return 1;
	}

	fp = fopen(outfile, "w");
	if (!fp) {
		fprintf(stderr, "Could not open outfile\n");
		return 1;
	}

	policy_file_init(&outpf);
	outpf.type = PF_USE_STDIO;
	outpf.fp = fp;

	if (policydb_write(&policydb, &outpf)) {
		fprintf(stderr, "Could not write policy\n");
		return 1;
	}

	fclose(fp);

	policydb_destroy(&policydb);

	return 0;
}
#endif