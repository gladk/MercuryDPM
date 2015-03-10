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

#include "IntersectionOfWalls.h"
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"

IntersectionOfWalls::IntersectionOfWalls()
        : BaseWall()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"InfiniteWall () finished"<<std::endl;
#endif	
}

///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
IntersectionOfWalls* IntersectionOfWalls::copy() const
{
    return new IntersectionOfWalls(*this);
}

void IntersectionOfWalls::clear()
{
    objects_.clear();
}

///Adds a wall to the set of finite walls, given an outward normal vector s.t. normal*x=normal*point
void IntersectionOfWalls::addObject(Vec3D normal, Vec3D point)
{
    addObject(normal, Vec3D::dot(normal, point));
}
///Adds a wall to the set of finite walls, given an outward normal vector s. t. normal*x=position
void IntersectionOfWalls::addObject(Vec3D normal_, Mdouble position_)
{
    //n is the index of the new wall
    unsigned int n = objects_.size();
    objects_.resize(n + 1);
    objects_[n].set(normal_, position_);
    
    // AB[n*(n-1)/2+m] is the direction of the intersecting line between walls m and n, m<n
    // A[n*(n-1)/2+m] is a point on the intersecting line between walls m and n, m<n
    // See http://www.netcomuk.co.uk/~jenolive/vect18d.html for finding the line where two planes meet
    AB_.resize(n * (n + 1) / 2);
    A_.resize(n * (n + 1) / 2);
    for (unsigned int m = 0; m < n; m++)
    {
        unsigned int id = (n - 1) * n / 2 + m;
        //first we Vec3D::Cross the wall normals and normalize to obtain AB
        AB_[id] = Vec3D::cross(objects_[m].get_Normal(), objects_[n].get_Normal());
        AB_[id] /= sqrt(AB_[id].getLengthSquared());
        //then we find a point A (using AB*x=0 as a third plane)
        Mdouble invdet = 1.0 / (+objects_[n].get_Normal().X * (objects_[m].get_Normal().Y * AB_[id].Z - AB_[id].Y * objects_[m].get_Normal().Z)
                - objects_[n].get_Normal().Y * (objects_[m].get_Normal().X * AB_[id].Z - objects_[m].get_Normal().Z * AB_[id].X)
                + objects_[n].get_Normal().Z * (objects_[m].get_Normal().X * AB_[id].Y - objects_[m].get_Normal().Y * AB_[id].X));
        A_[id] = Vec3D(+(objects_[m].get_Normal().Y * AB_[id].Z - AB_[id].Y * objects_[m].get_Normal().Z) * objects_[n].getPosition()
                - (objects_[n].get_Normal().Y * AB_[id].Z - objects_[n].get_Normal().Z * AB_[id].Y) * objects_[m].getPosition()
                + (objects_[n].get_Normal().Y * objects_[m].get_Normal().Z - objects_[n].get_Normal().Z * objects_[m].get_Normal().Y) * 0.0,
                -(objects_[m].get_Normal().X * AB_[id].Z - objects_[m].get_Normal().Z * AB_[id].X) * objects_[n].getPosition()
                        + (objects_[n].get_Normal().X * AB_[id].Z - objects_[n].get_Normal().Z * AB_[id].X) * objects_[m].getPosition()
                        - (objects_[n].get_Normal().X * objects_[m].get_Normal().Z - objects_[m].get_Normal().X * objects_[n].get_Normal().Z) * 0.0,
                +(objects_[m].get_Normal().X * AB_[id].Y - AB_[id].X * objects_[m].get_Normal().Y) * objects_[n].getPosition()
                        - (objects_[n].get_Normal().X * AB_[id].Y - AB_[id].X * objects_[n].get_Normal().Y) * objects_[m].getPosition()
                        + (objects_[n].get_Normal().X * objects_[m].get_Normal().Y - objects_[m].get_Normal().X * objects_[n].get_Normal().Y) * 0.0) * invdet;
    }
    
    // C[(n-2)*(n-1)*n/6+(m-1)*m/2+l] is a point intersecting walls l, m and n, l<m<n
    C_.resize((n - 1) * n * (n + 1) / 6);
    for (unsigned int m = 0; m < n; m++)
    {
        for (unsigned int l = 0; l < m; l++)
        {
            unsigned int id = (n - 2) * (n - 1) * n / 6 + (m - 1) * m / 2 + l;
            Mdouble invdet = 1.0 / (+objects_[n].get_Normal().X * (objects_[m].get_Normal().Y * objects_[l].get_Normal().Z - objects_[l].get_Normal().Y * objects_[m].get_Normal().Z)
                    - objects_[n].get_Normal().Y * (objects_[m].get_Normal().X * objects_[l].get_Normal().Z - objects_[m].get_Normal().Z * objects_[l].get_Normal().X)
                    + objects_[n].get_Normal().Z * (objects_[m].get_Normal().X * objects_[l].get_Normal().Y - objects_[m].get_Normal().Y * objects_[l].get_Normal().X));
            C_[id] = Vec3D(+(objects_[m].get_Normal().Y * objects_[l].get_Normal().Z - objects_[l].get_Normal().Y * objects_[m].get_Normal().Z) * objects_[n].getPosition()
                    - (objects_[n].get_Normal().Y * objects_[l].get_Normal().Z - objects_[n].get_Normal().Z * objects_[l].get_Normal().Y) * objects_[m].getPosition()
                    + (objects_[n].get_Normal().Y * objects_[m].get_Normal().Z - objects_[n].get_Normal().Z * objects_[m].get_Normal().Y) * objects_[l].getPosition(),
                    -(objects_[m].get_Normal().X * objects_[l].get_Normal().Z - objects_[m].get_Normal().Z * objects_[l].get_Normal().X) * objects_[n].getPosition()
                            + (objects_[n].get_Normal().X * objects_[l].get_Normal().Z - objects_[n].get_Normal().Z * objects_[l].get_Normal().X) * objects_[m].getPosition()
                            - (objects_[n].get_Normal().X * objects_[m].get_Normal().Z - objects_[m].get_Normal().X * objects_[n].get_Normal().Z) * objects_[l].getPosition(),
                    +(objects_[m].get_Normal().X * objects_[l].get_Normal().Y - objects_[l].get_Normal().X * objects_[m].get_Normal().Y) * objects_[n].getPosition()
                            - (objects_[n].get_Normal().X * objects_[l].get_Normal().Y - objects_[l].get_Normal().X * objects_[n].get_Normal().Y) * objects_[m].getPosition()
                            + (objects_[n].get_Normal().X * objects_[m].get_Normal().Y - objects_[m].get_Normal().X * objects_[n].get_Normal().Y) * objects_[l].getPosition()) * invdet;
        }
    }
}

void IntersectionOfWalls::createOpenPrism(std::vector<Vec3D> Points, Vec3D PrismAxis)
{
    objects_.clear();
    for (unsigned int i = 0; i < Points.size() - 1; i++)
        addObject(Vec3D::cross(Points[i] - Points[i + 1], PrismAxis), Points[i]);
}

void IntersectionOfWalls::createPrism(std::vector<Vec3D> Points, Vec3D PrismAxis)
{
    createOpenPrism(Points, PrismAxis);
    addObject(Vec3D::cross(Points.back() - Points.front(), PrismAxis), Points.front());
}

void IntersectionOfWalls::createOpenPrism(std::vector<Vec3D> Points)
{
    Vec3D PrismAxis = Vec3D::cross(
            Vec3D::getUnitVector(Points[1] - Points[0]),
            Vec3D::getUnitVector(Points[2] - Points[1]));
    objects_.clear();
    for (unsigned int i = 0; i < Points.size() - 1; i++)
        addObject(Vec3D::cross(Points[i] - Points[i + 1], PrismAxis), Points[i]);
}

void IntersectionOfWalls::createPrism(std::vector<Vec3D> Points)
{
    Vec3D PrismAxis = Vec3D::cross(
            Vec3D::getUnitVector(Points[2] - Points[0]),
            Vec3D::getUnitVector(Points[1] - Points[0]));
    createOpenPrism(Points, PrismAxis);
    addObject(Vec3D::cross(Points.back() - Points.front(), PrismAxis), Points.front());
}

bool IntersectionOfWalls::getDistanceAndNormal(const BaseParticle &P, Mdouble &distance, Vec3D &normal_return) const
{
    return getDistanceAndNormal(P.getPosition(), P.getInteractionRadius(), distance, normal_return);
}

///Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
bool IntersectionOfWalls::getDistanceAndNormal(const Vec3D& position, Mdouble  wallInteractionRadius, Mdouble &distance, Vec3D &normal_return) const
{
    static Mdouble distance_new;
    static Mdouble distance2;
    static Mdouble distance3;
    unsigned static int id;
    unsigned static int id2;
    unsigned static int id3;
    
    //if we are here, this is a finite wall
    distance = -1e20;
    distance2 = -1e20;
    distance3 = -1e20;
    //for all finite walls
    for (unsigned int i = 0; i < objects_.size(); i++)
    {
        //calculate the distance to the particle
        distance_new = objects_[i].get_distance(position);
        //return false if the distance to any one wall is too large (i.e. no contact)
        if (distance_new >= wallInteractionRadius)
            return false;
        //store the minimum distance and the respective wall in "distance" and "id"
        //and store up to two walls (id2, id3) and their distances (distance2, distance3), if the possible contact point is near the intersection between id and id2 (and id3)
        if (distance_new > distance)
        {
            if (distance > -wallInteractionRadius)
            {
                if (distance2 > -wallInteractionRadius)
                {
                    distance3 = distance;
                    id3 = id;
                }
                else
                {
                    distance2 = distance;
                    id2 = id;
                }
            }
            distance = distance_new;
            id = i;
        }
        else if (distance_new > -wallInteractionRadius)
        {
            if (distance2 > -wallInteractionRadius)
            {
                distance3 = distance_new;
                id3 = i;
            }
            else
            {
                distance2 = distance_new;
                id2 = i;
            }
        }
    }
    
    //if we are here, the closest wall is id; 
    //if distance2>-P.Radius (and distance3>-P.Radius), the possible contact point is near the intersection between id and id2 (and id3)
    if (distance2 > -wallInteractionRadius)
    {
        //D is the point on wall id closest to P
        Vec3D D = position + objects_[id].get_Normal() * distance;
        //If the distance of D to id2 is positive, the contact is with the intersection
        bool intersection_with_id2 = (objects_[id2].get_distance(D) > 0.0);
        
        if (distance3 > -wallInteractionRadius && (objects_[id3].get_distance(D) > 0.0))
        {
            if (intersection_with_id2)
            {
                //possible contact is with intersection of id,id2,id3
                //we know id2<id3
                unsigned int index =
                        (id < id2) ? ((id3 - 2) * (id3 - 1) * id3 / 6 + (id2 - 1) * id2 / 2 + id) :
                        (id < id3) ? ((id3 - 2) * (id3 - 1) * id3 / 6 + (id - 1) * id / 2 + id2) :
                                     ((id - 2) * (id - 1) * id / 6 + (id3 - 1) * id3 / 2 + id2);
                normal_return = position - C_[index];
                distance = sqrt(normal_return.getLengthSquared());
                if (distance >= wallInteractionRadius)
                    return false; //no contact
                normal_return /= -distance;
                return true; //contact with id,id2,id3
            }
            else
            {
                intersection_with_id2 = true;
                distance2 = distance3;
                id2 = id3;
            }
        }
        
        if (intersection_with_id2)
        { //possible contact is with intersection of id,id2
            unsigned int index = (id > id2) ? ((id - 1) * id / 2 + id2) : ((id2 - 1) * id2 / 2 + id);
            Vec3D AC = position - A_[index];
            normal_return = AC - AB_[index] * Vec3D::dot(AC, AB_[index]);
            distance = sqrt(normal_return.getLengthSquared());
            if (distance >= wallInteractionRadius)
                return false; //no contact
            normal_return /= -distance;
            return true; //contact with two walls
        }
    }
    //contact is with id
    normal_return = objects_[id].get_Normal();
    return true;
    
}

///reads wall
void IntersectionOfWalls::read(std::istream& is)
{
    BaseWall::read(is);
    std::string dummy;
    int n;
    is >> dummy >> n;
    
    Vec3D normal;
    Mdouble position;
    for (int i = 0; i < n; i++)
    {
        is >> dummy >> normal >> dummy >> position;
        addObject(normal, position);
    }
}
    

///reads wall
void IntersectionOfWalls::oldRead(std::istream& is)
{
    std::string dummy;
    int n;
    is >> dummy >> n;
    
    Vec3D normal;
    Mdouble position;
    for (int i = 0; i < n; i++)
    {
        is >> dummy >> normal >> dummy >> position;
        addObject(normal, position);
    }
    Vec3D velocity;
    is >> dummy >> velocity;
    setVelocity(velocity);
}

///outputs wall
void IntersectionOfWalls::write(std::ostream& os) const
        {
    BaseWall::write(os);
    os << " numIntersectionOfWallss " << objects_.size();
    for (std::vector<InfiniteWall>::const_iterator it = objects_.begin(); it != objects_.end(); ++it)
    {
        os << " normal " << it->get_Normal() << " position " << it->getPosition();
    }
}

std::string IntersectionOfWalls::getName() const
{
    return "IntersectionOfWalls";
}

BaseInteraction* IntersectionOfWalls::getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)
{
    Mdouble distance;
    Vec3D normal;

    if (getDistanceAndNormal(*P,distance,normal))
    {
        BaseInteraction* C = interactionHandler->getInteraction(P, this, timeStamp);
        C->setNormal(-normal);
        C->setDistance(distance);
        C->setOverlap(P->getRadius() - distance);
        ///todo{DK: What is the contact point for interactions with walls}
        C->setContactPoint(P->getPosition()-(P->getRadius()- 0.5 * C->getOverlap())*C->getNormal());
        return C;
    }
    else
    {
        return 0;
    }
}
