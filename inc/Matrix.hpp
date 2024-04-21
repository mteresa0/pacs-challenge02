#ifndef MATRIX
#define MATRIX

#include <map>
#include <vector>
#include <iostream>

namespace algebra
{
    enum STORAGE_ORDER {
        ROWS = 0,
        COLS = 1
    };

    using index_type = std::array<std::size_t, 2>;

    template<STORAGE_ORDER so>
    struct compareIndex
    {
        bool operator() (const index_type & lhs, const index_type & rhs) const;
    };

    template<typename T, STORAGE_ORDER SO = ROWS> 
    class Matrix {

    private:
        std::map<std::array<std::size_t, 2>, T, compareIndex<SO>> mmap; //dynamic matrix
        std::size_t c; // number of columns
        std::size_t r; // number of rows
        std::size_t nonzero; // number of non-zero elements
        bool isCompressed; // state: true if the matrix is compressed
        std::vector<std::size_t> ind_elem, ind_pos; // vectorS storing indexes for compressed format
        std::vector<T> mat_c; // compressed matrix

    public:
        Matrix(const std::map<std::array<std::size_t, 2>, T> & map, std::size_t nr, std::size_t nc): 
        mmap(map.begin(), map.end()), r(nr), c(nc), isCompressed(false), nonzero(map.size()) 
        {};

        bool is_compressed() const {return isCompressed;};
        
        void compress();
        void uncompress();

        void print() const {
            auto s1 = isCompressed?"compressed\n" : "uncompressed\n";
            std::cout << "\nstate:             " << s1;
            std::cout << "number of rows:    " << r << "\n";
            std::cout << "number of columns: " << c << "\n";
            std::cout << "non-zero:          " << nonzero << "\n";

            if (isCompressed)
            {
                auto s = (SO==ROWS)? "rows\n" : "columns\n";
                std::cout << "major: " << s;
                std::cout << "elem       ";
                for (auto i = 0; i<nonzero; ++i)
                    std::cout << mat_c[i] << " ";
                std::cout << "\n";

                std::cout << "ind_minor  ";
                for (auto i = 0; i<nonzero; ++i)
                    std::cout << ind_pos[i] << " ";
                std::cout << "\n";

                auto major = (SO==ROWS)?r:c;
                std::cout << "ind_major  ";
                for (auto i = 0; i<major+1; ++i)
                    std::cout << ind_elem[i] << " ";
                std::cout << "\n";
            }
        };

    }; // end Matrix class

} // end namespace algebra

#endif // MATRIX