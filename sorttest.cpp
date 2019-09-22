// filename:    sorttest.cpp
// author:      baobaobear
// create date: 2019-09-20

#define BAOBAO_SORT_SAFE_MALLOC 0
#include "sortlib.hpp"

#define TEST_TYPE_SIMPLE 1 // 0 TestClass; 1 int; 2 double;
#include "sorttest.hpp"

#include <vector>
#include <map>
#include <functional>
#include <cstdio>

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

typedef int32_t time_point;

#ifdef __linux__
#include <sys/time.h>
#else
#include <time.h>
#endif

time_point get_time()
{
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#else
    return clock();
#endif
}

int32_t get_time_diff(time_point from, time_point to)
{
    return to - from;
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
    std::make_heap(arr, arr + n, std::greater<sort_element_t>());
    std::make_heap(arr, arr + n / 2, std::greater<sort_element_t>());
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
    std::map<int, int(*)(int n, int len)> &gen_data_fun_map,
    std::map<int, void(*)(sort_element_t arr[], size_t n)> &gen_data_shuffle)
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
}

void init_test_func_map(
    std::map<std::string, void(*)(sort_element_t arr[], size_t n)> &test_func_map,
    std::map<std::string, int> &test_func_stable_map)
{
    // baobao's function
    {
        //test_func_map["bao_insert"] = baobao_warp::baobao_insert_sort;
        test_func_map["bao_heap"] = baobao_warp::baobao_heap_sort;
        test_func_map["bao_shell"] = baobao_warp::baobao_shell_sort;
        test_func_map["bao_merge"] = baobao_warp::baobao_merge_sort; test_func_stable_map["bao_merge"] = 1;
        test_func_map["bao_qsort"] = baobao_warp::baobao_quick_sort;
        test_func_map["bao_tim"] = baobao_warp::baobao_tim_sort; test_func_stable_map["bao_tim"] = 1;
    }
    // std function
    {
        test_func_map["std_heap"] = stdsort::std_heap_sort;
        test_func_map["std_stable"] = stdsort::std_stable_sort; test_func_stable_map["std_stable"] = 1;
        test_func_map["std_qsort"] = stdsort::c_quick_sort;
        test_func_map["std_sort"] = stdsort::std_sort;
    }
}

int main(void)
{
    time_point t;

    std::map<int, int(*)(int n, int len)> gen_data_fun_map;
    std::map<int, void(*)(sort_element_t arr[], size_t n)> gen_data_shuffle;
    std::map<std::string, void(*)(sort_element_t arr[], size_t n)> test_func_map;
    std::map<std::string, int> test_func_stable_map;
    std::map<int, void(*)(sort_element_t arr[], int n)> gen_data_func;

    init_gen_data_fun_map(gen_data_fun_map, gen_data_shuffle);
    init_test_func_map(test_func_map, test_func_stable_map);

    int base_size = 1000 * (!!TEST_TYPE_SIMPLE + 1);
    int arr_size = base_size * base_size * 2;
    std::vector<sort_element_t> arr(arr_size);

    printf("Begin %u\n", (unsigned)arr.size());

    printf(" -|");
    for (std::map<std::string, void (*)(sort_element_t arr[], size_t n)>::iterator it = test_func_map.begin();
         it != test_func_map.end();
         ++it)
    {
        printf("%12s|", it->first.c_str());
    }
    puts("");
    printf("-:|");
    for (std::map<std::string, void (*)(sort_element_t arr[], size_t n)>::iterator it = test_func_map.begin();
         it != test_func_map.end();
         ++it)
    {
        printf("%12s|", "-----------:");
    }
    puts("");
    std::map<std::string, std::map<int, int> > result_map;
    for (std::map<int, int (*)(int n, int len)>::iterator it_test = gen_data_fun_map.begin();
         it_test != gen_data_fun_map.end();
         ++it_test)
    {
        printf("%2d|", it_test->first);
        if (gen_data_func.find(it_test->first) == gen_data_func.end())
        {
            for (int i = 0; i < (int)arr.size(); ++i)
            {
                arr[i] = it_test->second(i, (int)arr.size());
            }
            if (gen_data_shuffle.find(it_test->first) != gen_data_shuffle.end())
            {
                gen_data_shuffle[it_test->first](&*arr.begin(), arr.size());
            }
        }
        else
        {
            gen_data_func[it_test->first](&*arr.begin(), (int)arr.size());
        }
#if TEST_TYPE_SIMPLE == 0
        for (int i = 0; i < (int)arr.size(); ++i)
        {
            arr[i].index = i;
        }
#endif
        for (std::map<std::string, void (*)(sort_element_t arr[], size_t n)>::iterator it = test_func_map.begin();
             it != test_func_map.end();
             ++it)
        {
            std::vector<sort_element_t> arr_c = arr;
            t = get_time();
            it->second(&*arr_c.begin(), (int)arr_c.size());
            int res_ms = (int)get_time_diff(t, get_time());
            printf("%12d", (int)res_ms);
            if (!baobao::check_sorted(arr_c.begin(), arr_c.end(), std::less<sort_element_t>()))
            {
                printf("E");
                result_map[it->first][it_test->first] = -1;
            }
            else
            {
                bool correct = true;
#if TEST_TYPE_SIMPLE == 0
                if (test_func_stable_map.find(it->first) != test_func_stable_map.end())
                {
                    if (!baobao::check_sorted_stable(arr_c.begin(), arr_c.end(), std::less<sort_element_t>()))
                    {
                        correct = false;
                        printf("E");
                        result_map[it->first][it_test->first] = -1;
                    }
                }
#endif
                if (correct)
                {
                    printf("|");
                    result_map[it->first][it_test->first] = res_ms;
                }
                fflush(stdout);
            }
        }
        puts("");
    }

    std::vector<std::string> index_name;
    std::vector<algo_score> final_score;

    for (std::map<std::string, void (*)(sort_element_t arr[], size_t n)>::iterator it_test = test_func_map.begin();
         it_test != test_func_map.end();
         ++it_test)
    {
        index_name.push_back(it_test->first);
        final_score.push_back(algo_score(0, (int)index_name.size() - 1, it_test->first));
        for (std::map<int, int (*)(int n, int len)>::iterator it = gen_data_fun_map.begin();
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

    printf("\n%-12s|", "test");
    for (std::map<int, int (*)(int n, int len)>::iterator it = gen_data_fun_map.begin();
         it != gen_data_fun_map.end();
         ++it)
    {
        printf("  %-3d|", it->first);
    }
    puts("score|");
    printf("------------|");
    for (std::map<int, int (*)(int n, int len)>::iterator it = gen_data_fun_map.begin();
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
        int sum = 0;
        for (std::map<int, int (*)(int n, int len)>::iterator it = gen_data_fun_map.begin();
             it != gen_data_fun_map.end();
             ++it)
        {
            int v = result_map[name][it->first];
            sum += v;
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
        printf("%5d|\n", it_test->score / 10);
    }
    printf("\n%u END\n", (unsigned)arr.size());
    fflush(stdout);

    return 0;
}
