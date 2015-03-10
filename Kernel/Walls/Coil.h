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

#ifndef COIL_H
#define COIL_H

#include "BaseWall.h"
#include "Math/Vector.h"

/*!
 * \brief This class defines a coil in the z-direction from a (constant) starting point, a (constant) length L, a (constant) radius r, a (constant) number or revelations N and a (constant) rotation speed (rev/s)
 * \details q is a new coordinate going from 0 to 1 and t is the time, x=xs+r*cos(2*pi(offset+N*q)), y=ys+r*sin(2*pi*(offset+N*q)) and z=zs+q*L
 * \todo IFCD: Can someone look at the details of the documentation of class Coil? I can't make sense of them.
 * \todo Coil is now fixed in Z-direction, centered around the Z-axis. Consider converting to more general
 *  parameters, with direction of choise and central axis of choice.
 */

class Coil : public BaseWall
{
public:
    /*!
     * \brief Default constructor, sets a coil with default parameters.
     */    
    Coil();
    
    /*!
     * \brief Copy constructor, makes a coil with the same properties as the input Coil.
     */
    Coil(const Coil& other);
    
    /*!
     * \brief Constructor in which all parameters are set.
     */    
    Coil(Vec3D Start, Mdouble L, Mdouble r, Mdouble N, Mdouble omega, Mdouble thickness);
    
    /*!
     * \brief Default destructor.
     */
    ~Coil();
    
    /*!
     * \brief Set all parameters of this Coil.
     */
    void set(Vec3D Start, Mdouble length, Mdouble radius, Mdouble numberOfRevelations, Mdouble omega, Mdouble thickness);

    /*!
     * \brief Copy this Coil and return a pointer to the copy, useful for polymorphism.
     */    
    Coil* copy() const override;
    
    /*!
     * \brief Compute the distance from the Coil for a given BaseParticle and return if there is a collision. If there is a collision, also return the normal vector of the interaction point.
     */   
    bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const override;

    /*!
     * \brief Get the interaction between this Coil and given BaseParticle at a given time.
     */    
    BaseInteraction* getInteractionWith(BaseParticle* P, Mdouble timeStamp, InteractionHandler* interactionHandler);

    /*!
     * \brief Rotate the Coil for a period dt, so that the offset_ changes with omega_*dt.
     */
    void move_time(Mdouble dt);
    
    /*!
     * \brief Reads a Coil from an input stream, for example a restart file.
     */
    void read(std::istream& is) override;
    
    /*!
     * \brief Reads an old-style Coil from an input stream, for example an old restart file.
     * \deprecated If you have old restart files, please convert them to the current
     * version of the restart files and use read(std::istream&) instead of oldRead(std::istream&).
     */    
    MERCURY_DEPRECATED
    void oldRead(std::istream& is);
    
   /*!
    * \brief Writes a Coil to an output stream, for example a restart file.
    */
    void write(std::ostream& os) const override;
    
   /*!
    * \brief Returns the name of the object, in this case the string "Coil".
    */
    std::string getName() const override;

private:
    /*!
     * \brief The centre of the lower end of the Coil.
     */
    Vec3D start_;///

   /*!
    * \brief The length of the Coil.
    */    
    Mdouble l_;
    
    /*!
     * The radius of the Coil.
     */
    Mdouble r_;
    
    /*!
     * The number of revelations of the Coil.
     */
    Mdouble n_;
    
    /*!
     * The rotation speed of the Coil, in rev/s.
     */
    Mdouble omega_;
    
    /*!
     * The amount the Coil has rotated, this is a number between 0 and 1.
     */
    Mdouble offset_;
    
    /*!
     * The thickness of the "spiral" of the Coil.
     */
    Mdouble thickness_;
};

#endif
