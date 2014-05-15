//Copyright (c) 2013-2014, The MercuryDPM Developers Team. All rights reserved.
//For the list of developers, see <http://www.MercuryDPM.org/Team>.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name MercuryDPM nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE MERCURYDPM DEVELOPERS TEAM BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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

