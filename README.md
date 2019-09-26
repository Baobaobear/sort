# Sort

[![Build Status](https://travis-ci.org/Baobaobear/sort.svg?branch=master)](https://travis-ci.org/Baobaobear/sort) [![Build status](https://ci.appveyor.com/api/projects/status/wtt8m5ss33jxl3og?svg=true)](https://ci.appveyor.com/project/Baobaobear/sort) [![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/) [![Standard](https://img.shields.io/badge/C++-98-orange.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

# Overview

This is a sorting library, compatible with C++98

## Algorithm table

algorithm     | stable | Best  |  Average  | Worst | Memory |
-------------------|---|-------|-----------|-----------|-
Insertion sort     |yes| n     | n^2       | n^2       | 1
Heapsort           |no | n     | nlogn     | nlogn     | 1
Shellsort          |no | nlogn | n^(4/3) ? | n^(4/3) ? | 1
Introsort (qsort)  |no | n     | nlogn     | nlogn     | logn
Mergesort          |yes| n     | nlogn     | nlogn     | n
Mergesort in-place |yes| n     | n(logn)^2 | n(logn)^2 | logn
Timsort            |yes| n     | nlogn     | nlogn     | n

# Usage

Here is the demo, or you can try [demo.cpp](demo.cpp)

```c
#include "sortlib.hpp"
#include <cstdlib>

int main(void)
{
    std::vector<int> arr(100);

    for (size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = rand();
    }

    baobao::sort::tim_sort(arr.begin(), arr.end());
    return 0;
}
```

Call it like STL as well

# Performance

Run the code [sorttest.cpp](sorttest.cpp), it will output the result

Build with `g++ -std=c++03 -O3 sorttest.cpp` on Centos 7 x64, gcc version is 8.3.1

#### Sorting 2,000,000 TestClass

TestClass 8 |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 | Avg |
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |   11|   61|   64|  206|  177|   74|  106|   87|   82|  114|  187|  131|  108|
std::sort   |  128|   76|   64|  184|  185|  145|  139|   79|  178|  109|  184|  155|  135|
bao_tim     |    7|   31|   19|  267|  275|  226|  170|   12|  148|   95|  260|  149|  138|
bao_merge   |    6|   16|  169|  266|  279|  224|  205|   57|  165|   96|  269|  155|  158|
bao_shell   |  132|  133|  159|  431|  452|  260|  185|  145|  233|  148|  409|  148|  236|
bao_mer_in  |    5|   14|  564|  489|  473|  383|  367|   54|  307|   93|  465|  152|  280|
std::stable |  530|  293|  324|  379|  354|  334|  330|  301|  319|  315|  349|  180|  334|
std_qsort   |  405|  205|  594|  476|  495|  432|  358|  228|  323|  270|  488|  293|  380|
bao_heap    |   22|  239|  248|  854|  822|  344|  321|  247|  606|  219|  845|  217|  415|
std::heap   |  236|  251|  313|  866|  891|  377|  356|  226|  667|  274|  900|  266|  468|

#### Sorting 8,000,000 int

int         |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 | Avg |
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |    8|  117|  138|  593|  569|  131|   95|  161|  133|  252|  568|  419|  265|
bao_tim     |    5|   52|   15|  677|  659|  313|  162|   10|  121|  216|  640|  413|  273|
std::sort   |   88|  135|  130|  585|  579|  225|  194|  146|  250|  230|  558|  434|  296|
bao_merge   |    7|   32|  131|  694|  700|  323|  175|   95|  139|  208|  678|  468|  304|
std::stable |  179|  189|  188|  699|  695|  390|  209|  194|  191|  297|  657|  478|  363|
bao_mer_in  |    8|   34|  298|  825|  795|  405|  261|   94|  213|  210|  786|  465|  366|
bao_shell   |  109|  121|  175| 1011|  986|  397|  138|  120|  326|  223|  929|  450|  415|
std_qsort   |  308|  301|  339| 1032| 1011|  655|  467|  362|  429|  456|  995|  755|  592|
std::heap   |  476|  478|  562| 2048| 1980|  626|  565|  488| 1384|  508| 2001|  701|  984|
bao_heap    |   22|  536|  594| 2096| 2103|  694|  602|  512| 1375|  601| 2115|  638|  990|

# Benchmark of random shuffle data 

The x-axis is `log2(length)`

The y-axis is `time / length * 1000000`

#### Sorting TestClass

[![](img/benchmark_class8.png)](img/benchmark_class8.png)

#### Sorting int

[![](img/benchmark_int.png)](img/benchmark_int.png)



# License

This project is licensed under the MIT License.

