/* btrfs.h
 * Generic btrfs header file. Thanks to whoever it was who wrote
 * https://btrfs.wiki.kernel.org/index.php/On-disk_Format - you saved me a lot of time!
 *
 * I release this file, and this file only, into the public domain - do whatever
 * you want with it. You don't have to, but I'd appreciate if you let me know if you
 * use it anything cool - mark@harmstone.com. */

#pragma once

#include <stdint.h>

using le64 = uint64_t;
using le32 = uint32_t;
using le16 = uint16_t;

static const uint64_t superblock_addrs[] = { 0x10000, 0x4000000, 0x4000000000, 0x4000000000000, 0 };

#define BTRFS_MAGIC         0x4d5f53665248425f
#define MAX_LABEL_SIZE      0x100
#define SUBVOL_ROOT_INODE   0x100

enum class btrfs_key_type : uint8_t {
    INODE_ITEM = 0x01,
    INODE_REF = 0x0C,
    INODE_EXTREF = 0x0D,
    XATTR_ITEM = 0x18,
    ORPHAN_INODE = 0x30,
    DIR_ITEM = 0x54,
    DIR_INDEX = 0x60,
    EXTENT_DATA = 0x6C,
    EXTENT_CSUM = 0x80,
    ROOT_ITEM = 0x84,
    ROOT_BACKREF = 0x90,
    ROOT_REF = 0x9C,
    EXTENT_ITEM = 0xA8,
    METADATA_ITEM = 0xA9,
    TREE_BLOCK_REF = 0xB0,
    EXTENT_DATA_REF = 0xB2,
    EXTENT_REF_V0 = 0xB4,
    SHARED_BLOCK_REF = 0xB6,
    SHARED_DATA_REF = 0xB8,
    BLOCK_GROUP_ITEM = 0xC0,
    FREE_SPACE_INFO = 0xC6,
    FREE_SPACE_EXTENT = 0xC7,
    FREE_SPACE_BITMAP = 0xC8,
    DEV_EXTENT = 0xCC,
    DEV_ITEM = 0xD8,
    CHUNK_ITEM = 0xE4,
    TEMP_ITEM = 0xF8,
    DEV_STATS = 0xF9,
    SUBVOL_UUID = 0xFB,
    SUBVOL_REC_UUID = 0xFC
};

#define BTRFS_ROOT_ROOT         1
#define BTRFS_ROOT_EXTENT       2
#define BTRFS_ROOT_CHUNK        3
#define BTRFS_ROOT_DEVTREE      4
#define BTRFS_ROOT_FSTREE       5
#define BTRFS_ROOT_TREEDIR      6
#define BTRFS_ROOT_CHECKSUM     7
#define BTRFS_ROOT_UUID         9
#define BTRFS_ROOT_DATA_RELOC   0xFFFFFFFFFFFFFFF7

enum class btrfs_compression_type : uint8_t {
    none = 0,
    zlib = 1,
    lzo = 2,
    zstd = 3
};

enum class btrfs_file_extent_item_type : uint8_t {
    inline_extent = 0,
    reg = 1,
    prealloc = 2
};

#define BLOCK_FLAG_DATA         0x001
#define BLOCK_FLAG_SYSTEM       0x002
#define BLOCK_FLAG_METADATA     0x004
#define BLOCK_FLAG_RAID0        0x008
#define BLOCK_FLAG_RAID1        0x010
#define BLOCK_FLAG_DUPLICATE    0x020
#define BLOCK_FLAG_RAID10       0x040
#define BLOCK_FLAG_RAID5        0x080
#define BLOCK_FLAG_RAID6        0x100
#define BLOCK_FLAG_RAID1C3      0x200
#define BLOCK_FLAG_RAID1C4      0x400

#define EXTENT_CSUM_ID          0xFFFFFFFFFFFFFFF6

#define BTRFS_INODE_NODATASUM   0x001
#define BTRFS_INODE_NODATACOW   0x002
#define BTRFS_INODE_READONLY    0x004
#define BTRFS_INODE_NOCOMPRESS  0x008
#define BTRFS_INODE_PREALLOC    0x010
#define BTRFS_INODE_SYNC        0x020
#define BTRFS_INODE_IMMUTABLE   0x040
#define BTRFS_INODE_APPEND      0x080
#define BTRFS_INODE_NODUMP      0x100
#define BTRFS_INODE_NOATIME     0x200
#define BTRFS_INODE_DIRSYNC     0x400
#define BTRFS_INODE_COMPRESS    0x800

#define BTRFS_SUBVOL_READONLY   0x1

#define BTRFS_INCOMPAT_FLAGS_MIXED_BACKREF      0x0001
#define BTRFS_INCOMPAT_FLAGS_DEFAULT_SUBVOL     0x0002
#define BTRFS_INCOMPAT_FLAGS_MIXED_GROUPS       0x0004
#define BTRFS_INCOMPAT_FLAGS_COMPRESS_LZO       0x0008
#define BTRFS_INCOMPAT_FLAGS_COMPRESS_ZSTD      0x0010
#define BTRFS_INCOMPAT_FLAGS_BIG_METADATA       0x0020
#define BTRFS_INCOMPAT_FLAGS_EXTENDED_IREF      0x0040
#define BTRFS_INCOMPAT_FLAGS_RAID56             0x0080
#define BTRFS_INCOMPAT_FLAGS_SKINNY_METADATA    0x0100
#define BTRFS_INCOMPAT_FLAGS_NO_HOLES           0x0200
#define BTRFS_INCOMPAT_FLAGS_METADATA_UUID      0x0400
#define BTRFS_INCOMPAT_FLAGS_RAID1C34           0x0800

enum class btrfs_csum_type : uint16_t {
    crc32c = 0,
    xxhash = 1,
    sha256 = 2,
    blake2 = 3
};

#pragma pack(push, 1)

struct btrfs_uuid {
    uint8_t uuid[16];
} __attribute__((packed));

struct btrfs_key {
    uint64_t objectid;
    btrfs_key_type type;
    uint64_t offset;
} __attribute__((packed));

#define HEADER_FLAG_WRITTEN         0x000000000000001
#define HEADER_FLAG_SHARED_BACKREF  0x000000000000002
#define HEADER_FLAG_MIXED_BACKREF   0x100000000000000

struct btrfs_header {
    uint8_t csum[32];
    btrfs_uuid fsid;
    le64 bytenr;
    le64 flags;
    btrfs_uuid chunk_tree_uuid;
    le64 generation;
    le64 owner;
    le32 nritems;
    uint8_t level;
} __attribute__((packed));

struct btrfs_item {
    btrfs_key key;
    le32 offset;
    le32 size;
} __attribute__((packed));

struct btrfs_key_ptr {
    btrfs_key key;
    le64 blockptr;
    le64 generation;
} __attribute__((packed));

typedef struct {
    uint64_t dev_id;
    uint64_t num_bytes;
    uint64_t bytes_used;
    uint32_t optimal_io_align;
    uint32_t optimal_io_width;
    uint32_t minimal_io_size;
    uint64_t type;
    uint64_t generation;
    uint64_t start_offset;
    uint32_t dev_group;
    uint8_t seek_speed;
    uint8_t bandwidth;
    btrfs_uuid device_uuid;
    btrfs_uuid fs_uuid;
} DEV_ITEM;

#define SYS_CHUNK_ARRAY_SIZE 0x800
#define BTRFS_NUM_BACKUP_ROOTS 4

typedef struct {
    uint64_t root_tree_addr;
    uint64_t root_tree_generation;
    uint64_t chunk_tree_addr;
    uint64_t chunk_tree_generation;
    uint64_t extent_tree_addr;
    uint64_t extent_tree_generation;
    uint64_t fs_tree_addr;
    uint64_t fs_tree_generation;
    uint64_t dev_root_addr;
    uint64_t dev_root_generation;
    uint64_t csum_root_addr;
    uint64_t csum_root_generation;
    uint64_t total_bytes;
    uint64_t bytes_used;
    uint64_t num_devices;
    uint64_t reserved[4];
    uint8_t root_level;
    uint8_t chunk_root_level;
    uint8_t extent_root_level;
    uint8_t fs_root_level;
    uint8_t dev_root_level;
    uint8_t csum_root_level;
    uint8_t reserved2[10];
} superblock_backup;

typedef struct {
    uint8_t checksum[32];
    btrfs_uuid uuid;
    uint64_t sb_phys_addr;
    uint64_t flags;
    uint64_t magic;
    uint64_t generation;
    uint64_t root_tree_addr;
    uint64_t chunk_tree_addr;
    uint64_t log_tree_addr;
    uint64_t log_root_transid;
    uint64_t total_bytes;
    uint64_t bytes_used;
    uint64_t root_dir_objectid;
    uint64_t num_devices;
    uint32_t sector_size;
    uint32_t node_size;
    uint32_t leaf_size;
    uint32_t stripe_size;
    uint32_t n;
    uint64_t chunk_root_generation;
    uint64_t compat_flags;
    uint64_t compat_ro_flags;
    uint64_t incompat_flags;
    enum btrfs_csum_type csum_type;
    uint8_t root_level;
    uint8_t chunk_root_level;
    uint8_t log_root_level;
    DEV_ITEM dev_item;
    char label[MAX_LABEL_SIZE];
    uint64_t cache_generation;
    uint64_t uuid_tree_generation;
    uint64_t reserved[30];
    uint8_t sys_chunk_array[SYS_CHUNK_ARRAY_SIZE];
    superblock_backup backup[BTRFS_NUM_BACKUP_ROOTS];
    uint8_t reserved2[565];
} superblock;

enum class btrfs_dir_item_type : uint8_t {
    unknown = 0,
    reg_file = 1,
    dir = 2,
    chrdev = 3,
    blkdev = 4,
    fifo = 5,
    sock = 6,
    symlink = 7,
    xattr = 8,
};

struct btrfs_dir_item {
    btrfs_key location;
    le64 transid;
    le16 data_len;
    le16 name_len;
    btrfs_dir_item_type type;
} __attribute__ ((__packed__));

struct btrfs_timespec {
    le64 sec;
    le32 nsec;
} __attribute__((packed));

struct btrfs_inode_item {
    le64 generation;
    le64 transid;
    le64 size;
    le64 nbytes;
    le64 block_group;
    le32 nlink;
    le32 uid;
    le32 gid;
    le32 mode;
    le64 rdev;
    le64 flags;
    le64 sequence;
    le64 reserved[4];
    btrfs_timespec atime;
    btrfs_timespec ctime;
    btrfs_timespec mtime;
    btrfs_timespec otime;
} __attribute__((packed));

struct btrfs_root_item {
    btrfs_inode_item inode;
    le64 generation;
    le64 root_dirid;
    le64 bytenr;
    le64 byte_limit;
    le64 bytes_used;
    le64 last_snapshot;
    le64 flags;
    le32 refs;
    btrfs_key drop_progress;
    uint8_t drop_level;
    uint8_t level;
    le64 generation_v2;
    btrfs_uuid uuid;
    btrfs_uuid parent_uuid;
    btrfs_uuid received_uuid;
    le64 ctransid;
    le64 otransid;
    le64 stransid;
    le64 rtransid;
    btrfs_timespec ctime;
    btrfs_timespec otime;
    btrfs_timespec stime;
    btrfs_timespec rtime;
    le64 reserved[8];
} __attribute__((packed));

struct btrfs_stripe {
    le64 devid;
    le64 offset;
    btrfs_uuid dev_uuid;
} __attribute__((packed));

struct btrfs_chunk {
    le64 length;
    le64 owner;
    le64 stripe_len;
    le64 type;
    le32 io_align;
    le32 io_width;
    le32 sector_size;
    le16 num_stripes;
    le16 sub_stripes;
    btrfs_stripe stripe[1];
} __attribute__((packed));

struct btrfs_file_extent_item {
    le64 generation;
    le64 ram_bytes;
    btrfs_compression_type compression;
    uint8_t encryption;
    le16 other_encoding;
    btrfs_file_extent_item_type type;
    le64 disk_bytenr;
    le64 disk_num_bytes;
    le64 offset;
    le64 num_bytes;
} __attribute__ ((__packed__));

struct btrfs_inode_ref {
    le64 index;
    le16 name_len;
} __attribute__ ((__packed__));

#define EXTENT_ITEM_DATA            0x001
#define EXTENT_ITEM_TREE_BLOCK      0x002
#define EXTENT_ITEM_SHARED_BACKREFS 0x100

struct btrfs_extent_item {
    le64 refs;
    le64 generation;
    le64 flags;
} __attribute__ ((__packed__));

typedef struct {
    uint64_t offset;
} TREE_BLOCK_REF;

typedef struct {
    uint64_t root;
    uint64_t objid;
    uint64_t offset;
    uint32_t count;
} EXTENT_DATA_REF;

struct btrfs_block_group_item {
    le64 used;
    le64 chunk_objectid;
    le64 flags;
} __attribute__ ((__packed__));

struct btrfs_root_ref {
    le64 dirid;
    le64 sequence;
    le16 name_len;
} __attribute__ ((__packed__));

struct btrfs_dev_extent {
    le64 chunk_tree;
    le64 chunk_objectid;
    le64 chunk_offset;
    le64 length;
    btrfs_uuid chunk_tree_uuid;
} __attribute__ ((__packed__));

#pragma pack(pop)
