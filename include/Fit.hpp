#include <vector>

class TSpline3;

namespace Fit
{
    TSpline3 SmoothedSpline(const char *name, std::vector<Double_t> &x, std::vector<Double_t> &y, Double_t smoothing_factor = 0.0);
}