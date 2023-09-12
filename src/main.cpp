
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

// 定义一个函数，将字符串中的\xhh转换为对应的字符
std::string convert(std::string s) {
    std::stringstream ss;               // 定义一个stringstream对象
    ss << s;                            // 将字符串写入到stringstream中
    std::string result;                 // 存储结果
    char c;                             // 存储每个字符
    while (ss >> std::noskipws >> c) {  // 从stringstream中读取每个字符，不跳过空白字符
        result += c;                    // 添加到结果中
    }
    return result;
}

auto main(int argc, char* argv[]) -> int {
    std::cout << "Hello, World!" << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << convert(argv[i]) << std::endl;
        char c;
        sscanf(argv[i], "%c", &c);
        printf("CHAR: %c\n", c);
    }
    std::string s = "Hello\\x20world\\x21";  // 定义一个包含\xhh形式的转义字符的字符串
    std::cout << convert(s) << std::endl;    // 输出转换后的结果
    return 0;
}