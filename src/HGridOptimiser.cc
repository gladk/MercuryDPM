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

#include <iostream>
#include <iomanip>  
#include <limits> 
#include <math.h> 
#include "HGridOptimiser.h"
#include "Particles/BaseParticle.h"

void HGridOptimiser::Initialise(HGRID_base& problem, int verbosity)
{
    NCells=100;
    k=0.2;
    CellN.resize(NCells);
    for (std::vector<int>::iterator it = CellN.begin() ; it != CellN.end(); ++it)
    {
        *it=0;
    }
    
    rMin=nextafter(problem.getParticleHandler().getSmallestParticle()->get_InteractionRadius(),0.0);
    rMax=nextafter(problem.getParticleHandler().getLargestParticle()->get_InteractionRadius(),std::numeric_limits<double>::max());
    for (std::vector<BaseParticle*>::iterator it = problem.getParticleHandler().begin() ; it != problem.getParticleHandler().end(); ++it)
    {
        CellN[radius2Cell((*it)->get_InteractionRadius())]++;
    }
    NParticles=problem.getParticleHandler().getNumberOfObjects();
    
    if(verbosity>0)
    {
        std::cout<<"rMin="<<rMin<<" rMax="<<rMax<<" NParticles="<<NParticles<<std::endl;
        for (unsigned int i=0;i<CellN.size();i++)
        {
            std::cout<<"From"<<std::setw(8)<<cell2Min(i)<<" to"<<std::setw(8)<<cell2Max(i)<<" number="<<std::setw(5)<<CellN[i]<<std::endl;
        }
        std::cout<<"Domain size ["<<problem.get_xmin()<<","<<problem.get_xmax()<<"]x["<<problem.get_ymin()<<","<<problem.get_ymax()<<"]x["<<problem.get_zmin()<<","<<problem.get_zmax()<<"]"<<std::endl;
        /*std::cout<<"["<<CellN[0];
        for (unsigned int i=1;i<CellN.size();i++)
        {
            std::cout<<","<<CellN[i];
        }       
        std::cout<<"]"<<std::endl;*/
    }
    
    dimension=problem.get_dim();
    switch(dimension)
    {
        case 1:
            length=pow((problem.get_xmax()-problem.get_xmin())/NParticles,1);
        case 2:
            length=pow((problem.get_xmax()-problem.get_xmin())*(problem.get_ymax()-problem.get_ymin())/NParticles,1.0/2.0);
        case 3:
            length=pow((problem.get_xmax()-problem.get_xmin())*(problem.get_ymax()-problem.get_ymin())*(problem.get_zmax()-problem.get_zmin())/NParticles,1.0/3.0);
    }
}

int HGridOptimiser::radius2Cell(double r)
{
    int y=floor(NCells*(r-rMin)/(rMax-rMin));
    if(y<0) y=0;
    if(y>=NCells) y=NCells-1;
    return y;
}
double HGridOptimiser::cell2Min(int i)
{
    return rMin+(rMax-rMin)*i/NCells;
}
double HGridOptimiser::cell2Max(int i)
{
    return rMin+(rMax-rMin)*(i+1)/NCells;
}

///This function calculates:
///int(f(r)*r^power*dr,r=start..end)/int(f(r)*dr,r=0..omega)
double HGridOptimiser::pdfInt(double start, double end, int p)
{
    int startCell=radius2Cell(start);
    int endCell=radius2Cell(end);
    if(startCell<0||startCell>=NCells)
    {
        std::cout<<"Error in startCell (startCell="<<startCell<<" start="<<start<<" rMin="<<rMin<<" rMax="<<rMax<<" NCells="<<NCells<<std::endl;
    }
    if(endCell<0||endCell>=NCells)
    {
        std::cout<<"Error in endCell (endCell="<<endCell<<" end="<<end<<" rMin="<<rMin<<" rMax="<<rMax<<" NCells="<<NCells<<std::endl;
    }
    
    double teller=0;
    if(startCell==endCell)
    {
        teller=CellN[startCell]/(p+1)*(pow(end,p+1)-pow(start,p+1));
    }
    else if(endCell<startCell)
    {
        teller=0;
    }
    else
    {
        teller=CellN[startCell]/(p+1)*(pow(cell2Max(startCell),p+1)-pow(start,p+1));
        for(int i=startCell+1;i<endCell;i++)
        {
            teller+=CellN[i]/(p+1)*(pow(cell2Max(i),p+1)-pow(cell2Min(i),p+1));
        }
        teller+=CellN[endCell]/(p+1)*(pow(end,p+1)-pow(cell2Min(endCell),p+1));
    }
    double noemer=0;
    for(int i=0;i<NCells;i++)
    {
        noemer+=CellN[i]*(cell2Max(i)-cell2Min(i));
    }
   return teller/noemer;            
}

///diff(int(f(r)*r^power*dr,r=s..e)/int(f(r)*dr,r=0..omega),e)=f(e)*e^power/int(f(r)*dr,r=0..omega)
double HGridOptimiser::diffPdfInt(double x, int p)
{
    int xCell=radius2Cell(x);
    double noemer=0;
    for(int i=0;i<NCells;i++)
    {
        noemer+=CellN[i]*(cell2Max(i)-cell2Min(i));
    }
   return CellN[xCell]*pow(x,p)/noemer;            
}


///This function calculates:
///int((2*r/h+2)^d f(r) dr,r=s..e)/int(f(r) dr,r=s..e)+
///Used to calculated the expected number of cells to check at the level with maximum size h for particle radius between start and end
double HGridOptimiser::expectedCellsIntegral(double start, double end, int p, double h)
{
    int startCell=radius2Cell(start);
    int endCell=radius2Cell(end);
    
    double teller=0;
    double noemer=0;
    if(startCell==endCell)
    {
        teller=CellN[startCell]*h/2/(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*start+2,p+1));
        noemer=CellN[startCell]*(end-start);
    }
    else
    {
        teller=CellN[startCell]*h/2/(p+1)*(pow(2/h*cell2Max(startCell)+2,p+1)-pow(2/h*start+2,p+1));
        noemer=CellN[startCell]*(cell2Max(startCell)-start);
        for(int i=startCell+1;i<endCell;i++)
        {
            teller+=CellN[i]*h/2/(p+1)*(pow(2/h*cell2Max(i)+2,p+1)-pow(2/h*cell2Min(i)+2,p+1));
            noemer+=CellN[i]*(cell2Max(i)-cell2Min(i));
        }
        teller+=CellN[endCell]*h/2/(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*cell2Min(endCell)+2,p+1));
        noemer+=CellN[endCell]*(end-cell2Min(endCell));
    }
    if(noemer==0)
        return 0;
    else
        return teller/noemer;            
}

double HGridOptimiser::diffStartExpectedCellsIntegral(double start, double end, int p, double h)
{
    int startCell=radius2Cell(start);
    int endCell=radius2Cell(end);
    
    double teller=0;
    double noemer=0;
    double diffTeller=-CellN[startCell]*(pow(2/h*start+2,p));
    double diffNoemer=-CellN[startCell];
    if(startCell==endCell)
    {
        teller=CellN[startCell]*h/2/(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*start+2,p+1));
        noemer=CellN[startCell]*(end-start);
    }
    else
    {
        teller=CellN[startCell]*h/2/(p+1)*(pow(2/h*cell2Max(startCell)+2,p+1)-pow(2/h*start+2,p+1));
        noemer=CellN[startCell]*(cell2Max(startCell)-start);
        for(int i=startCell+1;i<endCell;i++)
        {
            teller+=CellN[i]*h/2/(p+1)*(pow(2/h*cell2Max(i)+2,p+1)-pow(2/h*cell2Min(i)+2,p+1));
            noemer+=CellN[i]*(cell2Max(i)-cell2Min(i));
        }
        teller+=CellN[endCell]*h/2/(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*cell2Min(endCell)+2,p+1));
        noemer+=CellN[endCell]*(end-cell2Min(endCell));
    }
    if(noemer==0)
        return 0;
    else
        return (diffTeller*noemer-teller*diffNoemer)/(noemer*noemer);
}

double HGridOptimiser::diffEndExpectedCellsIntegral(double start, double end, int p, double h)
{
    int startCell=radius2Cell(start);
    int endCell=radius2Cell(end);
    
    double teller=0;
    double noemer=0;
    double diffTeller=CellN[endCell]*(pow(2/h*end+2,p));
    double diffNoemer=CellN[endCell];
    if(startCell==endCell)
    {
        teller=CellN[startCell]*h/2/(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*start+2,p+1));
        noemer=CellN[startCell]*(end-start);
    }
    else
    {
        teller=CellN[startCell]*h/2/(p+1)*(pow(2/h*cell2Max(startCell)+2,p+1)-pow(2/h*start+2,p+1));
        noemer=CellN[startCell]*(cell2Max(startCell)-start);
        for(int i=startCell+1;i<endCell;i++)
        {
            teller+=CellN[i]*h/2/(p+1)*(pow(2/h*cell2Max(i)+2,p+1)-pow(2/h*cell2Min(i)+2,p+1));
            noemer+=CellN[i]*(cell2Max(i)-cell2Min(i));
        }
        teller+=CellN[endCell]*h/2/(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*cell2Min(endCell)+2,p+1));
        noemer+=CellN[endCell]*(end-cell2Min(endCell));
    }
    if(noemer==0)
        return 0;
    else
        return (diffTeller*noemer-teller*diffNoemer)/(noemer*noemer);
}

double HGridOptimiser::diffHExpectedCellsIntegral(double start, double end, int p, double h)
{
    int startCell=radius2Cell(start);
    int endCell=radius2Cell(end);
    
    double teller=0;
    double noemer=0;
    double diffTeller=0;
    if(startCell==endCell)
    {
        teller=CellN[startCell]*h/2/(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*start+2,p+1));
        diffTeller=CellN[startCell]/2/(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*start+2,p+1))-CellN[startCell]/h*(end*pow(2/h*end+2,p)-start*pow(2/h*start+2,p));
        noemer=CellN[startCell]*(end-start);
    }
    else
    {
        teller    =0.5*CellN[startCell]*h/(p+1)*(pow(2/h*cell2Max(startCell)+2,p+1)-pow(2/h*start+2,p+1));
        diffTeller=0.5*CellN[startCell]  /(p+1)*(pow(2/h*cell2Max(startCell)+2,p+1)-pow(2/h*start+2,p+1))-CellN[startCell]/h*(cell2Max(startCell)*pow(2/h*cell2Max(startCell)+2,p)-start*pow(2/h*start+2,p));        
        noemer=CellN[startCell]*(cell2Max(startCell)-start);
        for(int i=startCell+1;i<endCell;i++)
        {
            teller    +=0.5*CellN[i]*h/(p+1)*(pow(2/h*cell2Max(i)+2,p+1)-pow(2/h*cell2Min(i)+2,p+1));
            diffTeller+=0.5*CellN[i]  /(p+1)*(pow(2/h*cell2Max(i)+2,p+1)-pow(2/h*cell2Min(i)+2,p+1))-CellN[i]/h*(cell2Max(i)*pow(2/h*cell2Max(i)+2,p)-cell2Min(i)*pow(2/h*cell2Min(i)+2,p));
            noemer+=CellN[i]*(cell2Max(i)-cell2Min(i));
        }
        teller    +=0.5*CellN[endCell]*h/(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*cell2Min(endCell)+2,p+1));
        diffTeller+=0.5*CellN[endCell]  /(p+1)*(pow(2/h*end+2,p+1)-pow(2/h*cell2Min(endCell)+2,p+1))-CellN[endCell]/h*(end*pow(2/h*end+2,p)-cell2Min(endCell)*pow(2/h*cell2Min(endCell)+2,p));
        noemer+=CellN[endCell]*(end-cell2Min(endCell));
    }
    if(noemer==0)
        return 0;
    else
        return diffTeller/noemer;
}

void HGridOptimiser::calculateDiffWork(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, HGridMethod method, int verbosity)
{
    unsigned int NLevels=hGridCellSizes.size()-1;
    unsigned int NParams=hGridCellSizes.size();
    
    if(verbosity>0)
    {
        std::cout<<"Length scale="<<length<<std::endl;
    }
    if(verbosity>0)
    {
        std::cout<<"Level sizes:";
        for(unsigned int i=0;i<NParams;i++)
        {
            std::cout<<" "<<hGridCellSizes[i];
        }
        std::cout<<std::endl;
    }
    
    std::vector<double> particlesPerLevel;
    for(unsigned int i=0;i<NLevels;i++)
    {
        particlesPerLevel.push_back(pdfInt(0.5*hGridCellSizes[i],0.5*hGridCellSizes[i+1],0));
        if(particlesPerLevel.back()<0)
        {
            particlesPerLevel.back()=0;
        }
    }        
    if(verbosity>0)
    {
        std::cout<<"Particles per level: ";
        for(unsigned int i=0;i<NLevels;i++)
        {
            std::cout<<" "<<particlesPerLevel[i];
        }
        std::cout<<std::endl;
    }
    
    //How changes particlesPerLeve[i] when hGridCellSize[j] is changed
    std::vector< std::vector< double > > diffParticlesPerLevel;
    diffParticlesPerLevel.resize(NLevels);
    for(unsigned int i=0;i<NLevels;i++)
    {
        for(unsigned int j=0;j<NParams;j++)
        {
            diffParticlesPerLevel[i].push_back(0.0);
            if(j==i)
            {
                diffParticlesPerLevel[i][j]+=-0.5*diffPdfInt(0.5*hGridCellSizes[i],0);
            }
            if(j==i+1)
            {
                diffParticlesPerLevel[i][j]+= 0.5*diffPdfInt(0.5*hGridCellSizes[i+1],0);
            }
        }
    }
    if(verbosity>0)
    {
        std::cout<<"Diff Particles per level: "<<std::endl;
        for(unsigned int i=0;i<NLevels;i++)
        {
            for(unsigned int j=0;j<NParams;j++)
            {
                std::cout<<" "<<std::setw(12)<<diffParticlesPerLevel[i][j];
            }
            std::cout<<std::endl;
        }        
    }

    std::vector<double> cellsPerLevel;
    for(unsigned int i=0;i<NLevels;i++)
    {
        cellsPerLevel.push_back(pow(length/hGridCellSizes[i+1],dimension));
    }
    if(verbosity>0)
    {
        std::cout<<"Cells per level: ";
        for(unsigned int i=0;i<NLevels;i++)
        {
            std::cout<<" "<<cellsPerLevel[i];
        }
        std::cout<<std::endl;
    } 
    
    //How changes cellsPerLevel[i] when hGridCellSize[j] is changed    
    std::vector< std::vector< double > > diffCellsPerLevel;
    diffCellsPerLevel.resize(hGridCellSizes.size());
    for(unsigned int i=0;i<NLevels;i++)
    {
        for(unsigned int j=0;j<NParams;j++)
        {
            if(j==i+1)
            {
                diffCellsPerLevel[i].push_back(-pow(length/hGridCellSizes[i+1],dimension)*dimension/hGridCellSizes[i+1]);
            }
            else
            {
                diffCellsPerLevel[i].push_back(0.0);
            }
        }
    }
    if(verbosity>0)
    {
        std::cout<<"Diff Cells per level: "<<std::endl;
        for(unsigned int i=0;i<NLevels;i++)
        {
            for(unsigned int j=0;j<NParams;j++)
            {
                std::cout<<" "<<std::setw(12)<<diffCellsPerLevel[i][j];
            }
            std::cout<<std::endl;
        }        
    }
    
    std::vector<double> particlesPerCell;
    for(unsigned int i=0;i<NLevels;i++)
    {
        particlesPerCell.push_back(particlesPerLevel[i]/cellsPerLevel[i]);
    }
    if(verbosity>0)
    {
        std::cout<<"Particles per cell: ";
        for(unsigned int i=0;i<particlesPerCell.size();i++)
        {
            std::cout<<" "<<particlesPerCell[i];
        }
        std::cout<<std::endl;
    }
    
    //How changes particlesPerCell[i] when hGridCellSize[j] is changed    
    std::vector< std::vector< double > > diffParticlesPerCell;
    diffParticlesPerCell.resize(NLevels);
    for(unsigned int i=0;i<NLevels;i++)
    {
        for(unsigned int j=0;j<NParams;j++)
        {
            diffParticlesPerCell[i].push_back((diffParticlesPerLevel[i][j]*cellsPerLevel[i]-particlesPerLevel[i]*diffCellsPerLevel[i][j])/(cellsPerLevel[i]*cellsPerLevel[i]));
        }
    }
    if(verbosity>0)
    {
        std::cout<<"Diff Particles per Cell: "<<std::endl;
        for(unsigned int i=0;i<NLevels;i++)
        {
            for(unsigned int j=0;j<NParams;j++)
            {
                std::cout<<" "<<std::setw(12)<<diffParticlesPerCell[i][j];
            }
            std::cout<<std::endl;
        }        
    }
        
    double contactWork=0, overheadWork=0;
    std::vector<double> diffContactWork;
    std::vector<double> diffOverheadWork;
    diffContactWork.resize(NParams);
    diffOverheadWork.resize(NParams);
    for(unsigned int j=0;j<NParams;j++)
    {
        diffContactWork[j]=0;
        diffOverheadWork[j]=0;
    }

    for(unsigned int i=0;i<NLevels;i++)
    {
        contactWork+=0.5*pow(3,dimension)*particlesPerCell[i]*particlesPerLevel[i];
        overheadWork+=0.5*(pow(3,dimension)+1)*particlesPerLevel[i];
        for(unsigned int j=0;j<NParams;j++)
        {
            diffContactWork[j]+=0.5*pow(3,dimension)*(diffParticlesPerCell[i][j]*particlesPerLevel[i]+particlesPerCell[i]*diffParticlesPerLevel[i][j]);
            diffOverheadWork[j]+=0.5*(pow(3,dimension)+1)*diffParticlesPerLevel[i][j];
        }
        
        int startJ,endJ;
        switch(method)
        {
            case BOTTOMUP:
            {
                startJ=i+1;
                endJ=NLevels;
                break;
            }            
            case TOPDOWN:
            {
                startJ=0;
                endJ=i;
                break;
            }
        }
        for(unsigned int j=startJ;j<endJ;j++)
        {
            double numberOfCellToCheck;
            numberOfCellToCheck=expectedCellsIntegral(0.5*hGridCellSizes[i],0.5*hGridCellSizes[i+1],dimension,hGridCellSizes[j+1]);

            std::vector<double> diffNumberOfCellToCheck;
            for(unsigned int k=0;k<NParams;k++)
            {
                diffNumberOfCellToCheck.push_back(0.0);
                if(k==i)
                {
                    diffNumberOfCellToCheck[k]+=0.5*diffStartExpectedCellsIntegral(0.5*hGridCellSizes[i],0.5*hGridCellSizes[i+1],dimension,hGridCellSizes[j+1]);
                }
                if(k==i+1)
                {
                    diffNumberOfCellToCheck[k]+=0.5*diffEndExpectedCellsIntegral(0.5*hGridCellSizes[i],0.5*hGridCellSizes[i+1],dimension,hGridCellSizes[j+1]);
                }   
                if(k==j+1)
                {
                    diffNumberOfCellToCheck[k]+=diffHExpectedCellsIntegral(0.5*hGridCellSizes[i],0.5*hGridCellSizes[i+1],dimension,hGridCellSizes[j+1]);
                }
            }
            contactWork+=particlesPerLevel[i]*numberOfCellToCheck*particlesPerCell[j];
            overheadWork+=particlesPerLevel[i]*numberOfCellToCheck;
            //std::cout<<"i= "<<i<<" j= "<<j<<" NumberOfCellToCheck= "<<numberOfCellToCheck<<" diffNumberOfCellToCheck[2]= "<<diffNumberOfCellToCheck[2]<<std::endl;
            for(unsigned int k=0;k<NParams;k++)
            {
                diffContactWork[k]+=(diffParticlesPerLevel[i][k]*numberOfCellToCheck*particlesPerCell[j]+particlesPerLevel[i]*diffNumberOfCellToCheck[k]*particlesPerCell[j]+particlesPerLevel[i]*numberOfCellToCheck*diffParticlesPerCell[j][k]);
                diffOverheadWork[k]+=(diffParticlesPerLevel[i][k]*numberOfCellToCheck+particlesPerLevel[i]*diffNumberOfCellToCheck[k]);
            }
        }
    }           
    if(verbosity>0)
    {
        std::cout<<"Contact work: "<<contactWork<<std::endl;
        std::cout<<"Overhead work: "<<k*overheadWork<<std::endl;
        std::cout<<"Sum work: "<<contactWork+k*overheadWork<<std::endl;
        std::cout<<"diff Contactwork: ";
        for(unsigned int j=0;j<NParams;j++)
        {
            std::cout<<" "<<diffContactWork[j];
        }
        std::cout<<std::endl;
        std::cout<<"diff Overheadwork: ";
        for(unsigned int j=0;j<NParams;j++)
        {
            std::cout<<" "<<k*diffOverheadWork[j];
        }
        std::cout<<std::endl;
        std::cout<<"diff sum: ";
        for(unsigned int j=0;j<NParams;j++)
        {
            std::cout<<" "<<diffContactWork[j]+k*diffOverheadWork[j];
        }
        std::cout<<std::endl;        
    }
    dfdx.clear();
    for(unsigned int j=0;j<NParams;j++)
    {
        dfdx.push_back(diffContactWork[j]+k*diffOverheadWork[j]);
    }
}

double HGridOptimiser::calculateWork(std::vector<double>& hGridCellSizes, HGridMethod method, int verbosity)
{
    unsigned int NLevels=hGridCellSizes.size()-1;
    unsigned int NParams=hGridCellSizes.size();
    
    if(verbosity>0)
    {
        std::cout<<"Length scale="<<length<<std::endl;
    }
    if(verbosity>0)
    {
        std::cout<<"Level sizes:";
        for(unsigned int i=0;i<NParams;i++)
        {
            std::cout<<" "<<hGridCellSizes[i];
        }
        std::cout<<std::endl;
    }
    
    std::vector<double> particlesPerLevel;
    for(unsigned int i=0;i<NLevels;i++)
    {
        particlesPerLevel.push_back(pdfInt(0.5*hGridCellSizes[i],0.5*hGridCellSizes[i+1],0));
        if(particlesPerLevel.back()<0)
        {
            particlesPerLevel.back()=0;
        }
    }        
    if(verbosity>0)
    {
        std::cout<<"Particles per level: ";
        for(unsigned int i=0;i<NLevels;i++)
        {
            std::cout<<" "<<particlesPerLevel[i];
        }
        std::cout<<std::endl;
    }
    
    std::vector<double> cellsPerLevel;
    for(unsigned int i=0;i<NLevels;i++)
    {
        cellsPerLevel.push_back(pow(length/hGridCellSizes[i+1],dimension));
    }
    if(verbosity>0)
    {
        std::cout<<"Cells per level: ";
        for(unsigned int i=0;i<NLevels;i++)
        {
            std::cout<<" "<<cellsPerLevel[i];
        }
        std::cout<<std::endl;
    } 
    
    std::vector<double> particlesPerCell;
    for(unsigned int i=0;i<NLevels;i++)
    {
        particlesPerCell.push_back(particlesPerLevel[i]/cellsPerLevel[i]);
    }
    if(verbosity>0)
    {
        std::cout<<"Particles per cell: ";
        for(unsigned int i=0;i<particlesPerCell.size();i++)
        {
            std::cout<<" "<<particlesPerCell[i];
        }
        std::cout<<std::endl;
    }
        
    double contactWork=0, overheadWork=0;
    for(unsigned int i=0;i<NLevels;i++)
    {
        contactWork+=0.5*pow(3,dimension)*particlesPerCell[i]*particlesPerLevel[i];
        overheadWork+=0.5*(pow(3,dimension)+1)*particlesPerLevel[i];
        
        int startJ,endJ;
        switch(method)
        {
            case BOTTOMUP:
            {
                startJ=i+1;
                endJ=NLevels;
                break;
            }            
            case TOPDOWN:
            {
                startJ=0;
                endJ=i;
                break;
            }
        }
        for(unsigned int j=startJ;j<endJ;j++)
        {
            double numberOfCellToCheck;
            numberOfCellToCheck=expectedCellsIntegral(0.5*hGridCellSizes[i],0.5*hGridCellSizes[i+1],dimension,hGridCellSizes[j+1]);
            contactWork+=particlesPerLevel[i]*numberOfCellToCheck*particlesPerCell[j];
            overheadWork+=particlesPerLevel[i]*numberOfCellToCheck;
        
        }           
    }
  
    if(verbosity>0)
    {
        std::cout<<"Contact work: "<<contactWork<<std::endl;
        std::cout<<"Overhead work: "<<k*overheadWork<<std::endl;
    }    
    return contactWork+k*overheadWork;    
}

void HGridOptimiser::calcDfDx(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, HGridMethod method, int verbosity)
{
    if(verbosity>0)
    {
        std::cout<<"calcDfDx"<<std::endl;
    }
    double W=calculateWork(hGridCellSizes, method,verbosity-1);
    double dx=1e-10;
    double nW;
    dfdx.clear();
    for(unsigned int i=0;i<hGridCellSizes.size();i++)
    {
        hGridCellSizes[i]+=dx;
        nW=calculateWork(hGridCellSizes, method, verbosity-1);
        dfdx.push_back((nW-W)/dx);
        hGridCellSizes[i]-=dx;
        if(verbosity>0)
        {
            std::cout<<"i="<<i<<" Value="<<hGridCellSizes[i]<<" Change="<<nW-W<<" dfdx="<<dfdx.back()<<std::endl;        
        }   
    }
}

double HGridOptimiser::checkLimit(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, int verbosity)
{
    if(verbosity>0)
    {
        std::cout<<"checkLimits part 1 remove levels"<<std::endl;
    }

    dfdx[0]=0;
    dfdx.back()=0;
    for(unsigned int i=1;i<hGridCellSizes.size();i++)
    {
        if(std::abs(hGridCellSizes[i-1]-hGridCellSizes[i])<1e-6)
        {
            if(dfdx[i-1]<dfdx[i])
            {
                if(verbosity>0)
                {
                    std::cout<<"Remove level"<<i<<std::endl;
                }                
                if(i>0.5*hGridCellSizes.size())
                {
                    hGridCellSizes.erase(hGridCellSizes.begin()+i-1);
                    dfdx.erase(dfdx.begin()+i-1);
                }
                else
                {
                    hGridCellSizes.erase(hGridCellSizes.begin()+i);
                    dfdx.erase(dfdx.begin()+i);
                }
                i--;
            }
        }
    }
    
    if(verbosity>0)
    {
        std::cout<<"checkLimits part 2 calculate limit"<<std::endl;
        for(unsigned int i=0;i<hGridCellSizes.size();i++)
        {
            std::cout<<"i="<<i<<" Value="<<hGridCellSizes[i]<<" dfdx="<<dfdx[i]<<std::endl;
        }
    }    
    double maxStepSize=-std::numeric_limits<double>::max();
    double nmax;
    for(unsigned int i=1;i<hGridCellSizes.size();i++)
    {
        nmax=(hGridCellSizes[i-1]-hGridCellSizes[i])/(dfdx[i]-dfdx[i-1]);
        if(verbosity>0)
        {
                std::cout<<"Max f="<<nmax<<" because otherwise "<<hGridCellSizes[i-1]<<"+x*"<<dfdx[i-1]<<">"<<hGridCellSizes[i]<<"+x*"<<dfdx[i]<<std::endl;
        }
        if(nmax<0)
        {
            maxStepSize=std::max(nmax,maxStepSize);
        }
    }
    if(verbosity>0)
    {
        std::cout<<"maxStepSize="<<maxStepSize<<std::endl;
    }
    return maxStepSize;
}

void HGridOptimiser::applyStep(std::vector<double>& hGridCellSizes, std::vector<double>& dfdx, double stepsize, int verbosity)
{
    if(verbosity>0)
    {
        std::cout<<"Apply step:"<<std::endl;
    }
    for(unsigned int i=0;i<hGridCellSizes.size()-1;i++)
    {
        hGridCellSizes[i]+=stepsize*dfdx[i];
        if(verbosity>0)
        {
            std::cout<<"hGridCellSizes[i]"<<"+"<<stepsize<<"*"<<dfdx[i]<<"="<<hGridCellSizes[i]<<std::endl;
        }            
    }
}

double HGridOptimiser::goldenSectionSearch(std::vector<double>& startHGridCellSizes, std::vector<double>& searchDirection, double min, double cur, double max, HGridMethod method, int verbosity)
{
    std::vector<double> curHGridCellSizes=startHGridCellSizes;
    std::vector<double> xHGridCellSizes=startHGridCellSizes;
    
    applyStep(curHGridCellSizes, searchDirection, cur, verbosity-1);
    double curWork=calculateWork(curHGridCellSizes, method, verbosity-1);
    
    double x;
    double resphi=2.0-0.5*(1+std::sqrt(5));

    //Determine new probing point
    if(max-cur>cur-min)
    {
        //x between cur and max
        x=cur+resphi*(max-cur);
    }
    else
    {
        //x between min and cur
        x=cur-resphi*(cur-min);
    }
    
    if (std::abs(max-min)<1e-7*(std::abs(cur)+std::abs(x)))
    {
          return 0.5*(min + max); 
    }
    
    applyStep(xHGridCellSizes, searchDirection, x, 0);
    double xWork=calculateWork(xHGridCellSizes, method, 0);
    if(verbosity>0)
    {
        std::cout<<"min="<<min<<" max="<<max<<" cur="<<cur<<" curWork="<<curWork<<" x="<<x<<" xWork="<<xWork<<std::endl;
    }        
    if(xWork<curWork) //x is better
    {
        if(max-cur>cur-min)
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
        if(max-cur>cur-min)
        {
            return goldenSectionSearch(startHGridCellSizes, searchDirection, min, cur, x, method, verbosity);
        }
        else
        {
            return goldenSectionSearch(startHGridCellSizes, searchDirection, x, cur, max, method, verbosity);
        }
    }
}

void HGridOptimiser::getOptimalDistribution(std::vector<double>& hGridCellSizes, int numberOfLevels, HGridMethod method, int verbosity)
{
    hGridCellSizes.clear();
    for (unsigned int i=0; i<=numberOfLevels; i++)
    {
        hGridCellSizes.push_back(2.0*(rMin+(rMax-rMin)*i/numberOfLevels));
    }

    std::vector<double> dfdx;
    double step,max,nW,W;
    W=calculateWork(hGridCellSizes, method,verbosity-3);
    for(int i=0;i<100;i++)
    {
        calculateDiffWork(hGridCellSizes, dfdx, method,verbosity-3);            
        max=checkLimit(hGridCellSizes,dfdx,verbosity-3);
        step=goldenSectionSearch(hGridCellSizes, dfdx, max, 0.5*max, 0, method,verbosity-2);
        applyStep(hGridCellSizes,dfdx,step,verbosity-3);
        nW=calculateWork(hGridCellSizes, method,verbosity-3);
        if(verbosity>1)
        {
            std::cout<<"Correct step: old work="<<W<<" new work="<<nW<<" difference="<<nW-W<<" step="<<step/max<<std::endl;
        }               
        W=nW;
    }    
    calculateDiffWork(hGridCellSizes, dfdx, method,verbosity-2);
    if(verbosity>0)
    {
        std::cout<<"Work="<<nW<<std::endl;
        std::cout<<"Optimal cell sizes:";
        for(unsigned int i=0;i<hGridCellSizes.size();i++)
        {
            std::cout<<" "<<hGridCellSizes[i];
        }
        std::cout<<std::endl;
    }
}
    
