#ifndef OPERATIONS_INC
#define OPERATIONS_INC

#include "Matrix.hpp"

namespace algebra {

    template<typename T, STORAGE_ORDER SO>
    const Matrix<T, SO> operator*(const Matrix<T, SO> & mat, const std::vector<T> & v)
    {
        if (mat.r!=v.size()){
            throw std::logic_error("sizes do not match!");
        }

        Matrix<T, ROWS> res(mat.c, 1);

        if (mat.is_compressed())
        {      
            T sum = 0;
            std::size_t j = 0;
            for (std::size_t i = 0; i < mat.r; ++i){
                
                for (j=j; j<mat.ind_elem[i+1]; ++j)
                {
                    sum += mat({i,mat.ind_pos[j]}) * v[mat.ind_pos[j]];
                }
                if (sum!=0){
                    res.mmap[{i, 0}] = sum;
                    sum = 0;
                }
            }
            return res;
        }

        index_type upper_bound = {1,0};
        auto it = mat.mmap.begin();
        auto it_u = mat.mmap.begin(); // initialization

        T sum = 0;
        for (std::size_t i = 0; i<mat.r; ++i)
        {
            upper_bound[0] = i+1;
            it_u = mat.mmap.lower_bound(upper_bound);
            for (it = it; it!=it_u; ++it)
            {
                sum += it->second * v[it->first[1]];
            }
            if (sum!=0){
                res.mmap[{i,0}] = sum;
                sum = 0;
            }
        }

        return res;
        
    };    
    
    template<typename T>
    const Matrix<T,COLS> operator*(const Matrix<T, COLS> & mat, const std::vector<T> & v)
    {
        if (mat.r!=v.size()){
            throw std::logic_error("sizes do not match!");
        }

        Matrix<T, COLS> res(mat.c, 1);

        if (mat.is_compressed()){
            
            std::size_t j = 0;

            for (std::size_t i = 0; i < mat.c; ++i){
                for (j=j; j<mat.ind_elem[i+1]; ++j){
                    std::size_t i_r = mat.ind_pos[j];
                    res.mmap[{i_r,0}] += mat(i_r,i) * v[i];
                }
            }
            return res;
        }
        
        for (auto & [idx, nnz] : mat.mmap)
        {
            if (res.mmap.count({idx[0],0})>0){
                res({idx[0], 0}) += nnz*v[idx[1]];
            } else {
                res({idx[0], 0}) = nnz*v[idx[1]];
            }
        }

        return res;
    };

} // end namespace algebra

#endif // OPERATIONS_INC