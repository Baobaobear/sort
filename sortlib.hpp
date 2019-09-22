// filename:    sortlib.hpp
// author:      baobaobear
// create date: 2019-09-20
// This library is compatible with C++98

#pragma once

#include <cstddef>
#include <iterator>
#include <algorithm>

#include <cmath>
#include <cstring>

#if __cplusplus >= 201103L || _MSC_VER >= 1700
    #include <cstdint>
    #include <type_traits>
#else
    typedef unsigned int uint32_t;
    typedef int int32_t;
#endif

// BAOBAO_SORT_SAFE_MALLOC
// set to 1 means it allocated with new/delete
// 0 will be faster, but not safe
#ifndef BAOBAO_SORT_SAFE_MALLOC
#define BAOBAO_SORT_SAFE_MALLOC 1
#endif

namespace baobao
{
enum
{
    qsort_insertion_sort_threshold = 24,

    merge_insertion_sort_threshold = 16,

    merge_2_part_insertion_sort_threshold = 8,
    timsort_last_run_threshold = 64,
    timsort_min_run = 20,
    timsort_insert_gap = 8
};

namespace util
{

template<class BufferElementT>
BufferElementT* new_buffer(size_t size)
{
#if BAOBAO_SORT_SAFE_MALLOC
    return new BufferElementT[size];
#else
    return (BufferElementT*)malloc(size * sizeof(BufferElementT));
#endif
}

template<class BufferElementT>
void delete_buffer(BufferElementT* pointer)
{
#if BAOBAO_SORT_SAFE_MALLOC
    delete[] pointer;
#else
    free(pointer);
#endif
}

template<class T, class Comp>
inline void make_mid_pivot(T& l, T& mid, T& r, Comp compare)
{
    // one swap version
    //if (compare(l, mid))
    //{
    //    if (compare(r, mid))
    //    {
    //        if (compare(l, r))
    //        {
    //            std::swap(r, mid);
    //        }
    //        else
    //        {
    //            std::swap(l, mid);
    //        }
    //    }
    //}
    //else if (compare(mid, l))
    //{
    //    if (compare(mid, r))
    //    {
    //        if (compare(l, r))
    //        {
    //            std::swap(l, mid);
    //        }
    //        else
    //        {
    //            std::swap(r, mid);
    //        }
    //    }
    //}

    // insert sort version
    if (compare(r, mid))
    {
        if (compare(mid, l))
        {
            std::swap(l, r); // optional
            return;
        }
        std::swap(mid, r);
    }
    if (compare(mid, l))
    {
        std::swap(mid, l);
        if (compare(r, mid))
            std::swap(mid, r);
    }
}

template <class T, class Comp>
inline bool object_equal(const T& a, const T& b, Comp compare)
{
    return !(compare(a, b) || compare(b, a));
}

uint32_t fake_rand_simple()
{
    static uint32_t s_rnd = 0xfffffff;
    return ++s_rnd;
}

class LinearRandomNumberGenerator
{
    uint32_t rnd;
public:
    void set_seed(uint32_t seed)
    {
        rnd = seed;
    }
    uint32_t next()
    {
        // ANSI C
        // https://en.wikipedia.org/wiki/Linear_congruential_generator
        rnd = rnd * 1103515245u + 12345;
        return rnd;
    }
    static LinearRandomNumberGenerator& instance()
    {
        static LinearRandomNumberGenerator rng;
        return rng;
    }
};

void rand_seed(uint32_t seed)
{
    baobao::util::LinearRandomNumberGenerator::instance().set_seed(seed);
}

uint32_t rand_uint32()
{
    return baobao::util::LinearRandomNumberGenerator::instance().next();
}

} // namespace util

namespace sort
{

// stable sort
template <class RandomAccessIterator, class Comp>
void insert_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    for (RandomAccessIterator i = beg + 1; i != end; ++i)
    {
        if (compare(*i, *(i - 1)))
        {
            typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
            RandomAccessIterator j = i - 1;
            *i = *j;
            for (;j != beg && compare(val, *(j - 1)); --j)
            {
                *j = *(j - 1);
            }
            *j = val;
        }
    }
}

// stable sort
template <class RandomAccessIterator, class Comp>
inline void unguarded_insert_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    for (RandomAccessIterator i = beg; i < end; ++i)
    {
        if (compare(*i, *(i - 1)))
        {
            typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
            RandomAccessIterator j = i - 1;
            *i = *j;
            for (;compare(val, *(j - 1)); --j)
            {
                *j = *(j - 1);
            }
            *j = val;
        }
    }
}

// len < 24
template <class RandomAccessIterator, class Comp>
void q_insert_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    const int incre = 11;
    for (RandomAccessIterator i = beg + incre; i < end; i++)
    {
        typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
        RandomAccessIterator j = i - incre;
        if (compare(*i, *j))
        {
            *i = *j;
            *j = val;
        }
    }

    for (RandomAccessIterator i = beg + 1; i < end; ++i)
    {
        if (compare(*i, *(i - 1)))
        {
            typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
            RandomAccessIterator j = i - 1;
            *i = *j;
            for (;j != beg && compare(val, *(j - 1)); --j)
            {
                *j = *(j - 1);
            }
            *j = val;
        }
    }
}

// stable sort
template <class RandomAccessIterator, class Comp>
inline void insert_sort_part(RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, Comp compare)
{
    for (RandomAccessIterator i = mid; i < end; ++i)
    {
        if (compare(*i, *(i - 1)))
        {
            typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
            RandomAccessIterator j = i - 1;
            *i = *j;
            for (;j != beg && compare(val, *(j - 1)); --j)
            {
                *j = *(j - 1);
            }
            *j = val;
        }
    }
}

// stable sort
template <class RandomAccessIterator, class Comp>
inline void insert_sort_part_rev(RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, Comp compare)
{
    for (RandomAccessIterator i = mid; i < end; ++i)
    {
        if (!compare(*i, *(i - 1)))
        {
            typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
            RandomAccessIterator j = i - 1;
            *i = *j;
            for (;j != beg && !compare(val, *(j - 1)); --j)
            {
                *j = *(j - 1);
            }
            *j = val;
        }
    }
}

// stable sort
template <class RandomAccessIterator, class Comp>
inline void unguarded_insert_single(RandomAccessIterator i, Comp compare)
{
    if (compare(*i, *(i - 1)))
    {
        typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
        RandomAccessIterator j = i - 1;
        *i = *j;
        for (; compare(val, *(j - 1)); --j)
        {
            *j = *(j - 1);
        }
        *j = val;
    }
}

// stable sort
template <class RandomAccessIterator, class Comp>
inline void unguarded_insert_single_rev(RandomAccessIterator i, Comp compare)
{
    if (!compare(*i, *(i - 1)))
    {
        typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
        RandomAccessIterator j = i - 1;
        *i = *j;
        for (; !compare(val, *(j - 1)); --j)
        {
            *j = *(j - 1);
        }
        *j = val;
    }
}

template <class RandomAccessIterator, class Comp>
void shell_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type diff_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    diff_type len = end - beg;
    diff_type incre_list[61] = { 0, 10 };
    const double incre_factor = 2.9; // simple and fast enought

    diff_type incre_index = 0;
    for (diff_type i = 1; i < 60; ++i)
    {
        diff_type mul = incre_list[i];
        if (mul * incre_factor >= len)
        {
            incre_index = i;
            break;
        }
        mul = (diff_type)(mul * incre_factor);
        incre_list[i + 1] = (diff_type)mul;
    }

    for (; incre_index > 0; --incre_index)
    {
        diff_type incre = incre_list[incre_index];
        for (diff_type i = incre; i < len; i++)
        {
            diff_type pos = i;
            value_type val = *(beg + i);
            for (; pos >= incre && compare(val, *(beg + pos - incre)); pos -= incre)
                *(beg + pos) = *(beg + pos - incre);
            *(beg + pos) = val;
        }
    }

    baobao::sort::insert_sort(beg, beg + incre_list[1], compare);
    baobao::sort::unguarded_insert_sort(beg + incre_list[1], end, compare);
}

template <class RandomAccessIterator, class Comp>
void max_heapify(RandomAccessIterator arr, size_t index, size_t last, Comp compare)
{
    typename std::iterator_traits<RandomAccessIterator>::value_type temp = *(arr + index);
    for (size_t son; (son = index * 2 + 1) <= last; index = son)
    {
        if (son + 1 <= last && !compare(*(arr + son + 1), *(arr + son))) //arr[son] <= arr[son + 1]
            ++son;
        if (compare(temp, *(arr + son)))
            *(arr + index) = *(arr + son);
        else
            break;
    }
    *(arr + index) = temp;
}

template <class RandomAccessIterator, class Comp>
void heap_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type diff_type;
    diff_type len = end - beg;
    if (len > 1)
    {
        for (diff_type i = len / 2 - 1; i >= 0; --i)
            baobao::sort::max_heapify(beg, i, (size_t)len - 1, compare);
        for (diff_type i = len - 1; i > 0; --i)
        {
            std::swap(*beg, *(beg + i));
            baobao::sort::max_heapify(beg, 0, (size_t)i - 1, compare);
        }
    }
}

// stable sort
template <class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void merge_2_part(RandomAccessBufferIterator buf, RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, Comp compare)
{
    if (!compare(*mid, *(mid - 1)))
    {
        return;
    }
    while (beg < mid && !compare(*mid, *beg))
    {
        ++beg;
    }
    if (beg < mid)
    {
        RandomAccessIterator mid_last = mid - 1, last = end - 1;
        while (mid_last < last && !compare(*last, *mid_last))
        {
            --last;
        }
        if (mid_last == last)
        {
            return;
        }
        end = last + 1;
    }
    else
    {
        return;
    }

    if (end - beg < merge_2_part_insertion_sort_threshold)
    {
        baobao::sort::insert_sort(beg, end, compare);
        return;
    }
    if (mid - beg <= end - mid)
    {
#if BAOBAO_SORT_SAFE_MALLOC
        RandomAccessBufferIterator t = buf;
        for (RandomAccessIterator s = beg; s != mid; ++s, ++t)
        {
            *t = *s;
        }
#else
        memcpy(buf, &*beg, (char*)&*mid - (char*)&*beg);
#endif

        RandomAccessBufferIterator start1 = buf, start1_end = buf + (mid - beg);
        RandomAccessIterator start2 = mid, k = beg;
        if (start1 < start1_end && start2 < end)
        {
            while (true)
            {
                if (compare(*start2, *start1))
                {
                    *k++ = *start2++;
                    if (start2 >= end)
                        break;
                }
                else
                {
                    *k++ = *start1++;
                    if (start1 >= start1_end)
                        break;
                }
            }
        }
        while (start1 < start1_end)
        {
            *k++ = *start1++;
        }
    }
    else
    {
#if BAOBAO_SORT_SAFE_MALLOC
        RandomAccessBufferIterator t = buf;
        for (RandomAccessIterator s = mid; s != end; ++s, ++t)
        {
            *t = *s;
        }
#else
        memcpy(buf, &*mid, (char*)&*end - (char*)&*mid);
#endif
        RandomAccessBufferIterator start1 = buf + (end - mid) - 1, start1_end = buf;
        RandomAccessIterator start2 = mid - 1, k = end;
        if (start1 >= start1_end && start2 >= beg)
        {
            while (true)
            {
                if (compare(*start1, *start2))
                {
                    *--k = *start2--;
                    if (start2 < beg)
                        break;
                }
                else
                {
                    *--k = *start1--;
                    if (start1 < start1_end)
                        break;
                }
            }
        }
        while (start1 >= start1_end)
        {
            *--k = *start1--;
        }
    }
}

// stable sort
template <class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void merge_sort_recursive(RandomAccessBufferIterator buf, RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    size_t len = end - beg;
    if (len < merge_insertion_sort_threshold)
    {
        insert_sort(beg, end, compare);
        return;
    }
    RandomAccessIterator mid = beg + (len >> 1);
    baobao::sort::merge_sort_recursive(buf, beg, mid, compare);
    baobao::sort::merge_sort_recursive(buf, mid, end, compare);
    baobao::sort::merge_2_part(buf, beg, mid, end, compare);
}

// stable sort
template <class RandomAccessIterator, class Comp>
void merge_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    value_type * buf = baobao::util::new_buffer<value_type>((end - beg) / 2);
    baobao::sort::merge_sort_recursive(buf, beg, end, compare);
    baobao::util::delete_buffer(buf);
}

template <class RandomAccessIterator, class Comp>
void intro_sort_loop(RandomAccessIterator beg, RandomAccessIterator end, int deep, Comp compare)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type diff_type;
    diff_type n = end - beg;
    if (n < qsort_insertion_sort_threshold)
    {
        baobao::sort::q_insert_sort(beg, end, compare);
        return;
    }
    if (deep <= 0)
    {
        baobao::sort::heap_sort(beg, end, compare);
        return;
    }

    RandomAccessIterator l = beg, r = end - 1;
    uint32_t rnd = baobao::util::fake_rand_simple();
    diff_type h = n / 2, w = n / 4;
    baobao::util::make_mid_pivot(*(beg + (rnd ^ 0x12345678u) % w), *r, *(beg + rnd % w + h), compare);

    typename std::iterator_traits<RandomAccessIterator>::value_type pivot = *r;
    while (1)
    {
        while (compare(*l, pivot))//l < r &&
            ++l;
        while (l < r && !compare(*r, pivot))
            --r;
        if (l >= r)
            break;
        std::swap(*l++, *r);
    }
    std::swap(*l, *(end - 1));
    r = l + 1;
    while (r < end && baobao::util::object_equal(*r, pivot, compare)) ++r;

    if (l - beg < end - r)
    {
        baobao::sort::intro_sort_loop(beg, l, deep - 1, compare);
        baobao::sort::intro_sort_loop(r, end, deep - 1, compare);
    }
    else
    {
        baobao::sort::intro_sort_loop(r, end, deep - 1, compare);
        baobao::sort::intro_sort_loop(beg, l, deep - 1, compare);
    }
}

template <class RandomAccessIterator, class Comp>
void intro_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    double deep = log(end - beg) / log(1.5);
    baobao::sort::intro_sort_loop(beg, end, (int)deep, compare);
}

template <class RandomAccessIterator, class Comp>
RandomAccessIterator tim_sort_create_run(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    if (end - beg < timsort_last_run_threshold) // must bigger than timsort_min_run
    {
        baobao::sort::insert_sort(beg, end, compare);
        return end;
    }

    RandomAccessIterator run_end = beg + 2;

    if (compare(*(beg + 1), *beg))
    {
        while (run_end < end && compare(*run_end, *(run_end - 1)))
        {
            ++run_end;
        }

        if (run_end - beg < timsort_min_run)
        {
            baobao::sort::insert_sort_part_rev(beg, run_end, beg + timsort_min_run, compare);
            run_end = beg + timsort_min_run;
            while (run_end < end && compare(*run_end, *(run_end - timsort_insert_gap)))
            {
                baobao::sort::unguarded_insert_single_rev(run_end, compare);
                ++run_end;
            }
        }

        // reverse
        typename std::iterator_traits<RandomAccessIterator>::value_type t;
        for (RandomAccessIterator b = beg, e = run_end - 1; b < e; ++b, --e)
        {
            t = *b; *b = *e; *e = t;
        }
    }
    else
    {
        while (run_end < end && !compare(*run_end, *(run_end - 1)))
        {
            ++run_end;
        }

        if (run_end - beg < timsort_min_run)
        {
            baobao::sort::insert_sort_part(beg, run_end, beg + timsort_min_run, compare);
            run_end = beg + timsort_min_run;
            while (run_end < end && !compare(*run_end, *(run_end - timsort_insert_gap)))
            {
                baobao::sort::unguarded_insert_single(run_end, compare);
                ++run_end;
            }
        }
    }

    return run_end;
}

template <class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void tim_sort_force_stack_merge(RandomAccessBufferIterator buf, RandomAccessIterator* beg, RandomAccessIterator* end, Comp compare)
{
    while (end - beg > 1)
    {
        if (end - beg > 2)
        {
            if (end[0] - end[-1] <= end[-2] - end[-3])
            {
                baobao::sort::merge_2_part(buf, end[-2], end[-1], end[0], compare);
                end[-1] = end[0];
                --end;
            }
            else
            {
                baobao::sort::merge_2_part(buf, end[-3], end[-2], end[-1], compare);
                end[-2] = end[-1];
                end[-1] = end[0];
                --end;
            }
        }
        else
        {
            baobao::sort::merge_2_part(buf, end[-2], end[-1], end[0], compare);
            end[-1] = end[0];
            --end;
        }
    }
}

template <class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void tim_sort_stack_merge(RandomAccessBufferIterator buf, RandomAccessIterator* run_stack, RandomAccessIterator* &stack_top, RandomAccessIterator end, Comp compare)
{
    for (bool merged = true; merged && end - stack_top[0] > stack_top[0] - stack_top[-1]; )
    {
        merged = false;
        if (stack_top - run_stack >= 2)
        {
            if (stack_top - run_stack > 2)
            {
                if (stack_top[0] - stack_top[-2] >= stack_top[-2] - stack_top[-3]) // a + b >= c
                {
                    if (stack_top[0] - stack_top[-1] <= stack_top[-2] - stack_top[-3]) // a <= c
                    {
                        baobao::sort::merge_2_part(buf, stack_top[-2], stack_top[-1], stack_top[0], compare);
                        baobao::sort::merge_2_part(buf, stack_top[-3], stack_top[-2], stack_top[0], compare);
                        stack_top[-2] = stack_top[0];
                        stack_top -= 2;
                    }
                    else
                    {
                        baobao::sort::merge_2_part(buf, stack_top[-3], stack_top[-2], stack_top[-1], compare);
                        stack_top[-2] = stack_top[-1];
                        stack_top[-1] = stack_top[0];
                        --stack_top;
                    }
                    merged = true;
                    continue;
                }
            }
            if (stack_top[0] - stack_top[-1] >= stack_top[-1] - stack_top[-2]) // a >= b
            {
                baobao::sort::merge_2_part(buf, stack_top[-2], stack_top[-1], stack_top[0], compare);
                stack_top[-1] = stack_top[0];
                --stack_top;
                merged = true;
            }
        }
    }
}

template <class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void tim_sort_stack_merge_13(RandomAccessBufferIterator buf, RandomAccessIterator* run_stack, RandomAccessIterator* &stack_top, RandomAccessIterator end, Comp compare)
{
    while (stack_top - run_stack >= 2 && end - stack_top[0] > stack_top[0] - stack_top[-1])
    {
        if (stack_top - run_stack > 2 && (stack_top[0] - stack_top[-2]) * 4 / 3 >= stack_top[-1] - stack_top[-3]) // a >= c
        {
            baobao::sort::merge_2_part(buf, stack_top[-3], stack_top[-2], stack_top[-1], compare);
            stack_top[-2] = stack_top[-1];
            stack_top[-1] = stack_top[0];
            --stack_top;
            while (stack_top - run_stack > 2 && (stack_top[-1] - stack_top[-2]) * 4 / 3 >= (stack_top[-2] - stack_top[-3]))
            {
                baobao::sort::merge_2_part(buf, stack_top[-3], stack_top[-2], stack_top[-1], compare);
                stack_top[-2] = stack_top[-1];
                stack_top[-1] = stack_top[0];
                --stack_top;
            }
        }
        else if ((stack_top[0] - stack_top[-1]) * 4 / 3 >= (stack_top[-1] - stack_top[-2])) // 1.333 * a > b
        {
            baobao::sort::merge_2_part(buf, stack_top[-2], stack_top[-1], stack_top[0], compare);
            stack_top[-1] = stack_top[0];
            --stack_top;
        }
        else
        {
            break;
        }
    }
}

// stable sort
template <class RandomAccessIterator, class Comp>
void tim_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    if (end > beg)
    {
        RandomAccessIterator run_stack[78]; // ln(2^32)/ln(4/3) = 77.1
        RandomAccessIterator* stack_top = run_stack + 1;
        run_stack[0] = beg;
        run_stack[1] = beg = baobao::sort::tim_sort_create_run(beg, end, compare);

        if (beg < end)
        {
            value_type* buf = baobao::util::new_buffer<value_type>((end - run_stack[0]) / 2);
            while (beg < end)
            {
                RandomAccessIterator run = baobao::sort::tim_sort_create_run(beg, end, compare);
                *++stack_top = run;
                beg = run;

                //tim_sort_stack_merge<T, RandomAccessIterator, Comp>(buf, run_stack, stack_top, end);
                baobao::sort::tim_sort_stack_merge_13(buf, run_stack, stack_top, end, compare);
            }

            baobao::sort::tim_sort_force_stack_merge(buf, run_stack, stack_top, compare);
            baobao::util::delete_buffer(buf);
        }
    }
}

} // namespace sort

} // namespace baobao

#undef BAOBAO_SORT_MOVE
#undef BAOBAO_SORT_SAFE_MALLOC
