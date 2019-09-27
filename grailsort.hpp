/********* Grail sorting *********************************/
/*                                                       */
/* (c) 2013 by Andrey Astrelin                           */
/*                                                       */
/*                                                       */
/* Stable sorting that works in O(N*log(N)) worst time   */
/* and uses O(1) extra memory                            */
/*                                                       */
/* For sorting without external buffer                   */
/* use GrailSort()                                       */
/*                                                       */
/* For sorting with fixed external buffer (512 items)    */
/* use GrailSortWithBuffer()                             */
/*                                                       */
/* For sorting with dynamic external buffer (O(sqrt(N)) items) */
/* use GrailSortWithDynBuffer()                          */
/*                                                       */
/*********************************************************/

#define GRAIL_EXT_BUFFER_LENGTH 512

namespace Mrrl
{

template <class ValueType, class Comp>
inline bool grail_lesseq(const ValueType& a, const ValueType& b, Comp compare)
{
    return !compare(b, a);
}

template <class ValueType, class Comp>
inline bool grail_not_eq(const ValueType& a, const ValueType& b, Comp compare)
{
    return compare(a, b) || compare(b, a);
}

template <class ValueType, class Comp>
inline int grail_cmp(const ValueType& a, const ValueType& b, Comp compare)
{
    if (compare(a, b))
        return -1;
    else if (compare(b, a))
        return 1;
    return 0;
}

template <class ValueType>
inline void grail_swap(ValueType &a, ValueType &b)
{
    ValueType c = a;
    a = b;
    b = c;
}
template <class RandomAccessIterator>
inline void grail_swap1(RandomAccessIterator a, RandomAccessIterator b)
{
    grail_swap(*a, *b);
}

template <class RandomAccessIterator>
inline void grail_swapN(RandomAccessIterator a, RandomAccessIterator b, size_t n)
{
    while (n--) grail_swap1(a++, b++);
}

template <class RandomAccessIterator>
void swap_2_part_with_same_length(RandomAccessIterator beg, RandomAccessIterator mid)
{
    for (RandomAccessIterator l = beg, r = mid; l < mid; ++l, ++r)
    {
        std::swap(*l, *r);
    }
}

template <class RandomAccessIterator>
void grail_rotate(RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end)
{
    while (true)
    {
        if (mid - beg <= end - mid)
        {
            if (mid - beg > 0)
            {
                swap_2_part_with_same_length(beg, mid);
                RandomAccessIterator m = mid;
                mid += mid - beg;
                beg = m;
            }
            else
                break;
        }
        else
        {
            if (end - mid > 0)
            {
                swap_2_part_with_same_length(mid - (end - mid), mid);
                RandomAccessIterator m = mid;
                mid -= end - mid;
                end = m;
            }
            else
                break;
        }
    }
}

template <class RandomAccessIterator, class Comp>
RandomAccessIterator grail_BinSearchLeft(RandomAccessIterator beg, RandomAccessIterator end, RandomAccessIterator key, Comp compare)
{
    RandomAccessIterator a = beg - 1, b = end, c;
    while (a < b - 1)
    {
        c = a + ((b - a) >> 1);
        if (compare(*c, *key)) a = c;
        else b = c;
    }
    return b;
}
template <class RandomAccessIterator, class Comp>
RandomAccessIterator grail_BinSearchRight(RandomAccessIterator beg, RandomAccessIterator end, RandomAccessIterator key, Comp compare)
{
    RandomAccessIterator a = beg - 1, b = end, c;
    while (a < b - 1)
    {
        c = a + ((b - a) >> 1);
        if (compare(*key, *c)) b = c;
        else a = c;
    }
    return b;
}

// cost: 2*len+nk^2/2
template <class RandomAccessIterator, class Comp>
RandomAccessIterator grail_FindKeys(RandomAccessIterator beg, RandomAccessIterator end, RandomAccessIterator nkeys, Comp compare)
{
    RandomAccessIterator h0 = beg, u = beg + 1; // first key is always here
    int h = 1;
    while (u < end && h < nkeys - beg)
    {
        int r = (int)(grail_BinSearchLeft(h0, h0 + h, u, compare) - h0);
        if (r == h || grail_not_eq(*u, *(h0 + r), compare)) {
            grail_rotate(h0, h0 + h, u);
            h0 = u - h;
            grail_rotate(h0 + r, u, u + 1);
            ++h;
        }
        ++u;
    }
    grail_rotate(beg, h0, h0 + h);
    return beg + h;
}

// cost: min(L1,L2)^2+max(L1,L2)
template <class RandomAccessIterator, class Comp>
void grail_MergeWithoutBuffer(RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, Comp compare) {
    if (mid - beg < end - mid) {
        while (mid - beg) {
            RandomAccessIterator p = grail_BinSearchLeft(mid, end, beg, compare);
            if (p != mid) {
                grail_rotate(beg, mid, p);
                beg += p - mid;
                mid = p;
            }
            if (mid == end) break;
            do {
                ++beg;
            } while ((mid - beg) && grail_lesseq(*beg, *mid, compare));
        }
    }
    else {
        while (end - mid) {
            RandomAccessIterator p = grail_BinSearchRight(beg, mid, end - 1, compare);
            if (p != mid) {
                grail_rotate(p, mid, end);
                end -= mid - p;
                mid = p;
            }
            if (beg == mid) break;
            do {
                --end;
            } while ((end - mid) && grail_lesseq(*(mid - 1), *(end - 1), compare));
        }
    }
}

// arr[M..-1] - buffer, arr[0,L1-1]++arr[L1,L1+L2-1] -> arr[M,M+L1+L2-1]
template <class RandomAccessIterator, class Comp>
void grail_MergeLeft(RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, RandomAccessIterator M, Comp compare) {
    RandomAccessIterator p0 = beg, p1 = mid;
    while (p1 < end) {
        if (p0 == mid || compare(*p1, *p0)) {
            grail_swap1(M++, p1++);
        } else {
            grail_swap1(M++, p0++);
        }
    }
    if (M != p0) grail_swapN(M, p0, mid - p0);
}
template <class RandomAccessIterator, class Comp>
void grail_MergeRight(RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, RandomAccessIterator M, Comp compare) {
    RandomAccessIterator p0 = M - 1, p2 = end - 1, p1 = mid - 1;
    while (p1 >= beg) {
        if (p2 < mid || compare(*p2, *p1)) {
            grail_swap1(p0--, p1--);
        } else {
            grail_swap1(p0--, p2--);
        }
    }
    if (p2 != p0) while (p2 >= mid) grail_swap1(p0--, p2--);
}

template <class RandomAccessIterator, class Comp>
void grail_SmartMergeWithBuffer(RandomAccessIterator beg, int *alen1, int *atype, RandomAccessIterator end, RandomAccessIterator lkeys, Comp compare) {
    RandomAccessIterator p0 = lkeys, p1 = beg, p2 = beg + *alen1, q1 = p2, q2 = end;
    int ftype = !*atype;  // 1 if inverted
    while (p1 < q1 && p2 < q2) {
        if (grail_cmp(*p1, *p2, compare) - ftype < 0)
            grail_swap1(p0++, p1++);
        else grail_swap1(p0++, p2++);
    }
    if (p1 < q1) {
        *alen1 = q1 - p1;
        while (p1 < q1) grail_swap1(--q1, --q2);
    } else {
        *alen1 = q2 - p2;
        *atype = ftype;
    }
}
template <class RandomAccessIterator, class Comp>
void grail_SmartMergeWithoutBuffer(RandomAccessIterator beg, int *alen1, int *atype, RandomAccessIterator end, Comp compare) {
    int ftype;

    if (beg == end) return;
    RandomAccessIterator mid = beg + *alen1;
    ftype = !*atype;
    if ((mid - beg) && grail_cmp(*(mid - 1), *mid, compare) - ftype >= 0) {
        while (mid - beg) {
            RandomAccessIterator p = (ftype ? grail_BinSearchLeft(mid, end, beg, compare) : grail_BinSearchRight(mid, end, beg, compare));
            if (p > mid) {
                grail_rotate(beg, mid, p);
                beg += p - mid;
                mid = p;
            }
            if (mid == end) {
                *alen1 = mid - beg;
                return;
            }
            do {
                beg++;
            } while ((mid - beg) && grail_cmp(*beg, *mid, compare) - ftype < 0);
        }
    }
    *alen1 = end - beg; *atype = ftype;
}

/***** Sort With Extra Buffer *****/

template <class RandomAccessIterator, class Comp>
void grail_MergeLeftWithXBuf(RandomAccessIterator beg, RandomAccessIterator mid, RandomAccessIterator end, RandomAccessIterator M, Comp compare) {
    RandomAccessIterator p0 = beg, p1 = mid;
    while (p1 < end) {
        if (p0 == mid || compare(*p1, *p0)) *M++ = *p1++;
        else *M++ = *p0++;
    }
    if (M != p0) while (p0 < mid) *M++ = *p0++;
}

template <class RandomAccessIterator, class Comp>
void grail_SmartMergeWithXBuf(RandomAccessIterator beg, int *alen1, int *atype, RandomAccessIterator end, RandomAccessIterator lkeys, Comp compare) {
    RandomAccessIterator p0 = lkeys, p1 = beg, p2 = beg + *alen1, q1 = p2, q2 = end;
    int ftype = !*atype;  // 1 if inverted
    while (p1 < q1 && p2 < q2) {
        if (grail_cmp(*p1, *p2, compare) - ftype < 0)
            *p0++ = *p1++;
        else *p0++ = *p2++;
    }
    if (p1 < q1) {
        *alen1 = q1 - p1;
        while (p1 < q1) *--q2 = *--q1;
    }
    else {
        *alen1 = q2 - p2;
        *atype = ftype;
    }
}

// arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
// lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
// keys - arrays of keys, in same order as blocks. key<midkey means stream A
// nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B, that should go before nblock2 blocks.
// llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
template <class RandomAccessIterator, class Comp>
void grail_MergeBuffersLeftWithXBuf(RandomAccessIterator keys, RandomAccessIterator midkey, RandomAccessIterator arr, int nblock, int lblock, int nblock2, int llast, Comp compare) {
    RandomAccessIterator prest, pidx;
    int lrest, frest;

    if (nblock == 0) {
        int l = nblock2*lblock;
        grail_MergeLeftWithXBuf(arr, arr + l, arr + l + llast, arr - lblock, compare);
        return;
    }

    lrest = lblock;
    frest = compare(*keys, *midkey) ? 0 : 1;
    pidx = arr + lblock;
    for (int cidx = 1; cidx < nblock;cidx++, pidx += lblock) {
        prest = pidx - lrest;
        int fnext = compare(*(keys + cidx), *midkey) ? 0 : 1;
        if (fnext == frest) {
            //memcpy(prest - lblock, prest, lrest * sizeof(value_type)); // TODO
            for (int i = 0; i < lrest; ++i)
            {
                *(prest - lblock + i) = *(prest + i);
            }
            prest = pidx;
            lrest = lblock;
        }
        else {
            grail_SmartMergeWithXBuf(prest, &lrest, &frest, prest + lrest + lblock, prest - lblock, compare);
        }
    }
    prest = pidx - lrest;
    if (llast) {
        if (frest) {
            //memcpy(prest - lblock, prest, lrest * sizeof(value_type)); // TODO
            for (int i = 0; i < lrest; ++i)
            {
                *(prest - lblock + i) = *(prest + i);
            }
            prest = pidx;
            lrest = lblock*nblock2;
            frest = 0;
        }
        else {
            lrest += lblock*nblock2;
        }
        grail_MergeLeftWithXBuf(prest, prest + lrest, prest + lrest + llast, prest - lblock, compare);
    }
    else {
        //memcpy(prest - lblock, prest, lrest * sizeof(value_type));
        for (int i = 0; i < lrest; ++i)
        {
            *(prest - lblock + i) = *(prest + i);
        }
    }
}

/***** End Sort With Extra Buffer *****/

// build blocks of length K
// input: [-K,-1] elements are buffer
// output: first K elements are buffer, blocks 2*K and last subblock sorted
template <class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void grail_BuildBlocks(RandomAccessIterator beg, RandomAccessIterator end, int K, RandomAccessBufferIterator extbuf, int LExtBuf, Comp compare) {
    int h, restk, kbuf;
    kbuf = K < LExtBuf ? K : LExtBuf;
    while (kbuf&(kbuf - 1)) kbuf &= kbuf - 1;  // max power or 2 - just in case

    if (kbuf) {
        //memcpy(extbuf, &*(beg - kbuf), kbuf * sizeof(value_type)); // TODO
        for (int i = 0; i < kbuf; ++i)
        {
            *(extbuf + i) = *(beg - kbuf + i);
        }
        for (RandomAccessIterator m = beg + 1; m < end; m += 2) {
            int u = 0;
            if (compare(*m, *(m - 1))) u = 1;
            *(m - 3) = *(m - 1 + u);
            *(m - 2) = *(m - u);
        }
        if ((end - beg) % 2) *(end - 3) = *(end - 1);
        beg -= 2; end -= 2;
        for (h = 2; h < kbuf; h *= 2) {
            RandomAccessIterator p0 = beg;
            RandomAccessIterator p1 = end - 2 * h;
            while (p0 <= p1) {
                grail_MergeLeftWithXBuf(p0, p0 + h, p0 + h + h, p0 - h, compare);
                p0 += 2 * h;
            }
            if (end - p0 > h) {
                grail_MergeLeftWithXBuf(p0, p0 + h, end, p0 - h, compare);
            }
            else {
                for (; p0 < end; p0++) *(p0 - h) = *p0;
            }
            beg -= h; end -= h;
        }
        //memcpy(&*end, extbuf, kbuf * sizeof(value_type)); // TODO
        for (int i = 0; i < kbuf; ++i)
        {
            *(end + i) = *(extbuf + i);
        }
    }
    else {
        for (RandomAccessIterator m = beg + 1; m < end; m += 2) {
            int u = 0;
            if (compare(*m, *(m - 1))) u = 1;
            grail_swap1(m - 3, m - 1 + u);
            grail_swap1(m - 2, m - u);
        }
        if ((end - beg) % 2) grail_swap1(end - 1, end - 3);
        beg -= 2; end -= 2;
        h = 2;
    }
    for (; h < K; h *= 2) {
        RandomAccessIterator p0 = beg;
        RandomAccessIterator p1 = end - 2 * h;
        while (p0 <= p1) {
            grail_MergeLeft(p0, p0 + h, p0 + h + h, p0 - h, compare);
            p0 += 2 * h;
        }
        if (end - p0 > h) {
            grail_MergeLeft(p0, p0 + h, end, p0 - h, compare);
        }
        else grail_rotate(p0 - h, p0, end);
        beg -= h; end -= h;
    }
    restk = (end - beg) % (2 * K);
    RandomAccessIterator p = end - restk;
    if (restk <= K) grail_rotate(p, p + restk, p + restk + K);
    else grail_MergeRight(p, p + K, p + restk, p + restk + K, compare);
    while (p > beg) {
        p -= 2 * K;
        grail_MergeRight(p, p + K, p + K + K, p + K + K + K, compare);
    }
}

// arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
// lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
// keys - arrays of keys, in same order as blocks. key<midkey means stream A
// nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B, that should go before nblock2 blocks.
// llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
template <class RandomAccessIterator, class Comp>
void grail_MergeBuffersLeft(RandomAccessIterator keys, RandomAccessIterator midkey, RandomAccessIterator arr, int nblock, int lblock, bool havebuf, int nblock2, int llast, Comp compare) {
    RandomAccessIterator prest, pidx;
    int lrest, frest;

    if (nblock == 0) {
        int l = nblock2*lblock;
        if (havebuf) grail_MergeLeft(arr, arr + l, arr + l + llast, arr - lblock, compare);
        else grail_MergeWithoutBuffer(arr, arr + l, arr + l + llast, compare);
        return;
    }

    lrest = lblock;
    frest = compare(*keys, *midkey) ? 0 : 1;
    pidx = arr + lblock;
    for (int cidx = 1; cidx < nblock; cidx++, pidx += lblock) {
        prest = pidx - lrest;
        int fnext = compare(*(keys + cidx), *midkey) ? 0 : 1;
        if (fnext == frest) {
            if (havebuf) grail_swapN(prest - lblock, prest, lrest);
            prest = pidx;
            lrest = lblock;
        }
        else {
            if (havebuf) {
                grail_SmartMergeWithBuffer(prest, &lrest, &frest, prest + lrest + lblock, prest - lblock, compare);
            }
            else {
                grail_SmartMergeWithoutBuffer(prest, &lrest, &frest, prest + lrest + lblock, compare);
            }

        }
    }
    prest = pidx - lrest;
    if (llast) {
        if (frest) {
            if (havebuf) grail_swapN(prest - lblock, prest, lrest);
            prest = pidx;
            lrest = lblock*nblock2;
            frest = 0;
        }
        else {
            lrest += lblock*nblock2;
        }
        if (havebuf) grail_MergeLeft(prest, prest + lrest, prest + lrest + llast, prest - lblock, compare);
        else grail_MergeWithoutBuffer(prest, prest + lrest, prest + lrest + llast, compare);
    }
    else {
        if (havebuf) grail_swapN(prest, prest - lblock, lrest);
    }
}

template <class RandomAccessIterator, class Comp>
void grail_SortIns(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    for (RandomAccessIterator i = beg + 1; i < end; ++i)
    {
        if (compare(*i, *(i - 1)))
        {
            typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
            RandomAccessIterator j = i - 1;
            *i = *j;
            for (;j != beg && compare(val, *(j - 1)); --j)
            {
                *j = *(j - 1);
            }
            *j = val;
        }
    }
}

template <class RandomAccessIterator, class Comp>
void grail_LazyStableSort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare) {
    int L = (int)(end - beg);
    for (RandomAccessIterator m = beg + 1; m < end; m += 2) {
        if (compare(*m, *(m - 1))) grail_swap1(m - 1, m);
    }
    for (int h = 2; h < L; h *= 2) {
        RandomAccessIterator p0 = beg;
        RandomAccessIterator p1 = end - 2 * h;
        while (end - p0 >= 2 * h) {
            grail_MergeWithoutBuffer(p0, p0 + h, p0 + h + h, compare);
            p0 += 2 * h;
        }
        if (end - p0 > h) grail_MergeWithoutBuffer(p0, p0 + h, end, compare);
    }
}

// keys are on the left of arr. Blocks of length LL combined. We'll combine them in pairs
// LL and nkeys are powers of 2. (2*LL/lblock) keys are guarantied
template <class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void grail_CombineBlocks(RandomAccessIterator keys, RandomAccessIterator beg, RandomAccessIterator end, int LL, int lblock, bool havebuf, RandomAccessBufferIterator xbuf, Comp compare) {
    RandomAccessIterator arr1;

    int M = (int)((end - beg) / (2 * LL));
    int lrest = (int)((end - beg) % (2 * LL));
    if (lrest <= LL) {
        end -= lrest;
        lrest = 0;
    }
    if (xbuf)
    {
        //memcpy(xbuf, beg - lblock, lblock * sizeof(value_type)); // TODO
        for (int i = 0; i < lblock; ++i)
        {
            *(xbuf + i) = *(beg - lblock + i);
        }
    }
    for (int b = 0; b <= M; b++) {
        if (b == M && lrest == 0) break;
        arr1 = beg + b * 2 * LL;
        int NBlk = (b == M ? lrest : 2 * LL) / lblock;
        grail_SortIns(keys, keys + NBlk + (b == M ? 1 : 0), compare);
        int midkey = LL / lblock;
        for (int u = 1; u < NBlk; u++) {
            int p = u - 1;
            for (int v = u; v < NBlk; v++) {
                if (compare(*(arr1 + v*lblock), *(arr1 + p*lblock)) ||
                    (grail_lesseq(*(arr1 + v*lblock), *(arr1 + p*lblock), compare) && compare(*(keys + v), *(keys + p))))
                    p = v;
            }
            if (p != u - 1) {
                grail_swapN(arr1 + (u - 1)*lblock, arr1 + p*lblock, lblock);
                grail_swap1(keys + (u - 1), keys + p);
                if (midkey == u - 1 || midkey == p) midkey ^= (u - 1) ^ p;
            }
        }
        int nbl2 = 0, llast = 0;
        if (b == M) llast = lrest%lblock;
        if (llast != 0) {
            while (nbl2 < NBlk && compare(*(arr1 + NBlk*lblock), *(arr1 + (NBlk - nbl2 - 1)*lblock))) nbl2++;
        }
        if (xbuf) grail_MergeBuffersLeftWithXBuf(keys, keys + midkey, arr1, NBlk - nbl2, lblock, nbl2, llast, compare);
        else grail_MergeBuffersLeft(keys, keys + midkey, arr1, NBlk - nbl2, lblock, havebuf, nbl2, llast, compare);
    }
    if (xbuf) {
        for (RandomAccessIterator p = end; --p >= beg;) *p = *(p - lblock);
        //memcpy(beg - lblock, xbuf, lblock * sizeof(value_type)); // TODO
        for (int i = 0; i < lblock; ++i)
        {
            *(beg - lblock + i) = *(xbuf + i);
        }
    }
    else if (havebuf) while (--end >= beg) grail_swap1(end, end - lblock);
}


template <class RandomAccessIterator, class RandomAccessBufferIterator, class Comp>
void grail_commonSort(RandomAccessIterator beg, RandomAccessIterator end, RandomAccessBufferIterator extbuf, int LExtBuf, Comp compare) {
    int lblock, nkeys, findkeys;
    bool havebuf;

    if (end - beg < 16) {
        grail_SortIns(beg, end, compare);
        return;
    }

    lblock = 1;
    while (lblock*lblock < end - beg) lblock *= 2;
    nkeys = (int)((end - beg - 1) / lblock + 1);
    findkeys = (int)(grail_FindKeys(beg, end, beg + nkeys + lblock, compare) - beg);
    havebuf = true;
    if (findkeys < nkeys + lblock) {
        if (findkeys < 4) {
            grail_LazyStableSort(beg, end, compare);
            return;
        }
        nkeys = lblock;
        while (nkeys > findkeys) nkeys /= 2;
        havebuf = false;
        lblock = 0;
    }
    RandomAccessIterator ptr = beg + lblock + nkeys;
    int cbuf = havebuf ? lblock : nkeys;
    if (havebuf) grail_BuildBlocks(ptr, end, cbuf, extbuf, LExtBuf, compare);
    else grail_BuildBlocks(ptr, end, cbuf, (RandomAccessBufferIterator)NULL, 0, compare);

    // 2*cbuf are built
    while (end - ptr > (cbuf *= 2)) {
        int lb = lblock;
        bool chavebuf = havebuf;
        if (!havebuf) {
            if (nkeys > 4 && nkeys / 8 * nkeys >= cbuf) {
                lb = nkeys / 2;
                chavebuf = true;
            }
            else {
                int nk = 1;
                double s = (double)cbuf * findkeys / 2;
                while (nk < nkeys && s > 0.9) {
                    nk *= 2; s /= 8;
                }
                lb = (2 * cbuf) / nk;
            }
        }
        else {
#if 0
            if (LExtBuf != 0) {
                while (lb > LExtBuf && lb*lb > 2 * cbuf) lb /= 2;  // set size of block close to sqrt(new_block_length)
            }
#endif
        }
        grail_CombineBlocks(beg, ptr, end, cbuf, lb, chavebuf, (RandomAccessBufferIterator)(chavebuf && lb <= LExtBuf ? extbuf : NULL), compare);
    }
    grail_SortIns(beg, ptr, compare);
    grail_MergeWithoutBuffer(beg, ptr, end, compare);
}

template <class RandomAccessIterator, class Comp>
void GrailSort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    value_type* buf = NULL;
    grail_commonSort(beg, end, buf, 0, compare);
}

template <class RandomAccessIterator>
void GrailSort(RandomAccessIterator beg, RandomAccessIterator end) {
    GrailSort(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

template <class RandomAccessIterator, class Comp>
void GrailSortWithBuffer(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typename std::iterator_traits<RandomAccessIterator>::value_type ExtBuf[GRAIL_EXT_BUFFER_LENGTH];
    grail_commonSort(beg, end, ExtBuf, GRAIL_EXT_BUFFER_LENGTH, compare);
}

template <class RandomAccessIterator>
void GrailSortWithBuffer(RandomAccessIterator beg, RandomAccessIterator end)
{
    GrailSortWithBuffer(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

template <class RandomAccessIterator, class Comp>
void GrailSortWithDynBuffer(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    int L = 1;
    while (L * L < end - beg) L *= 2;
    value_type * ExtBuf = new value_type[L];
    if (ExtBuf == NULL)
    {
        GrailSortWithBuffer(beg, end, compare);
    }
    else
    {
        grail_commonSort(beg, end, ExtBuf, L, compare);
        delete[] ExtBuf;
    }
}

template <class RandomAccessIterator>
void GrailSortWithDynBuffer(RandomAccessIterator beg, RandomAccessIterator end)
{
    GrailSortWithDynBuffer(beg, end, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

} // namespace Mrrl
