# include <stdio.h>
# include <sys/stat.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>
# include <eint_t.h>
# include <bci.h>

# define incr_itr(__itr, __amount) __itr+=__amount;
char const* bcit_to_str(mdl_u8_t __type) {
	switch(__type) {
		case _bcit_w8: return "w8";
		case _bcit_w16: return "w16";
		case _bcit_w32: return "w32";
		case _bcit_w64: return "w64";
	}
	return "unknown";
}

mdl_uint_t bc_read(mdl_u8_t *__itr, mdl_u8_t __type) {
	mdl_uint_t val;

	switch(__type) {
		case _bcit_w8:
			val = *(mdl_u8_t*)__itr;
		break;
		case _bcit_w16:
			val = *(mdl_u16_t*)__itr;
		break;
		case _bcit_w32:
			val = *(mdl_u32_t*)__itr;
		break;
		case _bcit_w64:
			val = *(mdl_u64_t*)__itr;
		break;
	}

	return val;
}

void bcii_print(mdl_u8_t **__itr) {
	printf("bcii_print,	");

	mdl_u8_t type;
	printf("type: %s,	", bcit_to_str((type = *(mdl_u8_t*)*__itr)));
	incr_itr((*__itr), bci_sizeof(_bcit_w8));

	printf("addr: %u\n", *(mdl_u16_t*)*__itr);
	incr_itr((*__itr), bci_sizeof(_bcit_w16));
}

void bcii_exit(mdl_u8_t **__itr) {
	printf("bci_exit\n");
	incr_itr((*__itr), bci_sizeof(_bcit_w8));
}

void bcii_assign(mdl_u8_t **__itr) {
	printf("bcii_assign,	");

	mdl_u8_t type;
	printf("type: %s,	", bcit_to_str((type = *(mdl_u8_t*)*__itr)));
	incr_itr((*__itr), bci_sizeof(_bcit_w8));

	printf("addr: %u,	", *(mdl_u16_t*)*__itr);
	incr_itr((*__itr), bci_sizeof(_bcit_w16));

	printf("val: %u\n", bc_read(*__itr, type));
	incr_itr((*__itr), bci_sizeof(type));
}

void bcii_mov(mdl_u8_t **__itr) {
	printf("bcii_mov,	");

	mdl_u8_t dest_type, src_type;
	printf("dest_type: %s,	", bcit_to_str((dest_type = *(mdl_u8_t*)*__itr)));
	incr_itr((*__itr), bci_sizeof(_bcit_w8));

	printf("src_type: %s,	", bcit_to_str((src_type = *(mdl_u8_t*)*__itr)));
	incr_itr((*__itr), bci_sizeof(_bcit_w8));

	printf("dest_addr: %u,	", *(mdl_u16_t*)*__itr);
	incr_itr((*__itr), bci_sizeof(_bcit_w16));

	printf("src_addr: %u\n", *(mdl_u16_t*)*__itr);
	incr_itr((*__itr), bci_sizeof(_bcit_w16));
}

char const* aop_to_str(mdl_u8_t __aop) {
	switch(__aop) {
		case _bci_aop_add: return "add";
		case _bci_aop_mul: return "mul";
		case _bci_aop_sub: return "sub";
		case _bci_aop_div: return "div";
	}

	return "unknown";
}

void bcii_aop(mdl_u8_t **__itr) {
	printf("bcii_aop,	");

	mdl_u8_t aop;
	printf("aop_type: %s,	", aop_to_str((aop = *(mdl_u8_t*)*__itr)));
	incr_itr((*__itr), bci_sizeof(_bcit_w8));

	printf("l_type: %s,	", bcit_to_str(*(mdl_u8_t*)*__itr));
	incr_itr((*__itr), bci_sizeof(_bcit_w8));

	printf("r_type: %s,	", bcit_to_str(*(mdl_u8_t*)*__itr));
	incr_itr((*__itr), bci_sizeof(_bcit_w8));

	printf("l_addr: %u,	", *(mdl_u16_t*)*__itr);
	incr_itr((*__itr), bci_sizeof(_bcit_w16));

	printf("r_addr: %u,	", *(mdl_u16_t*)*__itr);
	incr_itr((*__itr), bci_sizeof(_bcit_w16));

	printf("dest_type: %s,	", bcit_to_str(*(mdl_u8_t*)*__itr));
	incr_itr((*__itr), bci_sizeof(_bcit_w8));

	printf("dest_addr: %u\n", *(mdl_u16_t*)*__itr);
	incr_itr((*__itr), bci_sizeof(_bcit_w16));
}

int main(int argc, char const *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "usage: [byte code file]\n");
		return -1;
	}

	char const *path = argv[1];

	int fd;
	if ((fd = open(path, O_RDONLY)) < 0) {
		fprintf(stderr, "failed to open file\n");
		return -1;
	}

	struct stat st;
	if (stat(path, &st) < 0) {
		close(fd);
		fprintf(stderr, "failed to stat file\n");
		return -1;
	}

	mdl_u8_t *bc = (mdl_u8_t*)malloc(st.st_size);
	read(fd, bc, st.st_size);

	for (mdl_u8_t *itr = bc; itr < bc+st.st_size;) {
		mdl_u8_t i = *itr;
		incr_itr(itr, bci_sizeof(_bcit_w8))

		switch(i) {
			case _bcii_print:
				bcii_print(&itr);
			break;
			case _bcii_exit:
				bcii_exit(&itr);
			break;
			case _bcii_assign:
				bcii_assign(&itr);
			break;
			case _bcii_mov:
				bcii_mov(&itr);
			break;
			case _bcii_aop:
				bcii_aop(&itr);
			break;

			default:
				fprintf(stderr, "instruction unknown!!\n");
			return -1;
		}
	}

	return 0;
}
