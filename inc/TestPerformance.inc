#ifndef TEST_PERFORMANCE_INC
#define TEST_PERFORMANCE_INC

#include "Matrix.hpp"
#include "TestPerformance.hpp"
#include "chrono.hpp"
#include <ostream>

namespace algebra {

    // template<class T> template<STORAGE_ORDER so> 
    // TestPerformance<T>::TestPerformance(const algebra::Matrix<T, so> &mat)


    template<typename T, STORAGE_ORDER so>
    void TestPerformance<T, so>::test_compress()
    {
        if(matrix.is_compressed())
            matrix.uncompress();

        clock.start();
        matrix.compress();
        clock.stop();
        
        auto time = clock.wallTime();

        constexpr auto major = (so==ROWS)? "CSR":"CSC";

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

        constexpr auto major = (so==ROWS)? "CSR":"CSC";

        std::cout << "Uncompressing " << major << " sparse matrix in "  << time << " microseconds.\n";

        return;
    };

    template<typename T, STORAGE_ORDER so>
    void TestPerformance<T, so>::test_SMV_multiplication()
    {
        std::vector<T> vec(matrix.get_rows(), 1);

        clock.start();
        auto res1 = matrix*vec;
        clock.stop();
        
        auto time = clock.wallTime();

        constexpr auto major = (so==ROWS)? "CSR":"CSC";
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




}// end namespace algebra

#endif // TEST_PERFORMANCE_INC