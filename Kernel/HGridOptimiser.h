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

