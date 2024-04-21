#ifndef MATRIX_IMPLEMENTATION
#define MATRIX_IMPLEMENTATION

#include "Matrix.hpp"

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

    template<typename T, STORAGE_ORDER SO>
    void Matrix<T, SO>::compress()
    {
        if (isCompressed)
        {
            std::cout << "The matrix is already compressed!\n";
            return;
        }

        std::size_t it = 0;

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

    ///// end class Matrix definitions

} // end namespace algebra



#endif // MATRIX_IMPLEMENTATION