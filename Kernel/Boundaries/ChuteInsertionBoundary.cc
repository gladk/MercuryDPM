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

#include "ChuteInsertionBoundary.h"
#include "Particles/BaseParticle.h"
#include "Math/RNG.h"

/*!
 * \details Default constructor. Initiates all properties with 0 value.
 */
//This is not the most beautiful code, but it works.
ChuteInsertionBoundary::ChuteInsertionBoundary(): InsertionBoundary()
{
    posMax_ = Vec3D(0.0, 0.0, 0.0);
    posMin_ = Vec3D(0.0, 0.0, 0.0);
    radMax_ = 0.0;
    radMin_ = 0.0;
    fixedParticleRadius_ = 0.0;
    inflowVelocity_ = 0.0;
    inflowVelocityVariance_ = 0.0;
}

/*!
 * \details Copy constructor. Calls InsertionBoundary parent copy constructor.
 */
ChuteInsertionBoundary::ChuteInsertionBoundary(const ChuteInsertionBoundary& other) 
        : InsertionBoundary(other)
{
    posMax_ = other.posMax_;
    posMin_ = other.posMin_;
    radMax_ = other.radMax_;
    radMin_ = other.radMin_;
    fixedParticleRadius_ = other.fixedParticleRadius_;
    inflowVelocity_ = other.inflowVelocity_;
    inflowVelocityVariance_ = other.inflowVelocityVariance_;
}

/*!
 * \details Copy method. Creates a copy on the heap.
 * \return      Pointer to the copy.
 */
ChuteInsertionBoundary* ChuteInsertionBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "ChuteInsertionBoundary::copy() const finished" << std::endl;
#endif		
    return new ChuteInsertionBoundary(*this);
}

/*!
 * \details Sets all the properties of the chute insertion boundary. 
 * \param[in] particleToCopy        Pointer to the BaseParticle which is used as a basis
 *                                  for the particles to be inserted
 * \param[in] maxFailed             The maximum number of times the insertion of a 
 *                                  particle may be tried and failed before the insertion
 *                                  of particles is considered done.
 *                                  NB: this property is used in the parent's 
 *                                  InsertionBoundary::checkBoundaryBeforeTimeStep().
 * \param[in] posMin                First defining corner of the chute insertion boundary
 * \param[in] posMax                Second defining corner of the chute insertion boundary
 * \param[in] radMin                Minimum radius of inserted particles
 * \param[in] radMax                Maximum radius of inserted particles
 * \param[in] fixedParticleRadius       radius of the fixed bottom (i.e., positioned 
 *                                      at the minimal Z-position) particles
 * \param[in] inflowVelocity            the mean particle velocity, which is in the X-direction
 * \param[in] inflowVelocityVariance    value of minimum (minus ~) and maximum added 
 *                                      velocities which are added to the given mean
 *                                      in each of the three dimensions. Expressed
 *                                      as a percentage of inflowVelocity_.
 *                                      See also the documentation of ChuteInsertionBoundary::generateParticle().
 */
void ChuteInsertionBoundary::set(BaseParticle* particleToCopy, unsigned int maxFailed, Vec3D posMin, Vec3D posMax, double radMin, double radMax, double fixedParticleRadius, double inflowVelocity, double inflowVelocityVariance)
{
    if (particleToCopy != nullptr)
    {
        setParticleToCopy(particleToCopy);
    }
    setMaxFailed(maxFailed);
    posMin_ = posMin;
    posMax_ = posMax;
    radMin_ = radMin;
    radMax_ = radMax;
    fixedParticleRadius_ = fixedParticleRadius;
    inflowVelocity_ = inflowVelocity;
    inflowVelocityVariance_ = inflowVelocityVariance;
}

/*!
 * \details Generates a particle within the boundary with random radius, position 
 * and velocity (within the allowed intervals). 
 * Notable properties:
 *      * The minimal Z-position is fixedParticleRadius_ (the radius of the particles
 *        fixed at the bottom) above the minimal Z-value of the boundary
 *      * The particles have an mean velocity of inflowVelocity_ in the X-direction.
 *        Furthermore, each particle has a velocity added in all three directions, 
 *        based on a given inflowVelocityVariance_, which is expressed as a percentage
 *        of the inflowVelocity_.
 * \param[in] random        a random number generator
 * \return                  pointer to the generated particle
 */
BaseParticle* ChuteInsertionBoundary::generateParticle(RNG& random)
{
    BaseParticle* P = getParticleToCopy()->copy();
    
    P->setRadius(random.getRandomNumber(radMin_, radMax_));
    
    Vec3D position, velocity;

    position.X = posMin_.X + P->getRadius();

    ///\todo comparing floating point with == or != is unsafe; should we introduce a Mdouble comparison function with a relative tolerance? \author weinhartt
    if ((posMax_.Y - posMin_.Y) == 2.0 * radMax_)
    {
        position.Y = posMin_.Y + P->getRadius();
    }
    else
    {
        position.Y = random.getRandomNumber(posMin_.Y + P->getRadius(), posMax_.Y - P->getRadius());
    }
    position.Z = random.getRandomNumber(posMin_.Z + fixedParticleRadius_ + P->getRadius(), posMax_.Z - P->getRadius());
    
    // The velocity components are first stored in a Vec3D, because if you pass them 
    // directly into setVelocity the compiler is allowed to change the order in 
    // which the numbers are generated
    velocity.X = inflowVelocity_ * random.getRandomNumber(-inflowVelocityVariance_, inflowVelocityVariance_) + inflowVelocity_;
    velocity.Y = inflowVelocity_ * random.getRandomNumber(-inflowVelocityVariance_, inflowVelocityVariance_);
    velocity.Z = inflowVelocity_ * random.getRandomNumber(-inflowVelocityVariance_, inflowVelocityVariance_);
    
    P->setPosition(position);
    P->setVelocity(velocity);
    
    return P;
}

/*!
 * \details Reads the boundary properties from an istream
 * \param[in,out] is    the istream
 */
void ChuteInsertionBoundary::read(std::istream& is)
{
    InsertionBoundary::read(is);
    std::string dummy;
    is >> dummy >> posMin_
            >> dummy >> posMax_
            >> dummy >> radMin_
            >> dummy >> radMax_
            >> dummy >> fixedParticleRadius_
            >> dummy >> inflowVelocity_
            >> dummy >> inflowVelocityVariance_;
}

/*!
 * \details Deprecated version of read().
 * \deprecated Should be gone by Mercury 2.0. Instead, use CubeInsertionBoundary::read().
 */
void ChuteInsertionBoundary::oldRead(std::istream& is)
{
    unsigned int maxFailed;
    std::string dummy;
    is >> dummy >> maxFailed
            >> dummy >> posMin_
            >> dummy >> posMax_
            >> dummy >> radMin_
            >> dummy >> radMax_
            >> dummy >> fixedParticleRadius_
            >> dummy >> inflowVelocity_
            >> dummy >> inflowVelocityVariance_;
    setMaxFailed(maxFailed);
}

/*!
 * \details Writes boundary's properties to an ostream
 * \param[in] os    the ostream
 */
void ChuteInsertionBoundary::write(std::ostream& os) const
        {
    InsertionBoundary::write(os);
    os << " posMin " << posMin_
            << " posMax " << posMax_
            << " radMin " << radMin_
            << " radMax " << radMax_
            << " fixedParticleRadius " << fixedParticleRadius_
            << " inflowVelocity " << inflowVelocity_
            << " inflowVelocityVariance " << inflowVelocityVariance_;
    
}

/*!
 * \details Returns the name of the object class
 * \return      the object's class' name, i.e. 'ChuteInsertionBoundary'
 */
std::string ChuteInsertionBoundary::getName() const
{
    return "ChuteInsertionBoundary";
}

