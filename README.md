# Sort

# Overview

This is a sorting library, compatible with C++98

## Sorting algorithm list

- Insertion Sort (stable)
- Heapsort
- Shellsort
- Introsort (quicksort + heapsort)
- Mergesort (stable)
- Timsort (stable)

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

    baobao::sort::tim_sort(arr.begin(), arr.end(), std::less<int>());
    return 0;
}
```

Call it like STL as well

# Performance

Run the code [sorttest.cpp](sorttest.cpp), it will output the result

Build with `g++ -std=c++11 -O3 sorttest.cpp` on Centos 7 x64, gcc version is 8.3.1

#### sorting int

test        |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |score|
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |    9|  121|  153|  580|  585|  131|   87|  171|  144|  256|  562|  279|
bao_tim     |    4|   45|   13|  685|  711|  325|  119|    8|  103|  207|  669|  288|
std_sort    |  111|  135|  116|  567|  577|  215|  194|  135|  269|  231|  558|  310|
bao_merge   |    9|   41|  143|  717|  709|  351|  166|   84|  119|  235|  689|  326|
std_stable  |  160|  178|  178|  721|  678|  352|  187|  188|  188|  283|  674|  378|
bao_shell   |  123|  114|  157|  994| 1000|  393|  152|  123|  355|  228|  986|  462|
std_qsort   |  288|  306|  333| 1016|  999|  657|  499|  355|  437|  456| 1028|  637|
std_heap    |  478|  422|  513| 1806| 1865|  563|  517|  411| 1194|  508| 1908| 1018|
bao_heap    |   26|  524|  545| 1909| 1910|  647|  589|  528| 1231|  615| 1941| 1046|

#### sorting TestClass

test        |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |score|
------------|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|----:|
bao_qsort   |   12|   77|   85|  181|  200|   66|   58|   94|   86|  121|  191|  117|
std_sort    |  134|   70|   72|  185|  203|  155|  130|   90|  172|  110|  193|  151|
bao_tim     |    5|   24|   22|  279|  293|  241|  174|    9|  155|  107|  259|  156|
bao_merge   |    8|   25|  187|  282|  279|  245|  196|   59|  165|  101|  297|  184|
bao_shell   |  142|  136|  169|  448|  479|  255|  204|  154|  210|  165|  405|  276|
std_stable  |  303|  290|  302|  337|  346|  347|  295|  290|  295|  316|  326|  344|
std_qsort   |  230|  204|  306|  470|  498|  394|  364|  230|  335|  247|  492|  377|
bao_heap    |   36|  234|  241|  837|  849|  375|  380|  229|  603|  230|  835|  484|
std_heap    |  232|  221|  271|  834|  874|  371|  385|  244|  659|  248|  877|  521|
