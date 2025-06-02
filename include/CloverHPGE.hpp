#ifndef CLOVERHPGE_HPP
#define CLOVERHPGE_HPP

#include <TString.h>
#include "Detector.hpp"

class CloverHPGE : public Detector
{
public:
    CloverHPGE(TString name, std::vector<Int_t> channels);
    ~CloverHPGE();
};

#endif // CLOVERHPGE_HPP