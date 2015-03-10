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

///takes data and fstat files and splits them into *.data.???? and *.fstat.???? files

#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h> 
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Math/ExtendedMath.h"
#include "Math/Helpers.h"
#include "Math/Vector.h"

class CFile {

public:

	///Constructor
	CFile(std::string name) : name_(name){

		//set file names
        std::string p4pName(name+".p4p");
        std::string p4cName(name+".p4c");
        std::string p4wName(name+".p4w");

        //open input file streams
		p4p.open(p4pName.c_str(), std::fstream::in);
        p4c.open(p4cName.c_str(), std::fstream::in);
        p4w.open(p4wName.c_str(), std::fstream::in);

		if (p4p.fail() || p4c.fail() || p4w.fail())
		{
            if (p4p.fail())
                std::cerr << "ERROR: Input file " << p4pName << " not found" << std::endl;
            if (p4c.fail())
                std::cerr << "ERROR: Input file " << p4cName << " not found" << std::endl;
            if (p4w.fail())
                std::cerr << "ERROR: Input file " << p4wName << " not found" << std::endl;
			p4p.close();
            p4c.close();
            p4w.close();
			exit(-1);
		} else {
			std::cout << "Files opened: " << p4pName << " and " << p4cName << " and " << p4wName << std::endl;
		}
	}

	///Destructor
	~CFile() {
		p4p.close();
        p4c.close();
        p4w.close();
		std::cout << "Files closed: " << name_ << ".p4p/p4w/p4c" << std::endl;
	}
		
	void copy(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, double timeMin, double timeMax, unsigned int periodic) {
        std::cout << "copy(..)" << std::endl;
        xmin_ = xmin;
        xmax_ = xmax;
        ymin_ = ymin;
        ymax_ = ymax;
        zmin_ = zmin;
        zmax_ = zmax;
        periodic_=periodic;
		writeP4P(timeMin,timeMax);
	}

	void writeP4P(double timeMin, double timeMax) {
        std::cout << "writeP4P(..)" << std::endl;

		unsigned int N;
		std::string line;
        std::fstream data;
        std::fstream fstat;
        double time;
        double ID, GROUP, VOLUME, MASS, VX, VY, VZ, Angular_Velocity_X, Angular_Velocity_Y, Angular_Velocity_Z;

        unsigned int counter = 0;
		while (p4p.good())
        {
            //read header
            std::string dummy;
            p4p >> dummy >> dummy;
            p4p >> time >> N;
            getline(p4p,line);
            getline(p4p,line);

            if (time>timeMax)
            {
                std::cout << "reading p4p (t " << time << " N " << N << "): timeMax reached; terminating" << std::endl;
                break;
            }
            else if (time<timeMin)
            {
                std::cout << "reading p4p (t " << time << " N " << N << "): below timeMin; skipped" << std::endl;
                for (unsigned int i=0; i<N; i++)
                    getline(p4p,line);
                continue;
            }

            std::cout << "reading p4p (t " << time << " N " << N << " timestep #" << counter << ")" << std::endl;

			//open data/fstat file
            std::string dataName = getName("data", counter);
			data.open(dataName.c_str(), std::fstream::out);
            std::string fstatName = getName("fstat", counter);
            fstat.open(fstatName.c_str(), std::fstream::out);
            std::cout << "Files opened: " << dataName << " and " << fstatName << std::endl;

            //write data file
            data<< N << " "
                << time << " "
                << xmin_ << " "
                << ymin_ << " "
                << zmin_ << " "
                << xmax_ << " "
                << ymax_ << " "
                << zmax_ << std::endl;

            position_.resize(N*1.1);
            id_.resize(N*1.1);
            for (unsigned int i=0; i<N; i++) {
                p4p >> ID;
                if (position_.size()<ID)
                {
                    position_.resize(ID*1.1);
                    id_.resize(ID*1.1);
                }
                Vec3D& P = position_[ID-1];
                id_[ID-1]=i;
                p4p >> GROUP
                    >> VOLUME
                    >> MASS
                    >> P.X
                    >> P.Y
                    >> P.Z
                    >> VX
                    >> VY
                    >> VZ
                    >> Angular_Velocity_X
                    >> Angular_Velocity_Y
                    >> Angular_Velocity_Z;
				data << P.X << " "
                    << P.Y << " "
                    << P.Z << " "
                    << VX << " "
                    << VY << " "
                    << VZ << " "
                    << pow(6.0/constants::pi*VOLUME,1.0/3.0)/2.0 << " "
                    << 0 << " "
                    << 0 << " "
                    << 0 << " "
                    << Angular_Velocity_X << " "
                    << Angular_Velocity_Y << " "
                    << Angular_Velocity_Z << " "
                    << 0 << std::endl;
                //std::cout << ID << " ";
			}
            std::cout << "written " << dataName << std::endl;

            writeP4C(fstat, time);
            std::cout << "written " << fstatName << std::endl;

            //close data file
            data.close();
            fstat.close();
            ++counter;

            p4p >> std::ws; //to make sure that p4p.good returns false at end of file
        }
        writeRestart(time, MASS/VOLUME);
    }

	void writeP4C(std::fstream& fstat, double timeData) {
        unsigned int N;
        std::string line;
        std::string dummy;
        double time;
        double P1, P2, delta, deltat, fn, ft;
        Vec3D C1, F, n, t;

        while (p4c.good()) {
            //read header
            p4c >> dummy >> dummy;
            p4c >> time >> N;
            getline(p4c,line);
            getline(p4c,line);

            if (time<timeData*0.999999)
            {
                std::cout << "reading p4c (t " << time << " N " << N << "): below timeMin; skipped" << std::endl;
                for (unsigned int i=0; i<N; i++)
                    getline(p4c,line);
                continue;
            }

            std::cout << "reading p4c (t " << time << " N " << N << ")" <<  std::endl;

            //write fstat file
            fstat << "# " << time << " " << N << std::endl;
            fstat << "# " << std::endl;
            fstat << "# " << std::endl;
            //double P1, P2, CX, CY, CZ, FX, FY, FZ;
            //time, i, j, x, y, z, delta, deltat, fn, ft, nx, ny, nz, tx, ty, tz

            for (unsigned int i=0; i<N; i++) {
                p4c >> P1
                    >> P2
                    >> C1
                    >> F;
                //std::cout << i << "|" << P1 << "|" << P2 << "|" << C1 << "|" << F << std::endl;
                Vec3D C2=C1;
                Mdouble pos1, pos2;
                Mdouble *c1, *c2;
                //std::cout << "C  " << C1 << std::endl;
                if (periodic_)
                {
                    if (periodic_==1)
                    {
                        pos1 = position_[P1 - 1].X;
                        pos2 = position_[P2 - 1].X;
                        c1 = &C1.X;
                        c2 = &C2.X;
                    }
                    else if (periodic_==2)
                    {
                        pos1 = position_[P1 - 1].Y;
                        pos2 = position_[P2 - 1].Y;
                        c1 = &C1.Y;
                        c2 = &C2.Y;
                    }
                    else //if (periodic_==3)
                    {
                        pos1 = position_[P1 - 1].Z;
                        pos2 = position_[P2 - 1].Z;
                        c1 = &C1.Z;
                        c2 = &C2.Z;
                    }
                    if (pos1 < (xmax_-xmin_)/3.0 && pos2 > 2.0*(xmax_-xmin_)/3.0)
                    {
                        if (*c1 < (xmax_ - xmin_) / 3.0)
                            *c2 += xmax_ - xmin_;
                        else if (*c1 > 2.0*(xmax_ - xmin_) / 3.0)
                            *c1 -= xmax_ - xmin_;
                    }
                    else if (pos1 < (xmax_-xmin_)/3.0 && pos2 > 2.0*(xmax_-xmin_)/3.0)
                    {
                        if (*c1 < (xmax_ - xmin_) / 3.0)
                            *c1 += xmax_ - xmin_;
                        else if (*c1 > 2.0*(xmax_ - xmin_) / 3.0)
                            *c2 -= xmax_ - xmin_;
                    }
                }
                //std::cout << "C1  " << C1 << std::endl;
                //std::cout << "C2  " << C2 << std::endl;
                //std::cout << "P0 " << position_[P1-1] << std::endl;
                //std::cout << "P1 " << position_[P2-1] << std::endl;
                delta=0.0;
                deltat=0.0;
                ft=0.0;
                t.setZero();
                fn=F.getLength();
                n=F/fn;

                fstat << time << " "
                    << id_[P1-1] << " "
                    << id_[P2-1] << " "
                    << C1 << " "
                    << delta << " "
                    << deltat << " "
                    << fn << " "
                    << ft << " "
                    << n << " "
                    << t << std::endl;
                fstat << time << " "
                    << id_[P2-1] << " "
                    << id_[P1-1] << " "
                    << C2 << " "
                    << delta << " "
                    << deltat << " "
                    << fn << " "
                    << ft << " "
                    << -n << " "
                    << t << std::endl;
            }
            writeP4W(fstat, timeData);
            break;
        }
	}

    void writeP4W(std::fstream& fstat, double timeData)
    {
        unsigned int N;
        std::string line;
        std::string dummy;
        double time;
        double P1, delta, deltat, fn, ft;
        Vec3D C, F, n, t;

        while (p4w.good()) {
            //read header
            p4w >> dummy >> dummy;
            p4w >> time >> N;
            getline(p4w,line);
            getline(p4w,line);

            if (time<timeData*0.999999)
            {
                std::cout << "reading p4w (t " << time << " N " << N << "): below timeMin; skipped" << std::endl;
                for (unsigned int i=0; i<N; i++)
                    getline(p4w,line);
                continue;
            }

            std::cout << "reading p4w (t " << time << " N " << N << ")" <<  std::endl;

            for (unsigned int i=0; i<N; i++) {
                p4w >> P1
                    >> C
                    >> F;
                delta=0.0;
                deltat=0.0;
                ft=0.0;
                t.setZero();
                fn=F.getLength();
                n=F/fn;
                fstat << time << " "
                    << id_[P1-1] << " "
                    << -1 << " "
                    << C << " "
                    << delta << " "
                    << deltat << " "
                    << fn << " "
                    << ft << " "
                    << n << " "
                    << t << std::endl;
            }
            break;
        }
    }

    void writeRestart(double timeData, double density)
    {
        std::cout << "restart.0000 " << timeData << std::endl;
        //open data/fstat file
        std::fstream restart;
        restart.open(name_ + ".restart", std::fstream::out);
        restart <<
            "restart_version 1.0 name " << name_ << "\n"
            "dataFile name " << name_ << ".data fileType MULTIPLE_FILES_PADDED saveCount 1 counter 1 nextSavedTimeStep 0\n"
            "fStatFile name " << name_ << ".fstat fileType MULTIPLE_FILES_PADDED saveCount 1 counter 1 nextSavedTimeStep 0\n"
            "eneFile name " << name_ << ".ene fileType ONE_FILE saveCount 1 counter 302 nextSavedTimeStep 0\n"
            "restartFile name " << name_ << ".restart fileType ONE_FILE saveCount 1 counter 1 nextSavedTimeStep 0\n"
            "statFile name " << name_ << ".stat fileType ONE_FILE saveCount 1 counter 0 nextSavedTimeStep 0\n"
            //"dataFile    fileType MULTIPLE_FILES_PADDED saveCount 1 counter 1 nextSavedTimeStep 0\n"
            //"fStatFile   fileType MULTIPLE_FILES_PADDED saveCount 1 counter 1 nextSavedTimeStep 0\n"
            //"eneFile     fileType ONE_FILE saveCount 1 counter 302 nextSavedTimeStep 0\n"
            //"restartFile fileType ONE_FILE saveCount 1 counter 1 nextSavedTimeStep 0\n"
            //"statFile    fileType ONE_FILE saveCount 1 counter 0 nextSavedTimeStep 0\n"
            "xMin " << xmin_<< " xMax " << xmax_<< " yMin " << ymin_<< " yMax " << ymax_<< " zMin " << zmin_<< " zMax " << zmax_<< "\n"
            "timeStep 0 time " << timeData << " ntimeSteps 0 timeMax " << timeData << "\n"
            "systemDimensions 3 particleDimensions 3 gravity 0 0 -9.8\n"
            "Species 1\n"
            "LinearViscoelasticSpecies id 0 density " << density << " stiffness 0 dissipation 0\n"
            "Walls 0\n";
        if (!periodic_)
            restart << "Boundaries 0\n";
        else
        {
            restart << "Boundaries 1\n";
            if (periodic_==1)
                restart << "PeriodicBoundary id 0 normal 1 0 0 scaleFactor 1 distanceLeft " << xmin_ << " distanceRight " << xmax_ << " shift " << xmax_-xmin_ << " 0 0\n";
            else if (periodic_==2)
                restart << "PeriodicBoundary id 0 normal 0 1 0 scaleFactor 1 distanceLeft " << ymin_ << " distanceRight " << ymax_ << " shift " << ymax_-ymin_ << " 0 0\n";
            else
                restart << "PeriodicBoundary id 0 normal 0 0 1 scaleFactor 1 distanceLeft " << zmin_ << " distanceRight " << zmax_ << " shift " << zmax_-zmin_ << " 0 0\n";
        }
        restart <<
            "Particles 0\n"
            "BaseParticle id 0 indSpecies 0 position 0 0 0 orientation 0 0 0 0 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 radius 0.5 invMass 1 invInertia 10\n"
            "Interactions 0\n";
        restart.close();
    }


    std::string getName(std::string type, unsigned int counter)
    {
        //set dataName
        std::stringstream name("");
        name << name_ << "." << type << ".";
        if (counter < 1000)
            name << "0";
        if (counter < 100)
            name << "0";
        if (counter < 10)
            name << "0";
        name << counter;
        return name.str();
    }


private:
	///These store the save file names, 
    std::string name_;

	///Stream used for data files
	std::fstream p4p;
    std::fstream p4c;
    std::fstream p4w;
    double xmin_, xmax_;
    double ymin_, ymax_;
    double zmin_, zmax_;
    std::vector<Vec3D> position_;
    std::vector<unsigned int> id_;
    unsigned int periodic_; // 0: no periodicity, 1-3: periodic in x-z
};

int main(int argc, char *argv[])
{
	if (argc<8)
    {
        if (argc < 2)
        {
            std::cout << "no arguments given, so a test file is converted; to convert your own p4 files, use:" << std::endl;
            std::cout << "convertP4Files.cpp name xmin xmax ymin ymax zmin zmax [timeMin [timeMax [periodic]]]" << std::endl;

            helpers::writeToFile("test.p4p",
                "TIMESTEP PARTICLES\n"
                    "0  2\n"
                    "ID GROUP VOLUME MASS PX PY PZ VX VY VZ Angular_Velocity_X Angular_Velocity_Y Angular_Velocity_Z\n"
                    "1 1 0.52359877559 1 0.5 0.5 0.5 1 0 0 0 0 1\n"
                    "3 1 0.52359877559 1 3.5 0.5 0.5 0 0 0 0 0 0\n"
                    "TIMESTEP PARTICLES\n"
                    "2  2\n"
                    "ID GROUP VOLUME MASS PX PY PZ VX VY VZ Angular_Velocity_X Angular_Velocity_Y Angular_Velocity_Z\n"
                    "1 1 0.52359877559 1 0.5 0.5 0.5 1 0 0 0 0 1\n"
                    "3 1 0.52359877559 1 3.5 0.5 0.5 0 0 0 0 0 0\n"
            );

            helpers::writeToFile("test.p4c",
                "TIMESTEP CONTACTS\n"
                    "0  1\n"
                    "P1 P2 CX CY CZ FX FY FZ\n"
                    "1 3 0 0.5 0.5 1 1 0\n"
                    "TIMESTEP CONTACTS\n"
                    "2  1\n"
                    "P1 P2 CX CY CZ FX FY FZ\n"
                    "1 3 4 0.5 0.5 1 1 0\n"
            );

            helpers::writeToFile("test.p4w",
                "TIMESTEP CONTACTS\n"
                    "0  2\n"
                    "P1 CX CY CZ FX FY FZ\n"
                    "1 0.5 0.5 0 0 0 1\n"
                    "3 3.5 0.5 0 0 0 1\n"
                    "TIMESTEP CONTACTS\n"
                    "2  2\n"
                    "P1 CX CY CZ FX FY FZ\n"
                    "1 0.5 0.5 0 0 0 1\n"
                    "3 3.5 0.5 0 0 0 1\n"
            );

            CFile files("test");
            files.copy(0,4,0,1,0,1,-1,1e20,1);
        }
        else
        {
            std::cerr << "convertP4Files.cpp problem_name xmin xmax ymin ymax zmin zmax [timeMin [timeMax [periodic]]]" << std::endl;
            return -1;
        }
    }
    else
    {
        std::string name(argv[1]);
        std::cout << "converting " << name << std::endl;

        double timeMin = -1;
        if (argc>8) {
            timeMin = atof(argv[8]);
            std::cout << "timeMin " << timeMin << std::endl;
        }

        double timeMax = 1e20;
        if (argc>9) {
            timeMax = atof(argv[9]);
            std::cout << "timeMax " << timeMax << std::endl;
        }

        unsigned int periodic = 0;
        if (argc>10) {
            periodic = atoi(argv[10]);
            std::cout << "periodic " << periodic << std::endl;
        }

        CFile files(name);
        files.copy(atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]),atof(argv[6]),atof(argv[7]),timeMin,timeMax,periodic);

        std::cout << "finished converting " << name << std::endl;
    }
}
//./ConvertP4Files ~/Documents/Work/Carlos/silo -0.075 0.075 0 0.04 -0.00465405 0.231798 -1 1e20 2
//~/MercuryAll/Trunk/Build/XBalls/xballs -f silo.data.0000 -format 14 -s 4.8 -solidf -v0 -sort -moh 130 -w 320