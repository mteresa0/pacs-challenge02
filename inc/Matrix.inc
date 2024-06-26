#ifndef MATRIX_INC
#define MATRIX_INC

#include "Matrix.hpp"
#include <stdexcept>
#include <fstream>

namespace algebra
{

    /// @brief compares indexes according to storage order
    /// @tparam so storage order 
    /// @param lhs 
    /// @param rhs 
    /// @return 
    template <STORAGE_ORDER so>
    bool compareIndex<so>::operator() (const index_type & lhs, const index_type & rhs) const noexcept
    {
        if constexpr(so == ROWS)
        {
            return (lhs[0]<rhs[0]) || ((lhs[0]==rhs[0]) && (lhs[1]<rhs[1]));
        }
        return (lhs[1]<rhs[1]) || ((lhs[1]==rhs[1]) && (lhs[0]<rhs[0]));
    };

    ///// class Matrix definitions

    
    /// @brief copy constructor: copies the data from a matrix with the other storing order
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @param mat matrix with the other storing order
    template<typename T, STORAGE_ORDER SO>
    Matrix<T,SO>::Matrix(const Matrix<T,Matrix<T,SO>::minor_index> &mat): 
    Matrix(mat.get_rows(), mat.get_cols())
    {

        if (mat.is_compressed()) {
            std::vector<std::size_t> v_minor_ind(std::move(mat.get_minor_vector()));
            std::vector<std::size_t> v_major_ind(std::move(mat.get_major_vector()));
            std::vector<T> v_values(std::move(mat.get_compressed_matrix()));
            index_type size({mat.get_rows(), mat.get_cols()});
            std::size_t j = 0;
            index_type current_index = {0,0};
            for(std::size_t i = 0; i<size[minor_index]; ++i){
                current_index[minor_index] = i;
                while(j<v_major_ind[i+1] && j<v_values.size()){
                    current_index[SO] = v_minor_ind[j];
                    dyn_mat[current_index] = v_values[j];
                    ++j;
                }
            }
        } else {
            for(auto it = mat.cbegin(); it!=mat.cend(); ++it)
            {
                dyn_mat[it->first] = it->second;
            }
        }
            
    };

    /// @brief resize the dimensions of a matrix
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @param idx 
    template<typename T, STORAGE_ORDER SO>
    void Matrix<T, SO>::resize(const index_type & idx){
        if (idx[0]<r || idx[1]<c)
            throw std::logic_error("size cannot decrease");

        r = idx[0]; c = idx[1];
        return;
    };

    /// @brief resize the dimensions of a matrix
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @param i_r 
    /// @param i_c 
    template<typename T, STORAGE_ORDER SO>
    void Matrix<T, SO>::resize(const std::size_t & i_r, const std::size_t & i_c){
        if (i_r<r || i_c<c)
            throw std::logic_error("size cannot decrease");

        r = i_r; c = i_c;
        return;
    };

    /// @brief get the first element as constant iterator of the dynamic matrix
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @return dyn_mat.cbegin()
    template<typename T, STORAGE_ORDER SO>
    Matrix<T,SO>::map_type::const_iterator Matrix<T, SO>::cbegin() const
    {
        if(isCompressed){
            throw std::logic_error("the matrix is compressed\n");
        }
        
        return dyn_mat.cbegin();
    }

    /// @brief get the end as constant iterator of the dynamic matrix
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @return dyn_mat.cend()
    template<typename T, STORAGE_ORDER SO>
    Matrix<T,SO>::map_type::const_iterator Matrix<T, SO>::cend() const
    {
        if(isCompressed){
            throw std::logic_error("the matrix is compressed\n");
        }
        
        return dyn_mat.cend();
    }

    /// @brief read sparse matrix in matrix market format from a file
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @param filename 
    template<typename T, STORAGE_ORDER SO>
    void Matrix<T,SO>::read_matrix_mmf(const std::string & filename){

        std::ifstream file(filename);
        if (!file){
            std::cerr << "file not found\n";
            return;
        }

        std::size_t r_, c_, nnz;
        std::string first_line;
        std::size_t i_r, i_c;
        T value;
        
        getline(file, first_line);  // read first line (not useful)      
        file >> r_ >> c_ >> nnz;    // read number of rows, columns and nonzero elements
        resize(r_, c_);
        for(std::size_t i = 0; i<nnz; ++i){
            file >> i_r >> i_c >> value;
            i_r -=1; i_c -=1;   // subtract 1 beacuse the first element is not 0
            dyn_mat[{i_r, i_c}] = value;
        }

        file.close();
        return;
    }

    /// @brief compress sparse matrix
    /// @tparam T value type
    /// @tparam SO storage order 
    template<typename T, STORAGE_ORDER SO>
    void Matrix<T, SO>::compress()
    {
        if (isCompressed)
        {
            std::cout << "The matrix is already compressed!\n";
            return;
        }

        std::size_t major = (SO==ROWS)?r:c;

        major_vec.reserve(major+1);
        minor_vec.reserve(nonzero());
        compressed_mat.reserve(nonzero());

        auto it1 = dyn_mat.begin(); auto it0 = it1;

        std::array<std::size_t,2> upperboundIndex = {0,0};

        std::size_t major_ind = 0;
        
        for (std::size_t i = 0; i<major; ++i)
        {
            it0 = it1; // update lower bound
            upperboundIndex[SO] = i+1;
            it1 = dyn_mat.lower_bound(upperboundIndex); // compute new upper bound

            major_vec.push_back(major_ind);
            for (auto it=it0; it!=it1; ++it)
            {
                minor_vec.push_back(it->first[minor_index]);
                compressed_mat.push_back(it->second);
                major_ind++;
            }
        }
        major_vec.push_back(nonzero());

        dyn_mat.clear();

        isCompressed = true;


        return;
    };

    /// @brief uncompress sparse matrix
    /// @tparam T value type
    /// @tparam SO storage order 
    template<typename T, STORAGE_ORDER SO>
    void Matrix<T, SO>::uncompress()
    {
        index_type current_index = {0,0};

        std::size_t major = (SO == ROWS) ? r : c;
        std::size_t i_elem = 0;

        std::size_t n_elem_loc;

        for(std::size_t i_major= 0; (i_major < major) && (i_elem<nonzero()); ++i_major)
        {
            n_elem_loc = major_vec[i_major+1] - major_vec[i_major];
            current_index[SO] = i_major;
            for (std::size_t i = 0; i<n_elem_loc; ++i)
            {
                current_index[minor_index] = minor_vec[i_elem];
                dyn_mat[current_index] = compressed_mat[i_elem];
                ++i_elem;
            }
        }

        minor_vec.clear();
        major_vec.clear();
        compressed_mat.clear();

        isCompressed = false;

        return;
    };

    /// @brief check if indeces are in range
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @param idx 
    /// @return true if the indeces are valid, false oterwise
    template<typename T, STORAGE_ORDER SO>
    bool Matrix<T, SO>::index_in_range(const index_type & idx) const {
        if(idx[0]<r && idx[1]<c){
            return true;
        }

        return false;
    }

    /// @brief 
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @return number of nonzero elements
    template<typename T, STORAGE_ORDER SO>
    std::size_t Matrix<T,SO>::nonzero() const 
    {
        if (isCompressed)
        {
            return compressed_mat.size();
        }
        return dyn_mat.size();
    }
    
    /// @brief call operator - getter
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @param idx indices
    /// @return element of the sparse matrix
    template<typename T, STORAGE_ORDER SO>
    const T Matrix<T, SO>::operator() (const index_type & idx) const 
    {
        
        // check out of range
        if(!index_in_range(idx))
        {
            throw std::out_of_range("Indeces are out of range");
        }
        
        if(isCompressed)
        {
            for (std::size_t i = major_vec[idx[SO]]; i<major_vec[idx[SO]+1] && idx[minor_index]>=minor_vec[i]; ++i)
            {
                if(minor_vec[i]==idx[minor_index])
                {
                    return compressed_mat[i];
                }
            }

            // if you arrived here it is because there is no match
            return T(0);
        }

        // else (it is uncompressed)

        // get bounds
        index_type bound={0,0}; 
        bound[SO] = idx[SO];
        auto it_l = dyn_mat.lower_bound(bound);
        bound[SO] = idx[SO]+1;
        auto it_u = dyn_mat.lower_bound(bound);

        bool id_passed = false;
        for(auto it = it_l; it!=it_u && !id_passed; ++it)
        {
            if(idx[minor_index]==it->first[minor_index])
                return it->second;
            
            if(it->first[minor_index]>idx[minor_index])
                id_passed = true;
        }

        return T(0);
    }

    /// @brief call operator - getter
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @param ir row index
    /// @param ic column index
    /// @return element of the sparse matrix
    template<typename T, STORAGE_ORDER SO>
    const T Matrix<T,SO>::operator() (const std::size_t & ir, const std::size_t & ic) const {
        return (*this)({ir,ic});
    }

    /// @brief call operator - setter
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @param idx indices
    /// @return 
    template<typename T, STORAGE_ORDER SO>
    T& Matrix<T, SO>::operator() (const index_type & idx)
    {
        if (!index_in_range(idx))
        {
            throw std::out_of_range("Index out of range");
        }

        if(isCompressed)
        {
            bool found = false;
            for (std::size_t i = major_vec[idx[SO]]; i<major_vec[idx[SO]+1] && idx[minor_index]>=minor_vec[i]; ++i)
            {
                if (minor_vec[i]==idx[minor_index])
                {
                    found = true;
                    return compressed_mat[i];
                }
            }

            if (!found){
                throw std::logic_error("cannot add element when the matrix is compressed");
            }
        }

        return dyn_mat[idx];
    }

    /// @brief call operator - setter
    /// @tparam T value type
    /// @tparam SO storage order 
    /// @param ir row index
    /// @param ic column index
    /// @return 
    template<typename T, STORAGE_ORDER SO>
    T& Matrix<T, SO>::operator() (const std::size_t & ir, const std::size_t & ic)
    {
        return (*this)({ir,ic});
    }

    /// @brief print
    /// @tparam T value type
    /// @tparam SO storage order 
    template<typename T, STORAGE_ORDER SO>
    void Matrix<T, SO>::print() const {
        std::cout << "\n\nnumber of rows:    " << r << "\n";
        std::cout << "number of columns: " << c << "\n";
        std::cout << "non-zero:          " << nonzero() << "\n";
        auto s1 = isCompressed?"compressed\n" : "uncompressed\n";
        std::cout << "state:             " << s1;
        auto s2 = (SO==ROWS)? "rows\n" : "columns\n";
        std::cout << "major:             " << s2;
    };

    /// @brief print all elements
    /// @tparam T value type
    /// @tparam SO storage order 
    template<typename T, STORAGE_ORDER SO>
    void Matrix<T, SO>::verbose_print() const {

        (*this).print();
        if (isCompressed)
        {
            std::cout << "elem       ";
            for (auto i = 0; i<nonzero(); ++i)
                std::cout << compressed_mat[i] << " ";
            std::cout << "\n";

            std::cout << "ind_minor  ";
            for (auto i = 0; i<nonzero(); ++i)
                std::cout << minor_vec[i] << " ";
            std::cout << "\n";

            auto major = (SO==ROWS)?r:c;
            std::cout << "ind_major  ";
            for (auto i = 0; i<major+1; ++i)
                std::cout << major_vec[i] << " ";
            std::cout << "\n";
        } else {
            for (auto it = dyn_mat.cbegin(); it!=dyn_mat.cend(); ++it)
            {
                std::cout << "[ " << it->first[0] << ", "<< it->first[1] << " ]  ->  " << it->second << "\n";
            }
        }
    }


    ///// end class Matrix definitions

} // end namespace algebra


#endif // MATRIX_INC