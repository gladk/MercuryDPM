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
 * \brief 
 * \todo Find out what this class does and document it.
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
     * \brief Assigns a BaseParticle of given radius to a certain cell.
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
     * \brief Computes the left bound of the cell with given ordinal number.
     */
    double cell2Min(unsigned int i);

    /*!
     * \brief Computes the right bound of the cell with given ordinal number.
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
    double expectedCellsIntegralCellNumerator(double start, double end, unsigned int i, int p, double h);

    /*!
     * \brief 
     */
    double diffHExpectedCellsIntegralCellNumerator(double start, double end, unsigned int i, int p, double h);

    /*!
     * \brief 
     */
    double expectedCellsIntegralCellDenominator(double start, double end, unsigned int i);

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
     * \brief The amount of work that has to be done to run a simulation using the HGrid, in steps.
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
    /*!
     * \brief Number of cells, usually called levels in the HGrid.
     */
    unsigned int numCells_;
    /*!
     * \brief Radius of the smallest particle, "rounded" to the largest double that is 
     *        smaller than the radius of each particle.
     */
    double rMin_;
    /*!
     * \brief Radius of the largest particle, "rounded" to the smallest double that
     *        is larger than the radius of each particle. 
     */
    double rMax_;///
    /*!
     * \brief The weighted length of the domain. 
     * \details The weighted length is computed by multiplying the lengths of all
     *          directions with each other, and then taking the appropriate type
     *          of root so that the unit is the same as that of a length (square 
     *          root for 2D, cube root for 3D).
     */
    double length_;
    /*!
     * \brief The ratio of the time required for a single geometric contact
     *        detection over the time required to retrieve information from a cell.
     *        This seems to be only used for checking the effort required by the HGrid, not
     *        to compute the cell sizes.
     */
    double cellCheckOverContactCheckRatio_;
    /*!
     * \brief The dimension of the system, usually 3, sometimes 2 or 1.
     */
    unsigned int dimension_;
    
    //Something with cells. Still have to find out which property the double represents.
    //Could be the number of particles in that cell (see initialise), but then 
    //you'd expect an unsigned int and not a double.
    std::vector<double> cellN_;///
    std::vector<double> intCellN;///
};

#endif

