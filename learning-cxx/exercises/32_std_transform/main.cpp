#include "../exercise.h"
#include <algorithm>
#include <string>
#include <vector>

// READ: `std::transform` <https://zh.cppreference.com/w/cpp/algorithm/transform>
// READ: `std::vector::begin` <https://zh.cppreference.com/w/cpp/container/vector/begin>

int main(int argc, char **argv) {
    std::vector<int> val{8, 13, 21, 34, 55};
    // TODO: 调用 `std::transform`，将 `v` 中的每个元素乘以 2，并转换为字符串，存入 `ans`
    // std::vector<std::string> ans
    std::vector<std::string> ans(val.size());
    
    // vector的cbegin和cend方法返回的是const iterator，要使之可修改则必须用begin与end。
    // 这里刚开始做的时候因为使用ans.cbegin()结果一直过不了编译。
    // 奇怪的是cpp reference中并没有注明两者的区别，给两者的函数签名伪代码中都标注为“const_iterator”，
    // 这造成了我很大的误解。现在来看应该是文档有误。
    std::transform(val.cbegin(), val.cend(), ans.begin(), [](int a) {return std::to_string(a * 2);});

    // for (int i = 0; i < val.size(); ++ i) ans.push_back(std::to_string(val[i]));

    ASSERT(ans.size() == val.size(), "ans size should be equal to val size");
    ASSERT(ans[0] == "16", "ans[0] should be 16");
    ASSERT(ans[1] == "26", "ans[1] should be 26");
    ASSERT(ans[2] == "42", "ans[2] should be 42");
    ASSERT(ans[3] == "68", "ans[3] should be 68");
    ASSERT(ans[4] == "110", "ans[4] should be 110");
    return 0;
}
