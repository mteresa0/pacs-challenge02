

#include "Matrix_impl.hpp"
#include "Operations.hpp"
#include "Matrix.hpp"
#include "chrono.hpp"
#include <map>
#include <iostream>

using namespace algebra;

int main() {

    Timings::Chrono clock;

    std::map<std::array<std::size_t, 2>, double> map_mat;
    map_mat[{1,1}] = 2;
    map_mat[{0,0}] = 1;
    map_mat[{2,2}] = 3;
    map_mat[{3,3}] = 4;

    map_mat[{1,0}] += 29;

    Matrix<double, algebra::ROWS> m(map_mat, 5,8);

    //// compress matrix
    m.compress(); m.uncompress();
    clock.start();
    m.compress();
    clock.stop();
    std::cout << "compress matrix: \n";
    std::cout << clock << "\n";

    //// uncompress matrix
    m.uncompress();
    m.compress();
    clock.start();
    m.uncompress();
    clock.stop();
    std::cout << "uncompress matrix: \n";
    std::cout << clock << "\n";

    m.compress();

    //// multiplications
    std::vector<double> vec = {1,2,3,4,5};
    auto res = m*vec; res = m*vec;
    clock.start();
    res = m*vec;
    clock.stop();
    std::cout << "compress SMV multiplication: \n";
    std::cout << clock << "\n";

    m.uncompress();
    res = m*vec; res = m*vec;
    clock.start();
    res = m*vec;
    clock.stop();
    std::cout << "uncompress SMV multiplication: \n";
    std::cout << clock << "\n";
    // res.compress();


    return 0;
}