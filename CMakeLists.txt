cmake_minimum_required(VERSION 4.2)
project(phingc C)

set(CMAKE_C_STANDARD 23)

find_package(LibXml2 REQUIRED)

add_executable(phingc main.c
        output_styles.c
        output_styles.h
        print_help.c
        print_help.h
        is_debug_enabled.c
        is_debug_enabled.h
        print_argument_debug.c
        print_argument_debug.h
        get_buildfile.c
        get_buildfile.h
        is_buildfile_valid.c
        is_buildfile_valid.h
        print_target_list.c
        print_target_list.h
        handle_arguments.c
        handle_arguments.h
        version.h
        phingc_target.c
        phingc_target.h)

target_compile_options(phingc PRIVATE -fsanitize=address)
target_link_options(phingc PRIVATE -fsanitize=address)

target_include_directories(phingc PRIVATE ${LIBXML2_INCLUDE_DIR})
target_link_libraries(phingc PRIVATE ${LIBXML2_LIBRARIES})
