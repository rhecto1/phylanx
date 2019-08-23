// Copyright (c) 2019 Bita Hasheminezhad
// Copyright (c) 2019 Hartmut Kaiser
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <phylanx/phylanx.hpp>

#include <hpx/hpx_main.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/testing.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <blaze/Math.h>

///////////////////////////////////////////////////////////////////////////////
phylanx::execution_tree::primitive_argument_type compile_and_run(
    std::string const& codestr)
{
    phylanx::execution_tree::compiler::function_list snippets;
    phylanx::execution_tree::compiler::environment env =
        phylanx::execution_tree::compiler::default_environment();

    auto const& code = phylanx::execution_tree::compile(codestr, snippets, env);
    return code.run();
}

///////////////////////////////////////////////////////////////////////////////
void test_bias_add_operation(std::string const& code,
    std::string const& expected_str)
{
    HPX_TEST_EQ(compile_and_run(code), compile_and_run(expected_str));
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    // 3d
    test_bias_add_operation(
        "bias_add([[[1,2,3],[4,5,6]],[[7,8,9],[10,11,12]]],[-1,-2,-3])",
        "[[[0., 0., 0.],[3., 3., 3.]],[[6., 6., 6.],[9., 9., 9.]]]");
    test_bias_add_operation(
        "bias_add([[[1,2,3],[4,5,6]],[[7,8,9],[10,11,12]]], "
        "[[-1,-2,-3],[-3,-2,-1]])",
        "[[[ 0., 0., 0.],[ 1., 3., 5.]],[[ 6., 6., 6.],[ 7., 9., 11.]]]");

    // 4d
    test_bias_add_operation(
        "bias_add([[[[ 0,  1,  2,  3,  4],[ 5,  6,  7,  8,  9],"
        "[10, 11, 12, 13, 14], [15, 16, 17, 18, 19]],"
        "[[20, 21, 22, 23, 24], [25, 26, 27, 28, 29],"
        "[30, 31, 32, 33, 34], [35, 36, 37, 38, 39]]],"
        "[[[40, 41, 42, 43, 44], [45, 46, 47, 48, 49],"
        "[50, 51, 52, 53, 54], [55, 56, 57, 58, 59]],"
        "[[60, 61, 62, 63, 64], [65, 66, 67, 68, 69],"
        "[70, 71, 72, 73, 74], [75, 76, 77, 78, 79]]]], "
        "[-5,-3,-1,-2, 0])",
        "[[[[-5., -2.,  1.,  1.,  4.], [ 0.,  3.,  6.,  6.,  9.],"
        "[ 5.,  8., 11., 11., 14.], [10., 13., 16., 16., 19.]],"
        "[[15., 18., 21., 21., 24.], [20., 23., 26., 26., 29.],"
        "[25., 28., 31., 31., 34.], [30., 33., 36., 36., 39.]]],"
        "[[[35., 38., 41., 41., 44.], [40., 43., 46., 46., 49.],"
        "[45., 48., 51., 51., 54.], [50., 53., 56., 56., 59.]],"
        "[[55., 58., 61., 61., 64.], [60., 63., 66., 66., 69.],"
        "[65., 68., 71., 71., 74.], [70., 73., 76., 76., 79.]]]]");
    test_bias_add_operation(
        "bias_add([[[[ 0,  1,  2,  3,  4],[ 5,  6,  7,  8,  9],"
        "[10, 11, 12, 13, 14], [15, 16, 17, 18, 19]],"
        "[[20, 21, 22, 23, 24], [25, 26, 27, 28, 29],"
        "[30, 31, 32, 33, 34], [35, 36, 37, 38, 39]]],"
        "[[[40, 41, 42, 43, 44], [45, 46, 47, 48, 49],"
        "[50, 51, 52, 53, 54], [55, 56, 57, 58, 59]],"
        "[[60, 61, 62, 63, 64], [65, 66, 67, 68, 69],"
        "[70, 71, 72, 73, 74], [75, 76, 77, 78, 79]]]], "
        "[[[-5,-3,-1,-2, 0]]])",
        "[[[[-5., -2.,  1.,  1.,  4.], [ 0.,  3.,  6.,  6.,  9.],"
        "[ 5.,  8., 11., 11., 14.], [10., 13., 16., 16., 19.]],"
        "[[15., 18., 21., 21., 24.], [20., 23., 26., 26., 29.],"
        "[25., 28., 31., 31., 34.], [30., 33., 36., 36., 39.]]],"
        "[[[35., 38., 41., 41., 44.], [40., 43., 46., 46., 49.],"
        "[45., 48., 51., 51., 54.], [50., 53., 56., 56., 59.]],"
        "[[55., 58., 61., 61., 64.], [60., 63., 66., 66., 69.],"
        "[65., 68., 71., 71., 74.], [70., 73., 76., 76., 79.]]]]");
    test_bias_add_operation(
        "bias_add([[[[ 0,  1,  2,  3,  4],[ 5,  6,  7,  8,  9],"
        "[10, 11, 12, 13, 14], [15, 16, 17, 18, 19]],"
        "[[20, 21, 22, 23, 24], [25, 26, 27, 28, 29],"
        "[30, 31, 32, 33, 34], [35, 36, 37, 38, 39]]],"
        "[[[40, 41, 42, 43, 44], [45, 46, 47, 48, 49],"
        "[50, 51, 52, 53, 54], [55, 56, 57, 58, 59]],"
        "[[60, 61, 62, 63, 64], [65, 66, 67, 68, 69],"
        "[70, 71, 72, 73, 74], [75, 76, 77, 78, 79]]]], "
        "[[[-3],[-1],[-2],[0]]])",
        "[[[[-3., -2., -1.,  0.,  1.], [ 4.,  5.,  6.,  7.,  8.],"
        "[ 8.,  9., 10., 11., 12.], [15., 16., 17., 18., 19.]],"
        "[[17., 18., 19., 20., 21.], [24., 25., 26., 27., 28.],"
        "[28., 29., 30., 31., 32.], [35., 36., 37., 38., 39.]]],"
        "[[[37., 38., 39., 40., 41.], [44., 45., 46., 47., 48.],"
        "[48., 49., 50., 51., 52.], [55., 56., 57., 58., 59.]],"
        "[[57., 58., 59., 60., 61.], [64., 65., 66., 67., 68.],"
        "[68., 69., 70., 71., 72.], [75., 76., 77., 78., 79.]]]]");
    test_bias_add_operation(
        "bias_add([[[[ 0,  1,  2,  3,  4],[ 5,  6,  7,  8,  9],"
        "[10, 11, 12, 13, 14], [15, 16, 17, 18, 19]],"
        "[[20, 21, 22, 23, 24], [25, 26, 27, 28, 29],"
        "[30, 31, 32, 33, 34], [35, 36, 37, 38, 39]]],"
        "[[[40, 41, 42, 43, 44], [45, 46, 47, 48, 49],"
        "[50, 51, 52, 53, 54], [55, 56, 57, 58, 59]],"
        "[[60, 61, 62, 63, 64], [65, 66, 67, 68, 69],"
        "[70, 71, 72, 73, 74], [75, 76, 77, 78, 79]]]], "
        "[[[-3]],[[0]]])",
        "[[[[-3., -2., -1.,  0.,  1.], [ 2.,  3.,  4.,  5.,  6.],"
        "[ 7.,  8.,  9., 10., 11.], [12., 13., 14., 15., 16.]],"
        "[[20., 21., 22., 23., 24.], [25., 26., 27., 28., 29.],"
        "[30., 31., 32., 33., 34.], [35., 36., 37., 38., 39.]]],"
        "[[[37., 38., 39., 40., 41.], [42., 43., 44., 45., 46.],"
        "[47., 48., 49., 50., 51.], [52., 53., 54., 55., 56.]],"
        "[[60., 61., 62., 63., 64.], [65., 66., 67., 68., 69.],"
        "[70., 71., 72., 73., 74.], [75., 76., 77., 78., 79.]]]]");
    test_bias_add_operation(
        "bias_add([[[[ 0,  1,  2,  3,  4],[ 5,  6,  7,  8,  9],"
        "[10, 11, 12, 13, 14], [15, 16, 17, 18, 19]],"
        "[[20, 21, 22, 23, 24], [25, 26, 27, 28, 29],"
        "[30, 31, 32, 33, 34], [35, 36, 37, 38, 39]]],"
        "[[[40, 41, 42, 43, 44], [45, 46, 47, 48, 49],"
        "[50, 51, 52, 53, 54], [55, 56, 57, 58, 59]],"
        "[[60, 61, 62, 63, 64], [65, 66, 67, 68, 69],"
        "[70, 71, 72, 73, 74], [75, 76, 77, 78, 79]]]], "
        "[[[-4,-3,-2,-1,0],[-5,-6,-7,-8,-9],[0,1,0,2,0],[1,0,1,0,2]]])",
        "[[[[-4., -2.,  0.,  2.,  4.], [ 0.,  0.,  0.,  0.,  0.],"
        "[10., 12., 12., 15., 14.], [16., 16., 18., 18., 21.]],"
        "[[16., 18., 20., 22., 24.], [20., 20., 20., 20., 20.],"
        "[30., 32., 32., 35., 34.], [36., 36., 38., 38., 41.]]],"
        "[[[36., 38., 40., 42., 44.], [40., 40., 40., 40., 40.],"
        "[50., 52., 52., 55., 54.], [56., 56., 58., 58., 61.]],"
        "[[56., 58., 60., 62., 64.], [60., 60., 60., 60., 60.],"
        "[70., 72., 72., 75., 74.], [76., 76., 78., 78., 81.]]]]");
    test_bias_add_operation(
        "bias_add([[[[ 0,  1,  2,  3,  4],[ 5,  6,  7,  8,  9],"
        "[10, 11, 12, 13, 14], [15, 16, 17, 18, 19]],"
        "[[20, 21, 22, 23, 24], [25, 26, 27, 28, 29],"
        "[30, 31, 32, 33, 34], [35, 36, 37, 38, 39]]],"
        "[[[40, 41, 42, 43, 44], [45, 46, 47, 48, 49],"
        "[50, 51, 52, 53, 54], [55, 56, 57, 58, 59]],"
        "[[60, 61, 62, 63, 64], [65, 66, 67, 68, 69],"
        "[70, 71, 72, 73, 74], [75, 76, 77, 78, 79]]]], "
        "[[[-4,-3,-2,-1,0]],[[-5,-6,-7,-8,-9]]])",
        "[[[[-4., -2.,  0.,  2.,  4.], [ 1.,  3.,  5.,  7.,  9.],"
        "[ 6.,  8., 10., 12., 14.], [11., 13., 15., 17., 19.]],"
        "[[15., 15., 15., 15., 15.], [20., 20., 20., 20., 20.],"
        "[25., 25., 25., 25., 25.], [30., 30., 30., 30., 30.]]],"
        "[[[36., 38., 40., 42., 44.], [41., 43., 45., 47., 49.],"
        "[46., 48., 50., 52., 54.], [51., 53., 55., 57., 59.]],"
        "[[55., 55., 55., 55., 55.], [60., 60., 60., 60., 60.],"
        "[65., 65., 65., 65., 65.], [70., 70., 70., 70., 70.]]]]");
    test_bias_add_operation(
        "bias_add([[[[ 0,  1,  2,  3,  4],[ 5,  6,  7,  8,  9],"
        "[10, 11, 12, 13, 14], [15, 16, 17, 18, 19]],"
        "[[20, 21, 22, 23, 24], [25, 26, 27, 28, 29],"
        "[30, 31, 32, 33, 34], [35, 36, 37, 38, 39]]],"
        "[[[40, 41, 42, 43, 44], [45, 46, 47, 48, 49],"
        "[50, 51, 52, 53, 54], [55, 56, 57, 58, 59]],"
        "[[60, 61, 62, 63, 64], [65, 66, 67, 68, 69],"
        "[70, 71, 72, 73, 74], [75, 76, 77, 78, 79]]]], "
        "[[[-3],[-2],[-1],[ 0]],[[-5],[-6],[-7],[-8]]])",
        "[[[[-3., -2., -1.,  0.,  1.], [ 3.,  4.,  5.,  6.,  7.],"
        "[ 9., 10., 11., 12., 13.], [15., 16., 17., 18., 19.]],"
        "[[15., 16., 17., 18., 19.], [19., 20., 21., 22., 23.],"
        "[23., 24., 25., 26., 27.], [27., 28., 29., 30., 31.]]],"
        "[[[37., 38., 39., 40., 41.], [43., 44., 45., 46., 47.],"
        "[49., 50., 51., 52., 53.], [55., 56., 57., 58., 59.]],"
        "[[55., 56., 57., 58., 59.], [59., 60., 61., 62., 63.],"
        "[63., 64., 65., 66., 67.], [67., 68., 69., 70., 71.]]]]");
    test_bias_add_operation(
        "bias_add([[[[ 0,  1,  2,  3,  4],[ 5,  6,  7,  8,  9],"
        "[10, 11, 12, 13, 14], [15, 16, 17, 18, 19]],"
        "[[20, 21, 22, 23, 24], [25, 26, 27, 28, 29],"
        "[30, 31, 32, 33, 34], [35, 36, 37, 38, 39]]],"
        "[[[40, 41, 42, 43, 44], [45, 46, 47, 48, 49],"
        "[50, 51, 52, 53, 54], [55, 56, 57, 58, 59]],"
        "[[60, 61, 62, 63, 64], [65, 66, 67, 68, 69],"
        "[70, 71, 72, 73, 74], [75, 76, 77, 78, 79]]]], "
        "[[[-3],[-2],[-1],[ 0]],[[-5],[-6],[-7],[-8]]])",
        "[[[[-3., -2., -1.,  0.,  1.], [ 3.,  4.,  5.,  6.,  7.],"
        "[ 9., 10., 11., 12., 13.], [15., 16., 17., 18., 19.]],"
        "[[15., 16., 17., 18., 19.], [19., 20., 21., 22., 23.],"
        "[23., 24., 25., 26., 27.], [27., 28., 29., 30., 31.]]],"
        "[[[37., 38., 39., 40., 41.], [43., 44., 45., 46., 47.],"
        "[49., 50., 51., 52., 53.], [55., 56., 57., 58., 59.]],"
        "[[55., 56., 57., 58., 59.], [59., 60., 61., 62., 63.],"
        "[63., 64., 65., 66., 67.], [67., 68., 69., 70., 71.]]]]");
    test_bias_add_operation(
        "bias_add([[[[ 0,  1], [ 2,  3]], [[ 4,  5], [ 6,  7]]],"
        "[[[ 8,  9], [10, 11]], [[12, 13], [14, 15]]]], "
        "[[[-4,-3],[-2,-1]],[[1,2],[3,4]]])",
        "[[[[-4., -2.], [ 0.,  2.]], [[ 5.,  7.], [ 9., 11.]]],"
        "[[[ 4.,  6.], [ 8., 10.]], [[13., 15.], [17., 19.]]]]");

    return hpx::util::report_errors();
}
