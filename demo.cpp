// filename:    demo.cpp
// author:      baobaobear
// create date: 2019-09-20

#include "sortlib.hpp"
#include "sorttest.hpp"
#include <vector>
#include <functional>
#include <cstdio>

template<class RandomAccessIterator, class Comp>
bool check_sorted(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    for (RandomAccessIterator i = beg + 1; i != end; ++i)
        if (compare(*i, *(i - 1)))
            return false;
    return true;
}

template <class RandomAccessIterator>
void print_check_sorted(RandomAccessIterator beg, RandomAccessIterator end)
{
    if (check_sorted(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>()))
    {
        printf("OK\n");
    }
    else
    {
        printf("wrong\n");
    }
}

int main(void)
{
    typedef int sort_t;
    const size_t count = 1000;
    std::vector<sort_t> arr;

    for (size_t i = 0; i < count; i++)
    {
        arr.push_back(i);
    }

    baobao::random_shuffle(arr.begin(), arr.end());
    baobao::sort::insert_sort(arr.begin(), arr.end());
    print_check_sorted(arr.begin(), arr.end());

    baobao::random_shuffle(arr.begin(), arr.end());
    baobao::sort::heap_sort(arr.begin(), arr.end());
    print_check_sorted(arr.begin(), arr.end());

    baobao::random_shuffle(arr.begin(), arr.end());
    baobao::sort::shell_sort(arr.begin(), arr.end());
    print_check_sorted(arr.begin(), arr.end());

    baobao::random_shuffle(arr.begin(), arr.end());
    baobao::sort::quick_sort(arr.begin(), arr.end());
    print_check_sorted(arr.begin(), arr.end());

    baobao::random_shuffle(arr.begin(), arr.end());
    baobao::sort::merge_sort(arr.begin(), arr.end());
    print_check_sorted(arr.begin(), arr.end());

    baobao::random_shuffle(arr.begin(), arr.end());
    baobao::sort::tim_sort(arr.begin(), arr.end());
    print_check_sorted(arr.begin(), arr.end());

    return 0;
}
