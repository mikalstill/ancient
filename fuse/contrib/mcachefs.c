/*
    mcachefs -- a caching filesystem to deal with slow filesystem accesses. This
    filesystem assumes that the backing store is fast and local! The filesystem is
    currently read only
    Copyright (C) 2004  Michael Still (mikal@stillhq.com)

    Heavily based on example code that is:
    Copyright (C) 2001  Miklos Szeredi (mszeredi@inf.bme.hu)

    This program can be distributed under the terms of the GNU GPL.
    See the file COPYING.
*/

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/statfs.h>
#include <sys/mman.h>
#include <sys/file.h>

#include "config.h"

#define FUNCTIONCALL 1

char *target;
char *backing;
int verbose;

/**********************************************************************
 Utility functions
**********************************************************************/

static char *mcachefs_makepath(const char *path, char *prefix)
{
  char *newpath;
  int len;

  len = strlen(path) + strlen(prefix) + 1;
  newpath = malloc(len + 1);
  if(newpath == NULL)
    {
      printf("  Failed to convert path\n");
      return NULL;
    }

  snprintf(newpath, len, "%s%s", prefix, path);
  return newpath;
}

static char *mcachefs_makerealpath(const char *path)
{
  return mcachefs_makepath(path, target);
}

static char *mcachefs_makebackingpath(const char *path)
{
  return mcachefs_makepath(path, backing);
}

int mcachefs_fileinbacking(const char *path)
{
  char *backingpath;
  int fd;
  
  backingpath = mcachefs_makebackingpath(path);
  if(backingpath == NULL)
    return 0;

  if((fd = open(backingpath, O_RDONLY)) > 0)
    {
      close(fd);
      return 1;
    }
  return 0;
}

void mcachefs_copytobacking(const char *path)
{
  char *realpath, *backingpath, *victim, *temppath, *tok;
  char *realfile, *backingfile;
  struct stat sb;
  int len, realfd, backingfd;

  /* Get useful paths: this has to happen here so that the client blocks */
  realpath = mcachefs_makerealpath(path);
  if(realpath == NULL)
    {
      printf("Backing store: couldn't make real file path\n");
      return;
    }
  
  backingpath = mcachefs_makebackingpath(path);
  if(backingpath == NULL)
    {
      printf("Backing store: couldn't make backing store file path\n");
      goto copyoutnofds;
    }

  printf("Backing store: copying %s to %s\n", realpath, backingpath);

  /* Does the path in the backing store exist? */
  victim = strdup(backingpath);
  if(victim == NULL)
    {
      printf("Backing store: couldn't make victim store file path\n");
      goto copyoutnofds;
    }
  len = strlen(victim);
  while((len > 0) && (victim[len] != '/'))
    len--;
  victim[len] = 0;
  
  len = strlen(victim) + 1;
  temppath = malloc(len + 1);
  if(!temppath)
    {
      printf("Backing store: could not allocate memory\n");
      free(victim);
      goto copyoutnofds;
    }
  
  tok = strtok(victim, "/");
  snprintf(temppath, len, "/%s", tok);
  while(tok)
    {
      if(stat(temppath, &sb) < 0)
	{
	  // The path element didn't exist
	  mkdir(temppath, S_IRWXU);
	}
      
      if(!S_ISDIR(sb.st_mode))
	{
	  printf("Backing store: %s not a directory\n", temppath);
	  free(victim);
	  free(temppath);
	  goto copyoutnofds;
	}
      
      tok = strtok(NULL, "/");
      if(tok)
	{
	  strncat(temppath, "/", len);
	  strncat(temppath, tok, len);
	}
    }
  free(victim);
  free(temppath);
  
  if ((realfd = open (realpath, O_RDONLY)) < 0)
    {
      printf("Backing store: couldn't open the real file\n");
      goto copyoutnofds;
    }
  
  if ((backingfd = open (backingpath, O_CREAT | O_RDWR, S_IRWXU)) < 0)
    {
      printf("Backing store: couldn't open the backing file\n");
      goto copyoutnobackingfd;
    }

  /* A simple mmap()ed copy -- we might want to one day offer byte
     range copying as well (that would be in the read method)*/
  if(fstat(realfd, &sb) < 0)
    {
      printf("Backing store: couldn't stat the real file\n");
      goto copyout;
    }
  
  if(ftruncate(backingfd, sb.st_size) < 0)
    {
      printf("Backing store: couldn't allocate space for backing data\n");
      goto copyout;
    }
  
  if ((int) (realfile =
       (char *) mmap (NULL, sb.st_size, PROT_READ, 
		      MAP_SHARED, realfd, 0)) == -1)
    {
      printf("Backing store: couldn't mmap the real file\n");
      goto copyout;
    }
  
  if ((int) (backingfile =
       (char *) mmap (NULL, sb.st_size, PROT_WRITE, 
		      MAP_SHARED, backingfd, 0)) == -1)
    {
      printf("Backing store: couldn't mmap the backing file\n");
      goto copyout;
    }
  
  memcpy(backingfile, realfile, sb.st_size);

  if(munmap(realfile, sb.st_size) < 0)
    {
      printf("Backing store: couldn't munmap the real file\n");
      goto copyout;
    }
  
  if(munmap(backingfile, sb.st_size) < 0)
    {
      printf("Backing store: couldn't munmap the backing file\n");
      goto copyout;
    }
  
 copyout:
  close(backingfd);

 copyoutnobackingfd:
  close(realfd);

 copyoutnofds:
  if(backingpath)
    free(backingpath);
  if(realpath)
    free(realpath);

  printf("Backing store: done\n");
  return;
}

/*********************************************************************/

static int mcachefs_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char *realpath;

    printf("mcachefs_getattr(path = %s, stbuf = ...\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }
    
    res = lstat(realpath, stbuf);

    free(realpath);
    if(res == -1)
      return -errno;
    return 0;
}

static int mcachefs_readlink(const char *path, char *buf, size_t size)
{
    int res;
    char *realpath;
    
    printf("mcachefs_readlink(path = %s, buf = ..., size = ...)\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }
    
    res = readlink(realpath, buf, size - 1);

    free(realpath);
    if(res == -1)
      return -errno;

    buf[res] = '\0';
    return 0;
}


static int mcachefs_getdir(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
    DIR *dp;
    struct dirent *de;
    int res = 0;
    char *realpath;

    printf("mcachefs_getdir(path = %s, h = ..., filler = ...)\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }

    dp = opendir(realpath);
    
    free(realpath);
    if(dp == NULL)
      return -errno;

    while((de = readdir(dp)) != NULL) {
        res = filler(h, de->d_name, de->d_type);
        if(res != 0)
            break;
    }

    closedir(dp);
    return res;
}

static int mcachefs_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res;
    char * realpath;

    printf("mcachefs_mknod(path = %s, mode = ***, rdev = ***\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }
    
    res = mknod(realpath, mode, rdev);

    free(realpath);
    if(res == -1)
      return -errno;
    return 0;
}

static int mcachefs_mkdir(const char *path, mode_t mode)
{
    int res;
    char *realpath;

    printf("mcachefs_mkdir(path = %s, mode = ***\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }

    res = mkdir(realpath, mode);
    
    free(realpath);
    if(res == -1)
      return -errno;
    return 0;
}

static int mcachefs_unlink(const char *path)
{
    int res;
    char *realpath;

    printf("mcachefs_unlink(path = %s)\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }

    res = unlink(realpath);

    free(realpath);
    if(res == -1)
      return -errno;
    return 0;
}

static int mcachefs_rmdir(const char *path)
{
    int res;
    char *realpath;

    printf("mcachefs_rmdir(path = %s)\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }

    res = rmdir(realpath);
    
    free(realpath);
    if(res == -1)
      return -errno;
    return 0;
}

static int mcachefs_symlink(const char *from, const char *to)
{
    int res;
    char *realfrom, *realto;

    printf("mcachefs_symlink(path = %s, to = %s\n", from, to);
    realfrom = mcachefs_makerealpath(from);
    if(realfrom == NULL)
      {
	return -ENOMEM;
      }
    realto = mcachefs_makerealpath(to);
    if(realto == NULL)
      {
	free(realfrom);
	return -ENOMEM;
      }

    res = symlink(realfrom, realto);

    free(realfrom);
    free(realto);
    if(res == -1)
      return -errno;
    return 0;
}

static int mcachefs_rename(const char *from, const char *to)
{
    int res;
    char *realfrom, *realto;

    printf("mcachefs_rename(from = %s, to = %s\n", from, to);
    realfrom = mcachefs_makerealpath(from);
    if(realfrom == NULL)
      {
	return -ENOMEM;
      }
    realto = mcachefs_makerealpath(to);
    if(realto == NULL)
      {
	free(realfrom);
	return -ENOMEM;
      }

    res = rename(realfrom, realto);

    free(realfrom);
    free(realto);
    if(res == -1)
      return -errno;
    return 0;
}

static int mcachefs_link(const char *from, const char *to)
{
    int res;
    char *realfrom, *realto;

    printf("mcachefs_link(from = %s, to = %s)\n", from, to);
    realfrom = mcachefs_makerealpath(from);
    if(realfrom == NULL)
      {
	return -ENOMEM;
      }
    realto = mcachefs_makerealpath(to);
    if(realto == NULL)
      {
	free(realfrom);
	return -ENOMEM;
      }

    res = link(realfrom, realto);

    free(realfrom);
    free(realto);
    if(res == -1)
        return -errno;

    return 0;
}

static int mcachefs_chmod(const char *path, mode_t mode)
{
    int res;
    char *realpath;

    printf("mcachefs_chmod(path = %s, mode = ***)\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }

    res = chmod(realpath, mode);

    free(realpath);
    if(res == -1)
        return -errno;
    
    return 0;
}

static int mcachefs_chown(const char *path, uid_t uid, gid_t gid)
{
    int res;
    char *realpath;

    printf("mcachefs_chown(path = %s, uid = ***, gid = ***)\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }

    res = lchown(realpath, uid, gid);

    free(realpath);
    if(res == -1)
        return -errno;

    return 0;
}

static int mcachefs_truncate(const char *path, off_t size)
{
    int res;
    char *realpath;
    
    printf("mcachefs_truncate(path = %s, size = ***)\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }

    res = truncate(realpath, size);

    free(realpath);
    if(res == -1)
        return -errno;

    return 0;
}

static int mcachefs_utime(const char *path, struct utimbuf *buf)
{
    int res;
    char *realpath;
    
    printf("mcachefs_utime(path = %s, buf = ***)\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }

    res = utime(realpath, buf);

    free(realpath);
    if(res == -1)
        return -errno;

    return 0;
}

/* mcachefs_open() is effectively an existance check. We also take the
   opportunity to fire off a copy process to pull the content into the backing
   store. This will hopefully have pulled the data that the client is going
   to ask for before the client needs it. If it hasn't then we're going
   to have to block... */
static int mcachefs_open(const char *path, int flags)
{
    int res;
    char *realpath;

    printf("mcachefs_open(path = %s, flags = ***)\n", path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	printf("Couldn't generate a real path for the open\n");
	return -ENOMEM;
      }

    res = open(realpath, flags);

    free(realpath);
    if(res == -1)
      {
	printf("File doesn't appear to exist for opening\n");
        return -errno;
      }

    close(res);

    /* The file existed -- pull it into the backing store */
    if(!mcachefs_fileinbacking(path))
      {
	printf("Inserting file into cache\n");
	mcachefs_copytobacking(path);
	printf("Finished inserting file into cache\n");
      }
    return 0;
}

static int mcachefs_read(const char *path, char *buf, size_t size, off_t offset)
{
    int fd;
    int res;
    char *backingpath;

    if(verbose > FUNCTIONCALL)
      printf("mcachefs_read(path = %s, buf = ..., size = %d, offset = %d)\n",
	     path, size, offset);

    if(!mcachefs_fileinbacking(path))
      mcachefs_copytobacking(path);

    backingpath = mcachefs_makebackingpath(path);
    if(backingpath == NULL)
      {
	return -ENOMEM;
      }

    fd = open(backingpath, O_RDONLY);

    free(backingpath);
    if(fd == -1)
        return -errno;
    
    res = pread(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
    
    close(fd);
    return res;
}

static int mcachefs_write(const char *path, const char *buf, size_t size,
                     off_t offset)
{
    int fd;
    int res;
    char *realpath;

    printf("mcachefs_write(path = %s, buf = ..., size = ..., offset = ...)\n",
	   path);
    realpath = mcachefs_makerealpath(path);
    if(realpath == NULL)
      {
	return -ENOMEM;
      }

    fd = open(realpath, O_WRONLY);

    free(realpath);
    if(fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
    
    close(fd);
    return res;
}

static int mcachefs_statfs(struct fuse_statfs *fst)
{
    struct statfs st;
    int rv = statfs("/",&st);

    printf("mcachefs_statfs(fst = ...)\n");

    if(!rv) {
    	fst->block_size  = st.f_bsize;
    	fst->blocks      = st.f_blocks;
    	fst->blocks_free = st.f_bavail;
    	fst->files       = st.f_files;
    	fst->files_free  = st.f_ffree;
    	fst->namelen     = st.f_namelen;
    }
    return rv;
}

static int mcachefs_release(const char *path, int flags)
{
    /* Just a stub.  This method is optional and can safely be left
       unimplemented */

    printf("mcachefs_release\n");

    (void) path;
    (void) flags;
    return 0;
}

static int mcachefs_fsync(const char *path, int isdatasync)
{
    /* Just a stub.  This method is optional and can safely be left
       unimplemented */

  printf("mcachefs_sync\n");

    (void) path;
    (void) isdatasync;
    return 0;
}

static struct fuse_operations mcachefs_oper = {
    .getattr	= mcachefs_getattr,
    .readlink	= mcachefs_readlink,
    .getdir	= mcachefs_getdir,
    .mknod	= mcachefs_mknod,
    .mkdir	= mcachefs_mkdir,
    .symlink	= mcachefs_symlink,
    .unlink	= mcachefs_unlink,
    .rmdir	= mcachefs_rmdir,
    .rename	= mcachefs_rename,
    .link	= mcachefs_link,
    .chmod	= mcachefs_chmod,
    .chown	= mcachefs_chown,
    .truncate	= mcachefs_truncate,
    .utime	= mcachefs_utime,
    .open	= mcachefs_open,
    .read	= mcachefs_read,
    .write	= mcachefs_write,
    .statfs	= mcachefs_statfs,
    .release	= mcachefs_release,
    .fsync	= mcachefs_fsync
    
};

int main(int argc, char *argv[])
{
  config_state *cfg;
  char *val;

  cfg = config_open("mcachefs");
  if(!cfg)
    {
      printf("Couldn't open config file\n");
      return 2;
    }
  target = config_getstring(cfg, "target");
  backing = config_getstring(cfg, "backing");
  val = config_getstring(cfg, "verbose");
  if(val)
    verbose = atoi(val);

  printf("Filesystem now serving requests...\n");
  printf("  config = 0x%08x\n", cfg);
  printf("  target = %s\n", target);
  printf("  backing = %s\n", backing);
  printf("  verbosity = %d\n", verbose);
  fuse_main(argc, argv, &mcachefs_oper);
  return 0;
}
