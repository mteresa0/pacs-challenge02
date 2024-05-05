
#include "Matrix.hpp"
#include "TestPerformance.hpp"
#include "chrono.hpp"
#include <map>
#include <iostream>
#include <complex>

using namespace algebra;

int main() {
    /// choose type
    // using type=std::complex<double>;
    using type=double;

    /// choose filename
    std::string filename = "data/lnsp_131.mtx";
    // std::string filename = "data/nnc666.mtx";

    TestPerformance<type, ROWS> benchmark_csr(filename);
    TestPerformance<type, COLS> benchmark_csc(filename);
    
    benchmark_csc.test_all();
    benchmark_csr.test_all();

    return 0;
}