#include <fuse.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

static int addfs_getattr(const char *path, struct stat *stbuf)
{
  printf("getattr: %s\n", path);
  memset(stbuf, 0, sizeof(struct stat));
  if(strcmp(path, "/") == 0)
  {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
  }
  else
  {
    stbuf->st_mode = S_IFREG | 0444;
    stbuf->st_nlink = 1;
    stbuf->st_size = 8;
  }
    
  return 0;
}

static int addfs_open(const char *path, int flags)
{
  return 0;
}

static int addfs_read(const char *path, char *buf, size_t size, off_t offset)
{
  int a, b, idx;
  char *tok, *p;

  printf("read: %s\n", path);
  for(idx = strlen(path) - 1; idx > 0; idx--);
  p = (char *) strdup(path + idx + 1);
  a = atoi(strtok(p, "+"));
  b = atoi(strtok(NULL, "+"));
  snprintf(buf, size, "%08d", a + b);
  return strlen(buf);
}

static struct fuse_operations addfs_oper = {
    .getattr	= addfs_getattr,
    .open	= addfs_open,
    .read	= addfs_read
};

int main(int argc, char *argv[])
{
  printf("addfs 0.1 starting up...\n");
  fuse_main(argc, argv, &addfs_oper);
  return 0;
}
