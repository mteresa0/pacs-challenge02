
#include "Matrix.hpp"
#include "TestPerformance.hpp"
#include "chrono.hpp"
#include <map>
#include <iostream>

using namespace algebra;

int main() {

    Timings::Chrono clock;
    
    Matrix<double, algebra::ROWS> m_row(0,0);
    Matrix<double, algebra::COLS> m_col(0,0);

    std::string filename = "lnsp_131.mtx";

    m_row.read_matrix_mmf(filename);
    m_col.read_matrix_mmf(filename);

    TestPerformance<double, ROWS> test_perf_csr(m_row);
    TestPerformance<double, COLS> test_perf_csc(m_col);
    
    test_perf_csr.test_compress();
    test_perf_csc.test_compress();
    test_perf_csr.test_uncompress();
    test_perf_csc.test_uncompress();
    test_perf_csr.test_SMV_multiplication();
    test_perf_csc.test_SMV_multiplication();

    return 0;
}