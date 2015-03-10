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


#include "MaserBoundary.h"
#include "DPMBase.h"
#include "BoundaryHandler.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"
#include "Species/ParticleSpecies.h"

MaserBoundary* MaserBoundary::copy() const
{
    return new MaserBoundary(*this);
}

void MaserBoundary::set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight)
{
    // factor is used to set normal to unit length
    normal_ = normal;
    distanceLeft_ = distanceLeft;
    distanceRight_ = distanceRight;
    shift_ = normal_ * (distanceRight_ - distanceLeft_);
}

void MaserBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
}

void MaserBoundary::write(std::ostream& os) const
{
    BaseBoundary::write(os);
}

std::string MaserBoundary::getName() const
{
    return "MaserBoundary";
}

Mdouble MaserBoundary::getDistance(BaseParticle &p)
{
    return getDistance(p.getPosition());
}

Mdouble MaserBoundary::getDistance(const Vec3D &position)
{
    Mdouble distance = Vec3D::dot(position, normal_);

    if (distance - distanceLeft_ < distanceRight_ - distance)
    {
        closestToLeftBoundary_ = true;
        return distance - distanceLeft_;
    }
    else
    {
        closestToLeftBoundary_ = false;
        return distanceRight_ - distance;
    }
}

void MaserBoundary::shiftPosition(BaseParticle* p)
{
    if (closestToLeftBoundary_)
    {
        p->move(shift_);
        closestToLeftBoundary_ = false;
    }
    else
    {
        p->move(-shift_);
        closestToLeftBoundary_ = true;
    }
}

void MaserBoundary::createPeriodicParticles(BaseParticle *P, ParticleHandler &pH)
{
    if (getDistance(*P) < P->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())
    {
        auto conversion = speciesConversionMaserToNormal_.find(P->getSpecies());
        if (conversion != speciesConversionMaserToNormal_.end()) //Test if it is a maser particle
        {
            BaseParticle* F0 = P->copy();

            F0->copyInteractionsForPeriodicParticles(*P);

            shiftPosition(F0);

            //If Particle is double shifted, get correct original particle
            BaseParticle* From = P;
            while (From->getPeriodicFromParticle() != nullptr)
            {
                From = From->getPeriodicFromParticle();
            }
            F0->setPeriodicFromParticle(From);

            pH.addObject(F0);

            if (closestToLeftBoundary_)
            {
            }
            else
            {
            }
        }
    }
}

bool MaserBoundary::checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH UNUSED)
{
    if (getDistance(*P) < 0)
    {
        auto conversion = speciesConversionMaserToNormal_.find(P->getSpecies());
        if (conversion != speciesConversionMaserToNormal_.end()) //Test if it is a maser particle
        {
            shiftPosition(P);
            /*if (closestToLeftBoundary_)
            {
                P->move(shift_);
                closestToLeftBoundary_ = false;
            }
            else
            {
                P->move(-shift_);
                closestToLeftBoundary_ = true;
                //BaseParticle* pCopy = pH.copyAndAddObject(P);
                //pCopy->setIndSpecies(conversion->second);
            }*/
        }
    }
    return false;
}

void MaserBoundary::addParticleToMaser(BaseParticle *P)
{
    auto conversion = speciesConversionNormalToMaser_.find(P->getSpecies());
    if (conversion == speciesConversionNormalToMaser_.end())
    {
        std::cout << "New conversion required" << std::endl;
        ParticleSpecies* newSpecies = getHandler()->getDPMBase()->speciesHandler.copyAndAddObject(P->getSpecies()->copy());
        speciesConversionNormalToMaser_.insert(std::pair<const ParticleSpecies*, const ParticleSpecies*>(P->getSpecies(), newSpecies));
        speciesConversionMaserToNormal_.insert(std::pair<const ParticleSpecies*, const ParticleSpecies*>(newSpecies, P->getSpecies()));
        P->setSpecies(newSpecies);
    }
    else
    {
        std::cout << "Conversion already known" << std::endl;
        P->setSpecies(conversion->second);
    }
}

