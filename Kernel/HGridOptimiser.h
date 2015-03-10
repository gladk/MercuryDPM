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

#ifndef HGRIDOPTIMIZER_H
#define HGRIDOPTIMIZER_H

#include <vector>
#include "MercuryBase.h"

/*!
 * \class HGridOptimiser
 * \brief
 */
class HGridOptimiser
{
public:
    /*!
     * \brief 
     */
    void initialise(const MercuryBase& problem, unsigned int numberOfCells, int verbosity);

    /*!
     * \brief 
     */
    void initialisePolyFunc(double omega, std::vector<double>& coeff, unsigned int numberOfCells, int verbosity);

    /*!
     * \brief 
     */
    unsigned int radius2Cell(double r);

    /*!
     * \brief 
     */
    unsigned int radius2IntCell(double r);

    /*!
     * \brief 
     */
    double intCell2Min(unsigned int i);

    /*!
     * \brief 
     */
    double intCell2Max(unsigned int i);

    /*!
     * \brief 
     */
    double cell2Min(unsigned int i);

    /*!
     * \brief 
     */
    double cell2Max(unsigned int i);

    /*!
     * \brief 
     */
    double pdfIntCell(double start, double end, unsigned int i, int p);

    /*!
     * \brief 
     */
    double pdfInt(double start, double end, int power);

    /*!
     * \brief 
     */
    double diffPdfInt(double x, int power);

    /*!
     * \brief 
     */
    double expectedCellsIntegralCellTeller(double start, double end, unsigned int i, int p, double h);

    /*!
     * \brief 
     */
    double diffHExpectedCellsIntegralCellTeller(double start, double end, unsigned int i, int p, double h);

    /*!
     * \brief 
     */
    double expectedCellsIntegralCellNoemer(double start, double end, unsigned int i);

    /*!
     * \brief 
     */
    double expectedCellsIntegral(double start, double end, int p, double h);

    /*!
     * \brief 
     */
    double diffStartExpectedCellsIntegral(double start, double end, int p, double h);

    /*!
     * \brief 
     */
    double diffEndExpectedCellsIntegral(double start, double end, int p, double h);

    /*!
     * \brief 
     */
    double diffHExpectedCellsIntegral(double start, double end, int p, double h);

    /*!
     * \brief 
     */
    double calculateWork(std::vector<double>& hGridCellSizes, HGridMethod method, int verbosity);

    /*!
     * \brief 
     */
    void calculateDiffWork(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, HGridMethod method, int verbosity);

    /*!
     * \brief 
     */
    void calcDfDx(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, HGridMethod method, int verbosity);

    /*!
     * \brief 
     */
    double checkLimit(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, int verbosity);

    /*!
     * \brief 
     */
    void applyStep(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, double stepsize, int verbosity);

    /*!
     * \brief 
     */
    double goldenSectionSearch(std::vector<double>& startHGridCellSizes, std::vector<double>& searchDirection, double min, double cur, double max, HGridMethod method, int verbosity);

    /*!
     * \brief 
     */
    void getOptimalDistribution(std::vector<double>& hGridCellSizes, unsigned int numberOfLevels, HGridMethod method, int verbosity);

    /*!
     * \brief
     */
    void histNumberParticlesPerCell(std::vector<double>& hGridCellSizes);

private:
    unsigned int NCells;///
    double rMin;///
    double rMax;///
    double length;///
    double cellCheckOverContactCheckRatio_;///
    int dimension;///
    std::vector<double> cellN;///
    std::vector<double> intCellN;///
};

#endif

