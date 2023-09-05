# Easygoing-Args

Easygoing Args is a header-only C++ argument parser that allows you to write minimal code, use less mental effort, and focus more on core functionality development.

## Features

- Use C++ macros and editor auto-completion to avoid memorizing the spelling of configuration options, data type, and API.
- Register and access configuration options without repeatedly typing their names as strings.
- Automatically generate short names for configuration options.

## Installation

## Usage

```C++

struct RequiredOptions {
    std::optional<std::string> extra;
    std::optional<std::vector<std::string>> inputs;
};
struct OptionalOptions {
    int file_number = 0;
    std::string file_name = "abc";
    bool verbose = false;
};

std::vector<int> numbers = {1, 2, 3};


int main(int argc, char** argv) {
    RequiredOptions requiredOptions;
    OptionalOptions optionalOptions;
    
    EZGARGS(optionalOptions.file_number);
    EZGARGS(optionalOptions.file_name).help("input file name.");
    EZGARGS(optioptionalOptionsons.verbose);
    EZGARGS(requiredOptions.extra);
    EZGARGS(requiredOptions.inputs);
    EZGARGS(numbers).split(",");

    ezgargs::parse(argc, argv);
}

```

## License

Easygoing-Args is licensed under the MIT License.
