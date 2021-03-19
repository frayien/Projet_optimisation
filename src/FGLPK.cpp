#include "FGLPK.h"

FGLPK::FGLPK(const char* name, int direction)
{
    m_linear_problem = glp_create_prob();
    glp_set_prob_name(m_linear_problem, name);
    glp_set_obj_dir(m_linear_problem, direction);
}

FGLPK::~FGLPK()
{
    glp_delete_prob(m_linear_problem);
    glp_free_env();
}

void FGLPK::add_row(const char* name, int bound_type, double low_bound, double high_bound)
{
    m_row_size++;

    glp_add_rows(m_linear_problem, m_row_size);
    glp_set_row_name(m_linear_problem, m_row_size, name);
    glp_set_row_bnds(m_linear_problem, m_row_size, bound_type, low_bound, high_bound);
}

void FGLPK::add_col(const char* name, int bound_type, double low_bound, double high_bound, double coef)
{
    m_col_size++;

    glp_add_cols(m_linear_problem, 1);
    glp_set_col_name(m_linear_problem, m_col_size, name);
    glp_set_col_bnds(m_linear_problem, m_col_size, bound_type, low_bound, high_bound);
    glp_set_obj_coef(m_linear_problem, m_col_size, coef);
}

void FGLPK::load_matrix(const std::unique_ptr<double[]> & matrix)
{
    std::unique_ptr<int[]>    ia = std::make_unique<int[]>   (1+m_row_size*m_col_size);
    std::unique_ptr<int[]>    ja = std::make_unique<int[]>   (1+m_row_size*m_col_size);
    std::unique_ptr<double[]> ar = std::make_unique<double[]>(1+m_row_size*m_col_size);

    for(int k = 0; k<m_row_size*m_col_size; ++k)
    {
        // on a :
        // a[i,j] = r
        // on veut ia, ja, ar tels que :
        // ia[k] = i, ja[k] = j, ar[k] = k
        // avec k = 1...N

        ia[k+1] = (k/m_row_size) + 1; // numéro de la ligne
        ja[k+1] = (k%m_row_size) + 1; // numéro de la colonne
        ar[k+1] = matrix[k];
    }

    glp_load_matrix(m_linear_problem, 4, ia.get(), ja.get(), ar.get());
}

void FGLPK::solve()
{
    glp_simplex(m_linear_problem, NULL);
}

void FGLPK::get_result(double & value, std::unique_ptr<double[]> & result)
{
    value = glp_get_obj_val(m_linear_problem);

    for(int i = 0; i<m_col_size; ++i)
    {
        result[i] = glp_get_col_prim(m_linear_problem, i+1);
    }
}