/* Copyright (c) Mark Harmstone 2026
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

module;

#include <stddef.h>
#include <stdint.h>
#include <string>
#include <span>

export module unicode;

using namespace std;

static constexpr size_t utf16_to_utf8_len(u16string_view sv) noexcept {
    size_t ret = 0;

    while (!sv.empty()) {
        if (sv[0] < 0x80)
            ret++;
        else if (sv[0] < 0x800)
            ret += 2;
        else if (sv[0] < 0xd800)
            ret += 3;
        else if (sv[0] < 0xdc00) {
            if (sv.length() < 2 || (sv[1] & 0xdc00) != 0xdc00) {
                ret += 3;
                sv = sv.substr(1);
                continue;
            }

            ret += 4;
            sv = sv.substr(1);
        } else
            ret += 3;

        sv = sv.substr(1);
    }

    return ret;
}

static constexpr void utf16_to_utf8_span(u16string_view sv, span<char> t) noexcept {
    auto ptr = t.begin();

    if (ptr == t.end())
        return;

    while (!sv.empty()) {
        if (sv[0] < 0x80) {
            *ptr = (uint8_t)sv[0];
            ptr++;

            if (ptr == t.end())
                return;
        } else if (sv[0] < 0x800) {
            *ptr = (uint8_t)(0xc0 | (sv[0] >> 6));
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)(0x80 | (sv[0] & 0x3f));
            ptr++;

            if (ptr == t.end())
                return;
        } else if (sv[0] < 0xd800) {
            *ptr = (uint8_t)(0xe0 | (sv[0] >> 12));
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)(0x80 | ((sv[0] >> 6) & 0x3f));
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)(0x80 | (sv[0] & 0x3f));
            ptr++;

            if (ptr == t.end())
                return;
        } else if (sv[0] < 0xdc00) {
            if (sv.length() < 2 || (sv[1] & 0xdc00) != 0xdc00) {
                *ptr = (uint8_t)0xef;
                ptr++;

                if (ptr == t.end())
                    return;

                *ptr = (uint8_t)0xbf;
                ptr++;

                if (ptr == t.end())
                    return;

                *ptr = (uint8_t)0xbd;
                ptr++;

                if (ptr == t.end())
                    return;

                sv = sv.substr(1);
                continue;
            }

            char32_t cp = 0x10000 | ((sv[0] & ~0xd800) << 10) | (sv[1] & ~0xdc00);

            *ptr = (uint8_t)(0xf0 | (cp >> 18));
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)(0x80 | ((cp >> 12) & 0x3f));
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)(0x80 | ((cp >> 6) & 0x3f));
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)(0x80 | (cp & 0x3f));
            ptr++;

            if (ptr == t.end())
                return;

            sv = sv.substr(1);
        } else if (sv[0] < 0xe000) {
            *ptr = (uint8_t)0xef;
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)0xbf;
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)0xbd;
            ptr++;

            if (ptr == t.end())
                return;
        } else {
            *ptr = (uint8_t)(0xe0 | (sv[0] >> 12));
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)(0x80 | ((sv[0] >> 6) & 0x3f));
            ptr++;

            if (ptr == t.end())
                return;

            *ptr = (uint8_t)(0x80 | (sv[0] & 0x3f));
            ptr++;

            if (ptr == t.end())
                return;
        }

        sv = sv.substr(1);
    }
}

export string utf16_to_utf8(u16string_view sv) {
    if (sv.empty())
        return "";

    string ret(utf16_to_utf8_len(sv), 0);

    utf16_to_utf8_span(sv, ret);

    return ret;
}
