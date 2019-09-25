// filename:    sorttest.cpp
// author:      baobaobear
// create date: 2019-09-20

#ifndef TEST_TYPE_SIMPLE
#define TEST_TYPE_SIMPLE 1 // 0 TestClass; 1 int; 2 double;
#define CONSOLE_OUTPUT 1
#else
#define CONSOLE_OUTPUT 0
#endif

#if TEST_TYPE_SIMPLE > 0 && !defined(BAOBAO_SORT_SAFE_MALLOC)
#define BAOBAO_SORT_SAFE_MALLOC 0
#endif

#ifndef BAOBAO_SORT_SAFE_MALLOC
#define BAOBAO_SORT_SAFE_MALLOC 0
#endif
#include "sortlib.hpp"

#include "sorttest.hpp"

#include <vector>
#include <map>
#include <functional>
#include <string>
#include <cstdio>

typedef int(*gen_data_func_t)(int n, int len);
typedef void(*arrar_function_t)(sort_element_t arr[], size_t n);

#ifdef BAO_SORT_LIB_STD11
#include <chrono>

typedef std::chrono::time_point<std::chrono::system_clock> time_point;

time_point get_time()
{
    return std::chrono::system_clock::now();
}

int64_t get_time_diff(time_point from, time_point to)
{
    return (int64_t)std::chrono::duration_cast<std::chrono::milliseconds>(to - from).count();
}

#else

typedef double time_point;

#ifdef _WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

time_point get_time()
{
#ifdef _WIN32
    return clock();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

int32_t get_time_diff(time_point from, time_point to)
{
    return (int32_t)(to - from);
}

#endif

struct algo_score
{
    int score;
    int index;
    std::string name;
    algo_score(int s = 0, int i = 0, std::string n = "")
        : score(s), index(i), name(n)
    {
    }
    bool operator<(const algo_score &s) const
    {
        return score < s.score;
    }
};

static void data_shuffle_func(sort_element_t arr[], size_t n)
{
    baobao::random_shuffle(arr, arr + n);
}

static void data_shuffle_shellsort(sort_element_t arr[], size_t n)
{
    baobao::random_shuffle(arr, arr + n);

    sort_element_t *beg = arr, *end = arr + n;
    size_t incre = 1;
    while (incre < (size_t)(end - beg) / 3)
        incre = incre * 3 + 1; // A003462, Pratt, 1971, Knuth, 1973
    for (; incre >= 10; incre /= 3)
    {
        sort_element_t *bound = beg + incre;
        for (sort_element_t *i = bound; i < end; ++i)
        {
            sort_element_t *p = i;
            sort_element_t val = *i;
            for (; p >= bound && val < *(p - incre); p -= incre)
                *p = *(p - incre);
            *p = val;
        }
    }
}

static void data_shuffle_heapsort(sort_element_t arr[], size_t n)
{
    baobao::random_shuffle(arr, arr + n);
    std::make_heap(arr, arr + n, baobao::less_rev<sort_element_t>());
    std::make_heap(arr, arr + n / 2, baobao::less_rev<sort_element_t>());
}

static int data_1(int n, int len)
{
    (void)n;
    (void)len;
    return 1;
}

static int data_xor_1(int n, int len)
{
    (void)len;
    return n ^ 1;
}

static int data_rev_xor_1(int n, int len)
{
    (void)len;
    return (1000000000 - n) ^ 1;
}

static int data_n(int n, int len)
{
    (void)len;
    return n;
}

static int data_half_equal(int n, int len)
{
    return (int)(n % (len / 2));
}

static int data_mod_32(int n, int len)
{
    (void)len;
    return n % 32;
}

static int data_mod_sqrt(int n, int len)
{
    return (n % (int)(sqrt(len / 2 + 0.5)));
}

static int data_xor_31(int n, int len)
{
    (void)len;
    return n ^ 31;
}

static void init_gen_data_fun_map(
    std::map<int, gen_data_func_t> &gen_data_fun_map,
    std::map<int, arrar_function_t> &gen_data_shuffle,
    std::map<int, int>& split_size_map
)
{
    gen_data_fun_map[1] = data_1;

    gen_data_fun_map[2] = data_xor_1;
    gen_data_fun_map[3] = data_rev_xor_1;
    gen_data_fun_map[4] = data_n;
    gen_data_shuffle[4] = data_shuffle_func;

    gen_data_fun_map[5] = data_half_equal;
    gen_data_shuffle[5] = data_shuffle_func;
    gen_data_fun_map[6] = data_mod_32;
    gen_data_shuffle[6] = data_shuffle_func;
    gen_data_fun_map[7] = data_mod_32;
    gen_data_fun_map[8] = data_xor_31;
    gen_data_fun_map[9] = data_mod_sqrt;

    gen_data_fun_map[10] = data_n;
    gen_data_shuffle[10] = data_shuffle_shellsort;
    gen_data_fun_map[11] = data_n;
    gen_data_shuffle[11] = data_shuffle_heapsort;
    gen_data_fun_map[12] = data_n;
    gen_data_shuffle[12] = data_shuffle_func;
    split_size_map[12] = 260;
}

void init_test_func_map(std::map<std::string, arrar_function_t> &test_func_map)
{
    // baobao's function
    {
        //test_func_map["bao_insert"] = baobao_warp::baobao_insert_sort;
        test_func_map["bao_heap"] = baobao_warp::baobao_heap_sort;
        test_func_map["bao_shell"] = baobao_warp::baobao_shell_sort;
        test_func_map["bao_merge"] = baobao_warp::baobao_merge_sort;
        test_func_map["bao_mer_in"] = baobao_warp::baobao_merge_sort_in_place;
        test_func_map["bao_qsort"] = baobao_warp::baobao_quick_sort;
        test_func_map["bao_tim"] = baobao_warp::baobao_tim_sort;
    }
    // std function
    {
#ifdef BAOBAO_UNIX_STDLIB_SORT
        test_func_map["unix_heap"] = stdsort::c_heap_sort;
        test_func_map["unix_merge"] = stdsort::c_merge_sort;
#endif
        test_func_map["std::heap"] = stdsort::std_heap_sort;
        test_func_map["std::stable"] = stdsort::std_stable_sort;
        test_func_map["std_qsort"] = stdsort::c_quick_sort;
        test_func_map["std::sort"] = stdsort::std_sort;
    }
}

int main(void)
{
    time_point t;

    std::map<int, gen_data_func_t> gen_data_fun_map;
    std::map<int, arrar_function_t> gen_data_shuffle;
    std::map<std::string, arrar_function_t> test_func_map;
    std::map<int, int> split_size_map;

    init_gen_data_fun_map(gen_data_fun_map, gen_data_shuffle, split_size_map);
    init_test_func_map(test_func_map);

    int base_size = 1000 * (!!TEST_TYPE_SIMPLE + 1);
    int arr_size = base_size * base_size * 2;
    std::vector<sort_element_t> arr(arr_size);

#if CONSOLE_OUTPUT == 1
    printf("Begin %u\n", (unsigned)arr.size());

    printf(" -|");
    for (std::map<std::string, arrar_function_t>::iterator it = test_func_map.begin();
         it != test_func_map.end();
         ++it)
    {
        printf("%12s|", it->first.c_str());
    }
    puts("");
    printf("-:|");
    for (std::map<std::string, arrar_function_t>::iterator it = test_func_map.begin();
         it != test_func_map.end();
         ++it)
    {
        printf("%12s|", "-----------:");
    }
    puts("");
#endif
    std::map<std::string, std::map<int, int> > result_map;
    std::map<std::string, int > unstable_map;
    for (std::map<int, gen_data_func_t>::iterator it_test = gen_data_fun_map.begin();
         it_test != gen_data_fun_map.end();
         ++it_test)
    {
        if (CONSOLE_OUTPUT)
            printf("%2d|", it_test->first);
        for (int i = 0; i < (int)arr.size(); ++i)
        {
            arr[i] = it_test->second(i, (int)arr.size());
        }
        if (gen_data_shuffle.find(it_test->first) != gen_data_shuffle.end())
        {
            gen_data_shuffle[it_test->first](&*arr.begin(), arr.size());
        }
#if TEST_TYPE_SIMPLE == 0
        for (int i = 0; i < (int)arr.size(); ++i)
        {
            arr[i].index = i;
        }
#endif
        for (std::map<std::string, arrar_function_t>::iterator it = test_func_map.begin();
             it != test_func_map.end();
             ++it)
        {
            std::vector<sort_element_t> arr_c = arr;
            int res_ms = 0;
            bool correct = true;
            if (split_size_map.find(it_test->first) != split_size_map.end())
            {
                int split_cnt = split_size_map[it_test->first];
                int last_size = (int)arr.size() - split_cnt;
                std::vector<sort_element_t> arr_c2 = arr;
                t = get_time();
                for (int i = 0; i < last_size; i += split_cnt)
                {
                    it->second(&*arr_c.begin() + i, split_cnt);
                }
                for (int i = 0; i < last_size; i += split_cnt)
                {
                    it->second(&*arr_c2.begin() + i, split_cnt);
                }
                res_ms = (int)get_time_diff(t, get_time());

                //for (size_t i = 0; i < arr.size(); i++) // avoid optimize arr_c2
                //{
                //    if (arr_c[i] < arr_c2[i] || arr_c2[i] < arr_c[i])
                //    {
                //        correct = false;
                //    }
                //}
            }
            else
            {
                t = get_time();
                it->second(&*arr_c.begin(), (int)arr_c.size());
                res_ms = (int)get_time_diff(t, get_time());
            }
            if (CONSOLE_OUTPUT)
                printf("%12d|", (int)res_ms);

            if (split_size_map.find(it_test->first) == split_size_map.end())
            {
                if (!baobao::check_sorted(arr_c.begin(), arr_c.end(), std::less<sort_element_t>()))
                {
                    correct = false;
                }
#if TEST_TYPE_SIMPLE == 0
                else
                {
                    if (!baobao::check_sorted_stable(arr_c.begin(), arr_c.end(), std::less<sort_element_t>()))
                    {
                        unstable_map[it->first] = 1;
                    }
                }
#endif
            }
            if (correct)
            {
                result_map[it->first][it_test->first] = res_ms;
            }
            else
            {
                result_map[it->first][it_test->first] = -1;
            }
            fflush(stdout);
        }
        if (CONSOLE_OUTPUT)
            printf("\n");
    }

    std::vector<std::string> index_name;
    std::vector<algo_score> final_score;

    for (std::map<std::string, arrar_function_t>::iterator it_test = test_func_map.begin();
         it_test != test_func_map.end();
         ++it_test)
    {
        index_name.push_back(it_test->first);
        final_score.push_back(algo_score(0, (int)index_name.size() - 1, it_test->first));
        for (std::map<int, gen_data_func_t>::iterator it = gen_data_fun_map.begin();
             it != gen_data_fun_map.end();
             ++it)
        {
            int v = result_map[it_test->first][it->first];
            if (v >= 0)
            {
                final_score.back().score += v;
            }
            else if (v == -1)
            {
                final_score.back().score += 1000 * 1000;
            }
        }
    }

#if TEST_TYPE_SIMPLE == 0
    printf("\n%-12s||\n------------|---\n", "stable test");
    for (std::map<std::string, arrar_function_t>::iterator it_test = test_func_map.begin();
        it_test != test_func_map.end();
        ++it_test)
    {
        printf("%-12s|%s\n", it_test->first.c_str(), (unstable_map.find(it_test->first) != unstable_map.end() ? "no" : "yes"));
    }
#endif

    char test_title[16] = "";
    if (TEST_TYPE_SIMPLE == 0)
    {
        sprintf(test_title, "TestClass %d", TEST_CLASS_SIZE);
    }
    else if (TEST_TYPE_SIMPLE == 1)
    {
        sprintf(test_title, "int");
    }
    else if (TEST_TYPE_SIMPLE == 2)
    {
        sprintf(test_title, "double");
    }
    else
    {
    }
    printf("\n%-12s|", test_title);
    for (std::map<int, gen_data_func_t>::iterator it = gen_data_fun_map.begin();
         it != gen_data_fun_map.end();
         ++it)
    {
        printf("  %-3d|", it->first);
    }
    puts(" Avg |");
    printf("------------|");
    for (std::map<int, gen_data_func_t>::iterator it = gen_data_fun_map.begin();
         it != gen_data_fun_map.end();
         ++it)
    {
        printf("%5s|", "----:");
    }
    puts("----:|");
    std::sort(final_score.begin(), final_score.end());
    for (std::vector<algo_score>::iterator it_test = final_score.begin();
         it_test != final_score.end();
         ++it_test)
    {
        std::string name = it_test->name;
        printf("%-12s|", name.c_str());
        for (std::map<int, gen_data_func_t>::iterator it = gen_data_fun_map.begin();
             it != gen_data_fun_map.end();
             ++it)
        {
            int v = result_map[name][it->first];
            if (v >= 0)
            {
                printf("%5d|", v);
            }
            else if (v == -1)
            {
                printf("%5s|", "Wrong");
            }
            else if (v == -2)
            {
                printf("%5s|", "Error");
            }
            else if (v == -3)
            {
                printf("%5s|", "Time");
            }
        }
        printf("%5d|\n", (int)(it_test->score / gen_data_fun_map.size()));
    }
    printf("\n%u END\n", (unsigned)arr.size());
    fflush(stdout);

    return 0;
}
