#include "../exercise.h"

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    // 原函数签名：Tensor4D(unsigned int const shape_[4], T const *data_)，
    // 但编译报错“candidate function template not viable: no known conversion from 'unsigned int[4]' to 'const unsigned int[]' for 1st argument”
    // 经过多种尝试，发现甚至去掉形参的“const”也会报错（可能这句话被编译器当成声明单个实数了），故修改为使用指针调用数组。
    Tensor4D(unsigned int const *shape_, T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for (int i = 0; i < 4; ++ i) {
            shape[i] = shape_[i];
            size *= shape[i];
        }
        data = new T[size];
        std::wmemcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
       Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        int s0[4], s1[4];
        for (int i = 0; i < 4; ++ i) {
            s0[i] = this -> shape[i];
            s1[i] = others.shape[i];
        }

        for (int i = 0; i < s1[0]; ++ i) {
            for (int j = 0; j < s1[1]; ++ j) {
                for (int k = 0; k < s1[2]; ++ k) {
                    for (int l = 0; l < s1[3]; ++ l) {

                        for (int mi = 0; mi < s0[0] / s1[0]; ++ mi) {
                            for (int mj = 0; mj < s0[1] / s1[1]; ++ mj) {
                                for (int mk = 0; mk < s0[2] / s1[2]; ++ mk) {
                                    for (int ml = 0; ml < s0[3] / s1[3]; ++ ml) {
                                        this -> data[(i + 1 + mi) * ((j + mj) * s0[2] * s0[3] + (k + mk) * s0[3] + l + ml)] += 
                                            others.data[(i + 1) * (j * s1[2] * s1[3] + k * s1[3] + l)];
                                    }
                                }
                            }
                        }

                    }
                }
            }
        }


        /*for (int i = 1; i <= others.shape[0]; ++ i) {
            for (int j = 1; j <= others.shape[1]; ++ j) {
                for (int k = 1; k <= others.shape[2]; ++ k) {
                    for (int l = 1; l <= others.shape[3]; ++ l) {
                        
                        for (int i1 = 1; i1 <= this -> shape[0] / others.shape[0]; ++ i1) {
                            for (int j1 = 1; j1 <= this -> shape[1] / others.shape[1]; ++ j1) {
                                for (int k1 = 1; k1 <= this -> shape[2] / others.shape[2]; ++ i1) {
                                    for (int l1 = 1; l1 <= this -> shape[3] / others.shape[3]; ++ l1) {
                                        this -> shape[i1 * ((j1 - 1) * (s0[2] * s0[3]) + k1 * s0[3] + l1) - 1] += 
                                            others.shape[i * ((j - 1) * (s1[2] * s1[3]) + k * s1[3] + l) - 1];
                                    }
                                }
                            }
                        }*/

        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16, 
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
