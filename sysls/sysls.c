/* ls implemented in system calls

From "strace ls":

open(".", O_RDONLY|O_NONBLOCK|O_LARGEFILE|O_DIRECTORY) = 3
fstat64(3, {st_mode=S_IFDIR|S_ISGID|0755, st_size=4096, ...}) = 0
fcntl64(3, F_SETFD, FD_CLOEXEC)         = 0
getdents64(3, ... 131 entries ..., 4096)  = 4072
getdents64(3, ... 59 entries ..., 4096)   = 2072
getdents64(3, ... 0 entries ..., 4096)    = 0
close(3)                                = 0
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fcntl.h>
#include <errno.h>
#include <syscall.h>

struct stat64 {
	unsigned long long	st_dev;
	unsigned char	__pad0[4];

	unsigned long	__st_ino;

	unsigned int	st_mode;
	unsigned int	st_nlink;

	unsigned long	st_uid;
	unsigned long	st_gid;

	unsigned long long	st_rdev;
	unsigned char	__pad3[4];

	long long	st_size;
	unsigned long	st_blksize;

	unsigned long	st_blocks;
	unsigned long	__pad4;

	unsigned long	st_atime;
	unsigned long	st_atime_nsec;

	unsigned long	st_mtime;
	unsigned int	st_mtime_nsec;

	unsigned long	st_ctime;
	unsigned long	st_ctime_nsec;

	unsigned long long	st_ino;
};

int main(int argc, char *argv[])
{
  unsigned long fd;
  struct stat64 sb;
  
  // long sys_open(const char __user * filename, int flags, int mode)
  printf("open syscall number: %d\n", __NR_open);
  fd = open(".", O_RDONLY|O_NONBLOCK|O_LARGEFILE|O_DIRECTORY, 0);
  printf(" --> got fd = %d\n", fd);
  printf("\n");

  // long sys_fstat64(unsigned long fd, struct stat64 __user * statbuf, long flags)
  printf("fstat64 syscall number: %d\n", __NR_fstat64);
  printf(" --> returned %d\n", fstat64(fd, &sb, 0));

  printf("fcntl64 syscall number: %d\n", __NR_fcntl64);
  printf("getdents64 syscall number: %d\n", __NR_getdents64);
  printf("close syscall number: %d\n", __NR_close);
}
