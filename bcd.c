# include <stdio.h>
# include <sys/stat.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>
# include <mdlint.h>
# include <mdl/bci.h>
# define incr_itr(__itr, __amount) __itr+=__amount;

char const* bcit_to_str(mdl_u8_t __type) {
	mdl_u8_t _signed = (__type&_bcit_msigned) == _bcit_msigned;
	switch(__type^(__type&0x3)) {
		case _bcit_void: return "void";
		case _bcit_8l: return _signed?"8l_s":"8l_u";
		case _bcit_16l: return _signed?"16l_s":"16l_u";
		case _bcit_32l: return _signed?"32l_s":"32l_u";
		case _bcit_64l: return _signed?"64l_s":"64l_u";
		case _bcit_addr: return "addr";
	}
	return "unknown";
}

mdl_uint_t bc_read(mdl_u8_t *__itr, mdl_u8_t __type) {
	mdl_uint_t val;
	switch(__type^(__type&0x3)) {
		case _bcit_void: return 0;
		case _bcit_8l:
			val = *(mdl_u8_t*)__itr;
		break;
		case _bcit_16l:
			val = *(mdl_u16_t*)__itr;
		break;
		case _bcit_32l:
			val = *(mdl_u32_t*)__itr;
		break;
		case _bcit_64l:
			val = *(mdl_u64_t*)__itr;
		break;
		case _bcit_addr:
			val = *(bci_addr_t*)__itr;
		break;
	}
	return val;
}

void bcii_print(mdl_u8_t **__itr) {
	printf("bcii_print.\t\t");

	mdl_u8_t type;
	printf("type_%s,\t\t", bcit_to_str((type = **__itr)));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("addr{0x%04x}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_as(mdl_u8_t **__itr) {
	printf("bcii_as,\t\t");

	mdl_u8_t type;
	printf("type_%s,\t\t", bcit_to_str((type = **__itr)));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("addr{0x%04x},\t\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("val{%u}\n", bc_read(*__itr, type));
	incr_itr((*__itr), bcit_sizeof(type));
}

void bcii_mov(mdl_u8_t **__itr) {
	printf("bcii_mov,\t\t");

	printf("type_%s,\t\t", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("dst_addr{0x%04x},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("src_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
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

char const* lop_to_str(mdl_u8_t __lop) {
	switch(__lop) {
		case _bci_lop_xor: return "xor";
		case _bci_lop_or: return "or";
		case _bci_lop_and: return "and";
	}
	return "unknown";
}

void bcii_aop(mdl_u8_t **__itr, bci_flag_t __flags) {
	printf("bcii_aop,\t\t");

	mdl_u8_t type;
	printf("aop_kind_%s,\t\t", aop_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("type_%s,\t\t", bcit_to_str((type = **__itr)));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("dst_addr{0x%04x},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	if (is_flag(__flags, _bcii_aop_fl_pm)) {
		printf("l_val{%u},\t\t", bc_read(*__itr, type));
		incr_itr((*__itr), bcit_sizeof(type));
	} else {
		printf("l_addr{0x%04x},\t\t", *(bci_addr_t*)*__itr);
		incr_itr((*__itr), bcit_sizeof(_bcit_addr));
	}

	if (is_flag(__flags, _bcii_aop_fr_pm)) {
		printf("l_val{%u}\n", bc_read(*__itr, type));
		incr_itr((*__itr), bcit_sizeof(type));
	} else {
		printf("r_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
		incr_itr((*__itr), bcit_sizeof(_bcit_addr));
	}
}

void bcii_lop(mdl_u8_t **__itr, bci_flag_t __flags) {
	printf("bcii_lop,\t\t");

	mdl_u8_t type;
	printf("lop_kind_%s,\t\t", lop_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("type_%s,\t\t", bcit_to_str((type = **__itr)));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("dst_addr{0x%04x},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	if (is_flag(__flags, _bcii_lop_fl_pm)) {
		printf("l_val{%u},\t\t", bc_read(*__itr, type));
		incr_itr((*__itr), bcit_sizeof(type));
	} else {
		printf("l_addr{0x%04x},\t\t", *(bci_addr_t*)*__itr);
		incr_itr((*__itr), bcit_sizeof(_bcit_addr));
	}

	if (is_flag(__flags, _bcii_lop_fr_pm)) {
		printf("l_val{%u}\n", bc_read(*__itr, type));
		incr_itr((*__itr), bcit_sizeof(type));
	} else {
		printf("r_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
		incr_itr((*__itr), bcit_sizeof(_bcit_addr));
	}
}

void bcii_cmp(mdl_u8_t **__itr) {
	printf("bcii_cmp,\t\t");

	printf("l_type_%s,\t\t", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("r_type_%s,\t\t", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("l_addr{0x%04x},\t\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("r_addr{0x%04x},\t\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("dst_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_cjmp(mdl_u8_t **__itr) {
	printf("bcii_cjmp,\t\t");

	printf("cond{%u},\t\t", **__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("jmpm_addr{0x%04x},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("cond_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_jmp(mdl_u8_t **__itr) {
	printf("bcii_jmp,\t\t");

	printf("jmpm_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_dr(mdl_u8_t **__itr) {
	printf("bcii_dr,\t\t");

	printf("type_%s,\t\t", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("src_addr{0x%04x},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("dst_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_conv(mdl_u8_t **__itr) {
	printf("bcii_conv,\t\t");

	printf("to_type_%s,\t\t", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("from_type_%s,\t\t", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("dst_addr{0x%04x},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("src_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_exc(mdl_u8_t **__itr) {
	printf("bcii_exc,\t\t");

	printf("ret_type_%s,\t\t", bcit_to_str(**__itr));
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("ret_addr{0x%04x},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("id_addr{0x%04x},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("arg_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
    incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_eeb_init(mdl_u8_t **__itr) {
	printf("bcii_eeb_init,\t\t");

	printf("blk_c{%u}\n", **__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));
}

void bcii_eeb_put(mdl_u8_t **__itr) {
	printf("bcii_eeb_put,\t\t");

	printf("blk_id{%u},\t\t", **__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_8l));

	printf("b_addr{0x%04x},\t", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));

	printf("e_addr{0x%04x}\n", *(bci_addr_t*)*__itr);
	incr_itr((*__itr), bcit_sizeof(_bcit_addr));
}

void bcii_la(mdl_u8_t **__itr) {
	printf("bcii_la,\t\t");

	printf("addr{0x%04x}\n", *(bci_addr_t*)*__itr);
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
	close(fd);

	mdl_u8_t *itr = bc;
	while(itr < bc+st.st_size) {
		fprintf(stdout, "0x%04x : ", itr-bc);
		mdl_u8_t i = *itr;
		incr_itr(itr, bcit_sizeof(_bcit_8l));

		bci_flag_t flags = *(bci_flag_t*)itr;
		incr_itr(itr, sizeof(bci_flag_t));

		switch(i) {
			case _bcii_lop:
				bcii_lop(&itr, flags);
			break;
			case _bcii_nop:
				printf("bcii_nop.\n");
			break;
			case _bcii_exc:
				bcii_exc(&itr);
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
				printf("bcii_exit,\t\tstatus addr{0x%04x}\n", *(bci_addr_t*)itr);
				incr_itr(itr, sizeof(bci_addr_t));
			break;
			case _bcii_as:
				bcii_as(&itr);
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
				printf("bcii_%s,\t\t", i == _bcii_incr? "incr":"decr");

				printf("type_%s,\t\t", bcit_to_str(*itr));
				incr_itr(itr, bcit_sizeof(_bcit_8l));

				printf("addr{0x%04x},\t\t", *(bci_addr_t*)itr);
				incr_itr(itr, bcit_sizeof(_bcit_addr));

				if (is_flag(flags, _bcii_iod_fbc_addr)) {
					printf("bc_addr{%u}\n", *(bci_addr_t*)itr);
					incr_itr(itr, bcit_sizeof(_bcit_addr));
				} else {
					mdl_u8_t no_bcit;
					printf("no_type_%s,\t\t", bcit_to_str((no_bcit = *itr)));
					incr_itr(itr, bcit_sizeof(_bcit_8l));

					printf("bc{%u}\n", bc_read(itr, no_bcit));
					incr_itr(itr, bcit_sizeof(no_bcit));
				}

				break;
			}
			case _bcii_eeb_init:
				bcii_eeb_init(&itr);
			break;
			case _bcii_eeb_put:
				bcii_eeb_put(&itr);
			break;
			case _bcii_la:
				bcii_la(&itr);
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
