#ifndef PEAKFITTING_HPP
#define PEAKFITTING_HPP

// Standard C++ includes

// ROOT includes
#include <Rtypes.h>

// Project includes

namespace PeakFitting
{
    Double_t radwarePeakModel(Double_t *x, Double_t *par); // Radware model for peaks

}

#endif // PEAKFITTING_HPP