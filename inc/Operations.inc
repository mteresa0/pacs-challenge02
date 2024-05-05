#ifndef OPERATIONS_INC
#define OPERATIONS_INC

#include "Matrix.hpp"
#include <complex>

namespace algebra {

    /// @brief multiplication operator (ROWS specialization)
    /// @tparam T value type
    /// @param mat 
    /// @param v 
    /// @return 
    template<typename T>
    const Matrix<T, ROWS> operator*(const Matrix<T, ROWS> & mat, const std::vector<T> & v)
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
                
                for (j=j; j<mat.major_vec[i+1]; ++j)
                {
                    sum += mat({i,mat.minor_vec[j]}) * v[mat.minor_vec[j]];
                }
                if (!is_zero(sum)){
                    res.dyn_mat[{i, 0}] = sum;
                    put_zero(sum);
                }
            }
            return res;
        }

        std::vector<T> vec_res(mat.c, 0);
        for (auto it = mat.dyn_mat.cbegin(); it!=mat.dyn_mat.cend(); ++it)//(auto & [idx, nnz] : mat.dyn_mat)
        {
            index_type idx = it->first;
            T value = it->second;
            vec_res[idx[0]] += value*v[idx[1]];
        }
        for (std::size_t i = 0; i<mat.r; ++i)
        {
            if(!is_zero(vec_res[i])) {
                res.dyn_mat[{i,0}] = vec_res[i];
            }
        }

        return res;
        
    };    
    
    /// @brief multiplication operator specialization with storing order COLS
    /// @tparam T value type
    /// @param mat 
    /// @param v 
    /// @return res = mat*v
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
                while (j<mat.major_vec[i+1]){
                    std::size_t i_r = mat.minor_vec[j];
                    vec_res[i_r] += mat({i_r, i})*v[i];
                    ++j;
                }
            }
            for (std::size_t k = 0; k<mat.r; ++k)
            {
                if(!is_zero(vec_res[k])) {
                    res.dyn_mat[{k,0}] = vec_res[k];
                }
            }
            return res;
        }

        std::vector<T> vec_res(mat.r, 0);
        for (auto it = mat.dyn_mat.cbegin(); it!=mat.dyn_mat.cend(); ++it)//(auto & [idx, nnz] : mat.dyn_mat)
        {
            index_type idx = it->first;
            T value = it->second;
            vec_res[idx[0]] += value*v[idx[1]];
        }
        for (std::size_t i = 0; i<mat.r; ++i)
        {
            if(!is_zero(vec_res[i])) {
                res.dyn_mat[{i,0}] = vec_res[i];
            }
        }

        return res;
    };

} // end namespace algebra

#endif // OPERATIONS_INC