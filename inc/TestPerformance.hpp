#ifndef TEST_PERFORMANCE_HPP
#define TEST_PERFORMANCE_HPP

#include "Matrix.hpp"
#include "chrono.hpp"

namespace algebra{

    template<typename T, STORAGE_ORDER so>
    class TestPerformance {
    private:
        Matrix<T, so> matrix;

        Timings::Chrono clock;
        
    public:

        TestPerformance(const Matrix<T, so> & mat): matrix(mat) {};

        void test_compress();
        void test_uncompress();
        void test_SMV_multiplication();


    }; // end class TestPerformance

}; // end namespace algebra


#include "TestPerformance.inc"

#endif // TEST_PERFORMANCE_HPP