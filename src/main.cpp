
#include "Matrix.hpp"
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


    std::size_t n_it = 1;

    std::cout << "ROWS\n";

    //// compress matrix
    m_row.compress(); m_row.uncompress();
    clock.start();
    m_row.compress();
    clock.stop();
    std::cout << "compress matrix: \n";
    std::cout << clock << "\n";

    //// uncompress matrix
    m_row.uncompress();
    m_row.compress();
    clock.start();
    m_row.uncompress();
    clock.stop();
    std::cout << "uncompress matrix: \n";
    std::cout << clock << "\n";

    m_row.compress();


    return 0;
}