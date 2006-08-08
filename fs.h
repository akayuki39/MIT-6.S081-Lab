// on-disk file system format

// second sector
struct superblock{
  int nblocks;
  int ninodes;
};

#define NDIRECT 13

// inodes start at the third sector
// and blocks start at (ninodes * sizeof(dinode) + 511) / 512
struct dinode {
  short type;
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT];
};

#define T_DIR 1
#define T_FILE 2
#define T_DEV 3

#define IPB (512 / sizeof(struct dinode))

#define DIRSIZ 14

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};
