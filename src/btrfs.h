/* btrfs.h
 * Generic btrfs header file. Thanks to whoever it was who wrote
 * https://btrfs.wiki.kernel.org/index.php/On-disk_Format - you saved me a lot of time!
 *
 * I release this file, and this file only, into the public domain - do whatever
 * you want with it. You don't have to, but I'd appreciate if you let me know if you
 * use it anything cool - mark@harmstone.com. */

#pragma once

#include <stdint.h>
#include <array>

using le64 = uint64_t;
using le32 = uint32_t;
using le16 = uint16_t;

static const uint64_t superblock_addrs[] = { 0x10000, 0x4000000, 0x4000000000, 0x4000000000000, 0 };

#define BTRFS_MAGIC         0x4d5f53665248425f
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

constexpr uint64_t BTRFS_ROOT_TREE_OBJECTID = 0x1;
constexpr uint64_t BTRFS_EXTENT_TREE_OBJECTID = 0x2;
constexpr uint64_t BTRFS_CHUNK_TREE_OBJECTID = 0x3;
constexpr uint64_t BTRFS_DEV_TREE_OBJECTID = 0x4;
constexpr uint64_t BTRFS_FS_TREE_OBJECTID = 0x5;
constexpr uint64_t BTRFS_ROOT_TREE_DIR_OBJECTID = 0x6;
constexpr uint64_t BTRFS_CSUM_TREE_OBJECTID = 0x7;
constexpr uint64_t BTRFS_UUID_TREE_OBJECTID = 0x9;
constexpr uint64_t BTRFS_EXTENT_CSUM_OBJECTID = 0xfffffffffffffff6;
constexpr uint64_t BTRFS_DATA_RELOC_TREE_OBJECTID = 0xfffffffffffffff7;

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

constexpr uint64_t BTRFS_BLOCK_GROUP_DATA = 1 << 0;
constexpr uint64_t BTRFS_BLOCK_GROUP_SYSTEM = 1 << 1;
constexpr uint64_t BTRFS_BLOCK_GROUP_METADATA = 1 << 2;
constexpr uint64_t BTRFS_BLOCK_GROUP_RAID0 = 1 << 3;
constexpr uint64_t BTRFS_BLOCK_GROUP_RAID1 = 1 << 4;
constexpr uint64_t BTRFS_BLOCK_GROUP_DUP = 1 << 5;
constexpr uint64_t BTRFS_BLOCK_GROUP_RAID10 = 1 << 6;
constexpr uint64_t BTRFS_BLOCK_GROUP_RAID5 = 1 << 7;
constexpr uint64_t BTRFS_BLOCK_GROUP_RAID6 = 1 << 8;
constexpr uint64_t BTRFS_BLOCK_GROUP_RAID1C3 = 1 << 9;
constexpr uint64_t BTRFS_BLOCK_GROUP_RAID1C4 = 1 << 10;

constexpr uint64_t BTRFS_INODE_NODATASUM = 1 << 0;
constexpr uint64_t BTRFS_INODE_NODATACOW = 1 << 1;
constexpr uint64_t BTRFS_INODE_READONLY = 1 << 2;
constexpr uint64_t BTRFS_INODE_NOCOMPRESS = 1 << 3;
constexpr uint64_t BTRFS_INODE_PREALLOC = 1 << 4;
constexpr uint64_t BTRFS_INODE_SYNC = 1 << 5;
constexpr uint64_t BTRFS_INODE_IMMUTABLE = 1 << 6;
constexpr uint64_t BTRFS_INODE_APPEND = 1 << 7;
constexpr uint64_t BTRFS_INODE_NODUMP = 1 << 8;
constexpr uint64_t BTRFS_INODE_NOATIME = 1 << 9;
constexpr uint64_t BTRFS_INODE_DIRSYNC = 1 << 10;
constexpr uint64_t BTRFS_INODE_COMPRESS = 1 << 11;
constexpr uint64_t BTRFS_INODE_ENCRYPT = 1 << 12;
constexpr uint64_t BTRFS_INODE_ROOT_ITEM_INIT = (uint64_t)1 << 31;

#define BTRFS_SUBVOL_READONLY   0x1

constexpr uint64_t BTRFS_FEATURE_INCOMPAT_MIXED_BACKREF = 1 << 0;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_DEFAULT_SUBVOL = 1 << 1;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_MIXED_GROUPS = 1 << 2;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_COMPRESS_LZO = 1 << 3;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_COMPRESS_ZSTD = 1 << 4;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_BIG_METADATA = 1 << 5;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_EXTENDED_IREF = 1 << 6;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_RAID56 = 1 << 7;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_SKINNY_METADATA = 1 << 8;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_NO_HOLES = 1 << 9;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_METADATA_UUID = 1 << 10;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_RAID1C34 = 1 << 11;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_ZONED = 1 << 12;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_EXTENT_TREE_V2 = 1 << 13;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_RAID_STRIPE_TREE = 1 << 14;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_ENCRYPT = 1 << 15;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_SIMPLE_QUOTA = 1 << 16;
constexpr uint64_t BTRFS_FEATURE_INCOMPAT_REMAP_TREE = 1 << 17;

enum class btrfs_csum_type : uint16_t {
    crc32c = 0,
    xxhash = 1,
    sha256 = 2,
    blake2 = 3
};

struct btrfs_uuid {
    uint8_t uuid[16];
} __attribute__((packed));

struct btrfs_key {
    uint64_t objectid;
    btrfs_key_type type;
    uint64_t offset;
} __attribute__((packed));

constexpr uint64_t BTRFS_HEADER_FLAG_WRITTEN = 1 << 0;
constexpr uint64_t BTRFS_HEADER_FLAG_RELOC = 1 << 1;
constexpr uint64_t BTRFS_HEADER_FLAG_MIXED_BACKREF = (uint64_t)1 << 56;

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

struct btrfs_dev_item {
    le64 devid;
    le64 total_bytes;
    le64 bytes_used;
    le32 io_align;
    le32 io_width;
    le32 sector_size;
    le64 type;
    le64 generation;
    le64 start_offset;
    le32 dev_group;
    uint8_t seek_speed;
    uint8_t bandwidth;
    btrfs_uuid uuid;
    btrfs_uuid fsid;
} __attribute__((packed));

struct btrfs_root_backup {
    le64 tree_root;
    le64 tree_root_gen;
    le64 chunk_root;
    le64 chunk_root_gen;
    le64 extent_root;
    le64 extent_root_gen;
    le64 fs_root;
    le64 fs_root_gen;
    le64 dev_root;
    le64 dev_root_gen;
    le64 csum_root;
    le64 csum_root_gen;
    le64 total_bytes;
    le64 bytes_used;
    le64 num_devices;
    le64 unused_64[4];
    uint8_t tree_root_level;
    uint8_t chunk_root_level;
    uint8_t extent_root_level;
    uint8_t fs_root_level;
    uint8_t dev_root_level;
    uint8_t csum_root_level;
    uint8_t unused_8[10];
} __attribute__((packed));

struct btrfs_super_block {
    uint8_t csum[32];
    btrfs_uuid fsid;
    le64 bytenr;
    le64 flags;
    le64 magic;
    le64 generation;
    le64 root;
    le64 chunk_root;
    le64 log_root;
    le64 __unused_log_root_transid;
    le64 total_bytes;
    le64 bytes_used;
    le64 root_dir_objectid;
    le64 num_devices;
    le32 sectorsize;
    le32 nodesize;
    le32 __unused_leafsize;
    le32 stripesize;
    le32 sys_chunk_array_size;
    le64 chunk_root_generation;
    le64 compat_flags;
    le64 compat_ro_flags;
    le64 incompat_flags;
    btrfs_csum_type csum_type;
    uint8_t root_level;
    uint8_t chunk_root_level;
    uint8_t log_root_level;
    btrfs_dev_item dev_item;
    std::array<char, 0x100> label;
    le64 cache_generation;
    le64 uuid_tree_generation;
    btrfs_uuid metadata_uuid;
    le64 nr_global_roots;
    le64 remap_root;
    le64 remap_root_generation;
    uint8_t remap_root_level;
    uint8_t reserved[199];
    std::array<uint8_t, 0x800> sys_chunk_array;
    btrfs_root_backup super_roots[4];
    uint8_t padding[565];
} __attribute__((packed));

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

constexpr uint64_t BTRFS_EXTENT_FLAG_DATA = 1 << 0;
constexpr uint64_t BTRFS_EXTENT_FLAG_TREE_BLOCK = 1 << 1;

struct btrfs_extent_item {
    le64 refs;
    le64 generation;
    le64 flags;
} __attribute__ ((__packed__));

struct btrfs_extent_inline_ref {
    btrfs_key_type type;
    le64 offset;
} __attribute__ ((__packed__));

struct btrfs_extent_data_ref {
    le64 root;
    le64 objectid;
    le64 offset;
    le32 count;
} __attribute__ ((__packed__));

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
