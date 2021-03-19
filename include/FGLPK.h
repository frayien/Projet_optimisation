/**
 * @file FGLPK.h
 * @author Adrien GARBANI (frayien@yahoo.fr)
 * @brief C++ wrapper for the gplk linear solver
 * @version 1.0
 * @date 2021-03-19
 */

#ifndef FNES_FGLPK
#define FNES_FGLPK

#include <memory>

#include <glpk.h>             /* GNU GLPK linear/mixed integer solver */

class FGLPK
{
public:
    FGLPK(const char* name, int direction);
    virtual ~FGLPK();

    void add_row(const char* name, int bound_type, double low_bound, double high_bound);
    void add_col(const char* name, int bound_type, double low_bound, double high_bound, double coef);
    void load_matrix(const std::unique_ptr<double[]> & matrix);

    void solve();

    void get_result(double & value, std::unique_ptr<double[]> & result);

private:
    glp_prob *m_linear_problem = nullptr;

    int m_row_size = 0;
    int m_col_size = 0;
};

#endif // FNES_FGLPK
