// Vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // for std::swap
#include <stdexcept> // for exceptions

typedef int Rank; // 秩 (Rank)
#define DEFAULT_CAPACITY 3 // 默认的初始容量 (实际应用中可设置为更大)

template <typename T>
class Vector { // 向量模板类
protected:
    Rank _size;      // 规模 (current number of elements)
    int _capacity;   // 容量 (total allocated space)
    T* _elem;        // 数据区 (pointer to the elements)

    void copyFrom(T const* A, Rank lo, Rank hi); // 复制数组区间 A[lo, hi)
    void expand(); // 空间不足时扩容
    void shrink(); // 装填因子过小时压缩

    bool bubble(Rank lo, Rank hi); // 扫描交换
    void bubbleSort(Rank lo, Rank hi); // 起泡排序算法

    Rank max(Rank lo, Rank hi); // 选取最大元素
    void selectionSort(Rank lo, Rank hi); // 选择排序算法

    void merge(Rank lo, Rank mi, Rank hi); // 归并算法
    void mergeSort(Rank lo, Rank hi); // 归并排序算法

    Rank partition(Rank lo, Rank hi); // 轴点构造算法
    void quickSort(Rank lo, Rank hi); // 快速排序算法

    void heapSort(Rank lo, Rank hi); // 堆排序

public:
    // 构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = T()); // 容量为c、规模为s、所有元素初始为v

    Vector(T const* A, Rank n); // 数组整体复制
    Vector(T const* A, Rank lo, Rank hi); // 区间复制

    Vector(Vector<T> const& V); // 向量整体复制
    Vector(Vector<T> const& V, Rank lo, Rank hi); // 区间复制

    // 析构函数
    ~Vector(); // 释放内部空间

    // 只读访问接口
    Rank size() const; // 规模
    bool empty() const; // 判空
    int disordered() const; // 判断向量是否已排序

    Rank find(T const& e) const; // 无序向量整体查找
    Rank find(T const& e, Rank lo, Rank hi) const; // 无序向量区间查找

    Rank search(T const& e) const; // 有序向量整体查找
    Rank search(T const& e, Rank lo, Rank hi) const; // 有序向量区间查找

    // 可写访问接口
    T& operator[](Rank r) const; // 重载下标操作符

    Vector<T>& operator=(Vector<T> const&); // 重载赋值操作符

    T remove(Rank r); // 删除秩为 r 的元素
    int remove(Rank lo, Rank hi); // 删除秩在区间 [lo, hi) 之内的元素

    Rank insert(Rank r, T const& e); // 插入元素
    Rank insert(T const& e); // 默认作为末元素插入

    void sort(Rank lo, Rank hi); // 对 [lo, hi) 排序
    void sort(); // 整体排序

    void unsort(Rank lo, Rank hi); // 对 [lo, hi) 置乱
    void unsort(); // 整体置乱

    int deduplicate(); // 无序去重
    int uniquify(); // 有序去重

    // 遍历
    void traverse(void (*visit)(T&)); // 遍历 (使用函数指针, 只读或局部性修改)

    template <typename VST>
    void traverse(VST& visit); // 遍历 (使用函数对象, 可全局性修改)
};

#include "Vector.cpp" // 包含实现部分

#endif // VECTOR_H
