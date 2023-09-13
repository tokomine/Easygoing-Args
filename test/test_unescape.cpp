#include "doctest/doctest.h"
#include "ezgargs/ezgargs.hpp"

namespace ezgargs {

TEST_CASE("Test simple escape sequences") {
    CHECK(CharTool::unEscape("\\'") == '\'');   // single quote
    CHECK(CharTool::unEscape("\\\"") == '\"');  // double quote
    CHECK(CharTool::unEscape("\\?") == '\?');   // question mark
    CHECK(CharTool::unEscape("\\\\") == '\\');  // backslash
    CHECK(CharTool::unEscape("\\a") == '\a');   // audible bell
    CHECK(CharTool::unEscape("\\b") == '\b');   // backspace
    CHECK(CharTool::unEscape("\\f") == '\f');   // form feed - new page
    CHECK(CharTool::unEscape("\\n") == '\n');   // line feed - new line
    CHECK(CharTool::unEscape("\\r") == '\r');   // carriage return
    CHECK(CharTool::unEscape("\\t") == '\t');   // horizontal tab
    CHECK(CharTool::unEscape("\\v") == '\v');   // vertical tab
}

TEST_CASE("Test numeric escape sequences") {
    CHECK(CharTool::unEscape("\\o{0}") == '\0');                             // octal value with braces (since C++23)
    CHECK(CharTool::unEscape("\\o{12}") == '\n');                            // octal value with braces (since C++23)
    CHECK(CharTool::unEscape("\\o{377}") == '\377');                         // octal value with braces (since C++23)
    CHECK_THROWS_AS(CharTool::unEscape("\\o{400}"), std::invalid_argument);  // octal value out of range
    CHECK_THROWS_AS(CharTool::unEscape("\\o{"), std::invalid_argument);      // missing closing brace for octal value

    CHECK(CharTool::unEscape("\\x0") == '\0');                             // hexadecimal value without braces
    CHECK(CharTool::unEscape("\\xA") == '\xA');                            // hexadecimal value without braces
    CHECK(CharTool::unEscape("\\xFF") == '\xFF');                          // hexadecimal value without braces
    CHECK_THROWS_AS(CharTool::unEscape("\\x100"), std::invalid_argument);  // hexadecimal value out of range

    CHECK(CharTool::unEscape("\\x{0}") == '\0');     // hexadecimal value with braces (since C++23)
    CHECK(CharTool::unEscape("\\x{A}") == '\xA');    // hexadecimal value with braces (since C++23)
    CHECK(CharTool::unEscape("\\x{FF}") == '\xFF');  // hexadecimal value with braces (since C++23)
    CHECK_THROWS_AS(CharTool::unEscape("\\x{100}"), std::invalid_argument);  // hexadecimal value out of range
    CHECK_THROWS_AS(CharTool::unEscape("\\x{"), std::invalid_argument);  // missing closing brace for hexadecimal value

    CHECK(CharTool::unEscape("\\0") == '\0');                             // octal value without braces
    CHECK(CharTool::unEscape("\\12") == '\n');                            // octal value without braces
    CHECK(CharTool::unEscape("\\377") == '\377');                         // octal value without braces
    CHECK_THROWS_AS(CharTool::unEscape("\\400"), std::invalid_argument);  // octal value out of range
}

TEST_CASE("Test invalid escape sequences") {
    CHECK_THROWS_AS(CharTool::unEscape("\\"), std::invalid_argument);   // incomplete escape sequence
    CHECK_THROWS_AS(CharTool::unEscape("\\c"), std::invalid_argument);  // implementation-defined escape sequence
    CHECK_THROWS_AS(CharTool::unEscape("\\z"), std::invalid_argument);  // unknown escape sequence
}

}  // namespace ezgargs
