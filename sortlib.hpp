// filename:    sortlib.hpp
// author:      baobaobear
// create date: 2019-09-20
// This library is compatible with C++03

#pragma once

#include <cstddef>
#include <iterator>
#include <algorithm>

#include <cmath>
#include <ctime>
#include <cstring>

#if __cplusplus >= 201103L || _MSC_VER >= 1700
    #include <cstdint>
    #include <type_traits>
#else
    typedef unsigned int uint32_t;
    typedef int int32_t;
#endif

namespace baobao
{
enum
{
    qsort_insertion_sort_threshold = 40,

    find_swap_bound_optimize_threshold = 256,

    merge_insertion_sort_threshold = 32,
    merge_2_part_insertion_sort_threshold = 32,

    timsort_last_run_threshold = 64,
    timsort_min_run = 32,
    timsort_insert_gap = 8,

    merge_sort_alloc_buffer = 1,
    merge_sort_stack_buffer_size = 16384
};

namespace util
{

template<class T, T V>
struct value_constant
{
    static const T value = V;
};

#if __cplusplus >= 201103L || _MSC_VER >= 1700

template< class T >
struct is_integral
    : baobao::util::value_constant<bool, std::is_integral<T>::value>
{
};

template< class T >
struct is_floating_point
    : baobao::util::value_constant<bool, std::is_floating_point<T>::value>
{
};

#else

template< class T >
struct is_integral
    : baobao::util::value_constant<bool, false>
{
};

template<>
struct is_integral<char>
    : baobao::util::value_constant<bool, true>
{
};

template<>
struct is_integral<unsigned char>
    : baobao::util::value_constant<bool, true>
{
};

template<>
struct is_integral<short>
    : baobao::util::value_constant<bool, true>
{
};

template<>
struct is_integral<unsigned short>
    : baobao::util::value_constant<bool, true>
{
};

template<>
struct is_integral<int>
    : baobao::util::value_constant<bool, true>
{
};

template<>
struct is_integral<unsigned>
    : baobao::util::value_constant<bool, true>
{
};

template<>
struct is_integral<long>
    : baobao::util::value_constant<bool, true>
{
};

template<>
struct is_integral<unsigned long>
    : baobao::util::value_constant<bool, true>
{
};

template< class T >
struct is_floating_point
    : baobao::util::value_constant<bool, false>
{
};

template<>
struct is_floating_point<float>
    : baobao::util::value_constant<bool, true>
{
};

template<>
struct is_floating_point<double>
    : baobao::util::value_constant<bool, true>
{
};

#endif

template< class T >
struct is_arithmetic
    : baobao::util::value_constant<bool, baobao::util::is_integral<T>::value || baobao::util::is_floating_point<T>::value>
{
};

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
    LinearRandomNumberGenerator()
    {
        rnd = (uint32_t)time(NULL);
    }
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
template <class RandomAccessIterator>
void insert_sort(RandomAccessIterator beg, RandomAccessIterator end)
{
    insert_sort(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
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

// len <= 40
template <class RandomAccessIterator, class Comp>
void q_insert_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    const int incre = 13;
    for (RandomAccessIterator i = beg + incre, j = beg; i < end; ++i, ++j)
    {
        if (compare(*i, *j))
        {
            std::swap(*i, *j);
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

// len <= 40
template <class RandomAccessIterator, class Comp>
void unguarded_q_insert_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    const int incre = 13;
    for (RandomAccessIterator i = beg + incre, j = beg; i < end; ++i, ++j)
    {
        if (compare(*i, *j))
        {
            std::swap(*i, *j);
        }
    }

    for (RandomAccessIterator i = beg + 1; i < end; ++i)
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
RandomAccessIterator insert_sort_limit(RandomAccessIterator beg, RandomAccessIterator end, Comp compare, int limit)
{
    for (RandomAccessIterator i = beg + 1; i < end; ++i)
    {
        if (!compare(*i, *(i - 1)))
        {
            ++limit;
            continue;
        }
        typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
        RandomAccessIterator j = i - 1;
        *i = *j;
        for (;j != beg && compare(val, *(j - 1)); --j)
        {
            if (--limit <= 0)
            {
                *j = val;
                return i;
            }
            *j = *(j - 1);
        }
        *j = val;
    }
    return end;
}

template <class RandomAccessIterator, class Comp>
RandomAccessIterator unguarded_insert_sort_limit(RandomAccessIterator beg, RandomAccessIterator end, Comp compare, int limit)
{
    for (RandomAccessIterator i = beg + 1; i < end; ++i)
    {
        if (!compare(*i, *(i - 1)))
        {
            ++limit;
            continue;
        }
        typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
        RandomAccessIterator j = i - 1;
        *i = *j;
        for (; compare(val, *(j - 1)); --j)
        {
            if (--limit <= 0)
            {
                *j = val;
                return i;
            }
            *j = *(j - 1);
        }
        *j = val;
    }
    return end;
}

template <class RandomAccessIterator, class Comp>
void shell_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type diff_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    diff_type len = end - beg;
    diff_type incre_list[61] = { 0, 9, 34, 182, 836, 4025, 19001, 90358, 428481, 2034035, 9651787, 45806244, 217378076, 1031612713 };
    const double incre_factor = 2.9; // simple and fast enought

    if (!baobao::util::is_arithmetic<value_type>::value)
    {
        incre_list[1] = 10;
    }
    diff_type incre_index = 0;
    for (diff_type i = 1; i < 60; ++i)
    {
        diff_type mul = incre_list[i];
        if (mul * incre_factor >= len)
        {
            incre_index = i;
            break;
        }
        if (!baobao::util::is_arithmetic<value_type>::value || incre_list[i + 1] == 0)
        {
            incre_list[i + 1] = (diff_type)(mul * incre_factor);
        }
    }

    for (; incre_index > 0; --incre_index)
    {
        bool swaped = false;
        diff_type incre = incre_list[incre_index];
        for (diff_type i = incre; i < len; i++)
        {
            if (compare(*(beg + i), *(beg + i - incre)))
            {
                value_type val = *(beg + i);
                *(beg + i) = *(beg + i - incre);
                diff_type pos = i - incre;
                for (; pos >= incre && compare(val, *(beg + pos - incre)); pos -= incre)
                {
                    *(beg + pos) = *(beg + pos - incre);
                }
                *(beg + pos) = val;
                swaped = true;
            }
        }
        if (!swaped)
        {
            RandomAccessIterator last = baobao::sort::insert_sort_limit(beg, end, compare, 1);
            if (last == end)
            {
                return;
            }
            if (last - beg >= incre << 1)
            {
                beg += ((last - beg) / incre - 1) * incre;
                len = end - beg;
            }
        }
    }

    baobao::sort::insert_sort(beg, beg + incre_list[1], compare);
    baobao::sort::unguarded_insert_sort(beg + incre_list[1], end, compare);
}

template <class RandomAccessIterator>
void shell_sort(RandomAccessIterator beg, RandomAccessIterator end)
{
    shell_sort(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

template <class RandomAccessIterator, class Comp>
void max_heapify_p(RandomAccessIterator first, RandomAccessIterator target, RandomAccessIterator last, Comp compare)
{
    typename std::iterator_traits<RandomAccessIterator>::value_type temp = *target;
    --first;
    RandomAccessIterator son;
    for (; (son = target + (target - first)) < last; target = son)
    {
        if (compare(*son, *(son + 1)))
            ++son;
        if (compare(temp, *son))
            *target = *son;
        else
            break;
    }
    if (son == last && compare(temp, *son))
    {
        *target = *son;
        target = son;
    }
    *target = temp;
}

template <class RandomAccessIterator, class Comp>
void heap_sort_p(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type diff_type;
    diff_type len = end - beg;
    if (len > 1)
    {
        for (diff_type i = len / 2 - 1; i >= 0; --i)
            baobao::sort::max_heapify_p(beg, beg + i, end - 1, compare);
        for (; --end > beg; )
        {
            std::swap(*beg, *end);
            baobao::sort::max_heapify_p(beg, beg, end - 1, compare);
        }
    }
}

template <class RandomAccessIterator, class Comp>
void max_heapify_1(RandomAccessIterator arr, size_t index, size_t last, Comp compare)
{
    typename std::iterator_traits<RandomAccessIterator>::value_type temp = arr[index];
    size_t child;
    for (; (child = index << 1) <= last; index = child)
    {
        if (child < last &&
            compare(*(arr + child), *(arr + child + 1)))
            ++child;
        if (compare(temp, *(arr + child)))
            *(arr + index) = *(arr + child);
        else
            break;
    }
    //if (child == last && compare(temp, *(arr + child)))
    //{
    //    *(arr + index) = *(arr + child);
    //    index = child;
    //}
    *(arr + index) = temp;
}

template <class RandomAccessIterator, class Comp>
void heap_sort_1(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typename std::iterator_traits<RandomAccessIterator>::difference_type length = end - beg;
    RandomAccessIterator parr = beg - 1;
    for (size_t i = length / 2; i > 0; --i)
        max_heapify_1(parr, i, length, compare);
    for (size_t i = length - 1; i > 0; --i)
    {
        std::swap(*beg, *(beg + i));
        max_heapify_1(parr, 1, i, compare);
    }
}

template <class RandomAccessIterator, class Comp>
void heap_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
#if defined(_MSC_VER)
    heap_sort_1(beg, end, compare);
#else
    heap_sort_p(beg, end, compare);
#endif
}

template <class RandomAccessIterator>
void heap_sort(RandomAccessIterator beg, RandomAccessIterator end)
{
    heap_sort(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

// stable sort
template <bool safecopy, class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void merge_2_part_force(RandomAccessBufferIterator buf, RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, Comp compare)
{
    if (mid - beg <= end - mid)
    {
        if (safecopy)
        {
            RandomAccessBufferIterator t = buf;
            for (RandomAccessIterator s = beg; s != mid; ++s, ++t)
            {
                *t = *s;
            }
        }
        else
            memcpy((char*)buf, (char*)&*beg, (char*)&*mid - (char*)&*beg);

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
        if (safecopy)
        {
            RandomAccessBufferIterator t = buf;
            for (RandomAccessIterator s = mid; s != end; ++s, ++t)
            {
                *t = *s;
            }
        }
        else
            memcpy((char*)buf, (char*)&*mid, (char*)&*end - (char*)&*mid);

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
template <bool safecopy, class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
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
        baobao::sort::insert_sort_part(beg, mid, end, compare);
        return;
    }

    baobao::sort::merge_2_part_force<safecopy>(buf, beg, mid, end, compare);
}

template <class RandomAccessIterator>
void swap_2_part_with_same_length(RandomAccessIterator beg, RandomAccessIterator mid)
{
    for (RandomAccessIterator l = beg, r = mid; l < mid; ++l, ++r)
    {
        std::swap(*l, *r);
    }
}

template <bool safecopy, class RandomAccessIterator, class RandomAccessBufferIterator>
RandomAccessIterator swap_2_part_with_buffer(RandomAccessBufferIterator buf, int bufsize, RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end)
{
    RandomAccessIterator ret = beg + (end - mid);
    while (true)
    {
        if (mid - beg <= end - mid)
        {
            if (mid - beg > 0)
            {
                if (mid - beg > bufsize)
                {
                    swap_2_part_with_same_length(beg, mid);
                    RandomAccessIterator m = mid;
                    mid += mid - beg;
                    beg = m;
                }
                else
                {
                    if (safecopy)
                    {
                        RandomAccessBufferIterator t = buf;
                        for (RandomAccessIterator s = beg; s != mid; ++s, ++t)
                        {
                            *t = *s;
                        }
                        RandomAccessIterator m = beg;
                        for (RandomAccessIterator s = mid; s < end; ++s, ++m)
                        {
                            *m = *s;
                        }
                        for (RandomAccessBufferIterator it = buf; it < t; ++m, ++it)
                        {
                            *m = *t;
                        }
                    }
                    else
                    {
                        memcpy((char*)buf, (char*)&*beg, (char*)&*mid - (char*)&*beg);
                        memcpy((char*)beg, (char*)&*mid, (char*)&*end - (char*)&*mid);
                        memcpy((char*)&*(beg + (end - mid)), (char*)buf, (char*)&*mid - (char*)&*beg);
                    }
                    return ret;
                }
            }
            else
                return ret;
        }
        else
        {
            if (end - mid > 0)
            {
                if (end - mid > bufsize)
                {
                    swap_2_part_with_same_length(mid - (end - mid), mid);
                    RandomAccessIterator m = mid;
                    mid -= end - mid;
                    end = m;
                }
                else
                {
                    if (safecopy)
                    {
                        RandomAccessBufferIterator t = buf;
                        for (RandomAccessIterator s = mid; s != end; ++s, ++t)
                        {
                            *t = *s;
                        }
                        RandomAccessIterator m = end - 1;
                        for (RandomAccessIterator s = mid - 1; s >= beg; --s, --m)
                        {
                            *m = *s;
                        }
                        for (--t; t >= buf; --m, --t)
                        {
                            *m = *t;
                        }
                    }
                    else
                    {
                        memcpy((char*)buf, (char*)&*mid, (char*)&*end - (char*)&*mid);
                        memmove((char*)(end - (mid - beg)), (char*)&*beg, (char*)&*mid - (char*)&*beg);
                        memcpy((char*)&*beg, (char*)buf, (char*)&*end - (char*)&*mid);
                    }
                    return ret;
                }
            }
            else
                return ret;
        }
    }
}

template <class RandomAccessIterator, class Comp>
std::pair<RandomAccessIterator, RandomAccessIterator> find_swap_bound(RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, Comp compare)
{
    if (end - mid < find_swap_bound_optimize_threshold)
    {
        RandomAccessIterator pivot_r = mid + (end - mid) / 2 + 1;
        RandomAccessIterator pivot_l = std::upper_bound(beg, mid, *pivot_r, compare);

        pivot_r = std::lower_bound(pivot_r, end, *pivot_l, compare);

        return std::make_pair(pivot_l, pivot_r);
    }
    else
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::difference_type diff_type;

        RandomAccessIterator pivot_l = beg;
        RandomAccessIterator pivot_l_map = std::lower_bound(mid, end, *pivot_l, compare);

        if (pivot_l_map - mid > (end - mid) / 2 || mid - pivot_l <= pivot_l_map - mid)
        {
            return std::make_pair(pivot_l, pivot_l_map);
        }

        RandomAccessIterator pivot_r = std::lower_bound(beg, mid, *(end - 1), compare);
        RandomAccessIterator pivot_r_map = std::lower_bound(mid, end, *pivot_r, compare);

        if (mid - pivot_r > (mid - beg) / 2 || mid - pivot_r >= pivot_r_map - mid)
        {
            return std::make_pair(pivot_r, pivot_r_map);
        }

        do 
        {
            RandomAccessIterator pivot_m = pivot_l + (pivot_r - pivot_l) / 2;
            pivot_m = std::lower_bound(pivot_l, pivot_r, *pivot_m, compare);
            if (pivot_m == pivot_l)
            {
                pivot_m = std::upper_bound(pivot_l, pivot_r, *pivot_m, compare);
                if (pivot_m == pivot_r)
                {
                    {
                        diff_type pivot_m_diff = (mid - pivot_l) - (pivot_l_map - mid);
                        if (pivot_l + pivot_m_diff <= pivot_r)
                        {
                            return std::make_pair(pivot_l + pivot_m_diff, pivot_l_map);
                        }
                    }
                    {
                        diff_type pivot_m_diff = (mid - pivot_r) - (pivot_r_map - mid);
                        if (pivot_l_map <= pivot_r_map + pivot_m_diff)
                        {
                            return std::make_pair(pivot_r, pivot_r_map + pivot_m_diff);
                        }
                    }
                    return std::make_pair(pivot_l, pivot_l_map);
                }
            }
            RandomAccessIterator pivot_m_map = std::lower_bound(pivot_l_map, pivot_r_map, *pivot_m, compare);
            diff_type pivot_m_diff = (mid - pivot_m) - (pivot_m_map - mid);
            if (pivot_m_diff > 0)
            {
                pivot_l = pivot_m;
                pivot_l_map = pivot_m_map;
            }
            else if (pivot_m_diff < 0)
            {
                pivot_r = pivot_m;
                pivot_r_map = pivot_m_map;
            }
            else
            {
                return std::make_pair(pivot_m, pivot_m_map);
            }
        } while (true);

        return std::make_pair(pivot_l, pivot_l_map);
    }
}

// stable sort
template <bool safecopy, class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void merge_2_part_with_buffer(RandomAccessBufferIterator buf, int bufsize, RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, Comp compare)
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
        if (mid_last < last)
        {
            end = last + 1;
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }

    if (end - beg < merge_2_part_insertion_sort_threshold || end - mid < 3 || mid - beg < 3)
    {
        baobao::sort::insert_sort_part(beg, mid, end, compare);
        return;
    }

    if (end - mid <= bufsize || mid - beg <= bufsize)
    {
        baobao::sort::merge_2_part_force<safecopy>(buf, beg, mid, end, compare);
        return;
    }

    std::pair<RandomAccessIterator, RandomAccessIterator> pair = find_swap_bound(beg, mid, end, compare);

    RandomAccessIterator swap_mid = baobao::sort::swap_2_part_with_buffer<safecopy>(buf, bufsize, pair.first, mid, pair.second);
    if (beg < pair.first)
        baobao::sort::merge_2_part_with_buffer<safecopy>(buf, bufsize, beg, pair.first, swap_mid, compare);
    if (pair.second < end)
        baobao::sort::merge_2_part_with_buffer<safecopy>(buf, bufsize, swap_mid, pair.second, end, compare);
}

// stable sort
template <bool safecopy, class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void merge_sort_recursive(RandomAccessBufferIterator buf, RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    size_t len = end - beg;
    if (len < merge_insertion_sort_threshold)
    {
        insert_sort(beg, end, compare);
        return;
    }
    RandomAccessIterator mid = beg + (len >> 1);
    baobao::sort::merge_sort_recursive<safecopy>(buf, beg, mid, compare);
    baobao::sort::merge_sort_recursive<safecopy>(buf, mid, end, compare);
    baobao::sort::merge_2_part<safecopy>(buf, beg, mid, end, compare);
}

// stable sort
template <class RandomAccessIterator, class Comp>
void merge_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    if (end - beg > 1)
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
        value_type * buf = (value_type*)malloc((end - beg) / 2 * sizeof(value_type));
        baobao::sort::merge_sort_recursive<false>(buf, beg, end, compare);
        free(buf);
    }
}

// stable sort
template <class RandomAccessIterator, class Comp>
void merge_sort_s(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    if (end - beg > 1)
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
        value_type * buf = new value_type[(end - beg) / 2];
        baobao::sort::merge_sort_recursive<true>(buf, beg, end, compare);
        delete[] buf;
    }
}

// stable sort
template <class RandomAccessIterator>
void merge_sort(RandomAccessIterator beg, RandomAccessIterator end)
{
    merge_sort(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

// stable sort
template <class RandomAccessIterator>
void merge_sort_s(RandomAccessIterator beg, RandomAccessIterator end)
{
    merge_sort_s(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

// stable sort
template <bool safecopy, class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void merge_sort_recursive_with_buffer(RandomAccessBufferIterator buf, int bufsize, RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    size_t len = end - beg;
    if (len < merge_insertion_sort_threshold)
    {
        insert_sort(beg, end, compare);
        return;
    }
    RandomAccessIterator mid = beg + (len >> 1);
    baobao::sort::merge_sort_recursive_with_buffer<safecopy>(buf, bufsize, beg, mid, compare);
    baobao::sort::merge_sort_recursive_with_buffer<safecopy>(buf, bufsize, mid, end, compare);
    baobao::sort::merge_2_part_with_buffer<safecopy>(buf, bufsize, beg, mid, end, compare);
}

// stable sort
template <bool safecopy, class RandomAccessIterator, class Comp>
void merge_sort_with_buffer(RandomAccessIterator beg, RandomAccessIterator end, Comp compare, bool buffered)
{
    if (end - beg > 1)
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
        if (buffered)
        {
            if (merge_sort_alloc_buffer)
            {
                int buf_size = (int)sqrt(end - beg + 0.1);
                value_type * buf = new value_type[buf_size];
                baobao::sort::merge_sort_recursive_with_buffer<safecopy>(buf, buf_size, beg, end, compare);
                delete[] buf;
            }
            else
            {
                // not really in-place, uses a fixed size of buffer in stack
                value_type buf[merge_sort_stack_buffer_size / sizeof(value_type)];
                baobao::sort::merge_sort_recursive_with_buffer<safecopy>(buf, sizeof(buf) / sizeof(value_type), beg, end, compare);
            }
        }
        else
        {
            baobao::sort::merge_sort_recursive_with_buffer<false>((value_type*)NULL, 0, beg, end, compare);
        }
    }
}

// stable sort
template <class RandomAccessIterator, class Comp>
void merge_sort_buffer(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    merge_sort_with_buffer<false>(beg, end, compare, true);
}

// stable sort
template <class RandomAccessIterator>
void merge_sort_buffer(RandomAccessIterator beg, RandomAccessIterator end)
{
    merge_sort_with_buffer<false>(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>(), true);
}

// stable sort
template <class RandomAccessIterator, class Comp>
void merge_sort_buffer_s(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    merge_sort_with_buffer<true>(beg, end, compare, true);
}

// stable sort
template <class RandomAccessIterator>
void merge_sort_buffer_s(RandomAccessIterator beg, RandomAccessIterator end)
{
    merge_sort_with_buffer<true>(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>(), true);
}

// stable sort
template <class RandomAccessIterator>
void merge_sort_in_place(RandomAccessIterator beg, RandomAccessIterator end)
{
    merge_sort_with_buffer<false>(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>(), false);
}

template <class RandomAccessIterator, class Comp>
RandomAccessIterator quick_sort_partition(RandomAccessIterator beg, RandomAccessIterator end, Comp compare, bool& swaped)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type diff_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    RandomAccessIterator l = beg, r = end - 1;
    uint32_t rnd = baobao::util::fake_rand_simple();
    diff_type n = end - beg, h = (n - 1) / 2;
    if (baobao::util::is_arithmetic<value_type>::value)
    {
        diff_type w = h;
        baobao::util::make_mid_pivot(*(beg + rnd % w), *r, *(beg + h + rnd % w), compare);
    }
    else
    {
        if (n < 1 << 14)
        {
            diff_type w = h;
            baobao::util::make_mid_pivot(*(beg + rnd % w), *r, *(beg + h + rnd % w), compare);
        }
        else
        {
            diff_type w = h - 3;
            util::make_mid_pivot(*(beg),               *(r),           *(beg + h + 1),     compare);
            util::make_mid_pivot(*(beg + 1),           *(beg + h - 1), *(r - 1),           compare);
            util::make_mid_pivot(*(beg + 2 + rnd % w), *(beg + h),     *(r - 2 - rnd % w), compare);
            util::make_mid_pivot(*(beg + h - 1),       *(r),           *(beg + h),         compare);
        }
    }

    value_type pivot = *r;

    while (compare(*l, pivot))
        ++l;
    while (l < r && !compare(*r, pivot))
        --r;
    if (l < r)
    {
        std::swap(*l++, *r--);
        swaped = true;
        while (1)
        {
            while (compare(*l, pivot))
                ++l;
            while (!compare(*r, pivot))
                --r;
            if (l >= r)
                break;
            std::swap(*l++, *r--);
        }
    }
    std::swap(*l, *(end - 1));
    return l;
}

template <class RandomAccessIterator, class Comp>
void quick_sort_loop(RandomAccessIterator beg, RandomAccessIterator end, int deep, Comp compare, bool leftmost)
{
    if (end - beg <= qsort_insertion_sort_threshold)
    {
        if (leftmost)
        {
            baobao::sort::q_insert_sort(beg, end, compare);
        }
        else
        {
            baobao::sort::unguarded_q_insert_sort(beg, end, compare);
        }
        return;
    }
    if (deep <= 0)
    {
        baobao::sort::heap_sort(beg, end, compare);
        return;
    }

    bool swaped = false;
    RandomAccessIterator l = baobao::sort::quick_sort_partition(beg, end, compare, swaped), r = l + 1;
    if (!swaped && !compare(*(l - 1), *beg))
    {
        RandomAccessIterator i = leftmost ? baobao::sort::insert_sort_limit(beg, l, compare, 1)
            : baobao::sort::unguarded_insert_sort_limit(beg, l, compare, 1);
        if (i == l)
        {
            i = baobao::sort::unguarded_insert_sort_limit(r, end, compare, 1);
        }
        if (i == end)
        {
            return;
        }
        if (i >= l)
        {
            while (r < end && baobao::util::object_equal(*r, *l, compare)) ++r;
            return baobao::sort::quick_sort_loop(r, end, deep - 1, compare, false);
        }
    }
    while (r < end && baobao::util::object_equal(*r, *l, compare)) ++r;

    baobao::sort::quick_sort_loop(beg, l, deep - 1, compare, leftmost);
    baobao::sort::quick_sort_loop(r, end, deep - 1, compare, false);
}

template <class RandomAccessIterator, class Comp>
void quick_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    if (end - beg > 1)
    {
        double deep = log(end - beg) / log(1.5);
        baobao::sort::quick_sort_loop(beg, end, (int)deep, compare, true);
    }
}

template <class RandomAccessIterator>
void quick_sort(RandomAccessIterator beg, RandomAccessIterator end)
{
    quick_sort(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

template <class RandomAccessIterator, class Comp>
RandomAccessIterator tim_sort_create_run(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    if (end - beg < timsort_last_run_threshold) // must bigger than timsort_min_run
    {
        baobao::sort::insert_sort(beg, end, compare);
        return end;
    }

    bool ascending;
    RandomAccessIterator run_end = beg + 3;

    if (compare(*(beg + 1), *beg))
    {
        if (!compare(*(beg + 2), *beg))
        {
            std::swap(*beg, *(beg + 1));
            ascending = true;
        }
        else if (compare(*(beg + 2), *(beg + 1)))
        {
            ascending = false;
        }
        else
        {
            std::swap(*(beg + 2), *(beg + 1));
            ascending = false;
        }
    }
    else
    {
        if (compare(*(beg + 2), *beg))
        {
            std::swap(*beg, *(beg + 1));
            ascending = false;
        }
        else if (!compare(*(beg + 2), *(beg + 1)))
        {
            ascending = true;
        }
        else
        {
            std::swap(*(beg + 2), *(beg + 1));
            ascending = true;
        }
    }

    if (!ascending)
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

template <bool safecopy, class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void tim_sort_force_stack_merge(RandomAccessBufferIterator buf, RandomAccessIterator* beg, RandomAccessIterator* end, Comp compare)
{
    while (end - beg > 1)
    {
        if (end - beg > 2)
        {
            if (end[0] - end[-1] <= end[-2] - end[-3])
            {
                baobao::sort::merge_2_part<safecopy>(buf, end[-2], end[-1], end[0], compare);
                end[-1] = end[0];
                --end;
            }
            else
            {
                baobao::sort::merge_2_part<safecopy>(buf, end[-3], end[-2], end[-1], compare);
                end[-2] = end[-1];
                end[-1] = end[0];
                --end;
            }
        }
        else
        {
            baobao::sort::merge_2_part<safecopy>(buf, end[-2], end[-1], end[0], compare);
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

template <bool safecopy, class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void tim_sort_stack_merge_13(RandomAccessBufferIterator buf, RandomAccessIterator* run_stack, RandomAccessIterator* &stack_top, RandomAccessIterator end, Comp compare)
{
    while (stack_top - run_stack >= 2 && end - stack_top[0] > stack_top[0] - stack_top[-1])
    {
        if (stack_top - run_stack > 2 && (stack_top[0] - stack_top[-2]) * 4 / 3 >= stack_top[-1] - stack_top[-3]) // a >= c
        {
            baobao::sort::merge_2_part<safecopy>(buf, stack_top[-3], stack_top[-2], stack_top[-1], compare);
            stack_top[-2] = stack_top[-1];
            stack_top[-1] = stack_top[0];
            --stack_top;
            while (stack_top - run_stack > 2 && (stack_top[-1] - stack_top[-2]) * 4 / 3 >= (stack_top[-2] - stack_top[-3]))
            {
                baobao::sort::merge_2_part<safecopy>(buf, stack_top[-3], stack_top[-2], stack_top[-1], compare);
                stack_top[-2] = stack_top[-1];
                stack_top[-1] = stack_top[0];
                --stack_top;
            }
        }
        else if ((stack_top[0] - stack_top[-1]) * 4 / 3 >= (stack_top[-1] - stack_top[-2])) // 1.333 * a > b
        {
            baobao::sort::merge_2_part<safecopy>(buf, stack_top[-2], stack_top[-1], stack_top[0], compare);
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
template <bool safecopy, class RandomAccessIterator, class Comp>
void tim_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    if (end - beg > 1)
    {
        RandomAccessIterator run_stack[78]; // ln(2^32)/ln(4/3) = 77.1
        RandomAccessIterator* stack_top = run_stack + 1;
        run_stack[0] = beg;
        run_stack[1] = beg = baobao::sort::tim_sort_create_run(beg, end, compare);

        if ((end - beg) * sizeof(value_type) > 4096)
        {
            value_type* buf = safecopy ? new value_type[(end - run_stack[0]) / 2]
                : (value_type*)malloc((end - run_stack[0]) / 2 * sizeof(value_type));
            while (beg < end)
            {
                RandomAccessIterator run = baobao::sort::tim_sort_create_run(beg, end, compare);
                *++stack_top = run;
                beg = run;

                //tim_sort_stack_merge<T, RandomAccessIterator, Comp>(buf, run_stack, stack_top, end);
                baobao::sort::tim_sort_stack_merge_13<safecopy>(buf, run_stack, stack_top, end, compare);
            }

            baobao::sort::tim_sort_force_stack_merge<safecopy>(buf, run_stack, stack_top, compare);
            if (safecopy)
                delete[] buf;
            else
                free(buf);
        }
        else if (end > beg)
        {
            typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
            value_type buf[merge_sort_stack_buffer_size / sizeof(value_type)];
            while (beg < end)
            {
                RandomAccessIterator run = baobao::sort::tim_sort_create_run(beg, end, compare);
                *++stack_top = run;
                beg = run;

                //tim_sort_stack_merge<T, RandomAccessIterator, Comp>(buf, run_stack, stack_top, end);
                baobao::sort::tim_sort_stack_merge_13<safecopy>(buf, run_stack, stack_top, end, compare);
            }

            baobao::sort::tim_sort_force_stack_merge<safecopy>(buf, run_stack, stack_top, compare);
        }
    }
}

// stable sort
template <class RandomAccessIterator>
void tim_sort(RandomAccessIterator beg, RandomAccessIterator end)
{
    tim_sort<false>(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

// stable sort
template <class RandomAccessIterator>
void tim_sort_s(RandomAccessIterator beg, RandomAccessIterator end)
{
    tim_sort<true>(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

template<class RandomAccessIterator>
struct indirect_sort_iter_warp
{
    size_t index;
    RandomAccessIterator it;
    indirect_sort_iter_warp()
    { }
};

template<class RandomAccessIterator, class Comp>
struct indirect_sort_comp_warp
{
    Comp compare;
    indirect_sort_comp_warp(Comp _compare)
        : compare(_compare)
    { }
    indirect_sort_comp_warp(const indirect_sort_comp_warp& comp_warp)
        : compare(comp_warp.compare)
    { }
    bool operator ()(const indirect_sort_iter_warp<RandomAccessIterator>& a, const indirect_sort_iter_warp<RandomAccessIterator>& b) const
    {
        if (compare(*a.it, *b.it))
        {
            return true;
        }
        else if (compare(*b.it, *a.it))
        {
            return false;
        }
        else
        {
            return a.index < b.index;
        }
    }
};

// stable sort, less swap method, works fine for fat class and small size of array
template <class RandomAccessIterator, class Comp>
void indirect_qsort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    if (end - beg > 1)
    {
        size_t size = (size_t)(end - beg);
        indirect_sort_iter_warp<RandomAccessIterator>* ptr = new indirect_sort_iter_warp<RandomAccessIterator>[size], *ptr_beg = ptr, *ptr_end = ptr + size;
        for (size_t i = 0; i < size; ++i)
        {
            ptr[i].it = beg + i;
            ptr[i].index = i;
        }
        baobao::sort::quick_sort(ptr_beg, ptr_end, indirect_sort_comp_warp<RandomAccessIterator, Comp>(compare));
        for (size_t i = 0; i < size; ++i)
        {
            ptr[i].index = ptr[i].it - beg;
        }

        for (size_t index = 0; index < size; ++index)
        {
            if (ptr[index].index)
            {
                size_t i = index;
                typename std::iterator_traits<RandomAccessIterator>::value_type v = *ptr[i].it;
                for (; ptr[i].index > index;)
                {
                    size_t j = ptr[i].index;
                    *ptr[i].it = *ptr[j].it;
                    ptr[i].index = 0;
                    i = j;
                }
                *ptr[i].it = v;
                ptr[i].index = 0;
            }
        }
        delete[] ptr;
    }
}

// stable sort
template <class RandomAccessIterator>
void indirect_qsort(RandomAccessIterator beg, RandomAccessIterator end)
{
    indirect_qsort(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

} // namespace sort

} // namespace baobao
