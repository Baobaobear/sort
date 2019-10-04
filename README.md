# Sort

[![Build Status](https://travis-ci.org/Baobaobear/sort.svg?branch=master)](https://travis-ci.org/Baobaobear/sort) [![Build status](https://ci.appveyor.com/api/projects/status/wtt8m5ss33jxl3og?svg=true)](https://ci.appveyor.com/project/Baobaobear/sort) [![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/) [![Standard](https://img.shields.io/badge/C++-03-orange.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

# Overview

This is a sorting library, compatible with C++03

## Algorithm table

Algorithm     | Stable|Best|  Average  |   Worst   | Mem | Header | Name |
-------------------|---|---|-----------|-----------|-----|--------| -----|
Insertion sort     |yes| n | n^2       | n^2       | 1   | sortlib.hpp | insert_sort |
Heapsort           |no | n | nlogn     | nlogn     | 1   | sortlib.hpp | heap_sort |
Shellsort          |no | n | n^(4/3) ? | n^(4/3) ? | 1   | sortlib.hpp | shell_sort |
Quicksort          |no | n | nlogn     | nlogn     | logn| sortlib.hpp | quick_sort |
Quicksort indirect |yes| n | nlogn     | nlogn     | logn| sortlib.hpp | indirect_qsort |
Mergesort          |yes| n | nlogn     | nlogn     | n   | sortlib.hpp | merge_sort |
Mergesort buffer   |yes| n | n(logn)^2 | n(logn)^2 | logn| sortlib.hpp | merge_sort_buffer |
Mergesort in-place |yes| n | n(logn)^2 | n(logn)^2 | logn| sortlib.hpp | merge_sort_in_place |
Timsort            |yes| n | nlogn     | nlogn     | n   | sortlib.hpp | tim_sort |
Grailsort dynamic  |yes| n | nlogn     | nlogn | sqrt(n) | grailsort.hpp | grail_sort |
Grailsort buffer   |yes| n | nlogn     | nlogn | 1       | grailsort.hpp | grail_sort_buffer |
Grailsort in-place |yes| n | nlogn     | nlogn | 1       | grailsort.hpp |grail_sort_in_place|
Wikisort           |yes| n | nlogn     | nlogn     | 1   | wikisort.hpp | wiki_sort |

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

Note:  
`merge_sort_s`, `merge_sort_buffer_s`, `tim_sort_s` is the safe copy version if you overload operator `=` and do something different

# Performance

Run the code [sorttest.cpp](sorttest.cpp), it will output the result

Build with `g++ -std=c++03 -O3 sorttest.cpp` on Centos 7 x64, gcc version is 8.3.1

Functions name with `bao_` perfix are in `sortlib.hpp` header  
Functions name with `grail_` perfix are in `grailsort.hpp` header  
`std_qsort` is the `qsort` function in `stdlib.h` header

#### Sorting 2,000,000 TestClass

TestClass 8 |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 | Avg |
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |   11|   24|   30|  156|  157|   57|   49|   66|   79|   91|  151|  125|   83|
std::sort   |  108|   59|   59|  174|  167|  121|  125|   70|  153|   86|  162|  133|  118|
bao_tim     |    5|   10|   16|  227|  236|  203|  153|    9|  130|   79|  246|  116|  119|
bao_merge   |    5|   15|  153|  240|  246|  199|  161|   52|  144|   77|  230|  123|  137|
bao_shell   |   10|   14|  129|  364|  380|  198|  147|   88|  171|  104|  354|  138|  174|
bao_mer_buf |    5|   15|  156|  383|  365|  220|  181|   57|  233|   92|  318|  125|  179|
wiki_sort   |   14|   71|  220|  352|  359|  321|  270|  144|  243|  178|  340|  146|  221|
bao_mer_in  |    6|   14|  123|  521|  555|  264|  227|   53|  243|   86|  485|  164|  228|
std::stable |  292|  280|  279|  332|  326|  309|  291|  285|  273|  301|  334|  150|  287|
std_qsort   |  172|  171|  266|  442|  428|  359|  315|  193|  277|  221|  428|  261|  294|
grail_dyn   |   83|  288|  349|  444|  466|  457|  334|  301|  441|  305|  423|  221|  342|
bao_heap    |   33|  184|  181|  739|  776|  280|  294|  173|  489|  177|  721|  184|  352|
bao_indir   |   81|   56|   61|  734|  761|  568|  566|  119|  585|  138|  661|  156|  373|
std::heap   |  205|  191|  269|  852|  772|  311|  303|  197|  505|  204|  785|  223|  401|

#### Sorting 4,500,000 int

int         |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 | Avg |
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |    4|   12|   23|  270|  272|   79|   38|   70|   64|  118|  267|  191|  117|
bao_tim     |    2|    3|    6|  320|  324|  144|   59|    5|   48|  104|  305|  196|  126|
std::sort   |   44|   57|   70|  281|  289|  110|   88|   72|  125|  118|  270|  220|  145|
bao_merge   |    3|   19|   66|  337|  339|  145|   85|   66|   52|  108|  317|  217|  146|
bao_mer_buf |    3|   16|   93|  381|  370|  164|  119|   55|   80|  102|  344|  213|  161|
bao_shell   |    4|    5|   55|  412|  417|  171|   54|   36|  153|   87|  384|  221|  166|
std::stable |   87|   86|   89|  348|  341|  175|   87|   98|   92|  150|  332|  246|  177|
wiki_sort   |   17|   39|   89|  420|  410|  229|  127|   86|  132|  166|  396|  243|  196|
grail_dyn   |   63|  112|  136|  430|  448|  389|  158|  110|  336|  175|  394|  404|  262|
std_qsort   |  126|  131|  160|  493|  492|  331|  214|  160|  185|  227|  490|  381|  282|
bao_mer_in  |    3|   16|  100|  770|  766|  268|  238|   59|  170|  123|  747|  314|  297|
bao_heap    |    8|  235|  241|  848|  847|  262|  247|  220|  472|  243|  868|  307|  399|
std::heap   |  269|  215|  239|  880|  841|  283|  250|  205|  483|  244|  878|  357|  428|
bao_indir   |  115|   85|   96| 1459| 1510| 1127| 1155|  177| 1165|  265| 1338|  326|  734|

# Benchmark of random shuffle data 

The x-axis is `log2(length)`

The y-axis is `time / length * 1000000`

#### Sorting TestClass

[![](img/benchmark_class8.png)](img/benchmark_class8.png)

#### Sorting int

[![](img/benchmark_int.png)](img/benchmark_int.png)



# License

This project is licensed under the MIT License.

