# Sort

[![Build Status]][Travis CI][![Appveyor status]][Appveyor] [![Language]](https://isocpp.org/)[![Standard]][Standard Cpp][![License]][MIT]

# Overview

This is a highly-optimized sorting library, compatible with C++03

## Algorithm table

Algorithm     | Stable|Best| Average | Worst | Mem | Header | Name |
-------------------|---|---|---------|-------|-----|--------|------|
Insertion sort     |yes| n | n²      | n²    | 1   | sortlib.hpp | insert_sort         |
Heapsort           |no | n | n㏒n    | n㏒n  | 1   | sortlib.hpp | heap_sort           |
Shellsort |no | n | n<sup>5/4</sup> ? | n<sup>1.5</sup> ? | 1 | sortlib.hpp | shell_sort |
Quicksort          |no | n | n㏒n    | n㏒n  | ㏒n | sortlib.hpp | quick_sort          |
Quicksort indirect |yes| n | n㏒n    | n㏒n  | n   | sortlib.hpp | indirect_qsort      |
Mergesort          |yes| n | n㏒n    | n㏒n  | n   | sortlib.hpp | merge_sort          |
Mergesort buffer   |yes| n | n㏒²n   | n㏒²n | √n  | sortlib.hpp | merge_sort_buffer   |
Mergesort in-place |yes| n | n㏒²n   | n㏒²n | ㏒n | sortlib.hpp |merge_sort_in_place  |
Timsort            |yes| n | n㏒n    | n㏒n  | n   | sortlib.hpp | tim_sort            |
Timsort buffer     |yes| n | n㏒n    | n㏒n  | √n  | sortlib.hpp | tim_sort_buffer     |
[Grailsort]        |yes| n | n㏒n    | n㏒n  | √n  | grailsort.hpp | grail_sort        |
Grailsort buffer   |yes| n | n㏒n    | n㏒n  | 1   | grailsort.hpp | grail_sort_buffer |
Grailsort in-place |yes| n | n㏒n    | n㏒n  | 1   | grailsort.hpp |grail_sort_in_place|
[Wikisort]         |yes| n | n㏒n    | n㏒n  | 1   | wikisort.hpp  | wiki_sort         |

# Usage

Here is the demo, or you can try [demo.cpp]

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

### Note
`merge_sort_s`, `merge_sort_buffer_s`, `tim_sort_s` is the safe copy version if you overload operator `=` and do something different

# Performance

Run the code [sorttest.cpp], it will output the result

Build with `g++ -std=c++03 -O3 sorttest.cpp` on Centos 7 x64, gcc version is 8.3.1

Functions name with `bao_` perfix are in `sortlib.hpp` header  
Functions name with `grail_` perfix are in `grailsort.hpp` header  
`std_qsort` is the `qsort` function in `stdlib.h` header

#### Sorting 2,000,000 TestClass

TestClass 8 |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 | Avg |
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |   11|   27|   39|  190|  169|   63|   62|   79|   98|  170|  129|   94|
std::sort   |  143|   62|   80|  190|  178|  146|  138|   94|  101|  192|  144|  133|
bao_tim     |    8|   12|   18|  263|  273|  224|  195|    9|   94|  266|  152|  137|
bao_merge   |    8|   19|  193|  254|  261|  220|  184|   55|   92|  261|  147|  154|
bao_tim_buf |    6|   11|   21|  363|  404|  262|  203|   10|   90|  329|  145|  167|
bao_mer_buf |    6|   21|  156|  374|  357|  244|  223|   67|   94|  349|  145|  185|
bao_shell   |    9|   12|  159|  443|  455|  248|  171|  101|  132|  425|  158|  210|
wiki_sort   |   15|   87|  267|  397|  434|  375|  307|  151|  220|  402|  171|  256|
bao_mer_in  |    7|   19|  165|  592|  603|  309|  251|   58|   97|  581|  216|  263|
std::stable |  372|  344|  356|  378|  377|  380|  321|  337|  332|  364|  182|  340|
std_qsort   |  181|  215|  355|  463|  524|  416|  379|  227|  281|  469|  339|  349|
grail_sort  |  104|  336|  396|  531|  517|  520|  359|  364|  366|  494|  276|  387|
bao_heap    |   30|  258|  220|  844|  908|  332|  318|  199|  234|  889|  200|  402|
bao_indir   |  103|   65|   67|  855|  833|  679|  684|  129|  162|  784|  185|  413|
std::heap   |  235|  247|  287|  901|  938|  398|  339|  232|  240|  988|  274|  461|

#### Sorting 4,500,000 int

int         |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 | Avg |
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |    7|   16|   21|  289|  300|   79|   36|   80|  125|  293|  208|  132|
bao_tim     |    2|    3|    6|  364|  363|  172|   89|    7|  119|  345|  226|  154|
std::sort   |   55|   71|   67|  312|  313|  116|   94|   76|  142|  281|  230|  159|
bao_tim_buf |    3|    5|    7|  402|  406|  183|  100|    6|  112|  390|  238|  168|
bao_merge   |    6|   17|   74|  373|  362|  187|  122|   78|  127|  360|  235|  176|
bao_mer_buf |    6|   19|   67|  420|  401|  195|  129|   74|  115|  391|  247|  187|
bao_shell   |    5|    6|   65|  481|  459|  203|   73|   40|   95|  448|  245|  192|
std::stable |  101|   99|  106|  398|  369|  217|  110|  107|  171|  376|  260|  210|
wiki_sort   |   19|   51|   91|  452|  476|  259|  172|   93|  171|  448|  266|  227|
grail_sort  |   75|  131|  164|  460|  458|  433|  202|  130|  189|  423|  467|  284|
std_qsort   |  148|  149|  198|  536|  534|  368|  257|  187|  256|  515|  408|  323|
bao_mer_in  |    4|   19|  137|  898|  954|  330|  263|   89|  136|  926|  359|  374|
bao_heap    |   11|  247|  291| 1002|  957|  337|  278|  244|  282|  971|  311|  448|
std::heap   |  246|  259|  332|  996|  984|  303|  300|  249|  299|  982|  425|  488|
bao_indir   |  133|   96|  107| 1643| 1624| 1327| 1337|  225|  294| 1486|  357|  784|

# Benchmark of random shuffle data 

The x-axis is `log2(length)`

The y-axis is `time / length * 1000000`

#### Sorting TestClass

[![](img/benchmark_class8.png)](img/benchmark_class8.png)

#### Sorting int

[![](img/benchmark_int.png)](img/benchmark_int.png)



# License

This project is licensed under the MIT License.

[Build Status]:     https://travis-ci.org/Baobaobear/sort.svg?branch=master
[Travis CI]:        https://travis-ci.org/Baobaobear/sort
[Appveyor status]:  https://ci.appveyor.com/api/projects/status/wtt8m5ss33jxl3og?svg=true
[Appveyor]:         https://ci.appveyor.com/project/Baobaobear/sort
[Language]:         https://img.shields.io/badge/language-C++-blue.svg
[Standard]:         https://img.shields.io/badge/C++-03-orange.svg
[Standard Cpp]:     https://en.wikipedia.org/wiki/C%2B%2B#Standardization
[License]:          https://img.shields.io/badge/license-MIT-blue.svg
[MIT]:              https://opensource.org/licenses/MIT
[sorttest.cpp]:     sorttest.cpp
[demo.cpp]:         demo.cpp
[Grailsort]:        https://github.com/Mrrl/GrailSort
[Wikisort]:         https://github.com/BonzaiThePenguin/WikiSort
