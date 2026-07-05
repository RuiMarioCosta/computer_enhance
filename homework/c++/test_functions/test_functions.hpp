#pragma once

#include "parameters.hpp"

using overhead_test_func = status (*)(repetition_tester&, test_parameters&);

status write_to_all_bytes(repetition_tester& tester, test_parameters& params);
status read_via_fread(repetition_tester& tester, test_parameters& params);
status read_via_ifstream(repetition_tester& tester, test_parameters& params);
