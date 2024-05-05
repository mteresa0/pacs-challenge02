
#include "Matrix.hpp"
#include "TestPerformance.hpp"
#include "chrono.hpp"
#include <map>
#include <iostream>
#include <complex>

using namespace algebra;

int main() {

    Timings::Chrono clock;

    // using type=std::complex<double>;
    using type=double;

    std::string filename = "data/lnsp_131.mtx";
    // std::string filename = "data/nnc666.mtx";

    Matrix<type, ROWS> M_rows(filename);
    Matrix<type, COLS> M_cols(M_rows);

    TestPerformance<type, ROWS> test_perf_csr(filename);
    TestPerformance<type, COLS> test_perf_csc(filename);
    
    test_perf_csr.test_all();
    test_perf_csc.test_all();

    return 0;
}