#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace ezgargs {

inline auto hellowolrd() -> std::string {
    std::string s = "Hello, world!";
    std::cout << s << '\n';
    return s;
}

class CharTool {
  public:
    static constexpr int HEXADECIMAL = 16;
    static constexpr int DECIMAL = 10;
    static constexpr int OCTAL = 8;
    static constexpr int MAX_CHAR = 255;

    // A helper function to convert a hex digit to its decimal value
    static auto hex2dec(char c) -> int {
        if (std::isdigit(c) != 0) {
            return c - '0';
        }
        if (std::isupper(c) != 0) {
            return c - 'A' + DECIMAL;
        }
        if (std::islower(c) != 0) {
            return c - 'a' + DECIMAL;
        }
        throw std::invalid_argument("Invalid hex digit");
    }

    // A helper function to convert an octal digit to its decimal value
    static auto oct2dec(char c) -> int {
        if (c >= '0' && c <= '7') {
            return c - '0';
        }
        throw std::invalid_argument("Invalid octal digit");
    }

    // Numeric escape sequences arbitrary octal value
    static auto numericEscapeOctal(const char*& str) -> char {
        if (*(++str) != '{') {
            throw std::invalid_argument("Missing opening brace for octal value");
        }
        int value = 0;  // the decimal value of the octal sequence
        while (*(++str) != '}') {
            value = value * OCTAL + oct2dec(*str);  // accumulate the octal digits
        }
        if (value > MAX_CHAR) {
            throw std::invalid_argument("Octal value out of range");
        }
        return static_cast<char>(value);
    }

    // Numeric escape sequences arbitrary hexadecimal value
    static auto numbericEscapeHexadecimal(const char*& str) -> char {
        int value = 0;          // the decimal value of the hex sequence
        if (*(++str) == '{') {  // hex value with braces (since C++23)
            while (*(++str) != '}') {
                value = value * HEXADECIMAL + hex2dec(*str);  // accumulate the hex digits
            }
        } else {  // hex value without braces
            while (std::isxdigit(*str) != 0) {
                value = value * HEXADECIMAL + hex2dec(*str);  // accumulate the hex digits
                ++str;
            }
            --str;  // go back one character to avoid skipping the next character in the string
        }
        if (value > MAX_CHAR) {
            throw std::invalid_argument("Hexadecimal value out of range");
        }
        return static_cast<char>(value);
    }

    // A helper function to convert an octal digit to its decimal value
    static auto octalValue(const char*& str) -> char {
        int value = 0;  // the decimal value of the octal sequence
        int count = 0;  // the number of octal digits read
        while (count < 3 && *str >= '0' && *str <= '7') {
            value = value * OCTAL + (*str - '0');  // accumulate the octal digits
            ++str;
            ++count;
        }
        --str;  // go back one character to avoid skipping the next character in the string
        if (value > MAX_CHAR) {
            throw std::invalid_argument("Octal value out of range");
        }
        if (count == 0) {
            throw std::invalid_argument("Invalid escape sequence");
        }
        return static_cast<char>(value);
    }

    // A function to unescape the first character in a string
    static auto unEscape(const char* str) -> char {
        if (str == nullptr || *str == '\0') {
            return 0;
        }

        // If the string does not start with a backslash, return the first character
        if (*str != '\\') {
            return *str;
        }

        // If the string starts with a backslash, check the next character
        switch (*(++str)) {
            // Simple escape sequences
            case '\'':
                return '\'';  // single quote
            case '\"':
                return '\"';  // double quote
            case '\?':
                return '\?';  // question mark
            case '\\':
                return '\\';  // backslash
            case 'a':
                return '\a';  // audible bell
            case 'b':
                return '\b';  // backspace
            case 'f':
                return '\f';  // form feed - new page
            case 'n':
                return '\n';  // line feed - new line
            case 'r':
                return '\r';  // carriage return
            case 't':
                return '\t';  // horizontal tab
            case 'v':
                return '\v';  // vertical tab

            // Numeric escape sequences
            case 'o': {                          // octal value with braces (since C++23)
                return numericEscapeOctal(str);  // cast the value to a char and return it
            }
            case 'x': {                                 // hexadecimal value with or without braces
                return numbericEscapeHexadecimal(str);  // cast the value to a char and return it
            }
            default: {                   // octal value without braces or invalid escape sequence
                return octalValue(str);  // cast the value to a char and return it
            }
        }
    }
};

class StrTool {
  public:
    static auto split(const std::string& str, char delim) -> std::vector<std::string> {
        std::stringstream ss(str);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim)) {
            if (!item.empty()) {
                elems.push_back(item);
            }
        }
        return elems;
    }
};

class Argument {
  private:
    friend class EzgArgs;
    std::string name;
    std::string nickName;
    std::string helpMessage;
    std::string value;
    bool isRequired = true;
    std::function<void(const std::string&)> assign;
    char delim = ',';

  protected:
    auto getRequired() const -> bool { return isRequired; }
    auto setRequired(bool required) -> void { this->isRequired = required; }
    auto getName() -> std::string { return name; }
    auto getNickName() -> std::string { return nickName; }
    auto getHelpMessage() -> std::string { return helpMessage; }
    auto setName(const std::string& name) -> void { this->name = name; }
    auto setNickName(const std::string& nickName) -> void { this->nickName = nickName; }
    auto setHelpMessage(const std::string& helpMessage) -> void { this->helpMessage = helpMessage; }
    auto getAction() -> std::function<void(const std::string&)> { return assign; }
    auto getDelim() const -> char { return delim; }
    auto setDelim(char delim) -> void { this->delim = delim; }
    auto getValue() -> std::string { return value; }
    auto setValue(const std::string& value) -> void { this->value = value; }

  public:
    auto action(const std::function<void(const std::string&)>& assign) -> Argument& {
        this->assign = assign;
        return *this;
    }
    auto help(const std::string& helpMessage) -> Argument& {
        this->helpMessage = helpMessage;
        return *this;
    }
    auto nick(const std::string& nickName) -> Argument& {
        this->nickName = nickName;
        return *this;
    }
    auto optional() -> Argument& {
        isRequired = false;
        return *this;
    }
    auto required() -> Argument& {
        isRequired = true;
        return *this;
    }
};

class EzgArgs {
  private:
    std::map<std::string, Argument> argsMap;
    using Action = std::function<void(const std::string&)>;

  protected:
    // 注册
    auto regist(const std::string& name) -> Argument& {
        Argument arg;
        arg.setName(name);
        argsMap[name] = arg;
        return argsMap[name];
    }

    // char
    static auto runCharAction(char& ref) -> Action {
        return [&ref](const std::string& value) {
            if (value.starts_with("'") && value.ends_with("'")) {
                std::string sub = value.substr(1, value.size() - 2);
                ref = CharTool::unEscape(sub.c_str());
            } else {
                ref = CharTool::unEscape(value.c_str());
            }
        };
    }

    // bool
    static auto makeBoolAction(bool& ref) -> Action {
        return [&ref](const std::string& value) {
            if (value == "1" || value == "True" || value == "true" || value == "TRUE" || value == "t" || value == "T") {
                ref = true;
            } else if (value == "0" || value == "False" || value == "false" || value == "FALSE" || value == "f" ||
                       value == "F") {
                ref = false;
            } else {
                throw std::invalid_argument("invalid argument for bool");
            }
        };
    }

    // unsigned
    template <typename T>
    static auto makeUnsignedAction(T& ref) -> Action {
        return [&ref](const std::string& value) { ref = static_cast<T>(std::stoull(value)); };
    }

    // integral
    template <typename T>
    static auto makeIntegralAction(T& ref) -> Action {
        return [&ref](const std::string& value) { ref = static_cast<T>(std::stoll(value)); };
    }

    // arithmetic
    template <typename T>
    static auto makeArithmeticAction(T& ref) -> Action {
        return [&ref](const std::string& value) { ref = static_cast<T>(std::stold(value)); };
    }

    // string
    template <typename T>
    static auto makeStringAction(T& ref) -> Action {
        return [&ref](const std::string& value) { ref = value; };
    }

    // constructor
    template <typename T>
    static auto makeConstructorAction(T& ref) -> Action {
        return [&ref](const std::string& value) { ref = T(value); };
    }

  public:
    auto addArgument(const std::string& name) -> Argument& {
        auto& arg = regist(name);
        arg.setRequired(false);
        return arg;
    }

    template <typename T>
    auto addArgument(const std::string& name, T& ref) -> Argument& {
        auto& arg = regist(name);
        arg.action(makeAction(ref));
        return arg;
    }

    template <typename T>
    auto makeAction(T& ref) -> Action {
        if constexpr (std::is_same_v<T, bool>) {
            return makeBoolAction(ref);
        } else if constexpr (std::is_same_v<T, char>) {
            return makeCharAction(ref);
        } else if constexpr (std::is_unsigned_v<T>) {
            return makeUnsignedAction(ref);
        } else if constexpr (std::is_integral_v<T>) {
            return makeIntegralAction(ref);
        } else if constexpr (std::is_arithmetic_v<T>) {
            return makeArithmeticAction(ref);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return makeStringAction(ref);
        } else {
            return makeConstructorAction(ref);
        }
    }

    template <typename E>
    auto addArgument(const std::string& name, std::optional<E>& ref) -> Argument& {
        ref = E{};
        auto& arg = addArgument(name, ref.value());
        arg.setRequired(false);
        return arg;
    }

    template <typename E>
    auto addArgument(const std::string& name, std::vector<E>& ref) -> Argument& {
        auto& arg = regist(name);
        arg.action([&ref, &arg, this](const std::string& value) {
            std::vector<std::string> parts = StrTool::split(value, arg.getDelim());
            ref.clear();
            for (auto& part : parts) {
                ref.push_back(E{});
                Action action = makeAction(ref.back());
                action(part);
            }
        });
        return arg;
    }

    template <typename E>
    auto addArgument(const std::string& name, std::set<E>& ref) -> Argument& {
        auto& arg = regist(name);
        arg.action([&ref, &arg, this](const std::string& value) {
            std::vector<std::string> parts = StrTool::split(value, arg.getDelim());
            ref.clear();
            for (auto& part : parts) {
                auto it = ref.insert((E{}));
                Action action = makeAction(*it.first);
                action(part);
            }
        });
        return arg;
    }

  protected:
    // instance
    static auto instance() -> EzgArgs& {
        static EzgArgs instance;
        return instance;
    }

  private:
};

}  // namespace ezgargs