#include "doctest/doctest.h"
#include "ezgargs/ezgargs.hpp"

namespace ezgargs {

TEST_CASE("Test simple escape sequences") {
    CHECK(StrTool::unEscape("\\'") == '\'');   // single quote
    CHECK(StrTool::unEscape("\\\"") == '\"');  // double quote
    CHECK(StrTool::unEscape("\\?") == '\?');   // question mark
    CHECK(StrTool::unEscape("\\\\") == '\\');  // backslash
    CHECK(StrTool::unEscape("\\a") == '\a');   // audible bell
    CHECK(StrTool::unEscape("\\b") == '\b');   // backspace
    CHECK(StrTool::unEscape("\\f") == '\f');   // form feed - new page
    CHECK(StrTool::unEscape("\\n") == '\n');   // line feed - new line
    CHECK(StrTool::unEscape("\\r") == '\r');   // carriage return
    CHECK(StrTool::unEscape("\\t") == '\t');   // horizontal tab
    CHECK(StrTool::unEscape("\\v") == '\v');   // vertical tab
}

TEST_CASE("Test numeric escape sequences") {
    CHECK(StrTool::unEscape("\\o{0}") == '\0');                             // octal value with braces (since C++23)
    CHECK(StrTool::unEscape("\\o{12}") == '\n');                            // octal value with braces (since C++23)
    CHECK(StrTool::unEscape("\\o{377}") == '\377');                         // octal value with braces (since C++23)
    CHECK_THROWS_AS(StrTool::unEscape("\\o{400}"), std::invalid_argument);  // octal value out of range
    CHECK_THROWS_AS(StrTool::unEscape("\\o{"), std::invalid_argument);      // missing closing brace for octal value

    CHECK(StrTool::unEscape("\\x0") == '\0');                             // hexadecimal value without braces
    CHECK(StrTool::unEscape("\\xA") == '\xA');                            // hexadecimal value without braces
    CHECK(StrTool::unEscape("\\xFF") == '\xFF');                          // hexadecimal value without braces
    CHECK_THROWS_AS(StrTool::unEscape("\\x100"), std::invalid_argument);  // hexadecimal value out of range

    CHECK(StrTool::unEscape("\\x{0}") == '\0');     // hexadecimal value with braces (since C++23)
    CHECK(StrTool::unEscape("\\x{A}") == '\xA');    // hexadecimal value with braces (since C++23)
    CHECK(StrTool::unEscape("\\x{FF}") == '\xFF');  // hexadecimal value with braces (since C++23)
    CHECK_THROWS_AS(StrTool::unEscape("\\x{100}"), std::invalid_argument);  // hexadecimal value out of range
    CHECK_THROWS_AS(StrTool::unEscape("\\x{"), std::invalid_argument);  // missing closing brace for hexadecimal value

    CHECK(StrTool::unEscape("\\0") == '\0');                             // octal value without braces
    CHECK(StrTool::unEscape("\\12") == '\n');                            // octal value without braces
    CHECK(StrTool::unEscape("\\377") == '\377');                         // octal value without braces
    CHECK_THROWS_AS(StrTool::unEscape("\\400"), std::invalid_argument);  // octal value out of range
}

TEST_CASE("Test invalid escape sequences") {
    CHECK_THROWS_AS(StrTool::unEscape("\\"), std::invalid_argument);   // incomplete escape sequence
    CHECK_THROWS_AS(StrTool::unEscape("\\c"), std::invalid_argument);  // implementation-defined escape sequence
    CHECK_THROWS_AS(StrTool::unEscape("\\z"), std::invalid_argument);  // unknown escape sequence
}

}  // namespace ezgargs
