// Vector.cpp
#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "Vector.h"

// 构造函数
template <typename T>
Vector<T>::Vector(int c, int s, T v)
    : _size(s), _capacity(c), _elem(new T[c])
{
    for (Rank i = 0; i < s; ++i)
        _elem[i] = v; // s <= c
}

// 数组整体复制
template <typename T>
Vector<T>::Vector(T const* A, Rank n)
{
    copyFrom(A, 0, n);
}

// 区间复制
template <typename T>
Vector<T>::Vector(T const* A, Rank lo, Rank hi)
{
    copyFrom(A, lo, hi);
}

// 向量整体复制
template <typename T>
Vector<T>::Vector(Vector<T> const& V)
{
    copyFrom(V._elem, 0, V._size);
}

// 区间复制
template <typename T>
Vector<T>::Vector(Vector<T> const& V, Rank lo, Rank hi)
{
    copyFrom(V._elem, lo, hi);
}

// 析构函数
template <typename T>
Vector<T>::~Vector()
{
    delete[] _elem;
}

// 只读访问接口
template <typename T>
Rank Vector<T>::size() const
{
    return _size;
}

template <typename T>
bool Vector<T>::empty() const
{
    return _size == 0;
}

template <typename T>
int Vector<T>::disordered() const
{
    int n = 0;
    for (Rank i = 1; i < _size; ++i)
        if (_elem[i - 1] > _elem[i])
            n++;
    return n;
}

// 无序向量整体查找
template <typename T>
Rank Vector<T>::find(T const& e) const
{
    return find(e, 0, _size);
}

// 无序向量区间查找
template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const
{
    while ((lo < hi--) && (e != _elem[hi]));
    return hi;
}

// 有序向量整体查找
template <typename T>
Rank Vector<T>::search(T const& e) const
{
    return (_size <= 0) ? -1 : search(e, 0, _size);
}

// 有序向量区间查找 (二分查找)
template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{
    while (lo < hi) {
        Rank mi = (lo + hi) / 2;
        if (_elem[mi] < e)
            lo = mi + 1;
        else
            hi = mi;
    }
    return --lo;
}

// 可写访问接口
template <typename T>
T& Vector<T>::operator[](Rank r) const
{
    if (r < 0 || r >= _size)
        throw std::out_of_range("Index out of range");
    return _elem[r];
}

// 重载赋值操作符
template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V)
{
    if (_elem) delete[] _elem;
    copyFrom(V._elem, 0, V._size);
    return *this;
}

// 删除秩为 r 的元素
template <typename T>
T Vector<T>::remove(Rank r)
{
    if (r < 0 || r >= _size)
        throw std::out_of_range("Index out of range");
    T e = _elem[r];
    std::copy(_elem + r + 1, _elem + _size, _elem + r);
    _size--;
    shrink();
    return e;
}

// 删除秩在区间 [lo, hi) 之内的元素
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo < 0) lo = 0;
    if (hi > _size) hi = _size;
    if (lo >= hi) return 0;
    int n = hi - lo;
    std::copy(_elem + hi, _elem + _size, _elem + lo);
    _size -= n;
    shrink();
    return n;
}

// 插入元素
template <typename T>
Rank Vector<T>::insert(Rank r, T const& e)
{
    if (r < 0 || r > _size)
        throw std::out_of_range("Index out of range");
    expand();
    std::copy_backward(_elem + r, _elem + _size, _elem + _size + 1);
    _elem[r] = e;
    _size++;
    return r;
}

// 默认作为末元素插入
template <typename T>
Rank Vector<T>::insert(T const& e)
{
    return insert(_size, e);
}

// 排序
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
    // 根据需要选择排序算法
    // 这里以快速排序为例
    quickSort(lo, hi);
}

// 整体排序
template <typename T>
void Vector<T>::sort()
{
    sort(0, _size);
}

// 置乱
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi)
{
    std::random_shuffle(_elem + lo, _elem + hi);
}

// 整体置乱
template <typename T>
void Vector<T>::unsort()
{
    unsort(0, _size);
}

// 无序去重
template <typename T>
int Vector<T>::deduplicate()
{
    int oldSize = _size;
    Rank i = 1;
    while (i < _size)
        (find(_elem[i], 0, i) < 0) ? i++ : remove(i);
    return oldSize - _size;
}

// 有序去重
template <typename T>
int Vector<T>::uniquify()
{
    if (_size < 2) return 0;
    Rank i = 0, j = 1;
    while (j < _size) {
        if (_elem[i] != _elem[j])
            _elem[++i] = _elem[j];
        j++;
    }
    _size = i + 1;
    shrink();
    return j - i - 1;
}

// 遍历 (使用函数指针)
template <typename T>
void Vector<T>::traverse(void (*visit)(T&))
{
    for (Rank i = 0; i < _size; ++i)
        visit(_elem[i]);
}

// 遍历 (使用函数对象)
template <typename T>
template <typename VST>
void Vector<T>::traverse(VST& visit)
{
    for (Rank i = 0; i < _size; ++i)
        visit(_elem[i]);
}

// 复制数组区间 A[lo, hi)
template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi)
        _elem[_size++] = A[lo++];
}

// 扩容
template <typename T>
void Vector<T>::expand()
{
    if (_size < _capacity) return;
    _capacity = _capacity << 1; // 扩容为原来的2倍
    T* oldElem = _elem;
    _elem = new T[_capacity];
    std::copy(oldElem, oldElem + _size, _elem);
    delete[] oldElem;
}

// 压缩
template <typename T>
void Vector<T>::shrink()
{
    if (_capacity < DEFAULT_CAPACITY << 1) return;
    if (_size << 2 > _capacity) return; // 装填因子大于 25%
    _capacity = _capacity >> 1; // 压缩为原来的一半
    T* oldElem = _elem;
    _elem = new T[_capacity];
    std::copy(oldElem, oldElem + _size, _elem);
    delete[] oldElem;
}

// 扫描交换
template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
    bool sorted = true;
    while (++lo < hi)
        if (_elem[lo - 1] > _elem[lo]) {
            std::swap(_elem[lo - 1], _elem[lo]);
            sorted = false;
        }
    return sorted;
}

// 起泡排序算法
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    while (!bubble(lo, hi--));
}

// 选取最大元素
template <typename T>
Rank Vector<T>::max(Rank lo, Rank hi)
{
    Rank maxRank = lo;
    for (Rank i = lo + 1; i < hi; ++i)
        if (_elem[maxRank] < _elem[i])
            maxRank = i;
    return maxRank;
}

// 选择排序算法
template <typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi)
{
    for (Rank i = lo; i < hi; ++i) {
        Rank m = max(i, hi);
        if (m != i)
            std::swap(_elem[i], _elem[m]);
    }
}

// 归并算法
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
    T* A = _elem + lo;
    int lb = mi - lo;
    T* B = new T[lb];
    for (int i = 0; i < lb; ++i)
        B[i] = A[i];
    int lc = hi - mi;
    T* C = _elem + mi;
    int i = 0, j = 0, k = 0;
    while (i < lb && j < lc)
        A[k++] = (B[i] <= C[j]) ? B[i++] : C[j++];
    while (i < lb)
        A[k++] = B[i++];
    while (j < lc)
        A[k++] = C[j++];
    delete[] B;
}

// 归并排序算法
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (hi - lo < 2) return;
    Rank mi = (lo + hi) / 2;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}

// 轴点构造算法 (快速排序用)
template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi)
{
    std::swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);
    T pivot = _elem[lo];
    while (lo < hi) {
        while (lo < hi && _elem[--hi] >= pivot);
        if (lo < hi)
            _elem[lo] = _elem[hi];
        while (lo < hi && _elem[++lo] <= pivot);
        if (lo < hi)
            _elem[hi] = _elem[lo];
    }
    _elem[lo] = pivot;
    return lo;
}

// 快速排序算法
template <typename T>
void Vector<T>::quickSort(Rank lo, Rank hi)
{
    if (hi - lo < 2) return;
    Rank mi = partition(lo, hi);
    quickSort(lo, mi);
    quickSort(mi + 1, hi);
}

// 堆排序算法
template <typename T>
void Vector<T>::heapSort(Rank lo, Rank hi)
{
    // 需要实现堆的构造和调整，这里提供一个简单的实现示例
    // 注意：为了简洁性，这里不提供完整的堆实现
    // 实际应用中建议使用标准库中的堆函数，如 std::make_heap, std::push_heap, std::pop_heap
    std::make_heap(_elem + lo, _elem + hi);
    while (hi - lo > 1) {
        std::pop_heap(_elem + lo, _elem + hi);
        hi--;
    }
}

#endif // VECTOR_CPP
