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
bao_qsort   |   10|   22|   54|  153|  160|   64|   66|   71|   74|   91|  152|  111|   85|
bao_tim     |    6|    8|   15|  225|  226|  182|  153|    8|  128|   79|  221|  130|  115|
std::sort   |  102|   55|   56|  164|  175|  114|  156|   67|  153|   91|  169|  127|  119|
bao_merge   |    5|   14|  165|  224|  224|  184|  174|   50|  132|   76|  219|  127|  132|
bao_mer_buf |    5|   18|  123|  351|  351|  204|  178|   58|  210|   75|  318|  149|  170|
bao_shell   |  110|  113|  126|  389|  388|  210|  160|  115|  220|  129|  371|  140|  205|
bao_mer_in  |    5|   20|  120|  552|  537|  239|  200|   46|  234|   98|  523|  167|  228|
std::stable |  277|  290|  297|  333|  316|  291|  275|  294|  277|  288|  327|  160|  285|
std_qsort   |  170|  178|  258|  431|  422|  383|  301|  192|  299|  224|  416|  266|  295|
grail_buf   |   78|  292|  306|  454|  470|  398|  313|  289|  437|  279|  423|  217|  329|
grail_dyn   |   82|  291|  324|  454|  440|  403|  334|  299|  460|  303|  419|  215|  335|
grail_in    |   80|  310|  319|  464|  481|  404|  313|  285|  444|  307|  431|  237|  339|
bao_heap    |   28|  168|  187|  727|  728|  266|  257|  181|  517|  180|  700|  171|  342|
std::heap   |  213|  182|  231|  754|  745|  311|  295|  192|  499|  203|  759|  236|  385|

#### Sorting 4,500,000 int

int         |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 | Avg |
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |    4|   15|   24|  275|  270|   78|   43|   65|   73|  107|  258|  184|  116|
bao_tim     |    3|    3|    6|  324|  322|  141|   78|    4|   55|  100|  310|  213|  129|
std::sort   |   44|   54|   56|  286|  293|  108|   87|   69|  121|  117|  260|  214|  142|
bao_merge   |    4|   21|   57|  319|  333|  152|   97|   64|   60|  101|  328|  207|  145|
bao_mer_buf |    4|   18|   83|  348|  370|  158|  135|   59|   89|  101|  365|  217|  162|
std::stable |   84|   80|   83|  343|  337|  206|  101|   94|  107|  141|  314|  253|  178|
bao_shell   |   56|   55|   66|  467|  494|  189|   55|   52|  157|  107|  461|  220|  198|
grail_dyn   |   55|  104|  131|  423|  425|  384|  180|  102|  345|  180|  393|  389|  259|
grail_in    |   55|  115|  139|  450|  446|  376|  170|  109|  358|  171|  379|  416|  265|
grail_buf   |   75|  105|  163|  436|  446|  385|  186|  104|  349|  160|  406|  394|  267|
std_qsort   |  124|  132|  157|  514|  493|  309|  208|  187|  204|  233|  459|  377|  283|
bao_mer_in  |    3|   18|   99|  723|  769|  266|  258|   70|  184|  122|  732|  308|  296|
bao_heap    |    9|  210|  251|  762|  906|  257|  236|  230|  440|  270|  776|  287|  386|
std::heap   |  225|  203|  262|  845|  816|  270|  268|  206|  475|  248|  783|  346|  412|

# Benchmark of random shuffle data 

The x-axis is `log2(length)`

The y-axis is `time / length * 1000000`

#### Sorting TestClass

[![](img/benchmark_class8.png)](img/benchmark_class8.png)

#### Sorting int

[![](img/benchmark_int.png)](img/benchmark_int.png)



# License

This project is licensed under the MIT License.

