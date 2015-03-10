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

#include <iostream>
#include <iomanip>  
#include <limits> 
#include <math.h> 
#include "HGridOptimiser.h"
#include "Particles/BaseParticle.h"

void HGridOptimiser::initialise(const MercuryBase& problem, unsigned int numberOfCells, int verbosity)
{
    NCells = numberOfCells;
    cellCheckOverContactCheckRatio_ = 0.2;
    cellN.resize(NCells);
    for (std::vector<double>::iterator it = cellN.begin(); it != cellN.end(); ++it)
    {
        *it = 0;
    }
    
    rMin = nextafter(problem.particleHandler.getSmallestParticle()->getInteractionRadius(), 0.0);
    rMax = nextafter(nextafter(problem.particleHandler.getLargestParticle()->getInteractionRadius(), std::numeric_limits<double>::max()), std::numeric_limits<double>::max());
    for (std::vector<BaseParticle*>::const_iterator it = problem.particleHandler.begin(); it != problem.particleHandler.end(); ++it)
    {
        cellN[radius2Cell((*it)->getInteractionRadius())]++;
    }
    unsigned int NParticles = problem.particleHandler.getNumberOfObjects();
    
    intCellN.push_back(1.5 * cellN[0] - 0.5 * cellN[1]);
    for (std::vector<double>::iterator it = cellN.begin(); it != cellN.end(); ++it)
    {
        intCellN.push_back(*it);
    }
    intCellN.push_back(1.5 * cellN[NCells - 1] - 0.5 * cellN[NCells - 2]);
    
    if (verbosity > 0)
    {
        std::cout << "rMin=" << rMin << " rMax=" << rMax << " NParticles=" << NParticles << std::endl;
        for (unsigned int i = 0; i < cellN.size(); i++)
        {
            std::cout << "From" << std::setw(8) << cell2Min(i) << " to" << std::setw(8) << cell2Max(i) << " number=" << std::setw(5) << cellN[i] << std::endl;
        }
        std::cout << "Domain size [" << problem.getXMin() << "," << problem.getXMax() << "]x[" << problem.getYMin() << "," << problem.getYMax() << "]x[" << problem.getZMin() << "," << problem.getZMax() << "]" << std::endl;
        
        for (unsigned int i = 0; i < intCellN.size() - 1; i++)
        {
            std::cout << "From" << std::setw(8) << intCell2Min(i) << " to" << std::setw(8) << intCell2Max(i) << " linear from " << std::setw(8) << intCellN[i] << " to " << std::setw(8) << intCellN[i + 1] << std::endl;
        }
        /*std::cout<<"["<<cellN[0];
         for (unsigned int i=1;i<cellN.size();i++)
         {
         std::cout<<","<<cellN[i];
         }       
         std::cout<<"]"<<std::endl;*/
    }
    
    dimension = problem.getSystemDimensions();
    switch (dimension)
    {
        case 1:
            length = pow((problem.getXMax() - problem.getXMin()) / NParticles, 1);
            break;
        case 2:
            length = pow((problem.getXMax() - problem.getXMin()) * (problem.getYMax() - problem.getYMin()) / NParticles, 1.0 / 2.0);
            break;
        case 3:
            length = pow((problem.getXMax() - problem.getXMin()) * (problem.getYMax() - problem.getYMin()) * (problem.getZMax() - problem.getZMin()) / NParticles, 1.0 / 3.0);
            break;
    }
}

void HGridOptimiser::initialisePolyFunc(double omega, std::vector<double>& coeff, unsigned int numberOfCells, int verbosity)
{
    NCells = numberOfCells;
    cellCheckOverContactCheckRatio_ = 0.2;
    cellN.resize(NCells);
    for (std::vector<double>::iterator it = cellN.begin(); it != cellN.end(); ++it)
    {
        *it = 0;
    }
    
    rMin = nextafter(1, 0.0);
    rMax = nextafter(omega, std::numeric_limits<double>::max());
    for (unsigned int i = 0; i < cellN.size(); i++)
    {
        double start = cell2Min(i);
        double end = cell2Max(i);
        for (unsigned int j = 0; j < coeff.size(); j++)
        {
            cellN[i] += coeff[j] / (1.0 + j) * (std::pow(end, j + 1) - std::pow(start, j + 1));
        }
    }
    
    intCellN.push_back(1.5 * cellN[0] - 0.5 * cellN[1]);
    for (std::vector<double>::iterator it = cellN.begin(); it != cellN.end(); ++it)
    {
        intCellN.push_back(*it);
    }
    intCellN.push_back(1.5 * cellN[NCells - 1] - 0.5 * cellN[NCells - 2]);
    
    dimension = 1;
    length = 1.0;
    
    if (verbosity > 0)
    {
        std::cout << "rMin=" << rMin << " rMax=" << rMax << std::endl;
        for (unsigned int i = 0; i < cellN.size(); i++)
        {
            std::cout << "From" << std::setw(8) << cell2Min(i) << " to" << std::setw(8) << cell2Max(i) << " number=" << std::setw(5) << cellN[i] << std::endl;
        }
        
        for (unsigned int i = 0; i < intCellN.size() - 1; i++)
        {
            std::cout << "From" << std::setw(8) << intCell2Min(i) << " to" << std::setw(8) << intCell2Max(i) << " linear from " << std::setw(8) << intCellN[i] << " to " << std::setw(8) << intCellN[i + 1] << std::endl;
        }
    }
}

unsigned int HGridOptimiser::radius2Cell(double r)
{
    if (r < rMin || r >= rMax)
        throw;
    unsigned int y = static_cast<unsigned int>(floor(NCells * (r - rMin) / (rMax - rMin)));
    return y;
}

unsigned int HGridOptimiser::radius2IntCell(double r)
{
    if (r < rMin || r >= rMax)
        throw;
    unsigned int y = static_cast<unsigned int>(floor(NCells * (r - rMin) / (rMax - rMin) + 0.5));
    return y;
}

double HGridOptimiser::intCell2Min(unsigned int i)
{
    if (i == 0)
        return rMin;
    else
        return rMin + (rMax - rMin) * (i - 0.5) / NCells;
}

double HGridOptimiser::intCell2Max(unsigned int i)
{
    if (i == NCells)
        return rMax;
    else
        return rMin + (rMax - rMin) * (i + 0.5) / NCells;
}

double HGridOptimiser::cell2Min(unsigned int i)
{
    return rMin + (rMax - rMin) * i / NCells;
}

double HGridOptimiser::cell2Max(unsigned int i)
{
    return rMin + (rMax - rMin) * (i + 1) / NCells;
}

double HGridOptimiser::pdfIntCell(double start, double end, unsigned int i, int p)
{
    double a = (intCellN[i + 1] - intCellN[i]) / (intCell2Max(i) - intCell2Min(i));
    double b = (intCellN[i] * intCell2Max(i) - intCellN[i + 1] * intCell2Min(i)) / (intCell2Max(i) - intCell2Min(i));
    return (a / (p + 2) * (std::pow(end, p + 2) - std::pow(start, p + 2)) + b / (p + 1) * (std::pow(end, p + 1) - std::pow(start, p + 1)));
}

///This function calculates:
///int(f(r)*r^power*dr,r=start..end)/int(f(r)*dr,r=0..omega)
///with r=a*r+b
double HGridOptimiser::pdfInt(double start, double end, int p)
{
    //std::cout<<"pdfInit("<<start<<","<<end<<","<<p<<");"<<std::endl;
    unsigned int startCell = radius2IntCell(start);
    unsigned int endCell = radius2IntCell(end);
    
    double teller = 0;
    if (startCell == endCell)
    {
        teller = pdfIntCell(start, end, startCell, p);
    }
    else if (endCell < startCell)
    {
        teller = 0;
    }
    else
    {
        teller = pdfIntCell(start, intCell2Max(startCell), startCell, p);
        //std::cout<<"Teller+="<<pdfIntCell(start,intCell2Max(startCell),startCell,p)<<std::endl;
        for (unsigned int i = startCell + 1; i < endCell; i++)
        {
            teller += pdfIntCell(intCell2Min(i), intCell2Max(i), i, p);
            //std::cout<<"Teller+="<<pdfIntCell(intCell2Min(i),intCell2Max(i),i,p)<<std::endl;
        }
        teller += pdfIntCell(intCell2Min(endCell), end, endCell, p);
        //std::cout<<"Teller+="<<pdfIntCell(intCell2Min(endCell),end,endCell,p)<<std::endl;
    }
    double noemer = 0;
    for (unsigned int i = 0; i <= NCells; i++)
    {
        noemer += pdfIntCell(intCell2Min(i), intCell2Max(i), i, 0);
        //std::cout<<"Noemer+="<<pdfIntCell(intCell2Min(i),intCell2Max(i),i,0)<<std::endl;
    }
    return teller / noemer;
}

///diff(int(f(r)*r^power*dr,r=s..e)/int(f(r)*dr,r=0..omega),e)=f(e)*e^power/int(f(r)*dr,r=0..omega)
double HGridOptimiser::diffPdfInt(double x, int p)
{
    unsigned int xCell = radius2IntCell(x);
    double noemer = 0;
    for (unsigned int i = 0; i <= NCells; i++)
    {
        noemer += pdfIntCell(intCell2Min(i), intCell2Max(i), i, 0);
    }
    double a = (intCellN[xCell + 1] - intCellN[xCell]) / (intCell2Max(xCell) - intCell2Min(xCell));
    double b = (intCellN[xCell] * intCell2Max(xCell) - intCellN[xCell + 1] * intCell2Min(xCell)) / (intCell2Max(xCell) - intCell2Min(xCell));
    return (a * std::pow(x, p + 1) + b * std::pow(x, p)) / noemer;
}

double HGridOptimiser::expectedCellsIntegralCellTeller(double start, double end, unsigned int i, int p, double h)
{
    double a = (intCellN[i + 1] - intCellN[i]) / (intCell2Max(i) - intCell2Min(i));
    double b = (intCellN[i] * intCell2Max(i) - intCellN[i + 1] * intCell2Min(i)) / (intCell2Max(i) - intCell2Min(i));
    double r;
    r = start;
    double min = std::pow(2.0 * (r + h) / h, p) * (a * p * r - a * h + a * r + b * p + 2 * b) * (r + h) / ((p + 1) * (p + 2));
    r = end;
    double plus = std::pow(2.0 * (r + h) / h, p) * (a * p * r - a * h + a * r + b * p + 2 * b) * (r + h) / ((p + 1) * (p + 2));
    return plus - min;
}

double HGridOptimiser::diffHExpectedCellsIntegralCellTeller(double start, double end, unsigned int i, int p, double h)
{
    double a = (intCellN[i + 1] - intCellN[i]) / (intCell2Max(i) - intCell2Min(i));
    double b = (intCellN[i] * intCell2Max(i) - intCellN[i + 1] * intCell2Min(i)) / (intCell2Max(i) - intCell2Min(i));
    double r;
    r = start;
    //double min =std::pow(2.0*r/h+2.0,p)*(a*p*r-a*h+a*r+b*p+2*b)*(r+h)/((p+1)*(p+2));
    double min = (-2.0 * r / h / h * p * std::pow(2.0 * r / h + 2.0, p - 1) * (a * p * r - a * h + a * r + b * p + 2 * b) * (r + h) +
            -std::pow(2.0 * r / h + 2.0, p) * a * (r + h) +
            std::pow(2.0 * r / h + 2.0, p) * (a * p * r - a * h + a * r + b * p + 2 * b)) / ((p + 1) * (p + 2));
    r = end;
    //double plus=std::pow(2.0*r/h+2.0,p)*(a*p*r-a*h+a*r+b*p+2*b)*(r+h)/((p+1)*(p+2));
    double plus = (-2.0 * r / h / h * p * std::pow(2.0 * r / h + 2.0, p - 1) * (a * p * r - a * h + a * r + b * p + 2 * b) * (r + h) +
            -std::pow(2.0 * r / h + 2.0, p) * a * (r + h) +
            std::pow(2.0 * r / h + 2.0, p) * (a * p * r - a * h + a * r + b * p + 2 * b)) / ((p + 1) * (p + 2));
    return plus - min;
}

double HGridOptimiser::expectedCellsIntegralCellNoemer(double start, double end, unsigned int i)
{
    double a = (intCellN[i + 1] - intCellN[i]) / (intCell2Max(i) - intCell2Min(i));
    double b = (intCellN[i] * intCell2Max(i) - intCellN[i + 1] * intCell2Min(i)) / (intCell2Max(i) - intCell2Min(i));
    return (a / 2.0 * (pow(end, 2) - pow(start, 2)) + b * (pow(end, 1) - pow(start, 1)));
}

///This function calculates:
///int((2*r/h+2)^d f(r) dr,r=s..e)/int(f(r) dr,r=s..e)+
///Used to calculated the expected number of cells to check at the level with maximum size h for particle radius between start and end
double HGridOptimiser::expectedCellsIntegral(double start, double end, int p, double h)
{
    unsigned int startCell = radius2IntCell(start);
    unsigned int endCell = radius2IntCell(end);
    
    double teller = 0;
    double noemer = 0;
    if (startCell == endCell)
    {
        teller = expectedCellsIntegralCellTeller(start, end, startCell, p, h);
        noemer = expectedCellsIntegralCellNoemer(start, end, startCell);
    }
    else
    {
        teller = expectedCellsIntegralCellTeller(start, intCell2Max(startCell), startCell, p, h);
        noemer = expectedCellsIntegralCellNoemer(start, intCell2Max(startCell), startCell);
        for (unsigned int i = startCell + 1; i < endCell; i++)
        {
            teller += expectedCellsIntegralCellTeller(intCell2Min(i), intCell2Max(i), i, p, h);
            noemer += expectedCellsIntegralCellNoemer(intCell2Min(i), intCell2Max(i), i);
        }
        teller += expectedCellsIntegralCellTeller(intCell2Min(endCell), end, endCell, p, h);
        noemer += expectedCellsIntegralCellNoemer(intCell2Min(endCell), end, endCell);
        
    }
//    std::cout<<"New: teller="<<teller<<" noemer="<<noemer<<std::endl;
    return teller / noemer;
}

double HGridOptimiser::diffStartExpectedCellsIntegral(double start, double end, int p, double h)
{
    unsigned int startCell = radius2IntCell(start);
    unsigned int endCell = radius2IntCell(end);
    
    double teller = 0;
    double noemer = 0;
    
    double a = (intCellN[startCell + 1] - intCellN[startCell]) / (intCell2Max(startCell) - intCell2Min(startCell));
    double b = (intCellN[startCell] * intCell2Max(startCell) - intCellN[startCell + 1] * intCell2Min(startCell)) / (intCell2Max(startCell) - intCell2Min(startCell));
    
    double diffTeller = -(2.0 / h * p * std::pow(2.0 * (start + h) / h, p - 1) * (a * p * start - a * h + a * start + b * p + 2 * b) * (start + h) / ((p + 1) * (p + 2)) +
            std::pow(2.0 * (start + h) / h, p) * (a * p + a) * (start + h) / ((p + 1) * (p + 2)) +
            std::pow(2.0 * (start + h) / h, p) * (a * p * start - a * h + a * start + b * p + 2 * b) / ((p + 1) * (p + 2)));
    double diffNoemer = -(a * start + b);
    if (startCell == endCell)
    {
        teller = expectedCellsIntegralCellTeller(start, end, startCell, p, h);
        noemer = expectedCellsIntegralCellNoemer(start, end, startCell);
    }
    else
    {
        teller = expectedCellsIntegralCellTeller(start, intCell2Max(startCell), startCell, p, h);
        noemer = expectedCellsIntegralCellNoemer(start, intCell2Max(startCell), startCell);
        for (unsigned int i = startCell + 1; i < endCell; i++)
        {
            teller += expectedCellsIntegralCellTeller(intCell2Min(i), intCell2Max(i), i, p, h);
            noemer += expectedCellsIntegralCellNoemer(intCell2Min(i), intCell2Max(i), i);
        }
        teller += expectedCellsIntegralCellTeller(intCell2Min(endCell), end, endCell, p, h);
        noemer += expectedCellsIntegralCellNoemer(intCell2Min(endCell), end, endCell);
        
    }
    //std::cout<<"new: teller="<<teller<<" noemer="<<noemer<<" diffTeller="<<diffTeller<<" diffNoemer="<<diffNoemer<<std::endl;
    return (diffTeller * noemer - teller * diffNoemer) / (noemer * noemer);
}

double HGridOptimiser::diffEndExpectedCellsIntegral(double start, double end, int p, double h)
{
    unsigned int startCell = radius2IntCell(start);
    unsigned int endCell = radius2IntCell(end);
    
    double teller = 0;
    double noemer = 0;
    
    double a = (intCellN[endCell + 1] - intCellN[endCell]) / (intCell2Max(endCell) - intCell2Min(endCell));
    double b = (intCellN[endCell] * intCell2Max(endCell) - intCellN[endCell + 1] * intCell2Min(endCell)) / (intCell2Max(endCell) - intCell2Min(endCell));
    
    double diffTeller = (2.0 / h * p * std::pow(2.0 * (end + h) / h, p - 1) * (a * p * end - a * h + a * end + b * p + 2 * b) * (end + h) / ((p + 1) * (p + 2)) +
            std::pow(2.0 * (end + h) / h, p) * (a * p + a) * (end + h) / ((p + 1) * (p + 2)) +
            std::pow(2.0 * (end + h) / h, p) * (a * p * end - a * h + a * end + b * p + 2 * b) / ((p + 1) * (p + 2)));
    double diffNoemer = (a * end + b);
    if (startCell == endCell)
    {
        teller = expectedCellsIntegralCellTeller(start, end, startCell, p, h);
        noemer = expectedCellsIntegralCellNoemer(start, end, startCell);
    }
    else
    {
        teller = expectedCellsIntegralCellTeller(start, intCell2Max(startCell), startCell, p, h);
        noemer = expectedCellsIntegralCellNoemer(start, intCell2Max(startCell), startCell);
        for (unsigned int i = startCell + 1; i < endCell; i++)
        {
            teller += expectedCellsIntegralCellTeller(intCell2Min(i), intCell2Max(i), i, p, h);
            noemer += expectedCellsIntegralCellNoemer(intCell2Min(i), intCell2Max(i), i);
        }
        teller += expectedCellsIntegralCellTeller(intCell2Min(endCell), end, endCell, p, h);
        noemer += expectedCellsIntegralCellNoemer(intCell2Min(endCell), end, endCell);
        
    }
    //std::cout<<"new: teller="<<teller<<" noemer="<<noemer<<" diffTeller="<<diffTeller<<" diffNoemer="<<diffNoemer<<std::endl;
    return (diffTeller * noemer - teller * diffNoemer) / (noemer * noemer);
}

double HGridOptimiser::diffHExpectedCellsIntegral(double start, double end, int p, double h)
{
    unsigned int startCell = radius2IntCell(start);
    unsigned int endCell = radius2IntCell(end);
    
    double noemer = 0;
    double diffTeller = 0;
    
    if (startCell == endCell)
    {
        diffTeller = diffHExpectedCellsIntegralCellTeller(start, end, startCell, p, h);
        noemer = expectedCellsIntegralCellNoemer(start, end, startCell);
    }
    else
    {
        diffTeller = diffHExpectedCellsIntegralCellTeller(start, intCell2Max(startCell), startCell, p, h);
        noemer = expectedCellsIntegralCellNoemer(start, intCell2Max(startCell), startCell);
        for (unsigned int i = startCell + 1; i < endCell; i++)
        {
            diffTeller += diffHExpectedCellsIntegralCellTeller(intCell2Min(i), intCell2Max(i), i, p, h);
            noemer += expectedCellsIntegralCellNoemer(intCell2Min(i), intCell2Max(i), i);
        }
        diffTeller += diffHExpectedCellsIntegralCellTeller(intCell2Min(endCell), end, endCell, p, h);
        noemer += expectedCellsIntegralCellNoemer(intCell2Min(endCell), end, endCell);
    }
    return diffTeller / noemer;
}

void HGridOptimiser::calculateDiffWork(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, HGridMethod method, int verbosity)
{
    unsigned int NLevels = hGridCellSizes.size() - 1;
    unsigned int NParams = hGridCellSizes.size();
    
    if (verbosity > 0)
    {
        std::cout << "Length scale=" << length << std::endl;
    }
    if (verbosity > 0)
    {
        std::cout << "Level sizes:";
        for (unsigned int i = 0; i < NParams; i++)
        {
            std::cout << " " << hGridCellSizes[i];
        }
        std::cout << std::endl;
    }
    
    std::vector<double> particlesPerLevel;
    for (unsigned int i = 0; i < NLevels; i++)
    {
        particlesPerLevel.push_back(pdfInt(0.5 * hGridCellSizes[i], 0.5 * hGridCellSizes[i + 1], 0));
        if (particlesPerLevel.back() < 0)
        {
            particlesPerLevel.back() = 0;
        }
    }
    if (verbosity > 0)
    {
        std::cout << "Particles per level: ";
        for (unsigned int i = 0; i < NLevels; i++)
        {
            std::cout << " " << particlesPerLevel[i];
        }
        std::cout << std::endl;
    }
    
    //How changes particlesPerLeve[i] when hGridCellSize[j] is changed
    std::vector<std::vector<double> > diffParticlesPerLevel;
    diffParticlesPerLevel.resize(NLevels);
    for (unsigned int i = 0; i < NLevels; i++)
    {
        for (unsigned int j = 0; j < NParams; j++)
        {
            diffParticlesPerLevel[i].push_back(0.0);
            if (j == i)
            {
                diffParticlesPerLevel[i][j] += -0.5 * diffPdfInt(0.5 * hGridCellSizes[i], 0);
            }
            if (j == i + 1)
            {
                diffParticlesPerLevel[i][j] += 0.5 * diffPdfInt(0.5 * hGridCellSizes[i + 1], 0);
            }
        }
    }
    if (verbosity > 0)
    {
        std::cout << "Diff Particles per level: " << std::endl;
        for (unsigned int i = 0; i < NLevels; i++)
        {
            for (unsigned int j = 0; j < NParams; j++)
            {
                std::cout << " " << std::setw(12) << diffParticlesPerLevel[i][j];
            }
            std::cout << std::endl;
        }
    }
    
    std::vector<double> cellsPerLevel;
    for (unsigned int i = 0; i < NLevels; i++)
    {
        cellsPerLevel.push_back(pow(length / hGridCellSizes[i + 1], dimension));
    }
    if (verbosity > 0)
    {
        std::cout << "Cells per level: ";
        for (unsigned int i = 0; i < NLevels; i++)
        {
            std::cout << " " << cellsPerLevel[i];
        }
        std::cout << std::endl;
    }
    
    //How changes cellsPerLevel[i] when hGridCellSize[j] is changed    
    std::vector<std::vector<double> > diffCellsPerLevel;
    diffCellsPerLevel.resize(hGridCellSizes.size());
    for (unsigned int i = 0; i < NLevels; i++)
    {
        for (unsigned int j = 0; j < NParams; j++)
        {
            if (j == i + 1)
            {
                diffCellsPerLevel[i].push_back(-pow(length / hGridCellSizes[i + 1], dimension) * dimension / hGridCellSizes[i + 1]);
            }
            else
            {
                diffCellsPerLevel[i].push_back(0.0);
            }
        }
    }
    if (verbosity > 0)
    {
        std::cout << "Diff Cells per level: " << std::endl;
        for (unsigned int i = 0; i < NLevels; i++)
        {
            for (unsigned int j = 0; j < NParams; j++)
            {
                std::cout << " " << std::setw(12) << diffCellsPerLevel[i][j];
            }
            std::cout << std::endl;
        }
    }
    
    std::vector<double> particlesPerCell;
    for (unsigned int i = 0; i < NLevels; i++)
    {
        particlesPerCell.push_back(particlesPerLevel[i] / cellsPerLevel[i]);
    }
    if (verbosity > 0)
    {
        std::cout << "Particles per cell: ";
        for (unsigned int i = 0; i < particlesPerCell.size(); i++)
        {
            std::cout << " " << particlesPerCell[i];
        }
        std::cout << std::endl;
    }
    
    //How changes particlesPerCell[i] when hGridCellSize[j] is changed    
    std::vector<std::vector<double> > diffParticlesPerCell;
    diffParticlesPerCell.resize(NLevels);
    for (unsigned int i = 0; i < NLevels; i++)
    {
        for (unsigned int j = 0; j < NParams; j++)
        {
            diffParticlesPerCell[i].push_back((diffParticlesPerLevel[i][j] * cellsPerLevel[i] - particlesPerLevel[i] * diffCellsPerLevel[i][j]) / (cellsPerLevel[i] * cellsPerLevel[i]));
        }
    }
    if (verbosity > 0)
    {
        std::cout << "Diff Particles per Cell: " << std::endl;
        for (unsigned int i = 0; i < NLevels; i++)
        {
            for (unsigned int j = 0; j < NParams; j++)
            {
                std::cout << " " << std::setw(12) << diffParticlesPerCell[i][j];
            }
            std::cout << std::endl;
        }
    }
    
    double contactWork = 0, overheadWork = 0;
    std::vector<double> diffContactWork;
    std::vector<double> diffOverheadWork;
    diffContactWork.resize(NParams);
    diffOverheadWork.resize(NParams);
    for (unsigned int j = 0; j < NParams; j++)
    {
        diffContactWork[j] = 0;
        diffOverheadWork[j] = 0;
    }
    
    for (unsigned int i = 0; i < NLevels; i++)
    {
        contactWork += 0.5 * pow(3, dimension) * particlesPerCell[i] * particlesPerLevel[i];
        overheadWork += 0.5 * (pow(3, dimension) + 1) * particlesPerLevel[i];
        for (unsigned int j = 0; j < NParams; j++)
        {
            diffContactWork[j] += 0.5 * pow(3, dimension) * (diffParticlesPerCell[i][j] * particlesPerLevel[i] + particlesPerCell[i] * diffParticlesPerLevel[i][j]);
            diffOverheadWork[j] += 0.5 * (pow(3, dimension) + 1) * diffParticlesPerLevel[i][j];
        }
        
        unsigned int startJ, endJ;
        switch (method)
        {
            case BOTTOMUP:
                {
                startJ = i + 1;
                endJ = NLevels;
                break;
            }
            case TOPDOWN:
                {
                startJ = 0;
                endJ = i;
                break;
            }
        }
        for (unsigned int j = startJ; j < endJ; j++)
        {
            double numberOfCellToCheck;
            numberOfCellToCheck = expectedCellsIntegral(0.5 * hGridCellSizes[i], 0.5 * hGridCellSizes[i + 1], dimension, hGridCellSizes[j + 1]);
            
            std::vector<double> diffNumberOfCellToCheck;
            for (unsigned int k = 0; k < NParams; k++)
            {
                diffNumberOfCellToCheck.push_back(0.0);
                if (k == i)
                {
                    diffNumberOfCellToCheck[k] += 0.5 * diffStartExpectedCellsIntegral(0.5 * hGridCellSizes[i], 0.5 * hGridCellSizes[i + 1], dimension, hGridCellSizes[j + 1]);
                }
                if (k == i + 1)
                {
                    diffNumberOfCellToCheck[k] += 0.5 * diffEndExpectedCellsIntegral(0.5 * hGridCellSizes[i], 0.5 * hGridCellSizes[i + 1], dimension, hGridCellSizes[j + 1]);
                }
                if (k == j + 1)
                {
                    diffNumberOfCellToCheck[k] += diffHExpectedCellsIntegral(0.5 * hGridCellSizes[i], 0.5 * hGridCellSizes[i + 1], dimension, hGridCellSizes[j + 1]);
                }
            }
            contactWork += particlesPerLevel[i] * numberOfCellToCheck * particlesPerCell[j];
            overheadWork += particlesPerLevel[i] * numberOfCellToCheck;
            //std::cout<<"i= "<<i<<" j= "<<j<<" NumberOfCellToCheck= "<<numberOfCellToCheck<<" diffNumberOfCellToCheck[2]= "<<diffNumberOfCellToCheck[2]<<std::endl;
            for (unsigned int k = 0; k < NParams; k++)
            {
                diffContactWork[k] += (diffParticlesPerLevel[i][k] * numberOfCellToCheck * particlesPerCell[j] + particlesPerLevel[i] * diffNumberOfCellToCheck[k] * particlesPerCell[j] + particlesPerLevel[i] * numberOfCellToCheck * diffParticlesPerCell[j][k]);
                diffOverheadWork[k] += (diffParticlesPerLevel[i][k] * numberOfCellToCheck + particlesPerLevel[i] * diffNumberOfCellToCheck[k]);
            }
        }
    }
    if (verbosity > 0)
    {
        std::cout << "Contact work: " << contactWork << std::endl;
        std::cout << "Overhead work: " << cellCheckOverContactCheckRatio_ * overheadWork << std::endl;
        std::cout << "Sum work: " << contactWork + cellCheckOverContactCheckRatio_ * overheadWork << std::endl;
        std::cout << "diff Contactwork: ";
        for (unsigned int j = 0; j < NParams; j++)
        {
            std::cout << " " << diffContactWork[j];
        }
        std::cout << std::endl;
        std::cout << "diff Overheadwork: ";
        for (unsigned int j = 0; j < NParams; j++)
        {
            std::cout << " " << cellCheckOverContactCheckRatio_ * diffOverheadWork[j];
        }
        std::cout << std::endl;
        std::cout << "diff sum: ";
        for (unsigned int j = 0; j < NParams; j++)
        {
            std::cout << " " << diffContactWork[j] + cellCheckOverContactCheckRatio_ * diffOverheadWork[j];
        }
        std::cout << std::endl;
    }
    dfdx.clear();
    for (unsigned int j = 0; j < NParams; j++)
    {
        dfdx.push_back(diffContactWork[j] + cellCheckOverContactCheckRatio_ * diffOverheadWork[j]);
    }
}

double HGridOptimiser::calculateWork(std::vector<double>& hGridCellSizes, HGridMethod method, int verbosity)
{
    unsigned int NLevels = hGridCellSizes.size() - 1;
    unsigned int NParams = hGridCellSizes.size();
    
    if (verbosity > 1)
    {
        std::cout << "Length scale=" << length << std::endl;
    }
    if (verbosity > 0)
    {
        std::cout << "Level sizes:" << std::endl;
        for (unsigned int i = 0; i < NParams; i++)
        {
            std::cout << std::setw(10) << hGridCellSizes[i] << " ";
        }
        std::cout << std::endl;
    }
    
    std::vector<double> particlesPerLevel;
    for (unsigned int i = 0; i < NLevels; i++)
    {
        particlesPerLevel.push_back(pdfInt(0.5 * hGridCellSizes[i], 0.5 * hGridCellSizes[i + 1], 0));
        if (particlesPerLevel.back() < 0)
        {
            particlesPerLevel.back() = 0;
        }
    }
    if (verbosity > 0)
    {
        std::cout << "Particles per level:" << std::endl;
        for (unsigned int i = 0; i < NLevels; i++)
        {
            std::cout << std::setw(10) << particlesPerLevel[i] << " ";
        }
        std::cout << std::endl;
    }
    
    std::vector<double> cellsPerLevel;
    for (unsigned int i = 0; i < NLevels; i++)
    {
        cellsPerLevel.push_back(pow(length / hGridCellSizes[i + 1], dimension));
    }
    if (verbosity > 1)
    {
        std::cout << "Cells per level:" << std::endl;
        for (unsigned int i = 0; i < NLevels; i++)
        {
            std::cout << std::setw(10) << cellsPerLevel[i] << " ";
        }
        std::cout << std::endl;
    }
    
    std::vector<double> particlesPerCell;
    for (unsigned int i = 0; i < NLevels; i++)
    {
        particlesPerCell.push_back(particlesPerLevel[i] / cellsPerLevel[i]);
    }
    if (verbosity > 1)
    {
        std::cout << "Particles per cell:" << std::endl;
        for (unsigned int i = 0; i < particlesPerCell.size(); i++)
        {
            std::cout << std::setw(10) << particlesPerCell[i] << " ";
        }
        std::cout << std::endl;
    }
    
    std::vector<std::vector<double> > contactWork;
    std::vector<std::vector<double> > overheadWork;
    contactWork.resize(NLevels);
    overheadWork.resize(NLevels);
    for (unsigned int i = 0; i < NLevels; i++)
    {
        contactWork[i].resize(NLevels);
        overheadWork[i].resize(NLevels);
        for (unsigned int j = 0; j < NLevels; j++)
        {
            contactWork[i][j] = 0;
            overheadWork[i][j] = 0;
        }
    }

    for (unsigned int i = 0; i < NLevels; i++)
    {
        contactWork[i][i] += 0.5 * pow(3, dimension) * particlesPerCell[i] * particlesPerLevel[i];
        overheadWork[i][i] += 0.5 * (pow(3, dimension) + 1) * particlesPerLevel[i];
        
        unsigned int startJ, endJ;
        switch (method)
        {
            case BOTTOMUP:
                {
                startJ = i + 1;
                endJ = NLevels;
                break;
            }
            case TOPDOWN:
                {
                startJ = 0;
                endJ = i;
                break;
            }
        }
        for (unsigned int j = startJ; j < endJ; j++)
        {
            double numberOfCellToCheck;
            numberOfCellToCheck = expectedCellsIntegral(0.5 * hGridCellSizes[i], 0.5 * hGridCellSizes[i + 1], dimension, hGridCellSizes[j + 1]);
            contactWork[i][j] += particlesPerLevel[i] * numberOfCellToCheck * particlesPerCell[j];
            overheadWork[i][j] += particlesPerLevel[i] * numberOfCellToCheck;
            
        }
    }

    double totalContactWork = 0, totalOverheadWork = 0;
    for (unsigned int i = 0; i < NLevels; i++)
    {
        for (unsigned int j = 0; j < NLevels; j++)
        {
            totalContactWork += contactWork[i][j];
            totalOverheadWork += overheadWork[i][j];
        }
    }
    
    if (verbosity > 0)
    {
        std::cout << "Contact work: " << totalContactWork << std::endl;
        for (unsigned int i = 0; i < NLevels; i++)
        {
            for (unsigned int j = 0; j < NLevels; j++)
            {
                std::cout << std::setw(10) << contactWork[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Overhead work: " << totalOverheadWork << std::endl;
        for (unsigned int i = 0; i < NLevels; i++)
        {
            for (unsigned int j = 0; j < NLevels; j++)
            {
                std::cout << std::setw(10) << overheadWork[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Total work: " << totalContactWork + totalOverheadWork << std::endl;
    }
    return totalContactWork + cellCheckOverContactCheckRatio_ * totalOverheadWork;
}

void HGridOptimiser::calcDfDx(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, HGridMethod method, int verbosity)
{
    if (verbosity > 0)
    {
        std::cout << "calcDfDx" << std::endl;
    }
    double W = calculateWork(hGridCellSizes, method, verbosity - 1);
    double dx = 1e-7;
    double nW;
    dfdx.clear();
    for (unsigned int i = 0; i < hGridCellSizes.size(); i++)
    {
        hGridCellSizes[i] += dx;
        nW = calculateWork(hGridCellSizes, method, verbosity - 1);
        dfdx.push_back((nW - W) / dx);
        hGridCellSizes[i] -= dx;
        if (verbosity > 0)
        {
            std::cout << "i=" << i << " Value=" << hGridCellSizes[i] << " Change=" << nW - W << " dfdx=" << dfdx.back() << std::endl;
        }
    }
}

double HGridOptimiser::checkLimit(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, int verbosity)
{
    if (verbosity > 0)
    {
        std::cout << "checkLimits part 1 remove levels" << std::endl;
    }
    
    dfdx[0] = 0;
    dfdx.back() = 0;
    for (unsigned int i = 1; i < hGridCellSizes.size(); i++)
    {
        if (std::abs(hGridCellSizes[i - 1] - hGridCellSizes[i]) < 1e-6)
        {
            if (dfdx[i - 1] < dfdx[i])
            {
                if (verbosity > 0)
                {
                    std::cout << "Remove level" << i << std::endl;
                }
                if (i > 0.5 * hGridCellSizes.size())
                {
                    hGridCellSizes.erase(hGridCellSizes.begin() + i - 1);
                    dfdx.erase(dfdx.begin() + i - 1);
                }
                else
                {
                    hGridCellSizes.erase(hGridCellSizes.begin() + i);
                    dfdx.erase(dfdx.begin() + i);
                }
                i--;
            }
        }
    }
    
    if (verbosity > 0)
    {
        std::cout << "checkLimits part 2 calculate limit" << std::endl;
        for (unsigned int i = 0; i < hGridCellSizes.size(); i++)
        {
            std::cout << "i=" << i << " Value=" << hGridCellSizes[i] << " dfdx=" << dfdx[i] << std::endl;
        }
    }
    double maxStepSize = -std::numeric_limits<double>::max();
    double nmax;
    for (unsigned int i = 1; i < hGridCellSizes.size(); i++)
    {
        nmax = (hGridCellSizes[i - 1] - hGridCellSizes[i]) / (dfdx[i] - dfdx[i - 1]);
        if (verbosity > 0)
        {
            std::cout << "Max f=" << nmax << " because otherwise " << hGridCellSizes[i - 1] << "+x*" << dfdx[i - 1] << ">" << hGridCellSizes[i] << "+x*" << dfdx[i] << std::endl;
        }
        if (nmax < 0)
        {
            maxStepSize = std::max(nmax, maxStepSize);
        }
    }
    if (verbosity > 0)
    {
        std::cout << "maxStepSize=" << maxStepSize << std::endl;
    }
    return maxStepSize;
}

void HGridOptimiser::applyStep(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, double stepsize, int verbosity)
{
    if (verbosity > 0)
    {
        std::cout << "Apply step:" << std::endl;
    }
    for (unsigned int i = 0; i < hGridCellSizes.size() - 1; i++)
    {
        hGridCellSizes[i] += stepsize * dfdx[i];
        if (verbosity > 0)
        {
            std::cout << "hGridCellSizes[i]" << "+" << stepsize << "*" << dfdx[i] << "=" << hGridCellSizes[i] << std::endl;
        }
    }
}

double HGridOptimiser::goldenSectionSearch(std::vector<double>& startHGridCellSizes, std::vector<double>& searchDirection, double min, double cur, double max, HGridMethod method, int verbosity)
{
    std::vector<double> curHGridCellSizes = startHGridCellSizes;
    std::vector<double> xHGridCellSizes = startHGridCellSizes;
    
    applyStep(curHGridCellSizes, searchDirection, cur, verbosity - 1);
    double curWork = calculateWork(curHGridCellSizes, method, verbosity - 1);
    
    double x;
    double resphi = 2.0 - 0.5 * (1 + std::sqrt(5));
    
    //Determine new probing point
    if (max - cur > cur - min)
    {
        //x between cur and max
        x = cur + resphi * (max - cur);
    }
    else
    {
        //x between min and cur
        x = cur - resphi * (cur - min);
    }
    
    if (std::abs(max - min) < 1e-7 * (std::abs(cur) + std::abs(x)))
    {
        return 0.5 * (min + max);
    }
    
    applyStep(xHGridCellSizes, searchDirection, x, 0);
    double xWork = calculateWork(xHGridCellSizes, method, 0);
    if (verbosity > 0)
    {
        std::cout << "min=" << min << " max=" << max << " cur=" << cur << " curWork=" << curWork << " x=" << x << " xWork=" << xWork << std::endl;
    }
    if (xWork < curWork) //x is better
    {
        if (max - cur > cur - min)
        {
            return goldenSectionSearch(startHGridCellSizes, searchDirection, cur, x, max, method, verbosity);
        }
        else
        {
            return goldenSectionSearch(startHGridCellSizes, searchDirection, min, x, cur, method, verbosity);
        }
    }
    else //cur is better
    {
        if (max - cur > cur - min)
        {
            return goldenSectionSearch(startHGridCellSizes, searchDirection, min, cur, x, method, verbosity);
        }
        else
        {
            return goldenSectionSearch(startHGridCellSizes, searchDirection, x, cur, max, method, verbosity);
        }
    }
}

void HGridOptimiser::getOptimalDistribution(std::vector<double>& hGridCellSizes, unsigned int numberOfLevels, HGridMethod method, int verbosity)
{
    hGridCellSizes.clear();
    for (unsigned int i = 0; i <= numberOfLevels; i++)
    {
        hGridCellSizes.push_back(2.0 * (rMin + (rMax - rMin) * i / numberOfLevels));
    }
    
    std::vector<double> dfdx;
    double step, max, W, oW;
    W = calculateWork(hGridCellSizes, method, verbosity - 3);
    int stepNumber = 0;
    do
    {
        oW = W;
        calculateDiffWork(hGridCellSizes, dfdx, method, verbosity - 3);
        max = checkLimit(hGridCellSizes, dfdx, verbosity - 3);
        step = goldenSectionSearch(hGridCellSizes, dfdx, max, 0.5 * max, 0, method, verbosity - 2);
        applyStep(hGridCellSizes, dfdx, step, verbosity - 3);
        W = calculateWork(hGridCellSizes, method, verbosity - 3);
        while (W > oW)
        {
            if (verbosity > 1)
            {
                std::cout << stepNumber << " Bad step, trying closer search range" << std::endl;
            }
            applyStep(hGridCellSizes, dfdx, -step, verbosity - 3);
            max /= 2;
            step = goldenSectionSearch(hGridCellSizes, dfdx, max, 0.5 * max, 0, method, verbosity - 2);
            applyStep(hGridCellSizes, dfdx, step, verbosity - 3);
            W = calculateWork(hGridCellSizes, method, verbosity - 3);
        }
        ++stepNumber;
        if (verbosity > 1)
        {
            std::cout << stepNumber << " Correct step: old work=" << oW << " new work=" << W << " difference=" << oW - W << " step=" << step / max << std::endl;
        }
    } while (oW - W > 1e-13 && stepNumber);
    calculateDiffWork(hGridCellSizes, dfdx, method, verbosity - 2);
    if (verbosity > 0)
    {
        std::cout << "Work=" << W << std::endl;
        std::cout << "Optimal cell sizes:";
        for (unsigned int i = 0; i < hGridCellSizes.size(); i++)
        {
            std::cout << " " << hGridCellSizes[i];
        }
        std::cout << std::endl;
    }
}

void HGridOptimiser::histNumberParticlesPerCell(std::vector<double>& hGridCellSizes)
{
    unsigned int NLevels = hGridCellSizes.size() - 1;

    std::vector<double> particlesPerLevel;
    for (unsigned int i = 0; i < NLevels; i++)
    {
        particlesPerLevel.push_back(pdfInt(0.5 * hGridCellSizes[i], 0.5 * hGridCellSizes[i + 1], 0));
        if (particlesPerLevel.back() < 0)
        {
            particlesPerLevel.back() = 0;
        }
    }

    std::vector<double> cellsPerLevel;
    for (unsigned int i = 0; i < NLevels; i++)
    {
        cellsPerLevel.push_back(pow(length / hGridCellSizes[i + 1], dimension));
    }

    for (unsigned int i = 0; i < NLevels; i++)
    {
        double l = particlesPerLevel[i] / cellsPerLevel[i];
        std::cout << "Histogram for level " << i << ":";
        for (double k = 0; k <= 10; k++)
        {
            std::cout << " " << std::setw(8) << std::floor(std::pow(l, k) * std::exp(-l) / mathsFunc::factorial(k)*1e4*cellsPerLevel[i]);
        }
        std::cout << std::endl;
    }
}

