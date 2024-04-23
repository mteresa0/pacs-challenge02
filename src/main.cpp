
#include "Matrix_impl.hpp"
#include "Operations.hpp"
#include "Matrix.hpp"
#include <map>
#include <iostream>

using namespace algebra;

int main() {

    std::map<std::array<std::size_t, 2>, double> map_mat;
    map_mat[{1,1}] = 2;
    map_mat[{0,0}] = 1;
    map_mat[{2,2}] = 3;
    map_mat[{3,3}] = 4;

    map_mat[{1,0}] += 29;

    Matrix<double, algebra::COLS> m(map_mat, 5,8);
    // m.compress();
    m.verbose_print();

    std::vector<double> vec = {1,2,3,4,5};


    auto res = m*vec;
    // res.compress();
    // m.compress();

    res.verbose_print();


    return 0;
}