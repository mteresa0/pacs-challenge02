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
        
        static constexpr auto major = (so==ROWS)? "CSR" : "CSC";
        
    public:
        TestPerformance(const Matrix<T, so> & mat): matrix(mat) {};
        TestPerformance(const std::string & filename): matrix(filename) {};

        void test_compress();
        void test_uncompress();
        void test_SMV_multiplication();

        void test_all();

    }; // end class TestPerformance

    
    template<typename T, STORAGE_ORDER so>
    void TestPerformance<T, so>::test_compress()
    {
        if(matrix.is_compressed())
            matrix.uncompress();

        clock.start();
        matrix.compress();
        clock.stop();
        
        auto time = clock.wallTime();

        std::cout << "Compressing " << major << " sparse matrix in "  << time << " microseconds.\n";

        return;
    };

    template<typename T, STORAGE_ORDER so>
    void TestPerformance<T, so>::test_uncompress()
    {
        if(!matrix.is_compressed())
            matrix.compress();

        clock.start();
        matrix.uncompress();
        clock.stop();
        
        auto time = clock.wallTime();

        std::cout << "Uncompressing " << major << " sparse matrix in "  << time << " microseconds.\n";

        return;
    };

    template<typename T, STORAGE_ORDER so>
    void TestPerformance<T, so>::test_SMV_multiplication()
    {
        std::vector<T> vec(matrix.get_cols(), 1);

        clock.start();
        auto res1 = matrix*vec;
        clock.stop();
        
        auto time = clock.wallTime();

        auto state = (matrix.is_compressed())?"compressed":"uncompressed";

        std::cout << major << " " << state << " SMV multiplication in "  << time << " microseconds.\n";

        if (matrix.is_compressed())
            matrix.uncompress();
        else
            matrix.compress();
        
        clock.start();
        auto res2 = matrix*vec;
        clock.stop();
        
        time = clock.wallTime();

        state = (matrix.is_compressed())?"compressed":"uncompressed";
        
        std::cout << major << " " << state << " SMV multiplication in "  << time << " microseconds.\n";

        return;
    };

    template<typename T, STORAGE_ORDER so>
    void TestPerformance<T, so>::test_all()
    {
        test_compress();
        test_uncompress();
        test_SMV_multiplication();

        return;
    }

}; // end namespace algebra

#endif // TEST_PERFORMANCE_HPP