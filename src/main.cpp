#include "Matrix_impl.hpp"
#include "Matrix.hpp"
#include <map>
#include <iostream>

using namespace algebra;

int main() {

    std::map<std::array<std::size_t, 2>, double> map_mat;
    map_mat[{1,2}] = 6;
    map_mat[{0,2}] = 2;
    map_mat[{1,1}] = 5;
    map_mat[{3,2}] = 8;

    Matrix<double, algebra::ROWS> m(map_mat, 4,4);

    m.compress();

    m.print();

    m.uncompress();

    m.print();

    m.compress();

    m.print();



    return 0;
}