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

static int wt_getattr(const char *path, struct stat *stbuf)
{
    int res;
    res = lstat(path, stbuf);
    if(res == -1)
      return -errno;
    return 0;
}

static int wt_readlink(const char *path, char *buf, size_t size)
{
    int res;
    res = readlink(path, buf, size - 1);
    if(res == -1)
      return -errno;

    buf[res] = '\0';
    return 0;
}

static int wt_getdir(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
    DIR *dp;
    struct dirent *de;
    int res = 0;

    dp = opendir(path);
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

static int wt_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res;
    res = mknod(path, mode, rdev);
    if(res == -1)
      return -errno;
    return 0;
}

static int wt_mkdir(const char *path, mode_t mode)
{
    int res;
    res = mkdir(path, mode);
    if(res == -1)
      return -errno;
    return 0;
}

static int wt_unlink(const char *path)
{
    int res;
    res = unlink(path);
    if(res == -1)
      return -errno;
    return 0;
}

static int wt_rmdir(const char *path)
{
    int res;
    res = rmdir(path);
    if(res == -1)
      return -errno;
    return 0;
}

static int wt_symlink(const char *from, const char *to)
{
    int res;
    res = symlink(from, to);
    if(res == -1)
      return -errno;
    return 0;
}

static int wt_rename(const char *from, const char *to)
{
    int res;
    res = rename(from, to);
    if(res == -1)
      return -errno;
    return 0;
}

static int wt_link(const char *from, const char *to)
{
    int res;
    res = link(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int wt_chmod(const char *path, mode_t mode)
{
    int res;
    res = chmod(path, mode);
    if(res == -1)
        return -errno;
    
    return 0;
}

static int wt_chown(const char *path, uid_t uid, gid_t gid)
{
    int res;
    res = lchown(path, uid, gid);
    if(res == -1)
        return -errno;

    return 0;
}

static int wt_truncate(const char *path, off_t size)
{
    int res;
    res = truncate(path, size);
    if(res == -1)
        return -errno;

    return 0;
}

static int wt_utime(const char *path, struct utimbuf *buf)
{
    int res;
    res = utime(path, buf);
    if(res == -1)
        return -errno;

    return 0;
}

static int wt_open(const char *path, int flags)
{
    int res;

    res = open(path, flags);
    if(res == -1)
      {
	printf("File doesn't appear to exist for opening\n");
        return -errno;
      }
    close(res);
    return 0;
}

static int wt_read(const char *path, char *buf, size_t size, off_t offset)
{
    int fd;
    int res;

    fd = open(path, O_RDONLY);
    if(fd == -1)
        return -errno;
    
    res = pread(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
    
    close(fd);
    return res;
}

static int wt_write(const char *path, const char *buf, size_t size,
                     off_t offset)
{
    int fd;
    int res;

    fd = open(path, O_WRONLY);
    if(fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
    
    close(fd);
    return res;
}

static int wt_statfs(struct fuse_statfs *fst)
{
    struct statfs st;
    int rv = statfs("/",&st);

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

static struct fuse_operations wt_oper = {
    .getattr	= wt_getattr,
    .readlink	= wt_readlink,
    .getdir	= wt_getdir,
    .mknod	= wt_mknod,
    .mkdir	= wt_mkdir,
    .symlink	= wt_symlink,
    .unlink	= wt_unlink,
    .rmdir	= wt_rmdir,
    .rename	= wt_rename,
    .link	= wt_link,
    .chmod	= wt_chmod,
    .chown	= wt_chown,
    .truncate	= wt_truncate,
    .utime	= wt_utime,
    .open	= wt_open,
    .read	= wt_read,
    .write	= wt_write,
    .statfs	= wt_statfs,
    .release	= NULL,
    .fsync	= NULL
};

int main(int argc, char *argv[])
{
  printf("writethrough 0.1 starting up...\n");
  fuse_main(argc, argv, &wt_oper);
  return 0;
}
