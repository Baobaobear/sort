// filename:    sorttest.hpp
// author:      baobaobear
// create date: 2019-09-20

#pragma once

// TEST_TYPE_SIMPLE
// 0 TestClass; 1 int; 2 double;
#ifndef TEST_TYPE_SIMPLE
#define TEST_TYPE_SIMPLE 0
#endif

#include <algorithm>
#include <cstdlib>

#if __cplusplus >= 201103L || _MSC_VER >= 1700
#define BAO_SORT_LIB_STD11
#endif

#ifndef TEST_CLASS_SIZE
#define TEST_CLASS_SIZE 8
#endif

namespace baobao
{

typedef int radix_index_t;

struct TestClass
{
    static const int data_len = TEST_CLASS_SIZE - 2;
    int val;
    int index;
    int data[data_len];

    TestClass()
        : val(0)
        , index(0)
    {
    }

    TestClass(const TestClass& v)
        : val(v.val)
        , index(v.index)
    {
        for (int i = 0; i < data_len; ++i)
        {
            data[i] = v.data[i];
        }
    }

    bool operator < (const TestClass& v) const
    {
        return val < v.val;
    }

    //bool operator > (const TestClass& v) const
    //{
    //    return val > v.val;
    //}

    //bool operator == (const TestClass& v) const = delete;

    TestClass& operator = (const TestClass& v)
    {
        val = v.val;
        index = v.index;
        for (int i = 0; i < data_len; ++i)
        {
            data[i] = v.data[i];
        }
        return *this;
    }

    TestClass& operator = (int v)
    {
        val = v;
        return *this;
    }

    bool operator == (int v) const
    {
        return val == v;
    }

    radix_index_t get_index() const
    {
        return val;
    }
};

template<class RandomAccessIterator, class Comp>
bool check_sorted(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    for (RandomAccessIterator i = beg + 1; i != end; ++i)
        if (compare(*i, *(i - 1)))
            return false;
    return true;
}

template<class RandomAccessIterator, class Comp>
bool check_sorted_stable(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    for (RandomAccessIterator i = beg + 1; i != end; ++i)
    {
        if (compare(*i, *(i - 1)))
            return false;
        if (!compare(*(i - 1), *i))
        {
            if ((*(i - 1)).index > (*i).index)
            {
                return false;
            }
        }
    }
    return true;
}

uint32_t random_int(uint32_t max_int)
{
    return (uint32_t)(baobao::util::rand_uint32() % (max_int + 1));
}

template<class RandomAccessIterator>
void random_shuffle(RandomAccessIterator beg, RandomAccessIterator end)
{
    typename std::iterator_traits<RandomAccessIterator>::difference_type n;
    n = end - beg;
    while (--n >= 1)
    {
        std::swap(*(beg + n), *(beg + random_int((uint32_t)n)));
    }
}

template<class Element>
struct less_rev
{
    bool operator()(const Element& a, const Element& b) const
    {
        return b < a;
    }
};


}

#if !TEST_TYPE_SIMPLE
typedef baobao::TestClass sort_element_t;
#else
#if TEST_TYPE_SIMPLE == 1
typedef int sort_element_t;
#else
typedef double sort_element_t;
#endif
#endif

namespace stdsort
{

//quick_sort_c
int sort_element_t_cmp(const void *a, const void *b)
{
#if TEST_TYPE_SIMPLE
    return *(sort_element_t *)a - *(sort_element_t *)b;
#else
    if (*(sort_element_t *)a < *(sort_element_t *)b)
    {
        return -1;
    }
    if (*(sort_element_t *)b < *(sort_element_t *)a)
    {
        return 1;
    }
    return 0;
#endif
}

void c_quick_sort(sort_element_t arr[], size_t n)
{
    qsort(arr, n, sizeof(sort_element_t), sort_element_t_cmp);
}

#if !defined(_WIN32) && !defined(_WIN64) && !defined(__linux__) && !defined(__CYGWIN__)
#define BAOBAO_UNIX_STDLIB_SORT
void c_heap_sort(sort_element_t arr[], size_t n)
{
    heapsort(arr, n, sizeof(sort_element_t), sort_element_t_cmp);
}

void c_merge_sort(sort_element_t arr[], size_t n)
{
    mergesort(arr, n, sizeof(sort_element_t), sort_element_t_cmp);
}
#endif

void std_sort(sort_element_t arr[], size_t n)
{
    std::sort(arr, arr + n);
}

// stable sort
void std_stable_sort(sort_element_t arr[], size_t len)
{
    std::stable_sort(arr, arr + len);
}

template <class RandomAccessIterator>
void std_heap_sort(RandomAccessIterator beg, RandomAccessIterator end)
{
    std::make_heap(beg, end);
    std::sort_heap(beg, end);
}

void std_heap_sort(sort_element_t arr[], size_t length)
{
    std_heap_sort(arr, arr + length);
}

}


namespace baobao_warp
{

void baobao_insert_sort(sort_element_t arr[], size_t len)
{
    baobao::sort::insert_sort(arr, arr + len);
}

void baobao_q_insert_sort(sort_element_t arr[], size_t len)
{
    baobao::sort::q_insert_sort(arr, arr + len, std::less<sort_element_t>());
}

void baobao_heap_sort(sort_element_t arr[], size_t len)
{
    baobao::sort::heap_sort(arr, arr + len);
}

void baobao_shell_sort(sort_element_t arr[], size_t len)
{
    baobao::sort::shell_sort(arr, arr + len);
}

void baobao_merge_sort(sort_element_t arr[], size_t len)
{
    baobao::sort::merge_sort(arr, arr + len);
}

void baobao_merge_sort_s(sort_element_t arr[], size_t len)
{
    baobao::sort::merge_sort_s(arr, arr + len);
}

void baobao_merge_sort_buffer(sort_element_t arr[], size_t len)
{
    baobao::sort::merge_sort_buffer(arr, arr + len);
}

void baobao_merge_sort_buffer_s(sort_element_t arr[], size_t len)
{
    baobao::sort::merge_sort_buffer_s(arr, arr + len);
}

void baobao_merge_sort_in_place(sort_element_t arr[], size_t len)
{
    baobao::sort::merge_sort_in_place(arr, arr + len);
}

void baobao_quick_sort(sort_element_t arr[], size_t len)
{
    baobao::sort::quick_sort(arr, arr + len);
}

void baobao_tim_sort(sort_element_t arr[], size_t len)
{
    baobao::sort::tim_sort(arr, arr + len);
}

void baobao_tim_sort_s(sort_element_t arr[], size_t len)
{
    baobao::sort::tim_sort_s(arr, arr + len);
}

void baobao_indirect_qsort(sort_element_t arr[], size_t len)
{
    baobao::sort::indirect_qsort(arr, arr + len);
}


}

#include "grailsort.hpp"
#include "wikisort.hpp"

namespace baobao_warp
{

void grail_sort_in_place(sort_element_t arr[], size_t len)
{
    Mrrl::grail_sort_in_place(arr, arr + len);
}

void grail_sort_buf(sort_element_t arr[], size_t len)
{
    Mrrl::grail_sort_buffer(arr, arr + len);
}

void grail_sort_dynbuf(sort_element_t arr[], size_t len)
{
    Mrrl::grail_sort(arr, arr + len);
}

void wiki_sort(sort_element_t arr[], size_t len)
{
    Wiki::Sort(arr, arr + len);
}

}
