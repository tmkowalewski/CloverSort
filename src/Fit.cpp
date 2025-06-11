#include <stdexcept>
#include <TMatrixD.h>
#include <TSpline.h>
#include "Fit.hpp"

namespace Fit
{

    TSpline3 SmoothedSpline(const char *name, std::vector<Double_t> &x, std::vector<Double_t> &y, Double_t smoothingFactor)
    {
        // Finds smoothed cubic spline for given x and y data points
        // References:
        //  https://en.wikipedia.org/wiki/Smoothing_spline#Derivation_of_the_cubic_smoothing_spline
        //  https://bookdown.org/mike/data_analysis/sec-smoothing-splines.html
        //

        if (x.size() != y.size() || x.size() < 2)
        {
            throw std::invalid_argument("Input vectors for fitSmoothedSpline must have the same size and contain at least two points.");
        }

        Int_t n = x.size();
        std::vector<Double_t> h_vector(n - 1);
        for (Int_t i = 0; i < n - 1; ++i)
        {
            h_vector[i] = x[i + 1] - x[i];
        }

        TMatrixD delta_matrix(n - 2, n);
        for (Int_t i = 0; i < n - 2; ++i)
        {
            for (Int_t j = 0; j < n; ++j)
            {
                if (j == i)
                    delta_matrix(i, j) = 1 / h_vector[i];
                else if (j == i + 1)
                    delta_matrix(i, j) = -1 / h_vector[i] - 1 / h_vector[i + 1];
                else if (j == i + 2)
                    delta_matrix(i, j) = 1 / h_vector[i + 1];
            }
        }

        TMatrixD delta_matrix_transpose = delta_matrix;
        delta_matrix_transpose.T();

        TMatrixD w_matrix(n - 2, n - 2);
        for (Int_t i = 0; i < n - 2; ++i)
        {
            for (Int_t j = 0; j < n - 2; ++j)
            {
                if (j == i - 1)
                    w_matrix(i, j) = h_vector[i] / 6;
                else if (i == j - 1)
                    w_matrix(i, j) = h_vector[j] / 6;
                else if (i == j)
                    w_matrix(i, j) = (h_vector[i] + h_vector[i + 1]) / 3;
            }
        }

        TMatrixD I_matrix(n, n);
        for (Int_t i = 0; i < n; ++i)
        {
            I_matrix(i, i) = 1;
        }

        auto a_matrix = delta_matrix_transpose * w_matrix.Invert() * delta_matrix;
        a_matrix *= smoothingFactor;

        TMatrixD y_prime(n, 1);
        for (Int_t i = 0; i < n; ++i)
        {
            y_prime(i, 0) = y[i];
        }

        auto result_matrix = (I_matrix + a_matrix).Invert() * y_prime;
        std::vector<Double_t> smoothed_y(n);
        for (Int_t i = 0; i < n; ++i)
        {
            smoothed_y[i] = result_matrix(i, 0);
        }

        // Create a TSpline3 object with the provided name and data points

        return TSpline3(name, x.data(), smoothed_y.data(), x.size());
    }

} // namespace Fit