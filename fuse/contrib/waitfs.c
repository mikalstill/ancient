/*
    waitfs -- simulating a very slow filesystem. The filesystem is slow
    only for things which I think would take a lot of bandwidth, like
    reading a writing. Metadata accesses such as stat and readdir are not
    slow.
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
#include <sys/statfs.h>

static int wait_getattr(const char *path, struct stat *stbuf)
{
    int res;

    printf("wait_getattr(path = %s, stbuf = ...)\n", path);

    res = lstat(path, stbuf);
    printf("  S_ISDIR = %d\n", S_ISDIR(stbuf->st_mode));
    if(res == -1)
      return -errno;

    return 0;
}

static int wait_readlink(const char *path, char *buf, size_t size)
{
    int res;

    printf("wait_readlink(path = %s, buf = ..., size = ...)\n", path);

    res = readlink(path, buf, size - 1);
    if(res == -1)
        return -errno;

    buf[res] = '\0';
    return 0;
}


static int wait_getdir(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
    DIR *dp;
    struct dirent *de;
    int res = 0;

    printf("wait_getdir(path = %s, h = ..., filler = ...)\n", path);

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

static int wait_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res;

    printf("wait_mknod\n");

    res = mknod(path, mode, rdev);
    if(res == -1)
        return -errno;

    return 0;
}

static int wait_mkdir(const char *path, mode_t mode)
{
    int res;

    printf("wait_mkdir\n");

    res = mkdir(path, mode);
    if(res == -1)
        return -errno;

    return 0;
}

static int wait_unlink(const char *path)
{
    int res;

    usleep(300);
    printf("wait_unlink\n");

    res = unlink(path);
    if(res == -1)
        return -errno;

    return 0;
}

static int wait_rmdir(const char *path)
{
    int res;

    printf("wait_rmdir\n");

    res = rmdir(path);
    if(res == -1)
        return -errno;

    return 0;
}

static int wait_symlink(const char *from, const char *to)
{
    int res;

    printf("wait_symlink\n");

    res = symlink(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int wait_rename(const char *from, const char *to)
{
    int res;

    usleep(300);
    printf("wait_rename\n");

    res = rename(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int wait_link(const char *from, const char *to)
{
    int res;

    printf("wait_link\n");

    res = link(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int wait_chmod(const char *path, mode_t mode)
{
    int res;

    printf("wait_chmod\n");

    res = chmod(path, mode);
    if(res == -1)
        return -errno;
    
    return 0;
}

static int wait_chown(const char *path, uid_t uid, gid_t gid)
{
    int res;

    printf("wait_chown\n");

    res = lchown(path, uid, gid);
    if(res == -1)
        return -errno;

    return 0;
}

static int wait_truncate(const char *path, off_t size)
{
    int res;
    
    usleep(300);
    printf("wait_truncate\n");

    res = truncate(path, size);
    if(res == -1)
        return -errno;

    return 0;
}

static int wait_utime(const char *path, struct utimbuf *buf)
{
    int res;
    
    printf("wait_utime\n");

    res = utime(path, buf);
    if(res == -1)
        return -errno;

    return 0;
}

/* this is an access, which I will treat as metadata... */
static int wait_open(const char *path, int flags)
{
    int res;

    printf("wait_open\n");

    res = open(path, flags);
    if(res == -1) 
        return -errno;

    close(res);
    return 0;
}

static int wait_read(const char *path, char *buf, size_t size, off_t offset)
{
    int fd;
    int res;

    usleep(300);
    printf("wait_read\n");

    fd = open(path, O_RDONLY);
    if(fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
    
    close(fd);
    return res;
}

static int wait_write(const char *path, const char *buf, size_t size,
                     off_t offset)
{
    int fd;
    int res;

    usleep(300);
    printf("wait_write\n");

    fd = open(path, O_WRONLY);
    if(fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
    
    close(fd);
    return res;
}

static int wait_statfs(struct fuse_statfs *fst)
{
    struct statfs st;
    int rv = statfs("/",&st);

    printf("wait_statfs(fst = ...)\n");

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

static int wait_release(const char *path, int flags)
{
    /* Just a stub.  This method is optional and can safely be left
       unimplemented */

    printf("wait_release\n");

    (void) path;
    (void) flags;
    return 0;
}

static int wait_fsync(const char *path, int isdatasync)
{
    /* Just a stub.  This method is optional and can safely be left
       unimplemented */

  printf("wait_sync\n");

    (void) path;
    (void) isdatasync;
    return 0;
}

static struct fuse_operations wait_oper = {
    .getattr	= wait_getattr,
    .readlink	= wait_readlink,
    .getdir	= wait_getdir,
    .mknod	= wait_mknod,
    .mkdir	= wait_mkdir,
    .symlink	= wait_symlink,
    .unlink	= wait_unlink,
    .rmdir	= wait_rmdir,
    .rename	= wait_rename,
    .link	= wait_link,
    .chmod	= wait_chmod,
    .chown	= wait_chown,
    .truncate	= wait_truncate,
    .utime	= wait_utime,
    .open	= wait_open,
    .read	= wait_read,
    .write	= wait_write,
    .statfs	= wait_statfs,
    .release	= wait_release,
    .fsync	= wait_fsync
    
};

int main(int argc, char *argv[])
{
    printf("Starting up...\n");
    fuse_main(argc, argv, &wait_oper);
    return 0;
}
