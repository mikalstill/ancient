/*
  TiVo slice decoder

  Copyright 2000 Andrew Tridgell (tridge@samba.org)

  released under the GNU GPL v2 

  THIS PROGRAM AND DERIVATIVES MUST NOT BE USED TO AVOID
  SUBSCRIBING TO THE TIVO SERVICE IN COUNTRIES WHERE THAT
  SERVICE IS AVAILABLE. 
*/

#include "slice.h"

extern char *types[MAX_TYPES];
extern struct tag attrs[MAX_TYPES][MAX_ATTRS];

static int verbose;
static int save_files;

static void save_file(const char *fname, void *p, int size)
{
	int fd = open(fname, O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if (fd == -1) return;
	write(fd, p, size);
	close(fd);
}

static int decode_int(u8 *p, int *v)
{
	unsigned len=0;
	int leading_ones = 0;
	int i, num_bits;

	while (leading_ones < 6 &&
	       (p[0] & (1<<(7-leading_ones)))) leading_ones++;	
	len = leading_ones+1;
	num_bits = (len*7);
	(*v) = (p[0] & ((1<<(6-leading_ones))-1)) <<(8*leading_ones);
	for (i=1;i<=leading_ones;i++) {
		(*v) |= p[i]<<(8*(leading_ones-i));
	}
	if (p[0] & (1<<(6-leading_ones))) *v = -((1<<(num_bits-1)) - *v);
	
	return len;
}

static int decode_string(u8 *p, char **s)
{
	int ilen, slen;

	ilen = decode_int(p, &slen);

	(*s) = (char *)malloc(slen+1);
	memcpy(*s, p+ilen, slen);
	(*s)[slen] = 0;

	return slen + ilen;
}


static int grab_int(u8 **p)
{
	int v;
	int len = decode_int(*p, &v);

	(*p) += len;
	return v;
}

static u8 grab_byte(u8 **p)
{
	u8 ret = **p;
	(*p)++;
	return ret;
}

static char *grab_string(u8 **p)
{
	char *s;
	int len;
	len = decode_string(*p, &s);
	(*p) += len;
	return s;
}


static int decode_item(u8 *p, int rec_type, int t, int size)
{
	u8 *p0 = p;
	int v, v1, v2, nparts;
	static char *s;
	
	if (verbose > 1) {
		printf("[%02x:%d/%d] ", t, t>>2, t&3);
	}

	if ((t & 3) != attrs[rec_type][t>>2].type) {
		printf("BAD ATTRIBUTE %d ", attrs[rec_type][t>>2].type);
	}

	switch (t & 3) {
	case TYPE_INT:
		v = grab_int(&p);
		printf("%s: %d\n",
		       attrs[rec_type][t>>2].name,
		       v);
		break;
	case TYPE_STRING:
		if (s) free(s);
		s = grab_string(&p);
		printf("%s: {%s}\n",
		       attrs[rec_type][t>>2].name,
		       s);
		if (save_files) {
			rename("tmpfile.dat", s);
		}
		break;
	case TYPE_OBJECT:
		v = grab_int(&p);
		v1 = grab_int(&p);
		if (v1 == 1) {
			v2 = grab_int(&p);
			printf("%s: %s/%d/%d\n",
			       attrs[rec_type][t>>2].name,
			       types[v], v1, v2);
		} else {
			printf("%s: %s/%d\n",
			       attrs[rec_type][t>>2].name,
			       types[v], v1);
		}
		break;
	case TYPE_FILE:
		v1 = grab_int(&p);
		v2 = grab_int(&p);
		nparts = 1;
		if (v1 == 2) {
			nparts = grab_int(&p);
		}
		printf("%s: File of size %d/%d\n",
		       attrs[rec_type][t>>2].name, 
		       nparts, v2);
		if (save_files) {
			save_file(s?s:"tmpfile.dat", p, nparts*v2);
		}
		p += nparts*v2;
		break;
	}

	return (int)(p-p0);
}


static int decode_subrecord(u8 *p, int size)
{
	u8 *p0 = p;
	int rec_type = grab_byte(&p);
	int v = grab_int(&p);
	printf("\tSubrecord %s/%d {\n", types[rec_type], v);
	while (p < p0 + size) {
		int t;
		if (p[0] == 0) break;

		t = grab_byte(&p);
		printf("\t\t");
		p += decode_item(p, rec_type, t, size - (int)(p-p0));
	}
	printf("\t}\n");
	return (int)(p-p0);
}

static void prog_decode(u8 *p, int size)
{
	u8 x1, rec_type;
	int v1, v2;
	u8 *p0 = p;

	x1 = grab_byte(&p);
	rec_type = grab_int(&p);
	v1 = grab_int(&p);
	v2 = grab_int(&p);

	printf("%s/%d/%d/%d {\n", types[rec_type], x1, v1, v2);

	while (p < p0 + size) {
		int t = grab_byte(&p);
		if (t == 0) {
			if (p >= (p0 + size)) break;
			p += decode_subrecord(p, size - (int)(p-p0));
		} else {
			printf("\t");
			p += decode_item(p, rec_type, t, size - (int)(p-p0));
		}
	}

	if (p != p0 + size) {
		printf("ERROR: decoded %d bytes (should be %d)\n", 
		       (int)(p-p0), size);
	}

	printf("}\n\n");
}

static void guide_decode(u8 *p, int size)
{
	int ofs=0;
	printf("Guide type=%d\n\n", *p);
	ofs++;

	while (ofs<size) {
		int len = (p[ofs]<<24)|(p[ofs+1]<<16)|(p[ofs+2]<<8)|p[ofs+3];
		ofs += 4;

		if (verbose > 0) {
			printf("\n# Record at 0x%x of size %d\n", ofs, len);
		}

		prog_decode(p+ofs, len);
		ofs += len;
	}
}

static void usage(void)
{
	printf("usage: readguide [options] <file>\n");
	printf(" -v     increase verbosity\n");
	printf(" -f     save file contents\n");
	printf(" -h     show this help\n");
}

int main(int argc, char *argv[])
{
	u8 *p;
	int fd, c;
	struct stat st;
	int size;	
	extern char *optarg;
	extern int optind;
	char *env;

	while ((c = getopt(argc, argv, "vhf")) != -1 ){
		switch (c) {
		case 'v':
			verbose++;
			break;

		case 'f':
			save_files = 1;
			break;
		case 'h':
		default:
			usage();
			exit(1);
		}
	}

	argc -= optind;
	argv += optind;

	if (argc < 1) {
		printf("usage: readguide <file>\n");
		exit(1);
	}

	if ((env = getenv("TIVO_SCHEMA"))) {
		load_schema(env);
	} else {
		load_schema("schema.txt");
	}

	fd = open(argv[0], O_RDONLY);
	if (fd == -1) {
		perror(argv[0]);
		exit(1);
	}
	fstat(fd, &st);
	size = st.st_size;

	p = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (p == (u8 *)-1) {
		printf("mmap failed\n");
		exit(1);
	}

	setlinebuf(stdout);

	guide_decode(p, size);
	return 0;
}


