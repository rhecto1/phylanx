// Copyright (c) 2020 Hartmut Kaiser
// Copyright (c) 2020 Bita Hasheminezhad
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <phylanx/phylanx.hpp>

#include <hpx/hpx_init.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/testing.hpp>

#include <array>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <blaze/Math.h>
#include <blaze_tensor/Math.h>

///////////////////////////////////////////////////////////////////////////////
phylanx::execution_tree::primitive_argument_type compile_and_run(
    std::string const& name, std::string const& codestr)
{
    phylanx::execution_tree::compiler::function_list snippets;
    phylanx::execution_tree::compiler::environment env =
        phylanx::execution_tree::compiler::default_environment();

    auto const& code =
        phylanx::execution_tree::compile(name, codestr, snippets, env);
    return code.run().arg_;
}

void test_constant_d_operation(std::string const& name, std::string const& code,
    std::string const& expected_str)
{
    phylanx::execution_tree::primitive_argument_type result =
        compile_and_run(name, code);
    phylanx::execution_tree::primitive_argument_type comparison =
        compile_and_run(name, expected_str);

    HPX_TEST_EQ(result, comparison);
}

///////////////////////////////////////////////////////////////////////////////
void test_constant_4loc_1d_0()
{
    if (hpx::get_locality_id() == 0)
    {
        test_constant_d_operation("test_constant_4loc1d_0", R"(
            constant_d(42, list(13), 0, 4)
        )", R"(
            annotate_d([42.0, 42.0, 42.0, 42.0], "constant_vector_4_13",
                list("args",
                    list("locality", 0, 4),
                    list("tile", list("columns", 0, 4))))
        )");
    }
    else if (hpx::get_locality_id() == 1)
    {
        test_constant_d_operation("test_constant_4loc1d_0", R"(
            constant_d(42, list(13), 1, 4)
        )", R"(
            annotate_d([42.0, 42.0, 42.0], "constant_vector_4_13",
                list("args",
                    list("locality", 1, 4),
                    list("tile", list("columns", 4, 7))))
        )");
    }
    else if (hpx::get_locality_id() == 2)
    {
        test_constant_d_operation("test_constant_4loc1d_0", R"(
            constant_d(42, list(13), 2, 4)
        )", R"(
            annotate_d([42.0, 42.0, 42.0], "constant_vector_4_13",
                list("args",
                    list("locality", 2, 4),
                    list("tile", list("columns", 7, 10))))
        )");
    }
    else if (hpx::get_locality_id() == 3)
    {
        test_constant_d_operation("test_constant_4loc1d_0", R"(
            constant_d(42, list(13), 3, 4)
        )", R"(
            annotate_d([42.0, 42.0, 42.0], "constant_vector_4_13",
                list("args",
                    list("locality", 3, 4),
                    list("tile", list("columns", 10, 13))))
        )");
    }
}

void test_constant_4loc_2d_0()
{
    if (hpx::get_locality_id() == 0)
    {
        test_constant_d_operation("test_constant_4loc2d_0", R"(
            constant_d(42, list(4, 7), 0, 4)
        )", R"(
            annotate_d([[42.0, 42.0, 42.0, 42.0], [42.0, 42.0, 42.0, 42.0]],
                "constant_matrix_4_4x7",
                list("args",
                    list("locality", 0, 4),
                    list("tile", list("columns", 0, 4), list("rows", 0, 2))))
        )");
    }
    else if (hpx::get_locality_id() == 1)
    {
        test_constant_d_operation("test_constant_4loc2d_0", R"(
            constant_d(42, list(4, 7), 1, 4)
        )", R"(
            annotate_d([[42.0, 42.0, 42.0], [42.0, 42.0, 42.0]],
                "constant_matrix_4_4x7",
                list("args",
                    list("locality", 1, 4),
                    list("tile", list("columns", 4, 7), list("rows", 0, 2))))
        )");
    }
    else if (hpx::get_locality_id() == 2)
    {
        test_constant_d_operation("test_constant_4loc2d_0", R"(
            constant_d(42, list(4, 7), 2, 4)
        )", R"(
            annotate_d([[42.0, 42.0, 42.0, 42.0], [42.0, 42.0, 42.0, 42.0]],
                "constant_matrix_4_4x7",
                list("args",
                    list("locality", 2, 4),
                    list("tile", list("columns", 0, 4), list("rows", 2, 4))))
        )");
    }
    else if (hpx::get_locality_id() == 3)
    {
        test_constant_d_operation("test_constant_4loc2d_0", R"(
            constant_d(42, list(4, 7), 3, 4)
        )", R"(
            annotate_d([[42.0, 42.0, 42.0], [42.0, 42.0, 42.0]],
                "constant_matrix_4_4x7",
                list("args",
                    list("locality", 3, 4),
                    list("tile", list("columns", 4, 7), list("rows", 2, 4))))
        )");
    }
}

///////////////////////////////////////////////////////////////////////////////
int hpx_main(int argc, char* argv[])
{
    test_constant_4loc_1d_0();
    test_constant_4loc_2d_0();

    hpx::finalize();
    return hpx::util::report_errors();
}
int main(int argc, char* argv[])
{
    std::vector<std::string> cfg = {
        "hpx.run_hpx_main!=1"
    };

    return hpx::init(argc, argv, cfg);
}

