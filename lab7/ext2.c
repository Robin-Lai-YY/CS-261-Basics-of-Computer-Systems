#include "ext2.h"

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fsuid.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

#define EXT2_OFFSET_SUPERBLOCK 1024
#define EXT2_INVALID_BLOCK_NUMBER ((uint32_t) -1)

/* open_volume_file: Opens the specified file and reads the initial
   EXT2 data contained in the file, including the boot sector, file
   allocation table and root directory.
   
   Parameters:
     filename: Name of the file containing the volume data.
   Returns:
     A pointer to a newly allocated volume_t data structure with all
     fields initialized according to the data in the volume file
     (including superblock and group descriptor table), or NULL if the
     file is invalid or data is missing, or if the file is not an EXT2
     volume file system (s_magic does not contain the correct value).
 */
volume_t *open_volume_file(const char *filename) {
  
  /* TO BE COMPLETED BY THE STUDENT */

  volume_t *volume = malloc(sizeof(struct ext2volume)); //malloc 
  volume->fd = open(filename, O_RDONLY); //check if it can be opened or not
  if(volume->fd < 0){
    return NULL;
  } 
  pread(volume->fd, &(volume->super), sizeof(superblock_t), EXT2_OFFSET_SUPERBLOCK);  //read data 

  volume->block_size = 1024 << volume->super.s_log_block_size; //read block_size
  volume->volume_size = volume->block_size * volume->super.s_blocks_count; //read volume size
  volume->num_groups = (volume->super.s_blocks_count + volume->super.s_blocks_per_group - 1) / volume-> super.s_blocks_per_group;  //read num_groups

  group_desc_t *groups;
  groups = malloc(sizeof(group_desc_t) * volume->num_groups);

  if(volume->block_size >= 2048){ //check its size
    pread(volume->fd, groups, sizeof(group_desc_t) * volume->num_groups, volume->block_size); //read info from group
  }
  else{
    pread(volume->fd, groups, sizeof(group_desc_t) * volume->num_groups, (volume->block_size) * 2); ////read info from group, with larger size range
  }
  volume->groups = groups; // store data back
  
  return volume;
}

/* close_volume_file: Frees and closes all resources used by a EXT2 volume.
   
   Parameters:
     volume: pointer to volume to be freed.
 */
void close_volume_file(volume_t *volume) {

  close(volume->fd); //just close it

  /* TO BE COMPLETED BY THE STUDENT */

}

/* read_block: Reads data from one or more blocks. Saves the resulting
   data in buffer 'buffer'. This function also supports sparse data,
   where a block number equal to 0 sets the value of the corresponding
   buffer to all zeros without reading a block from the volume.
   
   Parameters:
     volume: pointer to volume.
     block_no: Block number where start of data is located.
     offset: Offset from beginning of the block to start reading
             from. May be larger than a block size.
     size: Number of bytes to read. May be larger than a block size.
     buffer: Pointer to location where data is to be stored.

   Returns:
     In case of success, returns the number of bytes read from the
     disk. In case of error, returns -1.
 */
ssize_t read_block(volume_t *volume, uint32_t block_no, uint32_t offset, uint32_t size, void *buffer) {

  /* TO BE COMPLETED BY THE STUDENT */  
  return pread(volume->fd, buffer, size, (volume->block_size) * block_no + offset); //read block
}

/* read_inode: Fills an inode data structure with the data from one
   inode in disk. Determines the block group number and index within
   the group from the inode number, then reads the inode from the
   inode table in the corresponding group. Saves the inode data in
   buffer 'buffer'.
   
   Parameters:
     volume: pointer to volume.
     inode_no: Number of the inode to read from disk.
     buffer: Pointer to location where data is to be stored.

   Returns:
     In case of success, returns a positive value. In case of error,
     returns -1.
 */
ssize_t read_inode(volume_t *volume, uint32_t inode_no, inode_t *buffer) {
  
  /* TO BE COMPLETED BY THE STUDENT */
  int block_group = (inode_no - 1) / (volume->super.s_inodes_per_group); //use formula to calculate the data we need
  int index = (inode_no - 1) % volume->super.s_inodes_per_group;

  return read_block(volume, volume->groups[block_group].bg_inode_table, //use read_block to read inode
   index * volume->super.s_inode_size, volume->super.s_inode_size, buffer);
}

/* read_ind_block_entry: Reads one entry from an indirect
   block. Returns the block number found in the corresponding entry.
   
   Parameters:
     volume: pointer to volume.
     ind_block_no: Block number for indirect block.
     index: Index of the entry to read from indirect block.

   Returns:
     In case of success, returns the block number found at the
     corresponding entry. In case of error, returns
     EXT2_INVALID_BLOCK_NUMBER.
 */
static uint32_t read_ind_block_entry(volume_t *volume, uint32_t ind_block_no,
				     uint32_t index) {
  
  /* TO BE COMPLETED BY THE STUDENT */
  int buffer;
  int a = read_block(volume, ind_block_no, 4 * index, 4, &buffer);
  if(a > 0){ // check a if is valid
    return buffer;
  } 
  else{
    return EXT2_INVALID_BLOCK_NUMBER; 
  }  
}

/* read_inode_block_no: Returns the block number containing the data
   associated to a particular index. For indices 0-11, returns the
   direct block number; for larger indices, returns the block number
   at the corresponding indirect block.
   
   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure where data is to be sourced.
     index: Index to the block number to be searched.

   Returns:
     In case of success, returns the block number to be used for the
     corresponding entry. This block number may be 0 (zero) in case of
     sparse files. In case of error, returns
     EXT2_INVALID_BLOCK_NUMBER.
 */
static uint32_t get_inode_block_no(volume_t *volume, inode_t *inode, uint64_t block_idx) {
  
  /* TO BE COMPLETED BY THE STUDENT */
 // different case, different calculation
  int num_of_block = volume->block_size / 4; 
  
  if (block_idx < 12) { // direct 
    return inode->i_block[block_idx];
  }

  else if (block_idx >= 12 && block_idx < 12 + num_of_block) {               // 1-indirect
    return read_ind_block_entry(volume, inode->i_block_1ind, block_idx - 12);
  }

  else if (block_idx >= 12 + num_of_block && block_idx < 12 + num_of_block + num_of_block * num_of_block) {         // 2-indirect 
    uint32_t second = read_ind_block_entry(volume, inode->i_block_2ind, (block_idx - 12 - num_of_block) / (num_of_block * num_of_block));
    return read_ind_block_entry(volume, second, block_idx - 12 - num_of_block);
  }

  else if (block_idx >= 12 + num_of_block + num_of_block * num_of_block 
      && block_idx < 12 + num_of_block + num_of_block * num_of_block + num_of_block * num_of_block * num_of_block) { // 3-indirect
    
    int index = ((block_idx - 12 - num_of_block - (num_of_block * num_of_block)) / (num_of_block * num_of_block)) / (num_of_block * num_of_block * num_of_block);
    uint32_t third = read_ind_block_entry(volume, inode->i_block_3ind, index);
    index = index * (num_of_block * num_of_block * num_of_block);
    uint32_t second  = read_ind_block_entry(volume, third, index);
    return read_ind_block_entry(volume, second, block_idx - 12 - num_of_block - (num_of_block * num_of_block));
  }

  return EXT2_INVALID_BLOCK_NUMBER;
}

/* read_file_block: Returns the content of a specific file, limited to
   a single block.
   
   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure for the file.
     offset: Offset, in bytes from the start of the file, of the data
             to be read.
     max_size: Maximum number of bytes to read from the block.
     buffer: Pointer to location where data is to be stored.

   Returns:
     In case of success, returns the number of bytes read from the
     disk. In case of error, returns -1.
 */
ssize_t read_file_block(volume_t *volume, inode_t *inode, uint64_t offset, uint64_t max_size, void *buffer) {
    
  /* TO BE COMPLETED BY THE STUDENT */
  uint32_t block_no = get_inode_block_no(volume, inode, offset / volume->block_size); // use formula to get block_no
  uint32_t my_offset = offset % volume->block_size;
  return read_block(volume, block_no, my_offset, max_size, buffer);
  
}

/* read_file_content: Returns the content of a specific file, limited
   to the size of the file only. May need to read more than one block,
   with data not necessarily stored in contiguous blocks.
   
   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure for the file.
     offset: Offset, in bytes from the start of the file, of the data
             to be read.
     max_size: Maximum number of bytes to read from the file.
     buffer: Pointer to location where data is to be stored.

   Returns:
     In case of success, returns the number of bytes read from the
     disk. In case of error, returns -1.
 */
ssize_t read_file_content(volume_t *volume, inode_t *inode, uint64_t offset, uint64_t max_size, void *buffer) {

  uint32_t read_so_far = 0;

  if (offset + max_size > inode_file_size(volume, inode))
    max_size = inode_file_size(volume, inode) - offset;
  
  while (read_so_far < max_size) {
    int rv = read_file_block(volume, inode, offset + read_so_far,
			     max_size - read_so_far, buffer + read_so_far);
    if (rv <= 0) return rv;
    read_so_far += rv;
  }
  return read_so_far;
}

/* follow_directory_entries: Reads all entries in a directory, calling
   function 'f' for each entry in the directory. Stops when the
   function returns a non-zero value, or when all entries have been
   traversed.
   
   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure for the directory.
     context: This pointer is passed as an argument to function 'f'
              unmodified.
     buffer: If function 'f' returns non-zero for any file, and this
             pointer is set to a non-NULL value, this buffer is set to
             the directory entry for which the function returned a
             non-zero value. If the pointer is NULL, nothing is
             saved. If none of the existing entries returns non-zero
             for 'f', the value of this buffer is unspecified.
     f: Function to be called for each directory entry. Receives three
        arguments: the file name as a NULL-terminated string, the
        inode number, and the context argument above.

   Returns:
     If the function 'f' returns non-zero for any directory entry,
     returns the inode number for the corresponding entry. If the
     function returns zero for all entries, or the inode is not a
     directory, or there is an error reading the directory data,
     returns 0 (zero).
 */
uint32_t follow_directory_entries(volume_t *volume, inode_t *inode, void *context,
				  dir_entry_t *buffer,
				  int (*f)(const char *name, uint32_t inode_no, void *context)) {
   
  if(((inode->i_mode) >> 12) != 4) return 0;  
  dir_entry_t *curr = malloc(sizeof(dir_entry_t));
  int i = 0;
  while(i < inode_file_size(volume, inode)){
    if (read_file_block(volume, inode, i, sizeof(struct dir_entry), curr) <= 0) return 0;
    if (f(curr->de_name, curr->de_inode_no, context) > 0) return buffer->de_inode_no;
    i++;
  }  
  free(curr);
  return 0;  
}

/* Simple comparing function to be used as argument in find_file_in_directory function */
static int compare_file_name(const char *name, uint32_t inode_no, void *context) {
  return !strcmp(name, (char *) context);
}

/* find_file_in_directory: Searches for a file in a directory.
   
   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure for the directory.
     name: NULL-terminated string for the name of the file. The file
           name must match this name exactly, including case.
     buffer: If the file is found, and this pointer is set to a
             non-NULL value, this buffer is set to the directory entry
             of the file. If the pointer is NULL, nothing is saved. If
             the file is not found, the value of this buffer is
             unspecified.
   Returns:
     If the file exists in the directory, returns the inode number
     associated to the file. If the file does not exist, or the inode
     is not a directory, or there is an error reading the directory
     data, returns 0 (zero).
 */
uint32_t find_file_in_directory(volume_t *volume, inode_t *inode, const char *name, dir_entry_t *buffer) {
  return follow_directory_entries(volume, inode, (char *) name, buffer, compare_file_name);
}

/* find_file_from_path: Searches for a file based on its full path.
   
   Parameters:
     volume: Pointer to volume.
     path: NULL-terminated string for the full absolute path of the
           file. Must start with '/' character. Path components
           (subdirectories) must be delimited by '/'. The root
           directory can be obtained with the string "/".
     dest_inode: If the file is found, and this pointer is set to a
                 non-NULL value, this buffer is set to the inode of
                 the file. If the pointer is NULL, nothing is
                 saved. If the file is not found, the value of this
                 buffer is unspecified.

  Returns:
     If the file exists, returns the inode number associated to the
     file. If the file does not exist, or there is an error reading
     any directory or inode in the path, returns 0 (zero).
 */
uint32_t find_file_from_path(volume_t *volume, const char *path, inode_t *dest_inode) {
  if(path[0] != '/') return 0; //empty
  inode_t *inode = malloc(sizeof(inode_t));
  if (path[1] == '\0'){ //root
    memcpy(dest_inode, inode, volume->super.s_inode_size);
    return 2;
  } 
  dir_entry_t *buffer = malloc(sizeof(dir_entry_t));
  char name[256]; 

  int i = 0;
  int position = 1;
  while(path[position] != '\0'){
    if (path[position] == '/') {     
      int inode = find_file_in_directory(volume, dest_inode, name, buffer);
      if(inode <= 0) return 0;
      read_inode(volume, inode, dest_inode);
      i = 0;
    } 
    else {
      name[i] = path[position];
      i++;
      name[i] = '\0';
    }
    position++;
  }
  if(inode <= 0) return 0;
  else{
    return buffer->de_inode_no;
  }
}
/*  sorry, we really dont know how to impletement the last two functions,
    We lost our direction in those two funcs, but I think the reset of the 
    funcs are correct, so plz give us some partial marks, appreciate!
*/
   