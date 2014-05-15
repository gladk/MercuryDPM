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

#ifndef CircularPeriodicBoundary_H
#define CircularPeriodicBoundary_H

////////////////////////////////////////////////////////////////////////////////////////////////////
///Defines a pair of periodic walls. The particles are in {x: position_left<=normal*x <position_right}, with normal being the outward unit normal vector of the right wall. If a particle moves outside these boundaries, it will be shifted.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CircularPeriodicBoundary : public BaseBoundary{
public:
	
	CircularPeriodicBoundary() : BaseBoundary()
	{
		innerRadius=1.0;
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "CircularPeriodicBoundary() finished" << std::endl;
		#endif				
	}
	
	CircularPeriodicBoundary(double innerRadius) : BaseBoundary()
	{
		this->innerRadius=innerRadius;
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "CircularPeriodicBoundary(double innerRadius) finished" << std::endl;
		#endif				
	}	
	
	CircularPeriodicBoundary* copy() const
	{
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "virtual CircularPeriodicBoundary* copy() const finished" << std::endl;
		#endif				
		return new CircularPeriodicBoundary(*this);
	}
	
	void rotateParticle(BaseParticle *P, double angle)
	{
		double R=sqrt(pow(P->get_Position().X,2)+pow(P->get_Position().Y,2));
		double alphaPos=atan2(P->get_Position().Y,P->get_Position().X);
		double V=sqrt(pow(P->get_Velocity().X,2)+pow(P->get_Velocity().Y,2));
		double alphaVel=atan2(P->get_Velocity().Y,P->get_Velocity().X);
		alphaPos+=angle;
		alphaVel+=angle;
		
        P->set_Position(Vec3D(cos(alphaPos)*R,sin(alphaPos*R),P->get_Position().Z));
        P->set_Velocity(Vec3D(cos(alphaVel)*V,sin(alphaVel*V),P->get_Velocity().Z));
		///todo{Do we need to update rotations and rotational velocitys?}
	}
	
	int createPeriodicParticles(BaseParticle *P, ParticleHandler &pH)
	{
		double R=sqrt(pow(P->get_Position().X,2)+pow(P->get_Position().Y,2));
		double alpha=atan2(P->get_Position().Y,P->get_Position().X);
		int i=log(R/innerRadius)/log(2.0)+1;
		double pieSize=2.0/pow(2.0,i)*constants::pi;
		int C=0;
		//std::cout<<"R="<<R<<" alpha="<<alpha<<" i="<<i<<" pieSize="<<pieSize<<std::endl;
		
		//Check if the particle is close to it's inner Radius or is is close to zero alpha (small y)
		if(i>0&&(R-(P->get_InteractionRadius()+pH.getLargestParticle()->get_InteractionRadius())<pow(2.0,i-1)*innerRadius||P->get_Position().Y<(P->get_InteractionRadius()+pH.getLargestParticle()->get_InteractionRadius())))
		{
			//std::cout<<"Going to shift because "<<R-P->get_Radius()<<"<"<<pow(2,i-1)*innerRadius<<" or "<<P->get_Position().Y<<"<"<<P->get_Radius()<<std::endl;
			//std::cout<<*P<<" has been shifted"<<std::endl;

			BaseParticle* F0=P->copy();
			rotateParticle(F0,pieSize);

			//If Particle is Mdouble shifted, get correct original particle			
			BaseParticle* From=P;
			while(From->get_PeriodicFromParticle()!=NULL)
				From=From->get_PeriodicFromParticle();		
			F0->set_periodicFromParticle(From);

			//std::cout<<*F0<<" is the result"<<std::endl;
			pH.addObject(F0);
			C++;
		}
		//Check here only for i>0 becuase for i=1 they both give the same particle
		if(i>1&&R*R*(1-pow(cos(alpha-pieSize),2))<P->get_InteractionRadius()+pH.getLargestParticle()->get_InteractionRadius())
		{
			//std::cout<<*P<<" has been shifted back"<<std::endl;

			BaseParticle* F0=P->copy();
			rotateParticle(F0,-pieSize);

			//If Particle is Mdouble shifted, get correct original particle			
			BaseParticle* From=P;
			while(From->get_PeriodicFromParticle()!=NULL)
				From=From->get_PeriodicFromParticle();		
			F0->set_periodicFromParticle(From);

			//std::cout<<*F0<<" is the result"<<std::endl;
			pH.addObject(F0);
			C++;
		}
		return C;
	}
	
	bool checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH)
	{
		double R=sqrt(pow(P->get_Position().X,2)+pow(P->get_Position().Y,2));
		double alpha=atan2(P->get_Position().Y,P->get_Position().X);
		int i=log(R/innerRadius)/log(2.0)+1;
		double pieSize=2.0/pow(2.0,i)*constants::pi;
		
		double oldR=sqrt(pow(P->get_Position().X-P->get_Displacement().X,2)+pow(P->get_Position().Y-P->get_Displacement().Y,2));
		///\todo{TW: Dinant, please confirm that i and oldI should be integer}
		int oldI=log(oldR/innerRadius)/log(2.0)+1.0;
		
		if(i>0&&i>oldI) //Particle moves outward so it may have to be deleted
		{
			//std::cout<<"Particle="<<P->get_Index()<<" moving outward with alpha="<<alpha<<" and pieSize="<<pieSize<<" ";
			if(alpha<0||alpha>pieSize)
			{
				if(alpha>2.0*pieSize)
				{
					//std::cout<<"and it is rotated into the pie"<<std::endl;
					rotateParticle(P,-pieSize);
				}
				else
				{
					//Delete particle
					//std::cout<<"and it should be deleted"<<std::endl;
					pH.removeObject(P->get_Index());
					return true;
				}
			}
			//else
				//std::cout<<"and nothing happens"<<std::endl;
		}
		else if (i>=0&&i<oldI) //Particle moves inward so it has to be coppied
		{
			//std::cout<<"Particle="<<P->get_Index()<<" moving inward and is thus coppied with alpha="<<alpha<<" and pieSize="<<pieSize<<std::endl;
			//std::cout<<"i="<<i<<" oldI="<<oldI<<" R="<<R<<" oldR="<<oldR<<std::endl;
			//std::cout<<"Position="<<P->get_Position()<<" Displacement="<<P->get_Displacement()<<std::endl;
			BaseParticle* F0=P->copy();
			F0->set_Displacement(Vec3D(0.0,0.0,0.0));
			if(alpha<0)
			{
				rotateParticle(P,pieSize);
				rotateParticle(F0,0.5*pieSize);
			}
			else if(alpha<0.5*pieSize)
			{
				rotateParticle(F0,0.5*pieSize);
			}
			else if(alpha<pieSize)
			{
				rotateParticle(F0,-0.5*pieSize);
			}
			else
			{
				rotateParticle(P,-pieSize);
				rotateParticle(F0,-0.5*pieSize);
			}
			pH.addObject(F0);
		}
		else if(i>0&&alpha<0)
		{
			//std::cout<<"Particle="<<P->get_Index()<<" i="<<i<<" R="<<R<<" alpha="<<alpha<<" positive rotated pieSize="<<pieSize<<std::endl;			
			rotateParticle(P,pieSize);
		}
		else if(i>0&&alpha>pieSize)
		{
			//std::cout<<"Particle="<<P->get_Index()<<" i="<<i<<" R="<<R<<" alpha="<<alpha<<" negative rotated pieSize="<<pieSize<<std::endl;			
			rotateParticle(P,-pieSize);
		}
		return false;
	}	
	
	///reads wall
	void read(std::istream& is)  { 
    std::string dummy;
		is >>  dummy >> innerRadius;
	}
	
	///outputs wall
	void print(std::ostream& os) const {
		os << "CircularPeriodicBoundary innerRadius "<<innerRadius;
	}
	

private:

	double innerRadius;
	
	
	//A particle is between to Radii in plane (i) and has two (straight) walls (plane 0 defines centre)
	//If it is close to its inner Radius it should be copied once with a positive rotation of 2*pi/2^i
	//If it is close to one of its straight walls is should be rotative with +/- 2*pi/2^i
	
	//Particles can only apply forces to real particles
	
	//If a particle crosses a straight wall is should simply be shifted
	//If a particle crosses its inner Radius it should be coppied
	//If a particle crosses its outer Radius it may need to be deleted

};
#endif
