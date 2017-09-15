# include <stdio.h>
# include <sys/stat.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>
# include <mdlint.h>
# include <bci.h>
# define incr_itr(__itr, __amount) __itr+=__amount;

char const* bcit_to_str(mdl_u8_t __type) {
	switch(__type) {
		case _bcit_void: return "void";
		case _bcit_w8: return "w8";
		case _bcit_w16: return "w16";
		case _bcit_w32: return "w32";
		case _bcit_w64: return "w64";
		case _bcit_addr: return "addr";
	}
//	printf("got: %u\n", __type);
	return "unknown";
}

mdl_uint_t bc_read(mdl_u8_t *__itr, mdl_u8_t __type) {
	mdl_uint_t val;

	switch(__type) {
		case _bcit_void: return 0;
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
		case _bcit_addr:
			val = *(bci_addr_t*)__itr;
		break;
	}
	return val;
}

void bcii_print(mdl_u8_t **__itr) {
	printf("bcii_print,	");

	mdl_u8_t type;
	printf("type_%s,	", bcit_to_str((type = **__itr)));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("addr{%u}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_exit(mdl_u8_t **__itr) {
	printf("exit\n");
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));
}

void bcii_assign(mdl_u8_t **__itr) {
	printf("bcii_assign,\t");

	mdl_u8_t type;
	printf("type_%s,\t", bcit_to_str((type = **__itr)));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("addr{%u},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("val{%u}\n", bc_read(*__itr, type));
	incr_itr((*__itr), bcit_sizeof(type));
}

void bcii_mov(mdl_u8_t **__itr) {
	printf("bcii_mov,	");

	printf("type_%s,	", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("dst_addr{%u},	", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("src_addr{%u}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
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

void bcii_aop(mdl_u8_t **__itr, bci_flag_t __flags) {
	printf("bcii_aop,	");

	mdl_u8_t aop, type;
	printf("aop_kind_%s,	", aop_to_str((aop = **__itr)));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("type_%s,	", bcit_to_str((type = **__itr)));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("dst_addr{%u},	", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	if (is_flag(__flags, _bcii_aop_fl_pm)) {
		printf("l_val{%u},	", bc_read(*__itr, type));
		incr_itr((*__itr), bcit_sizeof(type));
	} else {
		printf("l_addr{%u},	", *(bci_addr_t*)*__itr);
		incr_itr((*__itr), bcit_sizeof(_bcit_addr));
	}

	if (is_flag(__flags, _bcii_aop_fr_pm)) {
		printf("l_val{%u}\n", bc_read(*__itr, type));
		incr_itr((*__itr), bcit_sizeof(type));
	} else {
		printf("r_addr{%u}\n", *(bci_addr_t*)*__itr);
		incr_itr((*__itr), bcit_sizeof(_bcit_addr));
	}
}

void bcii_cmp(mdl_u8_t **__itr) {
	printf("bcii_cmp,	");

	printf("l_type_%s,	", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("r_type_%s,	", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("l_addr{%u},	", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("r_addr{%u},	", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("dst_addr{%u}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_cjmp(mdl_u8_t **__itr) {
	printf("bcii_cjmp,	");

	printf("cond{%u}, ", **__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("jmpm_addr{%u}, ", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("cond_addr{%u}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_jmp(mdl_u8_t **__itr) {
	printf("bcii_jmp,	");

	printf("jmpm_addr{%u}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_dr(mdl_u8_t **__itr) {
	printf("bcii_dr,	");

	printf("type_%s,   ", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("src_addr{%u},  ", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("dst_addr{%u}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_conv(mdl_u8_t **__itr) {
	printf("bcii_conv,	");

	printf("to_type_%s,  ", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("from_type_%s,   ", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("dst_addr{%u},	", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("src_addr{%u}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_extern_call(mdl_u8_t **__itr) {
	printf("bcii_extern_call,	");

	printf("ret_type_%s,	", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("ret_addr{%u},	", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("id_addr{%u},	", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("arg_addr{%u}\n", *(bci_addr_t*)*__itr);
    incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_eeb_init(mdl_u8_t **__itr) {
	printf("bcii_eeb_init,	");

	printf("blk_c{%u}\n", **__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));
}

void bcii_eeb_put(mdl_u8_t **__itr) {
	printf("bcii_eeb_put,	");

	printf("blk_id{%u}\n", **__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_w8));

	printf("b_addr{%u},	", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("e_addr{%u}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
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
		incr_itr(itr, bcit_sizeof(_bcit_w8));

		bci_flag_t flags = *(bci_flag_t*)itr;
		incr_itr(itr, sizeof(bci_flag_t));

		switch(i) {
			case _bcii_nop:
				printf("bcii_nop\n");
			break;
			case _bcii_extern_call:
				bcii_extern_call(&itr);
			break;
			case _bcii_conv:
				bcii_conv(&itr);
			break;
			case _bcii_dr:
				bcii_dr(&itr);
			break;
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
				bcii_aop(&itr, flags);
			break;
			case _bcii_cmp:
				bcii_cmp(&itr);
			break;
			case _bcii_cjmp:
				bcii_cjmp(&itr);
			break;
			case _bcii_jmp:
				bcii_jmp(&itr);
			break;
			case _bcii_incr: case _bcii_decr: {
				printf("bcii_%s,	", i == _bcii_incr? "incr":"decr");

				printf("type_%s,	", bcit_to_str(*itr));
				incr_itr(itr, bcit_sizeof(_bcit_w8));

				printf("addr{%u},	", *(bci_addr_t*)itr);
				incr_itr(itr, bcit_sizeof(_bcit_addr));

				mdl_u8_t no_bcit;
				printf("no_type_%s,	", bcit_to_str((no_bcit = *itr)));
				incr_itr(itr, bcit_sizeof(_bcit_w8));

				printf("amount{%u}\n", bc_read(itr, no_bcit));
				incr_itr(itr, bcit_sizeof(no_bcit));

				break;
			}
			case _bcii_eeb_init:
				bcii_eeb_init(&itr);
			break;
			case _bcii_eeb_put:
				bcii_eeb_put(&itr);
			break;
			default: {
				mdl_uint_t s;
				itr += (s = bcii_sizeof(itr, 0));
				if (s > 0) break;
				fprintf(stderr, "instruction unknown!! got: %x\n", i);
				return -1;
			}
		}
	}

	return 0;
}
