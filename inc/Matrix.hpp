#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <map>
#include <vector>
#include <iostream>
#include <complex>

namespace algebra
{
    enum STORAGE_ORDER {
        ROWS = std::size_t(0),
        COLS = std::size_t(1)
    };

    using index_type = std::array<std::size_t, 2>;

    template<STORAGE_ORDER so>
    struct compareIndex
    {
        bool operator() (const index_type & lhs, const index_type & rhs) const noexcept;
    };

    template<typename T, STORAGE_ORDER SO = ROWS> 
    class Matrix {
        using map_type = std::map<std::array<std::size_t, 2>, T, compareIndex<SO>>;

    private:
        map_type dyn_mat; //dynamic matrix
        std::size_t c; // number of columns
        std::size_t r; // number of rows
        bool isCompressed = false; // state: true if the matrix is compressed
        std::vector<std::size_t> major_vec, minor_vec; // vectors storing indexes for compressed format
        std::vector<T> compressed_mat; // compressed matrix
        
        bool index_in_range(const index_type & idx) const; // check if indexes are in range
        std::size_t nonzero() const; // returns nonzero elements

        static constexpr STORAGE_ORDER minor_index = (SO==ROWS) ? COLS : ROWS; // stores the other storing order

    public:
        explicit Matrix(std::size_t nr=0, std::size_t nc=0): r(nr), c(nc), isCompressed(false) {};

        /// @brief constructor: create matrix from existing map (without storing order)
        Matrix(const std::map<std::array<std::size_t, 2>, T> & map, std::size_t nr, std::size_t nc): 
        dyn_mat(map.begin(), map.end()), r(nr), c(nc), isCompressed(false) {};

        /// @brief constructor: read from matrix market format file
        /// @param filename 
        Matrix(const std::string & filename): Matrix() {read_matrix_mmf(filename);};

        // copy constructor
        Matrix(const Matrix &) = default;
        // constructor from other storage order 
        Matrix(const Matrix<T,minor_index> &);
        // move constructor
        Matrix(Matrix &&) = default;
        
        map_type::const_iterator cbegin() const;
        map_type::const_iterator cend() const;

        std::vector<T> get_compressed_matrix() const {return compressed_mat;};
        std::vector<std::size_t> get_minor_vector() const {return minor_vec;};
        std::vector<std::size_t> get_major_vector() const {return major_vec;};

        void read_matrix_mmf(const std::string & filename);

        void resize(const index_type &);
        void resize(const std::size_t & i_r, const std::size_t & i_c);

        /// @brief returns the state of the sparse matrix (compressed/uncompressed)
        bool is_compressed() const {return isCompressed;};
        
        void compress();
        void uncompress();

        const T operator() (const index_type & idx) const;
        const T operator() (const std::size_t & ir, const std::size_t & ic) const;

        T& operator()(const index_type & idx);
        T& operator()(const std::size_t & ir, const std::size_t & ic);

        void print() const;
        void verbose_print() const;

        /// @brief get number of rows 
        std::size_t get_rows() const {return r;};
        /// @brief get number of columns 
        std::size_t get_cols() const {return c;};

        template <typename U>
        friend const Matrix<U,ROWS> operator*(const Matrix<U,ROWS> &mat, const std::vector<U> & v);

        template <typename U>
        friend const Matrix<U,COLS> operator*(const Matrix<U,COLS> &mat, const std::vector<U> & v);

    }; // end Matrix class

    /// @brief 
    /// @tparam T value type
    /// @param value 
    /// @return 
    template<typename T>
    inline const bool is_zero(const T & value) {return value == T();};

    template<typename T>
    inline void put_zero(T & value) {value = T(); return;};
    
} // end namespace algebra

#include "Matrix.inc"
#include "Operations.inc"

#endif // MATRIX_HPP