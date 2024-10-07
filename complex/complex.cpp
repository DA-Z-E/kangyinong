#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>

// 复数类定义
class Complex {
public:
    double real; // 实部
    double imag; // 虚部

    // 构造函数
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // 计算复数的模
    double magnitude() const {
        return std::sqrt(real * real + imag * imag);
    }

    // 重载==运算符，用于比较两个复数是否相等（实部和虚部均相同）
    bool operator==(const Complex& other) const {
        return (real == other.real) && (imag == other.imag);
    }

    // 重载<运算符，用于排序（先按模排序，模相同则按实部排序）
    bool operator<(const Complex& other) const {
        double mag1 = this->magnitude();
        double mag2 = other.magnitude();
        if (mag1 != mag2)
            return mag1 < mag2;
        else
            return real < other.real;
    }

    // 重载>运算符，用于逆序排序
    bool operator>(const Complex& other) const {
        return other < *this;
    }

    // 打印复数
    void print() const {
        std::cout << "(" << real << (imag >= 0 ? " + " : " - ") << std::abs(imag) << "i)";
    }
};

// 生成随机复数
Complex generate_random_complex(double min = -100.0, double max = 100.0) {
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<double> dist(min, max);
    return Complex(dist(rng), dist(rng));
}

// 置乱函数
void shuffle_vector(std::vector<Complex>& vec) {
    std::shuffle(vec.begin(), vec.end(), std::mt19937(std::random_device()()));
}

// 查找函数，返回索引，若未找到则返回-1
int find_complex(const std::vector<Complex>& vec, const Complex& target) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == target)
            return static_cast<int>(i);
    }
    return -1;
}

// 插入函数，在指定位置插入复数
void insert_complex(std::vector<Complex>& vec, size_t position, const Complex& c) {
    if (position <= vec.size())
        vec.insert(vec.begin() + position, c);
    else
        std::cout << "插入位置超出范围！" << std::endl;
}

// 删除函数，删除指定索引的元素
void delete_complex(std::vector<Complex>& vec, size_t position) {
    if (position < vec.size())
        vec.erase(vec.begin() + position);
    else
        std::cout << "删除位置超出范围！" << std::endl;
}

// 唯一化函数，移除重复项
void uniquify_vector(std::vector<Complex>& vec) {
    // 先排序
    std::sort(vec.begin(), vec.end(), [](const Complex& a, const Complex& b) {
        if (a.real != b.real)
            return a.real < b.real;
        else
            return a.imag < b.imag;
    });

    // 使用unique和erase移除重复项
    vec.erase(std::unique(vec.begin(), vec.end(), [](const Complex& a, const Complex& b) {
        return a == b;
    }), vec.end());
}

// 气泡排序
void bubble_sort(std::vector<Complex>& vec) {
    size_t n = vec.size();
    bool swapped;
    for (size_t i = 0; i < n - 1; ++i) {
        swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (vec[j] > vec[j + 1]) { // 使用重载的>运算符
                std::swap(vec[j], vec[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

// 归并排序辅助函数
void merge(std::vector<Complex>& vec, size_t left, size_t mid, size_t right) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    // 创建临时向量
    std::vector<Complex> L(n1);
    std::vector<Complex> R(n2);

    for (size_t i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (size_t j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];

    // 合并临时向量回vec
    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            vec[k++] = L[i++];
        } else {
            vec[k++] = R[j++];
        }
    }

    // 复制剩余元素
    while (i < n1)
        vec[k++] = L[i++];
    while (j < n2)
        vec[k++] = R[j++];
}

// 归并排序主函数
void merge_sort_recursive(std::vector<Complex>& vec, size_t left, size_t right) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        merge_sort_recursive(vec, left, mid);
        merge_sort_recursive(vec, mid + 1, right);
        merge(vec, left, mid, right);
    }
}

void merge_sort(std::vector<Complex>& vec) {
    if (!vec.empty())
        merge_sort_recursive(vec, 0, vec.size() - 1);
}

// 区间查找函数，查找模介于[m1, m2)的所有元素
std::vector<Complex> range_search(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (const auto& c : vec) {
        double mag = c.magnitude();
        if (mag >= m1 && mag < m2)
            result.push_back(c);
    }
    return result;
}

// 打印向量
void print_vector(const std::vector<Complex>& vec) {
    for (const auto& c : vec)
        c.print(), std::cout << " ";
    std::cout << std::endl;
}

int main() {
    // 设置复数向量的大小
    const size_t VECTOR_SIZE = 20;

    // 随机生成复数向量（允许重复）
    std::vector<Complex> vec;
    for (size_t i = 0; i < VECTOR_SIZE; ++i) {
        vec.push_back(generate_random_complex(-10.0, 10.0));
    }

    // 打印初始向量
    std::cout << "初始复数向量：" << std::endl;
    print_vector(vec);

    // 置乱
    shuffle_vector(vec);
    std::cout << "置乱后的向量：" << std::endl;
    print_vector(vec);

    // 查找
    Complex target = vec[5]; // 选择一个存在的元素
    int index = find_complex(vec, target);
    if (index != -1)
        std::cout << "找到复数 ";
    else
        std::cout << "未找到复数 ";

    target.print();
    if (index != -1)
        std::cout << " 在索引 " << index << std::endl;
    else
        std::cout << std::endl;

    // 插入
    Complex new_complex(5.5, -3.3);
    insert_complex(vec, 10, new_complex);
    std::cout << "插入后的向量：" << std::endl;
    print_vector(vec);

    // 删除
    delete_complex(vec, 2);
    std::cout << "删除后的向量：" << std::endl;
    print_vector(vec);

    // 唯一化
    uniquify_vector(vec);
    std::cout << "唯一化后的向量：" << std::endl;
    print_vector(vec);

    // 准备不同顺序的向量用于排序性能测试
    std::vector<Complex> vec_sorted = vec; // 顺序
    std::vector<Complex> vec_shuffled = vec;
    shuffle_vector(vec_shuffled);
    std::vector<Complex> vec_reversed = vec_sorted;
    std::reverse(vec_reversed.begin(), vec_reversed.end());

    // 定义一个 lambda 来测量排序时间
    auto measure_sort_time = [](std::vector<Complex> vec_copy, void(*sort_func)(std::vector<Complex>&)) -> double {
        auto start = std::chrono::high_resolution_clock::now();
        sort_func(vec_copy);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        return elapsed.count();
    };

    // 排序性能测试
    // 顺序情况下
    double bubble_sorted_time = measure_sort_time(vec_sorted, bubble_sort);
    double merge_sorted_time = measure_sort_time(vec_sorted, merge_sort);
    std::cout << "顺序情况下：" << std::endl;
    std::cout << "气泡排序时间: " << bubble_sorted_time << " ms" << std::endl;
    std::cout << "归并排序时间: " << merge_sorted_time << " ms" << std::endl;

    // 乱序情况下
    double bubble_shuffled_time = measure_sort_time(vec_shuffled, bubble_sort);
    double merge_shuffled_time = measure_sort_time(vec_shuffled, merge_sort);
    std::cout << "乱序情况下：" << std::endl;
    std::cout << "气泡排序时间: " << bubble_shuffled_time << " ms" << std::endl;
    std::cout << "归并排序时间: " << merge_shuffled_time << " ms" << std::endl;

    // 逆序情况下
    double bubble_reversed_time = measure_sort_time(vec_reversed, bubble_sort);
    double merge_reversed_time = measure_sort_time(vec_reversed, merge_sort);
    std::cout << "逆序情况下：" << std::endl;
    std::cout << "气泡排序时间: " << bubble_reversed_time << " ms" << std::endl;
    std::cout << "归并排序时间: " << merge_reversed_time << " ms" << std::endl;

    // 对原始向量进行归并排序并打印
    std::vector<Complex> sorted_vec = vec;
    merge_sort(sorted_vec);
    std::cout << "排序后的向量（按模排序，模相同按实部排序）：" << std::endl;
    print_vector(sorted_vec);

    // 区间查找
    double m1 = 5.0, m2 = 10.0;
    std::vector<Complex> range_vec = range_search(sorted_vec, m1, m2);
    std::cout << "模介于 [" << m1 << ", " << m2 << ") 的元素：" << std::endl;
    print_vector(range_vec);

    return 0;
}
