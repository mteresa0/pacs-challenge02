#ifndef MATRIX_IMPLEMENTATION
#define MATRIX_IMPLEMENTATION

#include "Matrix.hpp"
#include <stdexcept>

namespace algebra
{

    /// @brief compares indexes according to storage order
    /// @tparam so 
    /// @param lhs 
    /// @param rhs 
    /// @return 
    template <STORAGE_ORDER so>
    bool compareIndex<so>::operator() (const index_type & lhs, const index_type & rhs) const
    {
        if constexpr(so == ROWS)
        {
            return (lhs[0]<rhs[0]) || ((lhs[0]==rhs[0]) && (lhs[1]<rhs[1]));
        }
        return (lhs[1]<rhs[1]) || ((lhs[1]==rhs[1]) && (lhs[0]<rhs[0]));
    };

    ///// class Matrix definitions

    /// @brief compress sparse matrix
    /// @tparam T 
    /// @tparam SO 
    template<typename T, STORAGE_ORDER SO>
    void Matrix<T, SO>::compress()
    {
        if (isCompressed)
        {
            std::cout << "The matrix is already compressed!\n";
            return;
        }

        std::size_t it = 0; //serve?

        std::size_t minor_index = (SO==ROWS)?1:0;
        std::size_t major = (SO==ROWS)?r:c;

        ind_elem.reserve(major+1);
        ind_pos.reserve(nonzero);
        mat_c.reserve(nonzero);

        auto it1 = mmap.begin(); auto it0 = it1;

        std::array<std::size_t,2> upperboundIndex = {0,0};

        std::size_t major_ind = 0;
        
        for (std::size_t i = 0; i<major; ++i)
        {
            it0 = it1; // update lower bound
            upperboundIndex[SO] = i+1;
            it1 = mmap.lower_bound(upperboundIndex); // compute new upper bound

            ind_elem.push_back(major_ind);
            for (auto it=it0; it!=it1; ++it)
            {
                ind_pos.push_back(it->first[minor_index]);
                mat_c.push_back(it->second);
                major_ind++;
            }
        }
        ind_elem.push_back(nonzero);

        mmap.clear();

        isCompressed = true;


        return;
    };

    /// @brief uncompress sparse matrix
    /// @tparam T 
    /// @tparam SO 
    template<typename T, STORAGE_ORDER SO>
    void Matrix<T, SO>::uncompress()
    {
        constexpr std::size_t minor_index = (SO == ROWS) ? 1 : 0;
        index_type current_index = {0,0};

        std::size_t major = (SO == ROWS) ? r : c;

        std::size_t i_elem = 0;

        for(std::size_t i_major= 0; (i_major < major) && (i_elem<nonzero); ++i_major)
        {
            std::size_t n_elem = ind_elem[i_major+1] - ind_elem[i_major];
            current_index[SO] = i_major;
            for (std::size_t i = 0; i<n_elem; ++i)
            {
                current_index[minor_index] = ind_pos[i_elem];
                mmap[current_index] = mat_c[i_elem];
                ++i_elem;
            }
        }

        ind_pos.clear();
        ind_elem.clear();
        mat_c.clear();

        isCompressed = false;

        return;
    };

    template<typename T, STORAGE_ORDER SO>
    bool Matrix<T, SO>::index_in_range(const index_type & idx) const {
        if(idx[0]<r && idx[1]<c){
            return true;
        }

        return false;
    }
    
    /// @brief call operator - getter
    /// @tparam T 
    /// @tparam SO 
    /// @param idx indices
    /// @return element of the sparse matrix
    template<typename T, STORAGE_ORDER SO>
    const T Matrix<T, SO>::operator() (const index_type & idx) const 
    {
        
        std::cout << "getter\n";
        // check out of range
        if(!index_in_range(idx))
        {
            return T(0);
        }
        
        if(isCompressed)
        {
            std::size_t minor_index = (SO==ROWS)?COLS:ROWS;
            for (std::size_t i = ind_elem[idx[SO]]; i<ind_elem[idx[SO]+1] && idx[minor_index]>=ind_pos[i]; ++i)
            {
                if(ind_pos[i]==idx[minor_index])
                {
                    return mat_c[i];
                }
            }

            // if you arrived here it is because there is no match
            return T(0);
        }

        // else (it is uncompressed)
        std::size_t minor_index = (SO==ROWS)?COLS:ROWS;

        // get bounds
        index_type bound={0,0}; 
        bound[SO] = idx[SO];
        auto it_l = mmap.lower_bound(bound);
        bound[SO] = idx[SO]+1;
        auto it_u = mmap.lower_bound(bound);

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
    /// @tparam T 
    /// @tparam SO 
    /// @param ir row index
    /// @param ic column index
    /// @return element of the sparse matrix
    template<typename T, STORAGE_ORDER SO>
    const T Matrix<T,SO>::operator() (const std::size_t & ir, const std::size_t & ic) const {
        return (*this)({ir,ic});
    }

    /// @brief call operator - setter
    /// @tparam T 
    /// @tparam SO 
    /// @param idx indices
    /// @return 
    template<typename T, STORAGE_ORDER SO>
    T& Matrix<T, SO>::operator() (const index_type & idx)
    {
        std::cout << "setter\n";
        if (!index_in_range(idx))
        {
            throw std::out_of_range("Index out of range");
        }

        if(isCompressed)
        {
            std::size_t minor_index = (SO==ROWS) ? COLS : ROWS;
            bool found = false;
            for (std::size_t i = ind_elem[idx[SO]]; i<ind_elem[idx[SO]+1] && idx[minor_index]>=ind_pos[i]; ++i)
            {
                if (ind_pos[i]==idx[minor_index])
                {
                    found = true;
                    return mat_c[i];
                }
            }

            if (!found){
                throw std::logic_error("cannot add element when the matrix is compressed");
            }
        }

        return mmap[idx];
    }

    /// @brief call operator - setter
    /// @tparam T 
    /// @tparam SO 
    /// @param ir row index
    /// @param ic column index
    /// @return 
    template<typename T, STORAGE_ORDER SO>
    T& Matrix<T, SO>::operator() (const std::size_t & ir, const std::size_t & ic)
    {
        return (*this)({ir,ic});
    }

    ///// end class Matrix definitions

} // end namespace algebra



#endif // MATRIX_IMPLEMENTATION