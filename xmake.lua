
set_project("Easygoing Args")

set_version("1.0.0", {build = "%Y%m%d%H%M"})
set_languages("cxx20")
add_requires("doctest")


target("test")
    set_kind("binary")
    add_includedirs("include")
    add_files("test/*.cpp")
    add_packages("doctest")


target("main")
    set_kind("binary")
    add_includedirs("include")
    add_files("src/*.cpp")
