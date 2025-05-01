#include "algebra.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>


Matrix create_matrix(int row, int col)
{
    Matrix m;
    int i,j;
    m.rows = row;
    m.cols = col;
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++){
            m.data[i][j] = 0;
        }
    }
    return m;
}

Matrix add_matrix(Matrix a, Matrix b)
{
    if (a.rows != b.rows || a.cols != b.cols) {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }

    Matrix result = create_matrix(a.rows,a.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return result;   
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    if (a.rows != b.rows || a.cols != b.cols) {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    return result;
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    if (a.cols != b.rows) {
        printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.rows, b.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            for (int k = 0; k < a.cols; k++) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return result;
}

Matrix scale_matrix(Matrix a, double k)
{
    Matrix result = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result.data[i][j] = a.data[i][j] * k;
        }
    }
    return result;
}

Matrix transpose_matrix(Matrix a)
{
    Matrix result = create_matrix(a.cols, a.rows);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result.data[j][i] = a.data[i][j];
        }
    }
    return result;
}

double det_matrix(Matrix a)
{
    if (a.rows != a.cols) {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    
    int n = a.rows;
    double det = 1;
    Matrix temp = create_matrix(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp.data[i][j] = a.data[i][j];
        }
    }
    
    for (int i = 0; i < n; i++) {
        if (temp.data[i][i] == 0) {
            bool swap_success = false;
            for (int j = i + 1; j < n; j++) {
                if (temp.data[j][i] != 0) {
                    for (int k = 0; k < n; k++) {
                        double tmp = temp.data[i][k];
                        temp.data[i][k] = temp.data[j][k];
                        temp.data[j][k] = tmp;
                    }
                    det = -det;
                    swap_success = true;
                    break;
                }
            }
            if (!swap_success) {
                return 0;
            }
        }
        
        for (int j = i + 1; j < n; j++) {
            double factor = temp.data[j][i] / temp.data[i][i];
            for (int k = i; k < n; k++) {
                temp.data[j][k] -= factor * temp.data[i][k];
            }
        }
        det *= temp.data[i][i];
    }
    
    return det;
}

Matrix inv_matrix(Matrix a)
{
   if (a.rows != a.cols) {
        printf("Error: The matrix must be a square matrix.\n");
        return create_matrix(0, 0);
    }
    
    int n = a.rows;
    double det = det_matrix(a);
    if (fabs(det) < 1e-10) {
        printf("Error: The matrix is singular.\n");
        return create_matrix(0, 0);
    }
    
    Matrix inv = create_matrix(n, n);
    Matrix adj = create_matrix(n, n);
    
    if (n == 1) {
        inv.data[0][0] = 1.0 / a.data[0][0];
        return inv;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Matrix minor = create_matrix(n-1, n-1);
            for (int k = 0, x = 0; k < n; k++) {
                if (k == i) continue;
                for (int l = 0, y = 0; l < n; l++) {
                    if (l == j) continue;
                    minor.data[x][y] = a.data[k][l];
                    y++;
                }
                x++;
            }
            double cofactor = ((i + j) % 2 == 0 ? 1 : -1) * det_matrix(minor);
            adj.data[j][i] = cofactor;
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv.data[i][j] = adj.data[i][j] / det;
        }
    }
    
    return inv;
}


int rank_matrix(Matrix a)
{
int rank = 0;
    Matrix temp = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            temp.data[i][j] = a.data[i][j];
        }
    }
    
    for (int i = 0; i < a.cols && rank < a.rows; i++) {
        if (temp.data[rank][i] == 0) {
            bool found = false;
            for (int j = rank + 1; j < a.rows; j++) {
                if (temp.data[j][i] != 0) {
                    for (int k = 0; k < a.cols; k++) {
                        double tmp = temp.data[rank][k];
                        temp.data[rank][k] = temp.data[j][k];
                        temp.data[j][k] = tmp;
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                continue;
            }
        }
        
        for (int j = rank + 1; j < a.rows; j++) {
            double factor = temp.data[j][i] / temp.data[rank][i];
            for (int k = i; k < a.cols; k++) {
                temp.data[j][k] -= factor * temp.data[rank][k];
            }
        }
        rank++;
    }
    
    return rank;
}

double trace_matrix(Matrix a)
{
if (a.rows != a.cols) {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    
    double trace = 0;
    for (int i = 0; i < a.rows; i++) {
        trace += a.data[i][i];
    }
    return trace;
}

void print_matrix(Matrix a)
{
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}
