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

#include "config.h"
#include <string.h>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <optional>
#include <format>
#include <memory>

#ifdef _MSC_VER

#ifdef _M_IX86
#define __i386__
#elif defined(_M_X64)
#define __x86_64__
#endif

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

struct relocation {
    relocation(uint64_t old_start, uint64_t length, uint64_t new_start) : old_start(old_start), length(length), new_start(new_start) { }

    uint64_t old_start;
    uint64_t length;
    uint64_t new_start;
};

static inline uint64_t sector_align(uint64_t v, uint64_t s) {
    return ((v + s - 1) / s) * s;
}

static const uint64_t image_subvol_id = 0x100;
static const char image_filename[] = "ntfs.img";
