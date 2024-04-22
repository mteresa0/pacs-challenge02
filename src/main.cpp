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

    const Matrix<double, algebra::ROWS> m(map_mat, 4,4);

    auto a = m(1,2);

    // std::cout << m(1,1) << "\n";

    // auto s = (m.is_compressed())? "is compressed\n":"uncompressed\n";
    // std::cout << s;

    // m({0,0}) = 108.0;

    

    // m.compress();

    // std::cout << m({1,2}) << "\n";
    // std::cout << m({0,0}) << "\n";

    return 0;
}