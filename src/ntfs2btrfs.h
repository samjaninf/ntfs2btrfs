/* Copyright (c) Mark Harmstone 2020
 *
 * This file is part of ntfs2btrfs.
 *
 * Ntfs2btrfs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public Licence as published by
 * the Free Software Foundation, either version 2 of the Licence, or
 * (at your option) any later version.
 *
 * Ntfs2btrfs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public Licence for more details.
 *
 * You should have received a copy of the GNU General Public Licence
 * along with Ntfs2btrfs. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "btrfs.h"
#include "config.h"
#include <string.h>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <optional>
#include <format>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _MSC_VER

#ifdef _M_IX86
#define __i386__
#elif defined(_M_X64)
#define __x86_64__
#endif

#endif

import buffer_t;

#ifdef _WIN32
class last_error : public std::exception {
public:
    last_error(std::string_view function, int le) {
        std::string nice_msg;

        {
            char* fm;

            if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
                               le, 0, reinterpret_cast<LPSTR>(&fm), 0, nullptr)) {
                try {
                    std::string_view s = fm;

                    while (!s.empty() && (s[s.length() - 1] == u'\r' || s[s.length() - 1] == u'\n')) {
                        s.remove_suffix(1);
                    }

                    nice_msg = s;
                } catch (...) {
                    LocalFree(fm);
                    throw;
                }

                LocalFree(fm);
            }
        }

        msg = std::string(function) + " failed (error " + std::to_string(le) + (!nice_msg.empty() ? (", " + nice_msg) : "") + ").";
    }

    const char* what() const noexcept {
        return msg.c_str();
    }

private:
    std::string msg;
};

class handle_closer {
public:
    typedef HANDLE pointer;

    void operator()(HANDLE h) {
        if (h == INVALID_HANDLE_VALUE)
            return;

        CloseHandle(h);
    }
};

typedef std::unique_ptr<HANDLE, handle_closer> unique_handle;
#endif

struct space {
    space(uint64_t offset, uint64_t length) : offset(offset), length(length) { }

    uint64_t offset;
    uint64_t length;
};

struct chunk {
    chunk(uint64_t offset, uint64_t length, uint64_t disk_start, uint64_t type) : offset(offset), length(length), disk_start(disk_start), type(type) { }

    uint64_t offset;
    uint64_t length;
    uint64_t disk_start;
    uint64_t type;
    std::list<space> space_list;
    bool added = false;
    uint64_t used = 0;
};

struct data_alloc {
    data_alloc(uint64_t offset, uint64_t length, uint64_t inode = 0, uint64_t file_offset = 0, bool relocated = false, bool not_in_img = false) :
    offset(offset), length(length), inode(inode), file_offset(file_offset), relocated(relocated), not_in_img(not_in_img) { }

    uint64_t offset;
    uint64_t length;
    uint64_t inode;
    uint64_t file_offset;
    bool relocated;
    bool not_in_img;
};

static bool inline operator<(const btrfs_key& a, const btrfs_key& b) {
    if (a.objectid < b.objectid)
        return true;
    else if (a.objectid > b.objectid)
        return false;

    if (a.type < b.type)
        return true;
    else if (a.type > b.type)
        return false;

    if (a.offset < b.offset)
        return true;

    return false;
}

class ntfs;

class root {
public:
    root(uint64_t id) : id(id) { }

    void create_trees(root& extent_root, enum btrfs_csum_type csum_type);
    void write_trees(ntfs& dev);

    uint64_t id;
    std::map<btrfs_key, buffer_t> items;
    std::list<buffer_t> trees;
    uint64_t tree_addr;
    uint8_t level;
    uint64_t metadata_size = 0;
    std::list<std::pair<uint64_t, uint8_t>> addresses, old_addresses;
    bool allocations_done = false;
    bool readonly = false;
    std::map<uint64_t, uint64_t> dir_seqs;
    std::map<uint64_t, uint64_t> dir_size;
};

// from sys/stat.h
#define __S_IFMT        0170000 /* These bits determine file type.  */
#define __S_IFDIR       0040000 /* Directory.  */
#define __S_IFCHR       0020000 /* Character device.  */
#define __S_IFBLK       0060000 /* Block device.  */
#define __S_IFREG       0100000 /* Regular file.  */
#define __S_IFIFO       0010000 /* FIFO.  */
#define __S_IFLNK       0120000 /* Symbolic link.  */
#define __S_IFSOCK      0140000 /* Socket.  */
#define __S_ISTYPE(mode, mask)  (((mode) & __S_IFMT) == (mask))

#ifndef S_ISDIR
#define S_ISDIR(mode)    __S_ISTYPE((mode), __S_IFDIR)
#endif

#ifndef S_IRUSR
#define S_IRUSR 0000400
#endif

#ifndef S_IWUSR
#define S_IWUSR 0000200
#endif

#ifndef S_IXUSR
#define S_IXUSR 0000100
#endif

#ifndef S_IRGRP
#define S_IRGRP (S_IRUSR >> 3)
#endif

#ifndef S_IWGRP
#define S_IWGRP (S_IWUSR >> 3)
#endif

#ifndef S_IXGRP
#define S_IXGRP (S_IXUSR >> 3)
#endif

#ifndef S_IROTH
#define S_IROTH (S_IRGRP >> 3)
#endif

#ifndef S_IWOTH
#define S_IWOTH (S_IWGRP >> 3)
#endif

#ifndef S_IXOTH
#define S_IXOTH (S_IXGRP >> 3)
#endif

#ifndef S_ISUID
#define S_ISUID 0004000
#endif

#ifndef S_ISGID
#define S_ISGID 0002000
#endif

#ifndef S_ISVTX
#define S_ISVTX 0001000
#endif

#pragma pack(push,1)

typedef struct {
    btrfs_extent_item extent_item;
    btrfs_extent_inline_ref eir;
} metadata_item;

typedef struct {
    btrfs_extent_item extent_item;
    btrfs_key_type type;
    btrfs_extent_data_ref edr;
} data_item;

typedef struct {
    btrfs_extent_item extent_item;
    btrfs_key_type type1;
    btrfs_extent_data_ref edr1;
    btrfs_key_type type2;
    btrfs_extent_data_ref edr2;
} data_item2;

#pragma pack(pop)

struct relocation {
    relocation(uint64_t old_start, uint64_t length, uint64_t new_start) : old_start(old_start), length(length), new_start(new_start) { }

    uint64_t old_start;
    uint64_t length;
    uint64_t new_start;
};

static inline uint64_t sector_align(uint64_t v, uint64_t s) {
    return ((v + s - 1) / s) * s;
}

template<>
struct std::formatter<enum btrfs_key_type> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();

        if (it != ctx.end() && *it != '}')
            throw format_error("invalid format");

        return it;
    }

    template<typename format_context>
    auto format(enum btrfs_key_type k, format_context& ctx) const {
        switch (k) {
            case btrfs_key_type::INODE_ITEM:
                return std::format_to(ctx.out(), "INODE_ITEM");
            case btrfs_key_type::INODE_REF:
                return std::format_to(ctx.out(), "INODE_REF");
            case btrfs_key_type::INODE_EXTREF:
                return std::format_to(ctx.out(), "INODE_EXTREF");
            case btrfs_key_type::XATTR_ITEM:
                return std::format_to(ctx.out(), "XATTR_ITEM");
            case btrfs_key_type::ORPHAN_INODE:
                return std::format_to(ctx.out(), "ORPHAN_INODE");
            case btrfs_key_type::DIR_ITEM:
                return std::format_to(ctx.out(), "DIR_ITEM");
            case btrfs_key_type::DIR_INDEX:
                return std::format_to(ctx.out(), "DIR_INDEX");
            case btrfs_key_type::EXTENT_DATA:
                return std::format_to(ctx.out(), "EXTENT_DATA");
            case btrfs_key_type::EXTENT_CSUM:
                return std::format_to(ctx.out(), "EXTENT_CSUM");
            case btrfs_key_type::ROOT_ITEM:
                return std::format_to(ctx.out(), "ROOT_ITEM");
            case btrfs_key_type::ROOT_BACKREF:
                return std::format_to(ctx.out(), "ROOT_BACKREF");
            case btrfs_key_type::ROOT_REF:
                return std::format_to(ctx.out(), "ROOT_REF");
            case btrfs_key_type::EXTENT_ITEM:
                return std::format_to(ctx.out(), "EXTENT_ITEM");
            case btrfs_key_type::METADATA_ITEM:
                return std::format_to(ctx.out(), "METADATA_ITEM");
            case btrfs_key_type::TREE_BLOCK_REF:
                return std::format_to(ctx.out(), "TREE_BLOCK_REF");
            case btrfs_key_type::EXTENT_DATA_REF:
                return std::format_to(ctx.out(), "EXTENT_DATA_REF");
            case btrfs_key_type::EXTENT_REF_V0:
                return std::format_to(ctx.out(), "EXTENT_REF_V0");
            case btrfs_key_type::SHARED_BLOCK_REF:
                return std::format_to(ctx.out(), "SHARED_BLOCK_REF");
            case btrfs_key_type::SHARED_DATA_REF:
                return std::format_to(ctx.out(), "SHARED_DATA_REF");
            case btrfs_key_type::BLOCK_GROUP_ITEM:
                return std::format_to(ctx.out(), "BLOCK_GROUP_ITEM");
            case btrfs_key_type::FREE_SPACE_INFO:
                return std::format_to(ctx.out(), "FREE_SPACE_INFO");
            case btrfs_key_type::FREE_SPACE_EXTENT:
                return std::format_to(ctx.out(), "FREE_SPACE_EXTENT");
            case btrfs_key_type::FREE_SPACE_BITMAP:
                return std::format_to(ctx.out(), "FREE_SPACE_BITMAP");
            case btrfs_key_type::DEV_EXTENT:
                return std::format_to(ctx.out(), "DEV_EXTENT");
            case btrfs_key_type::DEV_ITEM:
                return std::format_to(ctx.out(), "DEV_ITEM");
            case btrfs_key_type::CHUNK_ITEM:
                return std::format_to(ctx.out(), "CHUNK_ITEM");
            case btrfs_key_type::TEMP_ITEM:
                return std::format_to(ctx.out(), "TEMP_ITEM");
            case btrfs_key_type::DEV_STATS:
                return std::format_to(ctx.out(), "DEV_STATS");
            case btrfs_key_type::SUBVOL_UUID:
                return std::format_to(ctx.out(), "SUBVOL_UUID");
            case btrfs_key_type::SUBVOL_REC_UUID:
                return std::format_to(ctx.out(), "SUBVOL_REC_UUID");
            default:
                return std::format_to(ctx.out(), "{:x}", (uint8_t)k);
        }
    }
};

static const uint64_t image_subvol_id = 0x100;
static const char image_filename[] = "ntfs.img";

// decomp.cpp
buffer_t lznt1_decompress(std::string_view compdata, uint32_t size);
buffer_t do_lzx_decompress(std::string_view compdata, uint32_t size);
buffer_t do_xpress_decompress(std::string_view compdata, uint32_t size, uint32_t chunk_size);

// sha256.c
extern "C" void calc_sha256(uint8_t* hash, const void* input, size_t len);

// blake2b-ref.c
extern "C" void blake2b(void *out, size_t outlen, const void* in, size_t inlen);

// rollback.cpp
void rollback(const std::string& fn);

// ntfs2btrfs.cpp
std::string utf16_to_utf8(std::u16string_view sv);
