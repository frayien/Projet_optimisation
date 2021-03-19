
#include <iostream>

#include "FGLPK.h"

int _main(void)
{
    FGLPK glpk("short", GLP_MAX);

    glpk.add_row("p", GLP_UP, 0.0, 1.0);
    glpk.add_row("q", GLP_UP, 0.0, 2.0);

    glpk.add_col("x1", GLP_LO, 0.0, 0.0, 0.6);
    glpk.add_col("x2", GLP_LO, 0.0, 0.0, 0.5);

    std::unique_ptr<double[]> matrix = std::make_unique<double[]>(2*2);

    matrix[0] = 1.0; matrix[1] = 2.0;
    matrix[2] = 3.0; matrix[3] = 1.0;

    glpk.load_matrix(matrix);

    glpk.solve();

    double value;
    std::unique_ptr<double[]> result = std::make_unique<double[]>(2);

    glpk.get_result(value, result);

    std::cout << "z = " << value << "; x1 = " << result[0] << "; x2 = " << result[1] << "\n";

  return 0;
}