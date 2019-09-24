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
Introsort (qsort)  |no | n     | nlogn     | nlogn     | nlogn
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

#### sorting TestClass

test        |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 |score|
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |   12|   75|   69|  196|  191|   81|   55|   85|   87|  133|  192|  151|  132|
std_sort    |  134|   73|   68|  208|  202|  156|  143|   82|  190|  110|  195|  155|  171|
bao_tim     |    6|   31|   19|  276|  269|  230|  194|   12|  158|  100|  280|  148|  172|
bao_merge   |    6|   21|  172|  289|  302|  250|  201|   61|  180|  119|  293|  173|  206|
bao_shell   |  149|  145|  168|  488|  485|  274|  196|  151|  232|  170|  450|  179|  308|
std_stable  |  294|  316|  309|  360|  365|  348|  328|  316|  306|  317|  356|  187|  380|
std_qsort   |  215|  207|  313|  494|  494|  432|  373|  248|  350|  285|  496|  326|  423|
bao_heap    |   24|  217|  254|  860|  916|  373|  361|  233|  587|  276|  874|  237|  521|
bao_mer_in  |    7|   18|  868|  808|  787|  565|  522|   64|  450|  135|  724|  289|  523|
std_heap    |  248|  226|  310|  898|  926|  409|  373|  248|  636|  260|  883|  272|  568|

#### sorting int

test        |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 |score|
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |   10|  124|  153|  625|  607|  141|   98|  172|  146|  274|  595|  424|  336|
bao_tim     |    4|   59|   16|  707|  714|  334|  129|   11|  111|  222|  676|  427|  341|
std_sort    |   95|  125|  127|  607|  586|  246|  193|  147|  270|  248|  582|  437|  366|
bao_merge   |   10|   34|  169|  715|  714|  331|  167|   98|  105|  226|  698|  479|  374|
std_stable  |  177|  196|  204|  722|  728|  362|  215|  202|  215|  303|  689|  490|  450|
bao_shell   |  118|  123|  179| 1099| 1063|  425|  170|  137|  371|  260| 1019|  469|  543|
std_qsort   |  307|  341|  370| 1069| 1073|  710|  493|  379|  436|  496| 1072|  788|  753|
bao_mer_in  |   10|   31|  608| 2088| 2090|  918|  680|  129|  513|  355| 2012|  937| 1037|
bao_heap    |   35|  562|  598| 2142| 2150|  660|  636|  537| 1263|  596| 2130|  656| 1196|
std_heap    |  478|  537|  618| 2081| 2055|  719|  610|  518| 1381|  585| 2132|  759| 1247|

# License

This project is licensed under the MIT License.

