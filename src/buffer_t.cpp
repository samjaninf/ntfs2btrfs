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

#include <vector>
#include <stdint.h>

export module buffer_t;

using namespace std;

template<typename T, typename A = allocator<T>>
class default_init_allocator : public A {
public:
    typedef allocator_traits<A> a_t;

    template<typename U>
    struct rebind {
        using other = default_init_allocator<U, typename a_t::template rebind_alloc<U>>;
    };

    using A::A;

    template<typename U>
    void construct(U* ptr) noexcept(is_nothrow_default_constructible<U>::value) {
        ::new(static_cast<void*>(ptr)) U;
    }

    template<typename U, typename...Args>
    void construct(U* ptr, Args&&... args) {
        a_t::construct(static_cast<A&>(*this), ptr, forward<Args>(args)...);
    }
};

export using buffer_t = vector<uint8_t, default_init_allocator<uint8_t>>;
