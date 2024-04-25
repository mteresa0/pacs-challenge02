

#include "Matrix_impl.hpp"
#include "Operations.hpp"
#include "Matrix.hpp"
#include "chrono.hpp"
#include <map>
#include <iostream>

using namespace algebra;

int main() {

    Timings::Chrono clock;
    
    Matrix<double, algebra::ROWS> m_row(0,0);
    Matrix<double, algebra::ROWS> m_col(0,0);

    std::string filename = "lnsp_131.mtx";

    m_row.read_file(filename);
    m_col.read_file(filename);

    m_row.print();

    std::size_t n_it = 100000;

    std::cout << "  ROWS  \n";

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

    //// multiplications
    std::vector<double> vec(m_row.get_cols(),1);
    auto res_row = m_row*vec;
    clock.start();
    for (auto i =0; i<n_it; ++i)
        res_row = m_row*vec;
    clock.stop();
    std::cout << "compress SMV multiplication: \n";
    std::cout << clock << "\n";

    m_row.uncompress();
    clock.start();
    for (auto i =0; i<n_it; ++i)
        res_row = m_row*vec;
    clock.stop();
    std::cout << "uncompress SMV multiplication: \n";
    std::cout << clock << "\n";

    std::cout << "  COLS  \n";

    //// compress matrix
    m_col.compress(); m_col.uncompress();
    clock.start();
    m_col.compress();
    clock.stop();
    std::cout << "compress matrix: \n";
    std::cout << clock << "\n";

    //// uncompress matrix
    m_col.uncompress();
    m_col.compress();
    clock.start();
    m_col.uncompress();
    clock.stop();
    std::cout << "uncompress matrix: \n";
    std::cout << clock << "\n";

    m_col.compress();

    //// multiplications
    // std::vector<double> vec(m_col.get_cols(),1);
    auto res_col = m_col*vec;
    for (auto i =0; i<n_it; ++i)
        res_col = m_col*vec;
    clock.stop();
    std::cout << "compress SMV multiplication: \n";
    std::cout << clock << "\n";

    m_col.uncompress();
    clock.start();
    for (auto i =0; i<n_it; ++i)
        res_col = m_col*vec;
    clock.stop();
    std::cout << "uncompress SMV multiplication: \n";
    std::cout << clock << "\n";

    


    return 0;
}