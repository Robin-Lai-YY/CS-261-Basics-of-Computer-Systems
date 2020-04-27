#include "ext2.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

/* The FUSE version has to be defined before any call to relevant
   includes related to FUSE. */
#ifndef FUSE_USE_VERSION
#define FUSE_USE_VERSION 26
#endif
#include <fuse.h>

static void *ext2_init(struct fuse_conn_info *conn);
static void ext2_destroy(void *private_data);
static int ext2_getattr(const char *path, struct stat *stbuf);
static int ext2_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			off_t offset, struct fuse_file_info *fi);
static int ext2_open(const char *path, struct fuse_file_info *fi);
static int ext2_release(const char *path, struct fuse_file_info *fi);
static int ext2_read(const char *path, char *buf, size_t size, off_t offset,
		     struct fuse_file_info *fi);
static int ext2_readlink(const char *path, char *buf, size_t size);

static volume_t *volume;

static const struct fuse_operations ext2_operations = {
  .init = ext2_init,
  .destroy = ext2_destroy,
  .open = ext2_open,
  .read = ext2_read,
  .release = ext2_release,
  .getattr = ext2_getattr,
  .readdir = ext2_readdir,
  .readlink = ext2_readlink,
};

int main(int argc, char *argv[]) {
  
  char *volumefile = argv[--argc];
  volume = open_volume_file(volumefile);
  argv[argc] = NULL;
  
  if (!volume) {
    fprintf(stderr, "Invalid volume file: '%s'.\n", volumefile);
    exit(1);
  }
  
  fuse_main(argc, argv, &ext2_operations, volume);
  
  return 0;
}

/* ext2_init: Function called when the FUSE file system is mounted.
 */
static void *ext2_init(struct fuse_conn_info *conn) {
  
  printf("init()\n");
  
  return NULL;
}

/* ext2_destroy: Function called before the FUSE file system is
   unmounted.
 */
static void ext2_destroy(void *private_data) {
  
  printf("destroy()\n");
  
  close_volume_file(volume);
}

/* ext2_getattr: Function called when a process requests the metadata
   of a file. Metadata includes the file type, size, and
   creation/modification dates, among others (check man 2
   fstat). Typically FUSE will call this function before most other
   operations, for the file and all the components of the path.
   
   Parameters:
     path: Path of the file whose metadata is requested.
     stbuf: Pointer to a struct stat where metadata must be stored.
   Returns:
     In case of success, returns 0, and fills the data in stbuf with
     information about the file. If the file does not exist, should
     return -ENOENT.
 */
static int ext2_getattr(const char *path, struct stat *stbuf) {

  /* TO BE COMPLETED BY THE STUDENT */
  
  int sizeof_inode = sizeof(inode_t);
  inode_t *read_inode;
  read_inode = malloc(sizeof_inode);
  if (find_file_from_path(volume, path, read_inode) != 0) {

    stbuf->st_mode = read_inode->i_mode;
    stbuf->st_uid = read_inode->i_uid;
    stbuf->st_size = read_inode->i_size;
    stbuf->st_atime = read_inode->i_atime;
    stbuf->st_ctime = read_inode->i_ctime;
    stbuf->st_mtime = read_inode->i_mtime;
    stbuf->st_gid = read_inode->i_gid;
    stbuf->st_nlink = read_inode->i_links_count;
    stbuf->st_blocks = read_inode->i_blocks;

    stbuf->st_ino = find_file_from_path(volume, path, read_inode);
    stbuf->st_blksize = volume->block_size;
    
//     struct stat {
// 	mode_t			st_mode;       ok
// 	ino_t			st_ino;ok
// 	dev_t			st_dev;
// 	dev_t			st_rdev;
// 	nlink_t			st_nlink;ok
// 	uid_t			st_uid;ok
// 	gid_t			st_gid;ok
// 	off_t			st_size;ok
// 	struct timespec	st_atim;
// 	struct timespec	st_mtim;
// 	struct timespec st_ctim;
// 	blksize_t		st_blksize;
// 	blkcnt_t		st_blocks;
// };

    return 0;
  } 
  else {
    return -ENOENT;
  }
  //return -ENOSYS; // Function not implemented
}

/* ext2_readdir: Function called when a process requests the listing
   of a directory.
   
   Parameters:
     path: Path of the directory whose listing is requested.
     buf: Pointer that must be passed as first parameter to filler
          function.
     filler: Pointer to a function that must be called for every entry
             in the directory.  Will accept four parameters, in this
             order: buf (previous parameter), the filename for the
             entry as a string, a pointer to a struct stat containing
             the metadata of the file (optional, may be passed NULL),
             and an offset for the next call to ext2_readdir
             (optional, may be passed 0).
     offset: Will usually be 0. If a previous call to filler for the
             same path passed a non-zero value as the offset, this
             function will be called again with the provided value as
             the offset parameter. Optional.
     fi: Not used in this implementation of readdir.

   Returns:
     In case of success, returns 0, and calls the filler function for
     each entry in the provided directory. If the directory doesn't
     exist, returns -ENOENT.
 */
static int ext2_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi) {
  int sizeof_inode = sizeof(inode_t);
  inode_t *read_inode;
  read_inode = malloc(sizeof_inode);

  //int (*f)(const char *name, uint32_t inode_no, void *context)
  //int fuse_fill_dir_t(void *buf, const char *name,
				//const struct stat *stbuf, off_t off);



  //？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
  
  int wtfhelper_function(const char *name, uint32_t inode_no, void *context) {
    filler(buf, name, NULL, 0);
    return 0;
  }

  //？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？




  //If the directory doesn't exist, returns -ENOENT.
  
  if (find_file_from_path(volume, path, read_inode) == 0 || (read_inode->i_mode >> 12) != 4||follow_directory_entries(volume, read_inode, NULL, NULL, 0) == 0){
    free(read_inode);
    return -ENOENT;
  }
  //In case of success, returns 0, and calls the filler function for each entry in the provided directory.
  free(read_inode);
  return 0;
  /* TO BE COMPLETED BY THE STUDENT */
  //return -ENOSYS; // Function not implemented
}







/* ext2_open: Function called when a process opens a file in the file
   system.
   
   Parameters:
     path: Path of the file being opened.
     fi: Data structure containing information about the file being
         opened. Some useful fields include:
	 flags: Flags for opening the file. Check 'man 2 open' for
                information about which flags are available.
	 fh: File handle. The value you set to this handle will be
             passed unmodified to all other file operations involving
             the same file.

 */
static int ext2_open(const char *path, struct fuse_file_info *fi) {

  int sizeof_inode = sizeof(inode_t);
  inode_t *read_inode;
  read_inode = malloc(sizeof_inode);



  // If trying to open for writing, fail.
  //   -EACCES: If the open operation was for writing, and the file is read-only.
  if (fi->flags & O_WRONLY || fi->flags & O_RDWR){
    return -EACCES;
  }
    //    -EISDIR: If the path corresponds to a directory;

  if((read_inode->i_mode >> 12) == 4){
    free(read_inode);
    return EISDIR;
  }
  //  In case of success, returns 0. In case of error, may return one
  //  of these error codes:
  else if(find_file_from_path(volume, path, read_inode) != 0){
    fi->fh = find_file_from_path(volume, path, read_inode);
    free(read_inode);
    return 0;
  }
  //    -ENOENT: If the file does not exist;
  /* TO BE COMPLETED BY THE STUDENT */
  return -ENOENT; // Function not implemented
}

/* ext2_release: Function called when a process closes a file in the
   file system. If the open file is shared between processes, this
   function is called when the file has been closed by all processes
   that share it.
   
   Parameters:
     path: Path of the file being closed.
     fi: Data structure containing information about the file being
         opened. This is the same structure used in ext2_open.
   Returns:
     In case of success, returns 0. There is no expected error case.
 */
static int ext2_release(const char *path, struct fuse_file_info *fi) {

  /* TO BE COMPLETED BY THE STUDENT */
  return 0; // Function not implemented
}

/* ext2_read: Function called when a process reads data from a file in
   the file system. This function stores, in array 'buf', up to 'size'
   bytes from the file, starting at offset 'offset'. It may store less
   than 'size' bytes only in case of error or if the file is smaller
   than size+offset.
   
   Parameters:
     path: Path of the open file.
     buf: Pointer where data is expected to be stored.
     size: Maximum number of bytes to be read from the file.
     offset: Byte offset of the first byte to be read from the file.
     fi: Data structure containing information about the file being
         opened. This is the same structure used in ext2_open.

 */
static int ext2_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi) {




  int sizeof_inode = sizeof(inode_t);
  inode_t *read_inode;
  read_inode = malloc(sizeof_inode);

  int whether_found_path = find_file_from_path(volume, path, read_inode);
  int num_bytes_answer = read_file_content(volume, read_inode, offset, size, buf);
  //    -EISDIR: If the path corresponds to a directory;

  if ((read_inode->i_mode >> 12) == 4) {
    free(read_inode);
    return -EISDIR;
  }
  //    -ENOENT: If the file does not exist;
  else if ( whether_found_path == 0) {
    free(read_inode);
    return -ENOENT;
  }
  //  Returns:
  //  In case of success, returns the number of bytes actually read
  //  from the file--which may be smaller than size, or even zero, if
  //  (and only if) offset+size is beyond the end of the file. In case
  //  of error, may return one of these error codes (not all of them
  //  are required):
  else if (num_bytes_answer >= 0){
    free(read_inode);
    return num_bytes_answer;
  }
  //    -EIO: If there was an I/O error trying to obtain the data.
  return -EIO; 

  /* TO BE COMPLETED BY THE STUDENT */
  //return -ENOSYS; // Function not implemented
}

/* ext2_readlink: Function called when FUSE needs to obtain the target
   of a symbolic link. The target is stored in buffer 'buf', which
   stores up to 'size' bytes, as a NULL-terminated string. If the
   target is too long to fit in the buffer, the target should be
   truncated.
   
   Parameters:
     path: Path of the symbolic link file.
     buf: Pointer where symbolic link target is expected to be stored.
     size: Maximum number of bytes to be stored into the buffer,
           including the NULL byte.
   Returns:
     In case of success, returns 0 (zero). In case of error, may
     return one of these error codes (not all of them are required):
       -ENOENT: If the file does not exist;
       -EINVAL: If the path does not correspond to a symbolic link;
       -EIO: If there was an I/O error trying to obtain the data.
 */
static int ext2_readlink(const char *path, char *buf, size_t size) {

  int sizeof_inode = sizeof(inode_t);
  inode_t *read_inode;
  read_inode = malloc(sizeof_inode);

  // readlink(const char* path, char* buf, size_t size)
  //   If path is a symbolic link, fill buf with its target, up to size. 
  //   See readlink(2) for how to handle a too-small buffer and for error codes. 
  //   Not required if you don't support symbolic links.
  //   NOTE: Symbolic-link support requires only readlink and symlink. 
  //   FUSE itself will take care of tracking symbolic links in paths, 
  //   so your path-evaluation code doesn't need to worry about it.

//-ENOENT: If the file does not exist;
  if (find_file_from_path(volume, path, read_inode) == 0){
    return -ENOENT;
  }
  //-EINVAL: If the path does not correspond to a symbolic link;
  else if (read_inode->i_size <= 60){
    return -EINVAL;
  }
  //-EIO: If there was an I/O error trying to obtain the data.

  //try to read the the data
  else if(read_file_content(volume, read_inode, 0, size, buf) < 0){
    return -EIO;
  }
  //no chance...
  else{
    buf[size] = '\0';
    return 0;
  }
  
  

}
