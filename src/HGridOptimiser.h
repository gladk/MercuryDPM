// This file is part of MercuryDPM.
// 
// MercuryDPM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// MercuryDPM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with MercuryDPM.  If not, see <http://www.gnu.org/licenses/>.
// 
// Copyright 2013 The Mercury Developers Team
// For the list of developers, see <http://www.MercuryDPM.org/Team>

#ifndef HGRIDOptimiser_H
#define HGRIDOptimiser_H

#include <vector>
#include "HGRID_base.h"

class HGridOptimiser
{
  public:
    void Initialise(HGRID_base& problem, int verbosity);
    int radius2Cell(double r);
    double cell2Min(int i);
    double cell2Max(int i);
    double pdfInt(double start, double end, int power);
    double diffPdfInt(double x, int power);
    double expectedCellsIntegral(double start, double end, int p, double h);
    double diffStartExpectedCellsIntegral(double start, double end, int p, double h);
    double diffEndExpectedCellsIntegral(double start, double end, int p, double h);
    double diffHExpectedCellsIntegral(double start, double end, int p, double h);
    double calculateWork(std::vector<double>& hGridCellSizes, HGridMethod method, int verbosity);
    void calculateDiffWork(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, HGridMethod method, int verbosity);
    void calcDfDx(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, HGridMethod method, int verbosity);
    double checkLimit(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, int verbosity);
    void applyStep(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, double stepsize, int verbosity);
    double goldenSectionSearch(std::vector<double>& startHGridCellSizes, std::vector<double>& searchDirection, double min, double cur, double max, HGridMethod method, int verbosity);
    void getOptimalDistribution(std::vector<double>& hGridCellSizes, int numberOfLevels, HGridMethod method, int verbosity);
    

  private:
    int NCells, NParticles;
    double rMin;
    double rMax;
    double length;
    double k;
    int dimension;
    std::vector<int> CellN;
};

#endif

