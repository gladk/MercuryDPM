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

#include "MaserBoundary.h"
#include "DPMBase.h"
#include "BoundaryHandler.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"
#include "Species/BaseSpecies.h"

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
        BaseSpecies* newSpecies = getHandler()->getDPMBase()->speciesHandler.copyAndAddObject(P->getSpecies()->copy());
        speciesConversionNormalToMaser_.insert(std::pair<const BaseSpecies*, const BaseSpecies*>(P->getSpecies(), newSpecies));
        speciesConversionMaserToNormal_.insert(std::pair<const BaseSpecies*, const BaseSpecies*>(newSpecies, P->getSpecies()));
        P->setSpecies(newSpecies);
    }
    else
    {
        std::cout << "Conversion already known" << std::endl;
        P->setSpecies(conversion->second);
    }
}

