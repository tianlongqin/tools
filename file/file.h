#ifndef __FILE_HH_10245__
#define __FILE_HH_10245__

#define F_MT		S_IFMT //bit mask for the file type bit field
#define F_SOCK		S_IFSOCK // socket
#define F_LINK		S_IFLNK //symbolic link
#define F_REG		S_IFREG //regular file
#define F_BLK		S_IFBLK // block device
#define F_DIR		S_IFDIR // directory
#define F_CHR		S_IFCHR //character device
#define F_FIFO		S_IFIFO // FIFO
#ifdef __cplusplus 
extern "C" {
#endif

extern int Tfile_check_type(int fd, int type);// Determine whether the file type is fifo

#ifdef __cplusplus 
}
#endif

#endif
