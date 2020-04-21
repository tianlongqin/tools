#ifndef __FILE_HH_10245__
#define __FILE_HH_10245__

#ifdef __cplusplus 
extern "C" {
#endif

extern int Tfile_get_size_fd(int fd);
extern int Tfile_get_size_name(const char *name);
extern int Tfile_get_file_count(const char *path, uint32_t *dcount, uint32_t *fcount, int flags);

#ifdef __cplusplus 
}
#endif

#endif
