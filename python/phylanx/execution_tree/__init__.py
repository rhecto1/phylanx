# Copyright (c) 2017 Hartmut Kaiser
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

try:
    from _phylanx.execution_tree import *

except Exception:
    from _phylanxd.execution_tree import *
