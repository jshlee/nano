#include "nano/external/interface/ElecScaleFactorEvaluator.h"

ElecScaleFactorEvaluator::ElecScaleFactorEvaluator(){
  const unsigned int n = (pt_bins.size()-1)*(eta_bins.size()-1);
  // FIXME : check that these bins are monolothically increasing
  assert(values.size() == n);
  assert(errors.size() == n);
  
  // For cache
  width = pt_bins.size()-1;
};

double ElecScaleFactorEvaluator::operator()(const double x, const double y, const double shift){
  auto xbin = std::lower_bound(pt_bins.begin(), pt_bins.end(), x);
  if ( xbin == pt_bins.end() || xbin+1 == pt_bins.end() ) return 1;
  auto ybin = std::lower_bound(eta_bins.begin(), eta_bins.end(), y);
  if ( ybin == eta_bins.end() || ybin+1 == eta_bins.end() ) return 1;
  
  const int column = xbin-pt_bins.begin();
  const int row = ybin-eta_bins.begin();
  
  const int bin = row*width+column;
  const double value = values.at(bin);
  const double error = errors.at(bin);
  
  return std::max(0.0, value+shift*error);
};

double ElecScaleFactorEvaluator::getScaleFactor(const TParticle& p, const int pid, const double shift){
  const int aid = abs(p.GetPdgCode());
  if ( aid == pid ) {
    const double x = p.Pt(), y = p.Eta();
    
    auto xbin = std::lower_bound(pt_bins.begin(), pt_bins.end(), x);
    if ( xbin == pt_bins.end() || xbin+1 == pt_bins.end() ) return 1;
    auto ybin = std::lower_bound(eta_bins.begin(), eta_bins.end(), y);
    if ( ybin == eta_bins.end() || ybin+1 == eta_bins.end() ) return 1;
    
    const int column = xbin-pt_bins.begin();
    const int row = ybin-eta_bins.begin();
    
    const int bin = row*width+column;
    const double value = values.at(bin);
    const double error = errors.at(bin);
    
    return std::max(0.0, value+shift*error);
  }
  return 1;
};

//For Electrons
const std::vector<double> ElecScaleFactorEvaluator::pt_bins = {
  10.000000,  20.000000,  25.000000,  35.000000,  50.000000,  90.000000,  150.000000,  500.000000,
};

const std::vector<double> ElecScaleFactorEvaluator::eta_bins = {
  -2.500000,  -2.450000,  -2.400000,  -2.300000,  -2.200000,  -2.000000,  -1.800000,  -1.630000,
  -1.566000,  -1.444000,  -1.200000,  -1.000000,  -0.800000,  -0.600000,  -0.400000,  -0.200000,
  0.000000,  0.200000,  0.400000,  0.600000,  0.800000,  1.000000,  1.200000,  1.444000,  1.566000,  
  1.630000,  1.800000,  2.000000,  2.200000,  2.300000,  2.400000,  2.450000,  2.500000,
};

const std::vector<double> ElecScaleFactorEvaluator::values = {
  0.820574,  0.914201,  1.204555,  1.259523,  1.284585,  1.371290,  1.371290,  0.820574,  0.914201,
  1.018219,  1.064683,  1.085869,  1.159161,  1.159161,  0.820574,  0.914201,  0.936713,  0.979459,
  0.998948,  1.066373,  1.066373,  0.820574,  0.914201,  0.926672,  0.968959,  0.988239,  1.054942,
  1.054942,  0.820574,  0.914201,  0.920854,  0.962875,  0.982035,  1.048319,  1.048319,  0.822650,
  0.943662,  0.938773,  0.974295,  0.986769,  1.012784,  1.012784,  0.822650,  0.943662,  0.938742,
  0.974263,  0.986736,  1.012751,  1.012751,  0.822650,  0.943662,  0.935765,  0.971174,  0.983608,
  1.009540,  1.009540,  1.027119,  1.000000,  0.963129,  0.949311,  0.951415,  1.046549,  1.046549,
  0.959108,  0.958556,  0.948684,  0.959779,  0.962117,  0.983132,  0.983132,  0.959108,  0.958556,
  0.944806,  0.955857,  0.958185,  0.979114,  0.979114,  0.959108,  0.958556,  0.940914,  0.951918,
  0.954237,  0.975080,  0.975080,  0.922027,  0.946164,  0.928750,  0.939116,  0.940787,  0.967448,
  0.967448,  0.922027,  0.946164,  0.931667,  0.942066,  0.943742,  0.970487,  0.970487,  0.922027,
  0.946164,  0.928768,  0.939135,  0.940805,  0.967467,  0.967467,  0.922027,  0.946164,  0.927650,
  0.938005,  0.939673,  0.966303,  0.966303,  0.939571,  0.971467,  0.956460,  0.964745,  0.964437,
  1.002311,  1.002311,  0.939571,  0.971467,  0.960552,  0.968872,  0.968563,  1.006599,  1.006599,
  0.939571,  0.971467,  0.959560,  0.967872,  0.967562,  1.005560,  1.005560,  0.939571,  0.971467,
  0.959560,  0.967872,  0.967562,  1.005560,  1.005560,  0.966480,  0.972973,  0.961047,  0.964638,
  0.969985,  1.005581,  1.005581,  0.966480,  0.972973,  0.961047,  0.964638,  0.969985,  1.005581,
  1.005581,  0.966480,  0.972973,  0.960986,  0.964576,  0.969923,  1.005517,  1.005517,  0.993399,
  0.966270,  0.934961,  0.938570,  0.954434,  0.982697,  0.982697,  0.853659,  0.930496,  0.920844,
  0.963333,  0.974789,  0.987400,  0.987400,  0.853659,  0.930496,  0.923760,  0.966383,  0.977876,
  0.990527,  0.990527,  0.853659,  0.930496,  0.922830,  0.965410,  0.976891,  0.989530,  0.989530,
  0.815385,  0.891273,  0.889437,  0.940986,  0.965106,  1.013131,  1.013131,  0.815385,  0.891273,
  0.892197,  0.943906,  0.968101,  1.016275,  1.016275,  0.815385,  0.891273,  0.881921,  0.933034,
  0.956950,  1.004569,  1.004569,  0.815385,  0.891273,  0.864997,  0.915130,  0.938587,  0.985292,
  0.985292,  0.815385,  0.891273,  0.808385,  0.855236,  0.877158,  0.920806,  0.920806,
};

const std::vector<double> ElecScaleFactorEvaluator::errors = {
  0.023166,  0.008260,  0.020022,  0.019741,  0.019096,  0.034832,  0.034995,  0.023166,  0.008260,
  0.013810,  0.013398,  0.012429,  0.031672,  0.031851,  0.023166,  0.008260,  0.011610,  0.011117,
  0.009927,  0.030776,  0.030960,  0.023166,  0.008260,  0.010914,  0.010388,  0.009103,  0.030520,
  0.030706,  0.023166,  0.008260,  0.009268,  0.008643,  0.007047,  0.029971,  0.030160,  0.018241,
  0.012760,  0.014317,  0.008752,  0.008235,  0.012968,  0.013400,  0.018241,  0.012760,  0.013766,
  0.007819,  0.007235,  0.012357,  0.012810,  0.018241,  0.012760,  0.013900,  0.008052,  0.007486,
  0.012505,  0.012953,  0.068255,  0.179759,  0.181634,  0.028947,  0.029625,  0.043963,  0.044093,
  0.050900,  0.011640,  0.012184,  0.005716,  0.012984,  0.009120,  0.009725,  0.050900,  0.011640,
  0.012694,  0.006735,  0.013464,  0.009791,  0.010357,  0.050900,  0.011640,  0.012102,  0.005540,
  0.012907,  0.009011,  0.009622,  0.026763,  0.012595,  0.013023,  0.005219,  0.011578,  0.010897,
  0.011408,  0.026763,  0.012595,  0.014007,  0.007337,  0.012674,  0.012056,  0.012520,  0.026763,
  0.012595,  0.014109,  0.007529,  0.012787,  0.012174,  0.012633,  0.026763,  0.012595,  0.013665,
  0.006662,  0.012296,  0.011657,  0.012136,  0.026763,  0.012595,  0.013665,  0.006662,  0.012296,
  0.011657,  0.012136,  0.026763,  0.012595,  0.014109,  0.007529,  0.012787,  0.012174,  0.012633,
  0.026763,  0.012595,  0.014007,  0.007337,  0.012674,  0.012056,  0.012520,  0.026763,  0.012595,
  0.013023,  0.005219,  0.011578,  0.010897,  0.011408,  0.050900,  0.011640,  0.012102,  0.005540,
  0.012907,  0.009011,  0.009622,  0.050900,  0.011640,  0.012694,  0.006735,  0.013464,  0.009791,
  0.010357,  0.050900,  0.011640,  0.012184,  0.005716,  0.012984,  0.009120,  0.009725,  0.068255,
  0.179759,  0.181634,  0.028947,  0.029661,  0.043851,  0.043980,  0.018241,  0.012760,  0.013900,
  0.008052,  0.007486,  0.012505,  0.012953,  0.018241,  0.012760,  0.013766,  0.007819,  0.007235,
  0.012357,  0.012810,  0.018241,  0.012760,  0.014317,  0.008752,  0.008235,  0.012968,  0.013400,
  0.023166,  0.008260,  0.009268,  0.008643,  0.006915,  0.029971,  0.030160,  0.023166,  0.008260,
  0.010914,  0.010388,  0.009002,  0.030520,  0.030706,  0.023166,  0.008260,  0.011610,  0.011117,
  0.009834,  0.030776,  0.030960,  0.023166,  0.008260,  0.013810,  0.013398,  0.012354,  0.031672,
  0.031851,  0.023166,  0.008260,  0.020022,  0.019741,  0.019048,  0.034832,  0.034995,
};
