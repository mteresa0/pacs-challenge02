#ifndef OPERATIONS_INC
#define OPERATIONS_INC

#include "Matrix.hpp"
#include <complex>

namespace algebra {

    template<typename T, STORAGE_ORDER SO>
    const Matrix<T, SO> operator*(const Matrix<T, SO> & mat, const std::vector<T> & v)
    {
        if (mat.c!=v.size()){
            throw std::logic_error("sizes do not match!");
        }

        Matrix<T, ROWS> res(mat.r, 1);

        if (mat.is_compressed())
        {      
            T sum = 0;
            std::size_t j = 0;
            for (std::size_t i = 0; i < mat.r; ++i){
                
                for (j=j; j<mat.ind_elem[i+1]; ++j)
                {
                    sum += mat({i,mat.ind_pos[j]}) * v[mat.ind_pos[j]];
                }
                if (!is_zero(sum)){
                    res.mmap[{i, 0}] = sum;
                    put_zero(sum);
                }
            }
            return res;
        }

        std::vector<T> vec_res(mat.c, 0);
        for (auto it = mat.mmap.cbegin(); it!=mat.mmap.cend(); ++it)//(auto & [idx, nnz] : mat.mmap)
        {
            index_type idx = it->first;
            T value = it->second;
            vec_res[idx[0]] += value*v[idx[1]];
        }
        for (std::size_t i = 0; i<mat.r; ++i)
        {
            if(!is_zero(vec_res[i])) {
                res.mmap[{i,0}] = vec_res[i];
            }
        }

        return res;
        
    };    
    
    template<typename T>
    const Matrix<T,COLS> operator*(const Matrix<T, COLS> & mat, const std::vector<T> & v)
    {
        if (mat.c!=v.size()){
            throw std::logic_error("sizes do not match!");
        }

        Matrix<T, COLS> res(mat.r, 1);

        if (mat.is_compressed()){
            
            std::size_t j = 0;

            std::vector<T> vec_res(mat.r, 0);
            for (std::size_t i = 0; i < mat.c; ++i){
                while (j<mat.ind_elem[i+1]){
                    std::size_t i_r = mat.ind_pos[j];
                    vec_res[i_r] += mat({i_r, i})*v[i];
                    ++j;
                }
            }
            for (std::size_t k = 0; k<mat.r; ++k)
            {
                if(!is_zero(vec_res[k])) {
                    res.mmap[{k,0}] = vec_res[k];
                }
            }
            return res;
        }

        std::vector<T> vec_res(mat.r, 0);
        for (auto it = mat.mmap.cbegin(); it!=mat.mmap.cend(); ++it)//(auto & [idx, nnz] : mat.mmap)
        {
            index_type idx = it->first;
            T value = it->second;
            vec_res[idx[0]] += value*v[idx[1]];
        }
        for (std::size_t i = 0; i<mat.r; ++i)
        {
            if(!is_zero(vec_res[i])) {
                res.mmap[{i,0}] = vec_res[i];
            }
        }

        return res;
    };

} // end namespace algebra

#endif // OPERATIONS_INC