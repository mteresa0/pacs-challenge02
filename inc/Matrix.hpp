#ifndef MATRIX_HPP
#define MATRIX_HPP

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
        bool isCompressed = false; // state: true if the matrix is compressed
        std::vector<std::size_t> ind_elem, ind_pos; // vectorS storing indexes for compressed format
        std::vector<T> mat_c; // compressed matrix
        
        bool index_in_range(const index_type & idx) const; // check if indexes are in range
        std::size_t nonzero() const;

        template <typename U, STORAGE_ORDER so>
        friend const Matrix<U,so> operator*(const Matrix<U,so> &mat, const std::vector<U> & v);

        template <typename U>
        friend const Matrix<U,COLS> operator*(const Matrix<U,COLS> &mat, const std::vector<U> & v);


    public:
        Matrix(std::size_t nr, std::size_t nc): r(nr), c(nc), isCompressed(false) {};

        Matrix(const std::map<std::array<std::size_t, 2>, T> & map, std::size_t nr, std::size_t nc): 
        mmap(map.begin(), map.end()), r(nr), c(nc), isCompressed(false) 
        {};

        void read_file(const std::string & filename);

        void resize(const index_type &);
        void resize(const std::size_t & i_r, const std::size_t & i_c);

        bool is_compressed() const {return isCompressed;};
        
        void compress();
        void uncompress();

        const T operator() (const index_type & idx) const;
        const T operator() (const std::size_t & ir, const std::size_t & ic) const;

        T& operator()(const index_type & idx);
        T& operator()(const std::size_t & ir, const std::size_t & ic);

        void print() const;
        void verbose_print() const;

        std::size_t get_rows() {return r;};
        std::size_t get_cols() {return c;};

    }; // end Matrix class

} // end namespace algebra

#include "Matrix.inc"
#include "Operations.inc"

#endif // MATRIX_HPP