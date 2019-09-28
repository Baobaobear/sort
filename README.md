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
bao_qsort   |   11|   62|   67|  168|  166|   63|   57|   87|   92|   95|  173|  127|   97|
std::sort   |  116|   66|   67|  174|  170|  123|  121|   72|  160|  111|  164|  130|  122|
bao_tim     |    6|   31|   17|  279|  244|  219|  182|   13|  139|   87|  234|  126|  131|
bao_merge   |   18|   29|  173|  264|  244|  203|  170|   56|  144|   92|  232|  124|  145|
bao_mer_buf |    5|   18|  138|  395|  355|  222|  209|   51|  220|   85|  343|  126|  180|
bao_shell   |  121|  121|  141|  379|  403|  230|  166|  120|  192|  147|  348|  143|  209|
bao_mer_in  |    5|   17|  153|  571|  594|  282|  223|   53|  265|  116|  513|  169|  246|
std::stable |  287|  303|  294|  338|  317|  318|  305|  299|  297|  292|  322|  154|  293|
std_qsort   |  172|  176|  271|  419|  437|  361|  303|  213|  283|  222|  425|  263|  295|
grail_dyn   |   83|  318|  353|  478|  445|  424|  341|  300|  455|  309|  429|  257|  349|
bao_heap    |   33|  236|  197|  729|  745|  282|  267|  197|  447|  188|  754|  175|  354|
grail_buf   |   90|  319|  381|  481|  500|  442|  321|  292|  443|  336|  471|  219|  357|
grail_in    |   85|  318|  370|  489|  527|  442|  346|  314|  481|  337|  476|  242|  368|
std::heap   |  204|  190|  279|  786|  787|  307|  305|  186|  566|  206|  771|  216|  400|

#### Sorting 4,500,000 int

int         |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 | Avg |
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |    4|   42|   45|  288|  279|   75|   47|   64|   77|  102|  270|  193|  123|
bao_tim     |    2|   37|    6|  327|  333|  157|   76|    4|   64|  105|  316|  212|  136|
std::sort   |   80|   55|   67|  290|  306|  125|   91|   76|  122|  127|  285|  220|  153|
bao_merge   |    4|   17|   76|  347|  338|  182|  115|   58|   67|  102|  326|  234|  155|
bao_mer_buf |    4|   16|   84|  375|  375|  172|  133|   58|   92|  106|  355|  218|  165|
std::stable |   93|   88|  104|  349|  340|  189|  107|   99|   99|  150|  333|  257|  184|
bao_shell   |   52|   49|   68|  503|  487|  193|   57|   55|  146|  108|  460|  239|  201|
grail_dyn   |   59|  123|  140|  426|  431|  401|  188|  108|  355|  172|  399|  390|  266|
grail_buf   |   58|  116|  142|  439|  457|  399|  177|  111|  354|  176|  411|  404|  270|
grail_in    |   57|  126|  154|  450|  454|  394|  177|  118|  351|  180|  424|  428|  276|
std_qsort   |  125|  140|  160|  522|  500|  321|  219|  161|  187|  230|  497|  386|  287|
bao_mer_in  |    3|   16|  113|  764|  774|  272|  227|   58|  182|  129|  728|  312|  298|
bao_heap    |    9|  203|  218|  876|  838|  255|  244|  213|  412|  241|  831|  299|  386|
std::heap   |  231|  210|  249|  901|  857|  329|  273|  239|  488|  255|  889|  354|  439|

# Benchmark of random shuffle data 

The x-axis is `log2(length)`

The y-axis is `time / length * 1000000`

#### Sorting TestClass

[![](img/benchmark_class8.png)](img/benchmark_class8.png)

#### Sorting int

[![](img/benchmark_int.png)](img/benchmark_int.png)



# License

This project is licensed under the MIT License.

