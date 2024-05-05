#ifndef TEST_PERFORMANCE_HPP
#define TEST_PERFORMANCE_HPP

#include "Matrix.hpp"
#include "chrono.hpp"

namespace algebra{

    template<typename T, STORAGE_ORDER so>
    class TestPerformance {
    private:
        Matrix<T, so> matrix; // sparse matrix
        Timings::Chrono clock; // clock
        
        static constexpr auto major = (so==ROWS)? "CSR" : "CSC";
        
    public:
        TestPerformance(const Matrix<T, so> & mat): matrix(mat) {};
        TestPerformance(const std::string & filename): matrix(filename) {};

        void test_compress();
        void test_uncompress();
        void test_SMV_multiplication();

        void test_all();

    }; // end class TestPerformance

    
    /// @brief prints computational time used for compressing a sparse matrix
    /// @tparam T value type
    /// @tparam so storage order 
    template<typename T, STORAGE_ORDER so>
    void TestPerformance<T, so>::test_compress()
    {
        // here i don't care if it is computational costly, it is the only way to get the computational time for compressing a matrix
        if(matrix.is_compressed())
            matrix.uncompress();

        clock.start();
        matrix.compress();
        clock.stop();
        
        auto time = clock.wallTime();

        std::cout << "Compressing " << major << " sparse matrix in "  << time << " microseconds.\n";

        return;
    };
    
    /// @brief prints computational time used for uncompressing a sparse matrix
    /// @tparam T value type
    /// @tparam so storage order
    template<typename T, STORAGE_ORDER so>
    void TestPerformance<T, so>::test_uncompress()
    {
        
        // here i don't care if it is computational costly, it is the only way to get the computational time for uncompressing a matrix
        if(!matrix.is_compressed())
            matrix.compress();

        clock.start();
        matrix.uncompress();
        clock.stop();
        
        auto time = clock.wallTime();

        std::cout << "Uncompressing " << major << " sparse matrix in "  << time << " microseconds.\n";

        return;
    };

    /// @brief prints computational time for sparse matrix vecotr multiplications in both compressed and uncompressed case 
    /// @tparam T value type
    /// @tparam so storage order
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

    /// @brief prints computational time for compressing, uncompressing and SMV multiplication
    /// @tparam T value type
    /// @tparam so storage order
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