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
#define BTRFS_ROOT_FREE_SPACE   0xa
#define BTRFS_ROOT_DATA_RELOC   0xFFFFFFFFFFFFFFF7

enum class btrfs_compression : uint8_t {
    none = 0,
    zlib = 1,
    lzo = 2,
    zstd = 3
};

#define BTRFS_ENCRYPTION_NONE   0

#define BTRFS_ENCODING_NONE     0

enum class btrfs_extent_type : uint8_t {
    inline_extent = 0,
    regular = 1,
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

#define FREE_SPACE_CACHE_ID     0xFFFFFFFFFFFFFFF5
#define EXTENT_CSUM_ID          0xFFFFFFFFFFFFFFF6
#define BALANCE_ITEM_ID         0xFFFFFFFFFFFFFFFC

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

#define BTRFS_COMPAT_RO_FLAGS_FREE_SPACE_CACHE          0x1
#define BTRFS_COMPAT_RO_FLAGS_FREE_SPACE_CACHE_VALID    0x2

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

#define BTRFS_SUPERBLOCK_FLAGS_SEEDING   0x100000000

#define BTRFS_ORPHAN_INODE_OBJID         0xFFFFFFFFFFFFFFFB

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

typedef struct {
    uint8_t csum[32];
    btrfs_uuid fs_uuid;
    uint64_t address;
    uint64_t flags;
    btrfs_uuid chunk_tree_uuid;
    uint64_t generation;
    uint64_t tree_id;
    uint32_t num_items;
    uint8_t level;
} tree_header;

typedef struct {
    btrfs_key key;
    uint32_t offset;
    uint32_t size;
} leaf_node;

typedef struct {
    btrfs_key key;
    uint64_t address;
    uint64_t generation;
} internal_node;

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

typedef struct {
    btrfs_inode_item inode;
    uint64_t generation;
    uint64_t objid;
    uint64_t block_number;
    uint64_t byte_limit;
    uint64_t bytes_used;
    uint64_t last_snapshot_generation;
    uint64_t flags;
    uint32_t num_references;
    btrfs_key drop_progress;
    uint8_t drop_level;
    uint8_t root_level;
    uint64_t generation2;
    btrfs_uuid uuid;
    btrfs_uuid parent_uuid;
    btrfs_uuid received_uuid;
    uint64_t ctransid;
    uint64_t otransid;
    uint64_t stransid;
    uint64_t rtransid;
    btrfs_timespec ctime;
    btrfs_timespec otime;
    btrfs_timespec stime;
    btrfs_timespec rtime;
    uint64_t reserved[8];
} ROOT_ITEM;

typedef struct {
    uint64_t size;
    uint64_t root_id;
    uint64_t stripe_length;
    uint64_t type;
    uint32_t opt_io_alignment;
    uint32_t opt_io_width;
    uint32_t sector_size;
    uint16_t num_stripes;
    uint16_t sub_stripes;
} CHUNK_ITEM;

typedef struct {
    uint64_t dev_id;
    uint64_t offset;
    btrfs_uuid dev_uuid;
} CHUNK_ITEM_STRIPE;

typedef struct {
    uint64_t generation;
    uint64_t decoded_size;
    enum btrfs_compression compression;
    uint8_t encryption;
    uint16_t encoding;
    enum btrfs_extent_type type;
    uint8_t data[1];
} EXTENT_DATA;

typedef struct {
    uint64_t address;
    uint64_t size;
    uint64_t offset;
    uint64_t num_bytes;
} EXTENT_DATA2;

typedef struct {
    uint64_t index;
    uint16_t n;
    char name[1];
} INODE_REF;

typedef struct {
    uint64_t dir;
    uint64_t index;
    uint16_t n;
    char name[1];
} INODE_EXTREF;

#define EXTENT_ITEM_DATA            0x001
#define EXTENT_ITEM_TREE_BLOCK      0x002
#define EXTENT_ITEM_SHARED_BACKREFS 0x100

typedef struct {
    uint64_t refcount;
    uint64_t generation;
    uint64_t flags;
} EXTENT_ITEM;

typedef struct {
    btrfs_key firstitem;
    uint8_t level;
} EXTENT_ITEM2;

typedef struct {
    uint32_t refcount;
} EXTENT_ITEM_V0;

typedef struct {
    EXTENT_ITEM extent_item;
    btrfs_key firstitem;
    uint8_t level;
} EXTENT_ITEM_TREE;

typedef struct {
    uint64_t offset;
} TREE_BLOCK_REF;

typedef struct {
    uint64_t root;
    uint64_t objid;
    uint64_t offset;
    uint32_t count;
} EXTENT_DATA_REF;

typedef struct {
    uint64_t used;
    uint64_t chunk_tree;
    uint64_t flags;
} BLOCK_GROUP_ITEM;

typedef struct {
    uint64_t root;
    uint64_t gen;
    uint64_t objid;
    uint32_t count;
} EXTENT_REF_V0;

typedef struct {
    uint64_t offset;
} SHARED_BLOCK_REF;

typedef struct {
    uint64_t offset;
    uint32_t count;
} SHARED_DATA_REF;

static const uint8_t FREE_SPACE_EXTENT = 1;
static const uint8_t FREE_SPACE_BITMAP = 2;

typedef struct {
    uint64_t offset;
    uint64_t size;
    uint8_t type;
} FREE_SPACE_ENTRY;

typedef struct {
    btrfs_key key;
    uint64_t generation;
    uint64_t num_entries;
    uint64_t num_bitmaps;
} FREE_SPACE_ITEM;

typedef struct {
    uint64_t dir;
    uint64_t index;
    uint16_t n;
    char name[1];
} ROOT_REF;

typedef struct {
    uint64_t chunktree;
    uint64_t objid;
    uint64_t address;
    uint64_t length;
    btrfs_uuid chunktree_uuid;
} DEV_EXTENT;

#define BALANCE_FLAGS_DATA          0x1
#define BALANCE_FLAGS_SYSTEM        0x2
#define BALANCE_FLAGS_METADATA      0x4

#define BALANCE_ARGS_FLAGS_PROFILES         0x001
#define BALANCE_ARGS_FLAGS_USAGE            0x002
#define BALANCE_ARGS_FLAGS_DEVID            0x004
#define BALANCE_ARGS_FLAGS_DRANGE           0x008
#define BALANCE_ARGS_FLAGS_VRANGE           0x010
#define BALANCE_ARGS_FLAGS_LIMIT            0x020
#define BALANCE_ARGS_FLAGS_LIMIT_RANGE      0x040
#define BALANCE_ARGS_FLAGS_STRIPES_RANGE    0x080
#define BALANCE_ARGS_FLAGS_CONVERT          0x100
#define BALANCE_ARGS_FLAGS_SOFT             0x200
#define BALANCE_ARGS_FLAGS_USAGE_RANGE      0x400

typedef struct {
    uint64_t profiles;

    union {
            uint64_t usage;
            struct {
                    uint32_t usage_start;
                    uint32_t usage_end;
            } s;
    } u1;

    uint64_t devid;
    uint64_t drange_start;
    uint64_t drange_end;
    uint64_t vrange_start;
    uint64_t vrange_end;
    uint64_t convert;
    uint64_t flags;

    union {
            uint64_t limit;
            struct {
                    uint32_t limit_start;
                    uint32_t limit_end;
            } s;
    } u2;

    uint32_t stripes_start;
    uint32_t stripes_end;
    uint8_t reserved[48];
} BALANCE_ARGS;

typedef struct {
    uint64_t flags;
    BALANCE_ARGS data;
    BALANCE_ARGS metadata;
    BALANCE_ARGS system;
    uint8_t reserved[32];
} BALANCE_ITEM;

#define BTRFS_FREE_SPACE_USING_BITMAPS      1

typedef struct {
    uint32_t count;
    uint32_t flags;
} FREE_SPACE_INFO;

#define BTRFS_DEV_STAT_WRITE_ERRORS          0
#define BTRFS_DEV_STAT_READ_ERRORS           1
#define BTRFS_DEV_STAT_FLUSH_ERRORS          2
#define BTRFS_DEV_STAT_CORRUPTION_ERRORS     3
#define BTRFS_DEV_STAT_GENERATION_ERRORS     4

#define BTRFS_SEND_CMD_SUBVOL          1
#define BTRFS_SEND_CMD_SNAPSHOT        2
#define BTRFS_SEND_CMD_MKFILE          3
#define BTRFS_SEND_CMD_MKDIR           4
#define BTRFS_SEND_CMD_MKNOD           5
#define BTRFS_SEND_CMD_MKFIFO          6
#define BTRFS_SEND_CMD_MKSOCK          7
#define BTRFS_SEND_CMD_SYMLINK         8
#define BTRFS_SEND_CMD_RENAME          9
#define BTRFS_SEND_CMD_LINK           10
#define BTRFS_SEND_CMD_UNLINK         11
#define BTRFS_SEND_CMD_RMDIR          12
#define BTRFS_SEND_CMD_SET_XATTR      13
#define BTRFS_SEND_CMD_REMOVE_XATTR   14
#define BTRFS_SEND_CMD_WRITE          15
#define BTRFS_SEND_CMD_CLONE          16
#define BTRFS_SEND_CMD_TRUNCATE       17
#define BTRFS_SEND_CMD_CHMOD          18
#define BTRFS_SEND_CMD_CHOWN          19
#define BTRFS_SEND_CMD_UTIMES         20
#define BTRFS_SEND_CMD_END            21
#define BTRFS_SEND_CMD_UPDATE_EXTENT  22

#define BTRFS_SEND_TLV_UUID             1
#define BTRFS_SEND_TLV_TRANSID          2
#define BTRFS_SEND_TLV_INODE            3
#define BTRFS_SEND_TLV_SIZE             4
#define BTRFS_SEND_TLV_MODE             5
#define BTRFS_SEND_TLV_UID              6
#define BTRFS_SEND_TLV_GID              7
#define BTRFS_SEND_TLV_RDEV             8
#define BTRFS_SEND_TLV_CTIME            9
#define BTRFS_SEND_TLV_MTIME           10
#define BTRFS_SEND_TLV_ATIME           11
#define BTRFS_SEND_TLV_OTIME           12
#define BTRFS_SEND_TLV_XATTR_NAME      13
#define BTRFS_SEND_TLV_XATTR_DATA      14
#define BTRFS_SEND_TLV_PATH            15
#define BTRFS_SEND_TLV_PATH_TO         16
#define BTRFS_SEND_TLV_PATH_LINK       17
#define BTRFS_SEND_TLV_OFFSET          18
#define BTRFS_SEND_TLV_DATA            19
#define BTRFS_SEND_TLV_CLONE_UUID      20
#define BTRFS_SEND_TLV_CLONE_CTRANSID  21
#define BTRFS_SEND_TLV_CLONE_PATH      22
#define BTRFS_SEND_TLV_CLONE_OFFSET    23
#define BTRFS_SEND_TLV_CLONE_LENGTH    24

#define BTRFS_SEND_MAGIC "btrfs-stream"

typedef struct {
    uint8_t magic[13];
    uint32_t version;
} btrfs_send_header;

typedef struct {
    uint32_t length;
    uint16_t cmd;
    uint32_t csum;
} btrfs_send_command;

typedef struct {
    uint16_t type;
    uint16_t length;
} btrfs_send_tlv;

#pragma pack(pop)
