
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

    TestPerformance<type, ROWS> benchmark_csr(filename);
    TestPerformance<type, COLS> benchmark_csc(filename);
    
    benchmark_csc.test_all();
    benchmark_csr.test_all();

    return 0;
}