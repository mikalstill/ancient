/*
  TiVo slice encoder

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

static void usage(void)
{
	printf("usage: writeguide [options]\n");
	printf(" -v     increase verbosity\n");
	printf(" -h     show this help\n");
}

static u8 *buf;
static int buf_len;
static int buf_alloc;
static int rtype;

static char *line_ptr;
static int line_num;

static int match_type(char *s)
{
	int i;
	for (i=0;i<MAX_TYPES;i++) if (strcmp(types[i],s)==0) break;
	if (i == MAX_TYPES) return -1;
	return i;
}

static void parse_error(char *s)
{
	fprintf(stderr,"parse error at line %d: %s\n\t%s\n",
		line_num, s, line_ptr);
	exit(1);
}

static void buf_needed(int n)
{
	if (n + buf_len > buf_alloc) {
		buf_alloc = buf_alloc + n + 4096;
		buf = realloc(buf, buf_alloc);
		if (!buf) parse_error("unable to allocate buffer space");
	}
}

static void push_byte(u8 b)
{
	buf_needed(1);
	buf[buf_len++] = b;
}

static void push_int(int v)
{
	if (abs(v) < (1<<6)) {
		if (v < 0) v = (1<<7) + v;
		push_byte(v);
	} else if (abs(v) < (1<<13)) {
		if (v < 0) v = (1<<14) + v;
		push_byte((v>>8) | 0x80);
		push_byte(v&0xFF);
	} else if (abs(v) < (1<<20)) {
		if (v < 0) v = (1<<21) + v;
		push_byte((v>>16) | 0xc0);
		push_byte((v>>8)&0xFF);
		push_byte(v&0xFF);
	} else if (abs(v) < (1<<27)) {
		if (v < 0) v = (1<<28) + v;
		push_byte((v>>24) | 0xe0);
		push_byte((v>>16)&0xFF);
		push_byte((v>>8)&0xFF);
		push_byte(v&0xFF);
	} else {
		push_byte(0xf0);
		push_byte((v>>24)&0xFF);
		push_byte((v>>16)&0xFF);
		push_byte((v>>8)&0xFF);
		push_byte(v&0xFF);
	}
}

static void push_mem(u8 *p, int n)
{
	buf_needed(n);
	memcpy(&buf[buf_len], p, n);
	buf_len += n;
}

static void start_record(char *line)
{
	char *t1, *t2, *t3, *t4;
	int i;

	t1 = strtok(line,"/");
	t2 = strtok(NULL,"/");
	t3 = strtok(NULL,"/");
	t4 = strtok(NULL,"/");

	if (!t4) parse_error("bad record start");

	buf_len = 0;
	i = match_type(t1);
	if (i == -1) parse_error("unknown record type");

	rtype = i;

	/* skip length */
	buf_needed(4);
	buf_len += 4;

	/* record header */
	push_byte(atoi(t2));	
	push_int(rtype);
	push_int(atoi(t3));
	push_int(atoi(t4));
}

static void push_file(const char *fname, int nparts, int size)
{
	int fd = open(fname, O_RDONLY);
	char buf[8192];
	struct stat st;
	int n;

	if (fd == -1) {
		perror(fname);
		parse_error("can't open data file");
	}
	fstat(fd, &st);

	if (nparts == 0) {
		if (st.st_size % size != 0) {
			parse_error("file is not multiple of 131072");
		}
		nparts = st.st_size / size;
	}

	if (st.st_size != nparts * size) {
		parse_error("file is of wrong size");
	}
	if (nparts == 1) {
		push_int(1);
		push_int(size);
	} else {
		push_int(2);
		push_int(size);
		push_int(nparts);
	}
	while ((n = read(fd, buf, sizeof(buf))) > 0) {
		push_mem(buf, n);
	}
	close(fd);
	
}

static void start_subrecord(char *line)
{
	char *p;
	int v;

	line = strstr(line, "Subrecord ");
	line += 10;

	p = strchr(line, '/');
	if (!p) parse_error("bad subrecord");
	*p++ = 0;

	v = atoi(p);

	rtype = match_type(line);
	if (rtype == -1) parse_error("bad subrecord type");
	
	push_byte(0);
	push_byte(rtype);
	push_int(v);
}


static void add_item(char *line)
{
	int i, j;
	int v1, v2;
	int nparts, fsize;
	char *p, *t1, *t2, *t3, *tok, *q;
	static char *fname;

	p = strchr(line,':');
	*p = 0;
	p += 2;
	while (isspace(line[0])) line++;

	for (i=0;i<MAX_ATTRS;i++) {
		if (strcmp(line,attrs[rtype][i].name) == 0) break;
	}
	if (i == MAX_ATTRS) {
		fprintf(stderr, "unknown attr=[%s]\n", line);
		parse_error("unknown attribute");
	}

	switch (attrs[rtype][i].type) {
	case TYPE_INT:
		for (tok=strtok(p," "); tok; tok=strtok(NULL," ")) {
			push_byte(i<<2 | attrs[rtype][i].type);
			v1 = atoi(tok);
			push_int(v1);
		}
		break;
		
	case TYPE_STRING:
		if (p[0] != '{') parse_error("doesn't look like a string");
		p++;
		p = strdup(p);
		v1 = strlen(p);
		while (!strchr(p,'}')) {
			char line2[1024];
			if (!fgets(line2, sizeof(line2)-1, stdin)) {
				parse_error("error during multi-line string");
			}
			if (line2[strlen(line2)-1] == '\n') {
				line2[strlen(line2)-1] = 0;
			}
			p = realloc(p, v1 + strlen(line2) + 1);
			strcat(p, line2);
			v1 += strlen(line2);
		}

		v1--;
		push_byte(i<<2 | attrs[rtype][i].type);
		push_int(v1);
		push_mem(p, v1);
		if (fname) free(fname);
		fname = strndup(p, v1);
		free(p);
		break;

	case TYPE_OBJECT:
		q=NULL;
		for (tok=strtok_r(p," ", &q); tok; tok=strtok_r(NULL," ", &q)) {
			t1 = strtok(tok," /");
			t2 = strtok(NULL,"/");
			t3 = strtok(NULL,"/");

			j = match_type(t1);
			if (j == -1) parse_error("unknown link type");
			push_byte(i<<2 | attrs[rtype][i].type);
			push_int(j);
			v1 = atoi(t2);
			push_int(v1);
			if (t3) {
				v2 = atoi(t3);
				push_int(v2);
			}
		}
		break;

	case TYPE_FILE:
		sscanf(p, "File of size %d/%d", &nparts, &fsize);
		push_byte(i<<2 | attrs[rtype][i].type);
		push_file(fname, nparts, fsize);
		break;
	}
}

static void end_record(void)
{
	int v;
	push_byte(0);
	v = buf_len-4;
	buf[0] = (v>>24)&0xff;
	buf[1] = (v>>16)&0xff;
	buf[2] = (v>>8)&0xff;
	buf[3] = v&0xff;
	fwrite(buf, 1, buf_len, stdout);
}

static void end_subrecord(void)
{
}

static void guide_encode(FILE *fin)
{
	char line[1024];
	char line2[1024];

	line_ptr = line2;
	
	while (fgets(line, sizeof(line)-1, fin)) {
		line_num++;
		strcpy(line2, line);

		if (line[strlen(line)-1] == '\n') {
			line[strlen(line)-1] = 0;
		}

		if (line[0] == '#' || line[0] == 0) continue;

		if (strncmp(line,"Guide type", 10)==0) {
			u8 type = atoi(line+11);
			fwrite(&type, 1, 1, stdout);
		} else if (line[0] == '}') {
			end_record();
		} else if (strstr(line,"Subrecord ")) {
			start_subrecord(line);
		} else if (strchr(line,':')) {
			add_item(line);
		} else if (line[strlen(line)-1]=='}') {
			end_subrecord();
		} else {
			start_record(line);
		}
	}
}


int main(int argc, char *argv[])
{
	extern char *optarg;
	extern int optind;
	int c;
	char *env;

	while ((c = getopt(argc, argv, "vh")) != -1 ){
		switch (c) {
		case 'v':
			verbose++;
			break;

		case 'h':
		default:
			usage();
			exit(1);
		}
	}

	argc -= optind;
	argv += optind;

	if ((env = getenv("TIVO_SCHEMA"))) {
		load_schema(env);
	} else {
		load_schema("schema.txt");
	}

	guide_encode(stdin);
	return 0;
}


