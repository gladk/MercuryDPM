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

#include "HopperInsertionBoundary.h"
#include "Particles/BaseParticle.h"
#include "Math/RNG.h"

/*!
 * \details Default constructor. Sets all properties to 0. Calls the InsertionBoundary
 * parent default constructor as well.
 */
HopperInsertionBoundary::HopperInsertionBoundary() : InsertionBoundary()
{
    yMin_ = 0.0;
    yMax_ = 0.0;
    radMin_ = 0.0;
    radMax_ = 0.0;
    chuteAngle_ = 0.0;
    fixedParticleRadius_ = 0.0;
    isHopperCentred__ = true;
    hopperDim_ = 0;
    hopperAngle_ = 0.0;
    hopperLength_ = 0.0;
    hopperExitLength_ = 0.0;
    hopperHeight_ = 0.0;
    lift_ = 0.0;
    fillPercent_ = 0.0;
}

/*!
 * \details Copy constructor. Calls InsertionBoundary parent copy constructor.
 */
HopperInsertionBoundary::HopperInsertionBoundary(const HopperInsertionBoundary& other)
            : InsertionBoundary(other)
{
    yMin_ = other.yMin_;
    yMax_ = other.yMax_;
    radMin_ = other.radMin_;
    radMax_ = other.radMax_;
    chuteAngle_ = other.chuteAngle_;
    fixedParticleRadius_ = other.fixedParticleRadius_;
    isHopperCentred__ = other.isHopperCentred__;
    hopperDim_ = other.hopperDim_;
    hopperAngle_ = other.hopperAngle_;
    hopperLength_ = other.hopperLength_;
    hopperExitLength_ = other.hopperExitLength_;
    hopperHeight_ = other.hopperHeight_;
    lift_ = other.lift_;
    fillPercent_ = other.fillPercent_;
}

/*!
 * \details Copy method. Creates a copy on the heap.
 * \return      Pointer to the copy.
 */
HopperInsertionBoundary* HopperInsertionBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "HopperInsertionBoundary::copy() const finished" << std::endl;
#endif		
    return new HopperInsertionBoundary(*this);
}

/*!
 * \details Sets all the properties of the chute insertion boundary. 
 * See also the images provided at the top of this class' documentation.
 * 
 * \param[in] particleToCopy    Pointer to the BaseParticle which is used as a basis
 *                              for the particles to be inserted
 * \param[in] maxFailed         The maximum number of times the insertion of a 
 *                              particle may be tried and failed before the insertion
 *                              of particles is considered done.
 *                              NB: this property is used in the parent's 
 *                              InsertionBoundary::checkBoundaryBeforeTimeStep().
 * \param[in] yMin              The minimum y-position of the particle for a hopper
 *                              with vertical walls in the y-direction ('1D hopper')
 * \param[in] yMax              The maximum y-position of the particle for a hopper
 *                              with vertical walls in the y-direction ('1D hopper')
 * \param[in] radMin            Minimum radius of inserted particles
 * \param[in] radMax            Maximum radius of inserted particles
 * \param[in] chuteAngle            Angle of the chute as compared to the horizontal plane
 * \param[in] fixedParticleRadius   
 * \param[in] isHopperCentred_      
 * \param[in] hopperDim         Either 1 or 2. If 1, the insertion boundary has 
 *                              vertical walls in the y extrema. This is used
 *                              e.g. for a hopper with periodic walls in the y-direction.
 *                              If 2, the insertion boundary has the form of 
 *                              an inverted (truncated) pyramid, with equally inclined
 *                              walls in both AB and Y directions.
 * \param[in] hopperAngle       The angle of the hopper walls relative to the vertical (AC).
 * \param[in] hopperLength      The horizontal (AB-dir.) width at the top of the hopper
 * \param[in] hopperExitLength  The horizontal (AB-dir.) width at the square exit of the hopper
 * \param[in] hopperHeight      The vertical (AC-dir.) height of the hopper, measured
 *                              from the top of the hopper to the start of the chute.
 * \param[in] lift              
 * \param[in] fillPercent       Percentage of the height of the insertion boundary
 *                              up to which it should be filled. The part to be filled
 *                              reaches from the top of the hopper down to 
 *                              {fillPercent * (top - 'position A')}. 
 */
void HopperInsertionBoundary::set(BaseParticle* particleToCopy, int maxFailed, double yMin, 
        double yMax, double radMin, double radMax, double chuteAngle, double fixedParticleRadius, 
        bool isHopperCentred_, int hopperDim, double hopperAngle, double hopperLength, 
        double hopperExitLength, double hopperHeight, double lift, double fillPercent)
{
    setParticleToCopy(particleToCopy);
    setMaxFailed(maxFailed);
    yMin_ = yMin;
    yMax_ = yMax;
    radMin_ = radMin;
    radMax_ = radMax;
    chuteAngle_ = chuteAngle;
    fixedParticleRadius_ = fixedParticleRadius;
    isHopperCentred__ = isHopperCentred_;
    hopperDim_ = hopperDim;
    hopperAngle_ = hopperAngle;
    hopperLength_ = hopperLength;
    hopperExitLength_ = hopperExitLength;
    hopperHeight_ = hopperHeight;
    lift_ = lift;
    fillPercent_ = fillPercent;
}

/*!
 * \details 
 * This function generates random particles inside the hopper. The space in which
 * the particles are generated is a (truncated) triangle in 2D, or a (truncated) pyramid
 * in 3D. 
 * Some aspects of the hopper geometry and definitions which are useful in order 
 * to understand this particle generation code are:
 *    * In the hopper simulation, the direction of the CHUTE is defined to be
 *      the X-direction, while the normal of the chute is defined to be the Z-direction.
 *      This means that gravity is NOT in the negative Z-direction, but is actually
 *      on an angle w.r.t. the X and Z axes. 
 *      NB: AC and AB are unit vectors in the direction of negative gravity and 
 *      the 'normal' horizontal plane, respectively. 
 *    * The third dimensional direction, which points into the plane spanned by 
 *      the X and Z directions, or the AC and AB directions alternatively, is equal
 *      for either of the two definitions. Ergo, AD = the unit vector in the Y-direction. 
 *      This dimension is not used in the 2D case.
 *    * The hopper is filled with particles from the base of the inverted triangle/pyramid 
 *      (which defines the upper part of the hopper) down to a certain percentage 
 *      fillPercent_ of the hopper depth. fillPercent_=100 will fill entire inverted
 *      triangle/pyramid down to it tip (position A). 'gamma' is the random height 
 *      between the base and given fillPercent_ at which the generated particle is 
 *      placed.  NB: 'down' in this case means negative AC (vertical) direction.
 * 
 * See also the images provided at the top of this class' documentation.
 * 
 * \todo (BvdH) make a nice (InkScape) diagram with all useful properties in one image,
 * and some angles explaining the definitions of AB and AC directions, the P->setPosition 
 * operation and the calculation of the Z-direction of the Vec3D A position.
 * 
 * \param[in] random    random number generator
 * \return              pointer to the particle generated
 */
BaseParticle* HopperInsertionBoundary::generateParticle(RNG &random)
{
    // create particle and assign a random radius
    BaseParticle* P = getParticleToCopy()->copy();
    P->setRadius(random.getRandomNumber(radMin_, radMax_));
    
    //Define an orthogonal coordinate system this is useful in the hopper, see 
    //diagram in html documentation for details.
    static Mdouble s = sin(chuteAngle_);
    static Mdouble c = cos(chuteAngle_);
    static Mdouble Ht = tan(hopperAngle_);
    static Mdouble Hc = cos(hopperAngle_);
    static Vec3D AB = Vec3D(c, 0.0, s);
    static Vec3D AC = Vec3D(-s, 0.0, c);
    static Vec3D AD = Vec3D(0.0, 1.0, 0.0);
    
    //Point A is located in the centre of the hopper.
    static Vec3D A = Vec3D(isHopperCentred__ ? 40 : 0.0,    /// Bram: Where does the '40' come from??
            (yMax_ - yMin_) / 2.0,
            s * (-0.5 * (hopperLength_ - hopperExitLength_)) + c * hopperHeight_)
            + AB * 0.5 * hopperLength_
            + AC * (-0.5 * hopperLength_ / Ht);
    
    Mdouble gamma = random.getRandomNumber((100.0 - fillPercent_) / 100.0, 1.0);
    //std::cout<<"gamma="<<gamma<<"fillPercent="<<fillPercent_<<std::endl;
    
    Mdouble delta;
    
    if (hopperDim_ == 1)
    {
        /*!
         * The 'one-dimensional' hopper has sloped walls in the AB extrema, 
         * while the walls in the AD (Y) extrema are vertical.
         * 'delta' is the randomly generated Y-position of the particle and lies
         * between yMin_ and yMax_ (with a particle radius distance from the 
         * extrema on either side).
        */ 
        /// \bug for periodic walls this should be only minus one particle radius, this should be fixed at some point.
        /// Thomas' response: using one particle radius gives problems when the wall is 
        /// not orthogonal to the y-direction; the distance has to be slightly higher than one; 
        /// if you can figure out the exact value, then correct it please.
        /// \todo Thomas, could you check if this bug is still valid?  (BvdH)    
        delta = random.getRandomNumber(-0.5, 0.5) * (yMax_ - yMin_ - 2.0 * P->getRadius());
        
    }
    else
    {
        /*!
         * The 'two-dimensional' hopper has sloped walls in the extrema of both (horizontal) 
         * AB and AD directions. I.e., the HopperInsertionBoundary has the form of 
         * an inverted (and possibly truncated) pyramid.
         * The possible Y-positions of the generated particle (i.e. delta) in 
         * this case is dependent of both the vertical AC-position gamma, and the 
         * hopper angle (in this case in the form of its cosine, Hc).
         */
        delta = (random.getRandomNumber(-1.0, 1.0) * (0.5 * gamma * hopperLength_ - P->getRadius() / Hc));
    }
    //std::cout<<A<<" "<<AC<<" "<<AB<<" "<<AD<<" "<<Hc<<" "<<Ht<<"gamma="<<gamma<<" "<<hopperLength_<<" "<<delta<<std::endl;
    P->setPosition(A
            + AC * (gamma * 0.5 * hopperLength_ / Ht)
            + AB * (random.getRandomNumber(-1.0, 1.0) * (0.5 * gamma * hopperLength_ - P->getRadius() / Hc))
            + AD * delta);
    
    P->move(Vec3D(0.0, 0.0, lift_));
    P->setVelocity(Vec3D(0.0, 0.0, 0.0));
    
    return P;
}

/*!
 * \details Reads the boundary properties from an istream
 * \param[in,out] is    the istream
 */
void HopperInsertionBoundary::read(std::istream& is)
{
    InsertionBoundary::read(is);
    std::string dummy;
    is >> dummy >> yMin_
            >> dummy >> yMax_
            >> dummy >> radMin_
            >> dummy >> radMax_
            >> dummy >> chuteAngle_
            >> dummy >> fixedParticleRadius_
            >> dummy >> isHopperCentred__
            >> dummy >> hopperDim_
            >> dummy >> hopperAngle_
            >> dummy >> hopperLength_
            >> dummy >> hopperExitLength_
            >> dummy >> hopperHeight_
            >> dummy >> lift_
            >> dummy >> fillPercent_;
}

/*!
 * \details Deprecated version of read().
 * \deprecated Should be gone by Mercury 2.0. Instead, use CubeInsertionBoundary::read().
 */
void HopperInsertionBoundary::oldRead(std::istream& is)
{
    int maxFailed;
    std::string dummy;
    is >> dummy >> maxFailed
            >> dummy >> yMin_
            >> dummy >> yMax_
            >> dummy >> radMin_
            >> dummy >> radMax_
            >> dummy >> chuteAngle_
            >> dummy >> fixedParticleRadius_
            >> dummy >> isHopperCentred__
            >> dummy >> hopperDim_
            >> dummy >> hopperAngle_
            >> dummy >> hopperLength_
            >> dummy >> hopperExitLength_
            >> dummy >> hopperHeight_
            >> dummy >> lift_
            >> dummy >> fillPercent_;
    setMaxFailed(maxFailed);
}

/*!
 * \details Writes boundary's properties to an ostream
 * \param[in] os    the ostream
 */
void HopperInsertionBoundary::write(std::ostream& os) const
        {
    InsertionBoundary::write(os);
    os << " yMin " << yMin_
            << " yMax " << yMax_
            << " radMin " << radMin_
            << " radMax " << radMax_
            << " chuteAngle " << chuteAngle_
            << " fixedParticleRadius " << fixedParticleRadius_
            << " isHopperCentred_ " << isHopperCentred__
            << " hopperDim " << hopperDim_
            << " hopperAngle " << hopperAngle_
            << " hopperLength " << hopperLength_
            << " hopperExitLength " << hopperExitLength_
            << " hopperHeight " << hopperHeight_
            << " lift " << lift_
            << " fillPercent " << fillPercent_;
}

/*!
 * \details Returns the name of the object class
 * \return      the object's class' name, i.e. 'HopperInsertionBoundary'
 */
std::string HopperInsertionBoundary::getName() const
{
    return "HopperInsertionBoundary";
}

