/*************************************
** Tsunagari Tile Engine            **
** vector.h                         **
** Copyright 2017-2020 Paul Merrill **
*************************************/

// **********
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// **********

// BSD 3-Clause License
//
// Copyright (c) 2017, Electronic Arts
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef SRC_UTIL_VECTOR_H_
#define SRC_UTIL_VECTOR_H_

#include "os/c.h"
#include "util/algorithm.h"
#include "util/assert.h"
#include "util/int.h"
#include "util/likely.h"
#include "util/move.h"
#include "util/new.h"
#include "util/noexcept.h"

template<typename T>
class Vector {
 protected:
    T* mpBegin;
    T* mpEnd;
    T* mCapacity;

 protected:
    T*
    DoAllocate(size_t n) noexcept;
    void
    DoFree(T* p) noexcept;
    size_t
    GetNewCapacity(size_t currentCapacity) noexcept;

 public:
    typedef T* iterator;
    typedef const T* const_iterator;

 public:
    Vector() noexcept;
    explicit Vector(size_t n) noexcept;
    Vector(size_t n, const T& value) noexcept;
    Vector(const Vector<T>& x) noexcept;
    Vector(Vector<T>&& x) noexcept;

    ~Vector() noexcept;

    Vector<T>&
    operator=(const Vector<T>& x) noexcept;
    Vector<T>&
    operator=(Vector<T>&& x) noexcept;

    void
    swap(Vector<T>& x) noexcept;

    iterator
    begin() noexcept;
    const_iterator
    begin() const noexcept;

    iterator
    end() noexcept;
    const_iterator
    end() const noexcept;

    bool
    empty() const noexcept;
    size_t
    size() const noexcept;
    size_t
    capacity() const noexcept;

    void
    resize(size_t n) noexcept;
    void
    reserve(size_t n) noexcept;

    T*
    data() noexcept;
    const T*
    data() const noexcept;

    T& operator[](size_t n) noexcept;
    const T& operator[](size_t n) const noexcept;

    T&
    at(size_t n) noexcept;
    const T&
    at(size_t n) const noexcept;

    T&
    front() noexcept;
    const T&
    front() const noexcept;

    T&
    back() noexcept;
    const T&
    back() const noexcept;

    void
    push_back(const T& value) noexcept;
    void
    push_back(T&& value) noexcept;
    void
    push_back_nogrow(const T& value) noexcept;
    void
    push_back_nogrow(T&& value) noexcept;
    void
    pop_back() noexcept;

    template<class... Args>
    iterator
    emplace(iterator position, Args&&... args) noexcept;

    template<class... Args>
    void
    emplace_back(Args&&... args) noexcept;

    iterator
    insert(iterator position, const T& value) noexcept;
    iterator
    insert(iterator position, T&& value) noexcept;

    void
    append(const T* range, size_t n) noexcept;

    iterator
    erase(iterator position) noexcept;
    iterator
    erase(iterator first, iterator last) noexcept;
    // Same as erase, except it doesn't preserve order, but is faster because it
    // simply copies the last item in the vector over the erased position.
    iterator
    erase_unsorted(iterator position) noexcept;

    void
    clear() noexcept;
    // This is a unilateral reset to an initially empty state. No destructors
    // are called, no deallocation occurs.
    void
    reset_lose_memory() noexcept;

 protected:
    void
    DoAssign(const_iterator first, const_iterator last) noexcept;

    template<typename... Args>
    void
    DoInsertValue(iterator position, Args&&... args) noexcept;

    void
    DoInsertValuesEnd(size_t n) noexcept;  // Default constructs n values

    void
    DoInsertValuesEnd(const T* range, size_t n) noexcept;

    template<typename... Args>
    void
    DoInsertValueEnd(Args&&... args) noexcept;

    void
    DoClearCapacity() noexcept;

    void
    DoGrow(size_t n) noexcept;

    void
    DoSwap(Vector<T>& x) noexcept;
};


///////////////////////////////////////////////////////////////////////
// memory helpers
///////////////////////////////////////////////////////////////////////

template<typename T>
inline T*
move(T* first, T* last, T* dest) noexcept {
    for (; first != last; ++first, ++dest) {
        *dest = move_(*first);
    }
    return dest;
}

template<typename T>
inline T*
uninitialized_move(T* first, T* last, T* dest) noexcept {
    for (; first != last; ++dest, ++first) {
        new (static_cast<void*>(dest)) T(move_(*first));
    }
    return dest;
}

template<typename T, typename Count>
inline void
uninitialized_default_fill_n(T* first, Count n) noexcept {
    for (; n > 0; --n, ++first) {
        new (static_cast<void*>(first)) T;
    }
}

template<typename T>
inline void
destruct(T* first, T* last) noexcept {
    for (; first != last; ++first) {
        first->~T();
    }
}

template<typename T>
inline T*
uninitialized_copy(T* first, T* last, T* dest) noexcept {
    for (; first != last; ++dest, ++first) {
        new (static_cast<void*>(dest)) T(*first);
    }
    return dest;
}

template<typename T>
inline T*
move_backward(T* first, T* last, T* resultEnd) noexcept {
    for (; last != first; --resultEnd, --last) {
        *resultEnd = move_(*last);
    }
    return resultEnd;
}


///////////////////////////////////////////////////////////////////////
// Vector
///////////////////////////////////////////////////////////////////////

template<typename T>
inline T*
Vector<T>::DoAllocate(size_t n) noexcept {
    // Make sure we are not allocating 2GB of data.
    assert_(n < 0x80000000 / sizeof(T));

    // If n is zero, then we allocate no memory and just return nullptr.
    // This is fine, as our default ctor initializes with NULL pointers.
    if (likely(n)) {
        auto* p = (T*)malloc(n * sizeof(T));
        assert_(p != nullptr);
        return p;
    }
    else {
        return nullptr;
    }
}


template<typename T>
inline void
Vector<T>::DoFree(T* p) noexcept {
    if (p) {
        free(p);
    }
}


template<typename T>
inline size_t
Vector<T>::GetNewCapacity(size_t currentCapacity) noexcept {
    // This needs to return a value of at least currentCapacity and at least 1.
    return (currentCapacity > 0) ? (2 * currentCapacity) : 1;
}


template<typename T>
inline Vector<T>::Vector() noexcept
        : mpBegin(nullptr), mpEnd(nullptr), mCapacity(nullptr) {}


template<typename T>
inline Vector<T>::Vector(size_t n) noexcept {
    mpBegin = DoAllocate(n);
    mpEnd = mpBegin;
    mCapacity = mpBegin + n;

    uninitialized_default_fill_n(mpBegin, n);
    mpEnd = mpBegin + n;
}


template<typename T>
inline Vector<T>::Vector(const Vector<T>& x) noexcept {
    mpBegin = DoAllocate(x.size());
    mpEnd = mpBegin;
    mCapacity = mpBegin + x.size();

    mpEnd = uninitialized_copy(x.mpBegin, x.mpEnd, mpBegin);
}


template<typename T>
inline Vector<T>::Vector(Vector<T>&& x) noexcept
        : mpBegin(nullptr), mpEnd(nullptr), mCapacity(nullptr) {
    DoSwap(x);
}


template<typename T>
inline Vector<T>::~Vector() noexcept {
    destruct(mpBegin, mpEnd);
    free(mpBegin);
}


template<typename T>
Vector<T>&
Vector<T>::operator=(const Vector<T>& x) noexcept {
    assert_(false && "Copying a vector by value.");
    if (this != &x) {
        DoAssign(x.begin(), x.end());
    }

    return *this;
}


template<typename T>
Vector<T>&
Vector<T>::operator=(Vector<T>&& x) noexcept {
    if (this != &x) {
        swap(x);
    }
    return *this;
}


template<typename T>
inline typename Vector<T>::iterator
Vector<T>::begin() noexcept {
    return mpBegin;
}


template<typename T>
inline typename Vector<T>::const_iterator
Vector<T>::begin() const noexcept {
    return mpBegin;
}


template<typename T>
inline typename Vector<T>::iterator
Vector<T>::end() noexcept {
    return mpEnd;
}


template<typename T>
inline typename Vector<T>::const_iterator
Vector<T>::end() const noexcept {
    return mpEnd;
}


template<typename T>
bool
Vector<T>::empty() const noexcept {
    return (mpBegin == mpEnd);
}


template<typename T>
inline size_t
Vector<T>::size() const noexcept {
    return (size_t)(mpEnd - mpBegin);
}


template<typename T>
inline size_t
Vector<T>::capacity() const noexcept {
    return (size_t)(mCapacity - mpBegin);
}


template<typename T>
inline void
Vector<T>::resize(size_t n) noexcept {
    if (n > (size_t)(mpEnd - mpBegin)) {  // We expect that more often than not,
                                          // resizes will be upsizes.
        DoInsertValuesEnd(n - ((size_t)(mpEnd - mpBegin)));
    }
    else {
        destruct(mpBegin + n, mpEnd);
        mpEnd = mpBegin + n;
    }
}


template<typename T>
void
Vector<T>::reserve(size_t n) noexcept {
    // If the user wants to reduce the reserved memory, there is the
    // set_capacity function.
    if (n > size_t(mCapacity - mpBegin)) {
        // If n > capacity ...
        DoGrow(n);
    }
}


template<typename T>
inline T*
Vector<T>::data() noexcept {
    return mpBegin;
}


template<typename T>
inline const T*
Vector<T>::data() const noexcept {
    return mpBegin;
}


template<typename T>
inline T& Vector<T>::operator[](size_t n) noexcept {
    assert_(n < static_cast<size_t>(mpEnd - mpBegin));

    return *(mpBegin + n);
}


template<typename T>
inline const T& Vector<T>::operator[](size_t n) const noexcept {
    assert_(n < static_cast<size_t>(mpEnd - mpBegin));

    return *(mpBegin + n);
}


template<typename T>
inline T&
Vector<T>::at(size_t n) noexcept {
    // The difference between at() and operator[] is it signals
    // the requested position is out of range by throwing an
    // out_of_range exception.

    assert_(n < static_cast<size_t>(mpEnd - mpBegin));

    return *(mpBegin + n);
}


template<typename T>
inline const T&
Vector<T>::at(size_t n) const noexcept {
    assert_(n < static_cast<size_t>(mpEnd - mpBegin));

    return *(mpBegin + n);
}


template<typename T>
inline T&
Vector<T>::front() noexcept {
    assert_(mpEnd > mpBegin);

    return *mpBegin;
}


template<typename T>
inline const T&
Vector<T>::front() const noexcept {
    assert_(mpEnd > mpBegin);

    return *mpBegin;
}


template<typename T>
inline T&
Vector<T>::back() noexcept {
    assert_(mpEnd > mpBegin);

    return *(mpEnd - 1);
}


template<typename T>
inline const T&
Vector<T>::back() const noexcept {
    assert_(mpEnd > mpBegin);

    return *(mpEnd - 1);
}


template<typename T>
inline void
Vector<T>::push_back(const T& value) noexcept {
    if (mpEnd < mCapacity) {
        new (static_cast<void*>(mpEnd++)) T(value);
    }
    else {
        DoInsertValueEnd(value);
    }
}


template<typename T>
inline void
Vector<T>::push_back(T&& value) noexcept {
    if (mpEnd < mCapacity) {
        new (static_cast<void*>(mpEnd++)) T(move_(value));
    }
    else {
        DoInsertValueEnd(move_(value));
    }
}


template<typename T>
inline void
Vector<T>::push_back_nogrow(const T& value) noexcept {
    assert_(mpEnd < mCapacity);

    new (static_cast<void*>(mpEnd++)) T(value);
}


template<typename T>
inline void
Vector<T>::push_back_nogrow(T&& value) noexcept {
    assert_(mpEnd < mCapacity);

    new (static_cast<void*>(mpEnd++)) T(move_(value));
}


template<typename T>
inline void
Vector<T>::pop_back() noexcept {
    assert_(mpEnd > mpBegin);

    --mpEnd;
    mpEnd->~T();
}


template<typename T>
inline void
Vector<T>::append(const T* range, size_t n) noexcept {
    DoInsertValuesEnd(range, n);
}


template<typename T>
template<class... Args>
inline typename Vector<T>::iterator
Vector<T>::emplace(iterator position, Args&&... args) noexcept {
    const T* n = position - mpBegin;  // Save this because we might reallocate.

    if ((mpEnd == mCapacity) || (position != mpEnd)) {
        DoInsertValue(position, forward_<Args>(args)...);
    }
    else {
        new (static_cast<void*>(mpEnd)) T(forward_<Args>(args)...);
        ++mpEnd;  // Increment this after the construction above in case the
                  // construction throws an exception.
    }

    return mpBegin + n;
}

template<typename T>
template<class... Args>
inline void
Vector<T>::emplace_back(Args&&... args) noexcept {
    if (mpEnd < mCapacity) {
        new (static_cast<void*>(mpEnd)) T(forward_<Args>(
                args)...);  // If T has a move constructor, it will use it and
                            // this operation may be faster than otherwise.
        ++mpEnd;  // Increment this after the construction above in case the
                  // construction throws an exception.
    }
    else {
        DoInsertValueEnd(forward_<Args>(args)...);
    }
}


template<typename T>
inline typename Vector<T>::iterator
Vector<T>::insert(iterator position, const T& value) noexcept {
    assert_(position >= mpBegin && position <= mpEnd);

    // We implment a quick pathway for the case that the insertion position is
    // at the end and we have free capacity for it.

    // Save this because we might reallocate.
    size_t n = position - mpBegin;

    if ((mpEnd == mCapacity) || (position != mpEnd)) {
        DoInsertValue(position, value);
    }
    else {
        new (static_cast<void*>(mpEnd)) T(value);

        // Increment this after the construction above in case the construction
        // throws an exception.
        ++mpEnd;
    }

    return mpBegin + n;
}


template<typename T>
inline typename Vector<T>::iterator
Vector<T>::insert(iterator position, T&& value) noexcept {
    return emplace(position, move_(value));
}


template<typename T>
inline typename Vector<T>::iterator
Vector<T>::erase(iterator position) noexcept {
    assert_(position >= mpBegin && position < mpEnd);

    if ((position + 1) < mpEnd) {
        move(position + 1, mpEnd, position);
    }

    --mpEnd;
    mpEnd->~T();

    return position;
}


template<typename T>
inline typename Vector<T>::iterator
Vector<T>::erase(iterator first, iterator last) noexcept {
    assert_(first >= mpBegin && first < mpEnd);
    assert_(last >= first && last < mpEnd);

    if ((last + 1) < mpEnd) {
        move(last + 1, mpEnd, first);
    }
    for (auto it = mpEnd - last + first; it != mpEnd; ++it) {
        it->~T();
    }
    mpEnd -= last - first;

    return first;
}


template<typename T>
inline typename Vector<T>::iterator
Vector<T>::erase_unsorted(iterator position) noexcept {
    assert_(position >= mpBegin && position < mpEnd);

    *position = move_(*(mpEnd - 1));

    // pop_back();
    --mpEnd;
    mpEnd->~T();

    return position;
}


template<typename T>
inline void
Vector<T>::clear() noexcept {
    destruct(mpBegin, mpEnd);
    mpEnd = mpBegin;
}


template<typename T>
inline void
Vector<T>::reset_lose_memory() noexcept {
    // The reset function is a special extension function which unilaterally
    // resets the container to an empty state without freeing the memory of
    // the contained objects. This is useful for very quickly tearing down a
    // container built into scratch memory.
    mpBegin = mpEnd = mCapacity = nullptr;
}


// swap exchanges the contents of two containers. With respect to the containers
// allocators, the C11++ Standard (23.2.1/7) states that the behavior of a call
// to a container's swap function is undefined unless the objects being swapped
// have allocators that compare equal or
// allocator_traits<allocator_type>::propagate_on_container_swap::value is true
// (propagate_on_container_swap is false by default). EASTL doesn't have
// allocator_traits and so this doesn't directly apply, but EASTL has the
// effective behavior of propagate_on_container_swap = false for all allocators.
template<typename T>
inline void
Vector<T>::swap(Vector<T>& x) noexcept {
    // NOTE(rparolin): The previous implementation required T to be
    // copy-constructible in the fall-back case where allocators with unique
    // instances copied elements.  This was an unnecessary restriction and
    // prevented the common usage of vector with non-copyable types (eg.
    // eastl::vector<non_copyable> or eastl::vector<unique_ptr>).
    //
    // The previous implementation violated the following requirements of
    // vector::swap so the fall-back code has been removed.  EASTL implicitly
    // defines 'propagate_on_container_swap = false' therefore the fall-back
    // case is undefined behaviour.  We simply swap the contents and the
    // allocator as that is the common expectation of users and does not put the
    // container into an invalid state since it can not free its memory via its
    // current allocator instance.
    //
    // http://en.cppreference.com/w/cpp/container/vector/swap
    // "Exchanges the contents of the container with those of other. Does not
    // invoke any move, copy, or swap operations on individual elements."
    //
    // http://en.cppreference.com/w/cpp/concept/AllocatorAwareContainer
    // "Swapping two containers with unequal allocators if
    // propagate_on_container_swap is false is undefined behavior."

    DoSwap(x);
}


template<typename T>
inline void
Vector<T>::DoAssign(const_iterator first, const_iterator last) noexcept {
    iterator position(mpBegin);

    while ((position != mpEnd) && (first != last)) {
        *position = *first;
        ++first;
        ++position;
    }
    if (first == last) {
        erase(position, mpEnd);
    }
    else {
        DoInsertValuesEnd(first, last - first);
    }
}


template<typename T>
void
Vector<T>::DoClearCapacity() noexcept {
    // This function exists because set_capacity()
    // currently indirectly requires T to be
    // default-constructible, and some functions
    // that need to clear our capacity (e.g.
    // operator=) aren't supposed to require
    // default-constructibility.
    clear();
    Vector<T> temp(
            move_(*this));  // This is the simplest way to accomplish this,
    swap(temp);             // and it is as efficient as any other.
}


template<typename T>
void
Vector<T>::DoGrow(size_t n) noexcept {
    T* pNewData = DoAllocate(n);

    T* pNewEnd = uninitialized_move(mpBegin, mpEnd, pNewData);

    for (auto first = mpBegin, last = mpEnd; first != last; ++first) {
        first->~T();
    }
    DoFree(mpBegin);

    mpBegin = pNewData;
    mpEnd = pNewEnd;
    mCapacity = pNewData + n;
}


template<typename T>
inline void
Vector<T>::DoSwap(Vector<T>& x) noexcept {
    swap_(mpBegin, x.mpBegin);
    swap_(mpEnd, x.mpEnd);
    swap_(mCapacity, x.mCapacity);
}


template<typename T>
void
Vector<T>::DoInsertValuesEnd(size_t n) noexcept {
    if (n > size_t(mCapacity - mpEnd)) {
        size_t nPrevSize = size_t(mpEnd - mpBegin);
        size_t nGrowSize = GetNewCapacity(nPrevSize);
        size_t nNewSize = max_(nGrowSize, nPrevSize + n);
        T* pNewData = DoAllocate(nNewSize);

        T* pNewEnd = uninitialized_move(mpBegin, mpEnd, pNewData);

        uninitialized_default_fill_n(pNewEnd, n);
        pNewEnd += n;

        destruct(mpBegin, mpEnd);
        DoFree(mpBegin);

        mpBegin = pNewData;
        mpEnd = pNewEnd;
        mCapacity = pNewData + nNewSize;
    }
    else {
        uninitialized_default_fill_n(mpEnd, n);
        mpEnd += n;
    }
}


template<typename T>
void
Vector<T>::DoInsertValuesEnd(const T* range, size_t n) noexcept {
    if (n > size_t(mCapacity - mpEnd)) {
        size_t nPrevSize = size_t(mpEnd - mpBegin);
        size_t nGrowSize = GetNewCapacity(nPrevSize);
        size_t nNewSize = max_(nGrowSize, nPrevSize + n);
        T* pNewData = DoAllocate(nNewSize);
        T* pNewEnd = pNewData;

        pNewEnd = uninitialized_move(mpBegin, mpEnd, pNewEnd);
        pNewEnd = uninitialized_copy(
                const_cast<T*>(range), const_cast<T*>(range + n), pNewEnd);

        destruct(mpBegin, mpEnd);
        DoFree(mpBegin);

        mpBegin = pNewData;
        mpEnd = pNewEnd;
        mCapacity = pNewData + nNewSize;
    }
    else {
        mpEnd = uninitialized_copy(
                const_cast<T*>(range), const_cast<T*>(range + n), mpEnd);
    }
}


template<typename T>
template<typename... Args>
void
Vector<T>::DoInsertValue(iterator position, Args&&... args) noexcept {
    // To consider: It's feasible that the args is from a T comes from within
    // the current sequence itself and so we need to be sure to handle that
    // case. This is different from insert(position, const T&) because in this
    // case value is potentially being modified.

    assert_(position >= mpBegin && position <= mpEnd);

    if (mpEnd != mCapacity) {  // If size < capacity ...
        // We need to take into account the possibility that args is a T that
        // comes from within the vector itself. creating a temporary value on
        // the stack here is not an optimal way to solve this because sizeof(T)
        // may be too much for the given platform. An alternative solution may
        // be to specialize this function for the case of the argument being
        // const T& or T&&.
        assert_(position <
                mpEnd);  // While insert at end() is valid, our design is such
                         // that calling code should handle that case before
                         // getting here, as our streamlined logic directly
                         // doesn't handle this particular case due to resulting
                         // negative ranges.
        T value(forward_<Args>(
                args)...);  // Need to do this before the move_backward below
                            // because maybe args refers to something within the
                            // moving range.
        new (static_cast<void*>(mpEnd)) T(move_(
                *(mpEnd - 1)));  // mpEnd is uninitialized memory, so we must
                                 // construct into it instead of move into it
                                 // like we do with the other elements below.
        move_backward(
                position, mpEnd - 1, mpEnd);  // We need to go backward because
                                              // of potential overlap issues.
        position->~T();
        new (static_cast<void*>(position)) T(move_(
                value));  // Move the value argument to the given position.
        ++mpEnd;
    }
    else {  // else (size == capacity)
        size_t nPosSize =
                size_t(position - mpBegin);  // Index of the insertion position.
        size_t nPrevSize = size_t(mpEnd - mpBegin);
        size_t nNewSize = GetNewCapacity(nPrevSize);
        T* pNewData = DoAllocate(nNewSize);

        new (static_cast<void*>((pNewData + nPosSize))) T(forward_<Args>(
                args)...);  // Because the old data is potentially being moved
                            // rather than copied, we need to move
        T* pNewEnd = uninitialized_move(
                mpBegin,
                position,
                pNewData);  // the value first, because it might possibly be a
                            // reference to the old data being moved.
        pNewEnd = uninitialized_move(
                position,
                mpEnd,
                ++pNewEnd);  // Question: with exceptions disabled, do we
                             // asssume all operations are noexcept and thus
                             // there's no need for
                             // uninitialized_move_ptr_if_noexcept?

        destruct(mpBegin, mpEnd);
        DoFree(mpBegin);

        mpBegin = pNewData;
        mpEnd = pNewEnd;
        mCapacity = pNewData + nNewSize;
    }
}


template<typename T>
template<typename... Args>
void
Vector<T>::DoInsertValueEnd(Args&&... args) noexcept {
    size_t nPrevSize = size_t(mpEnd - mpBegin);
    size_t nNewSize = GetNewCapacity(nPrevSize);
    T* pNewData = DoAllocate(nNewSize);

    T* pNewEnd = uninitialized_move(mpBegin, mpEnd, pNewData);
    new (static_cast<void*>(pNewEnd)) T(forward_<Args>(args)...);
    pNewEnd++;

    destruct(mpBegin, mpEnd);
    DoFree(mpBegin);

    mpBegin = pNewData;
    mpEnd = pNewEnd;
    mCapacity = pNewData + nNewSize;
}


///////////////////////////////////////////////////////////////////////
// global operators
///////////////////////////////////////////////////////////////////////

template<typename T>
inline bool
operator==(const Vector<T>& a, const Vector<T>& b) noexcept {
    if (a.size() != b.size()) {
        return false;
    }
    for (auto abeg = a.begin(), bbeg = b.begin(), aend = a.end(); abeg != aend;
         ++abeg, ++bbeg) {
        if (!(*abeg == *bbeg)) {
            return false;
        }
    }
    return true;
}


template<typename T>
inline bool
operator!=(const Vector<T>& a, const Vector<T>& b) noexcept {
    if (a.size() != b.size()) {
        return true;
    }
    for (auto abeg = a.begin(), bbeg = b.begin(), aend = a.end(); abeg != aend;
         ++abeg, ++bbeg) {
        if (!(*abeg == *bbeg)) {
            return true;
        }
    }
    return false;
}

#endif  // SRC_UTIL_VECTOR_H_
