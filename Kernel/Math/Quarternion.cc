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

#include "Quarternion.h"

#include "Vector.h"
#include "Matrix.h"
#include "ExtendedMath.h"

Quarternion::Quarternion()
{
    reset();
}

Quarternion::Quarternion(const Quarternion& other)
{
    x_ = other.x_;
    y_ = other.y_;
    z_ = other.z_;
    w_ = other.w_;
}

Quarternion::Quarternion(double x, double y, double z, double w)
{
    x_ = x;
    y_ = y;
    z_ = z;
    w_ = w;
}

Quarternion::Quarternion(Vec3D axis, double angle)
{
    std::cout << "Axis=" << axis << " Angle=" << angle << std::endl;
    axis.normalize();
    x_ = std::sin(0.5 * angle) * axis.X;
    y_ = std::sin(0.5 * angle) * axis.Y;
    z_ = std::sin(0.5 * angle) * axis.Z;
    w_ = std::cos(0.5 * angle);
}

Quarternion::Quarternion(const Vec3D& a)
{
    x_ = sin(0.5 * a.X) * cos(0.5 * a.Y) * cos(0.5 * a.Z) - cos(0.5 * a.X) * sin(0.5 * a.Y) * sin(0.5 * a.Z);
    y_ = cos(0.5 * a.X) * sin(0.5 * a.Y) * cos(0.5 * a.Z) + sin(0.5 * a.X) * cos(0.5 * a.Y) * sin(0.5 * a.Z);
    z_ = cos(0.5 * a.X) * cos(0.5 * a.Y) * sin(0.5 * a.Z) - sin(0.5 * a.X) * sin(0.5 * a.Y) * cos(0.5 * a.Z);
    w_ = cos(0.5 * a.X) * cos(0.5 * a.Y) * cos(0.5 * a.Z) + sin(0.5 * a.X) * sin(0.5 * a.Y) * sin(0.5 * a.Z);
}

void Quarternion::getAxisAndAngle(Vec3D& axis, double& angle) const
        {
    angle = 2.0 * std::acos(w_);
    axis = Vec3D(x_ / std::sin(0.5 * angle), y_ / std::sin(0.5 * angle), z_ / std::sin(0.5 * angle));
}

Matrix3D Quarternion::getRotationMatrix() const
{
    double xx = w_ * w_ + x_ * x_ - y_ * y_ - z_ * z_;
    double xy = 2.0 * (x_ * y_ - w_ * z_);
    double xz = 2.0 * (w_ * y_ + x_ * z_);
    double yx = 2.0 * (x_ * y_ + w_ * z_);
    double yy = w_ * w_ - x_ * x_ + y_ * y_ - z_ * z_;
    double yz = 2.0 * (y_ * z_ - w_ * x_);
    double zx = 2.0 * (x_ * z_ - w_ * y_);
    double zy = 2.0 * (w_ * x_ + y_ * z_);
    double zz = w_ * w_ - x_ * x_ - y_ * y_ + z_ * z_;
    return Matrix3D(xx, xy, xz, yx, yy, yz, zx, zy, zz);
}

void Quarternion::reset()
{
    x_ = 0.0;
    y_ = 0.0;
    z_ = 0.0;
    w_ = 1.0;
}

void Quarternion::normalise()
{
    double n = std::sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
    x_ /= n;
    y_ /= n;
    z_ /= n;
    w_ /= n;
}

Quarternion Quarternion::operator *(const Quarternion &other) const
        {
    Quarternion result;
    result.w_ = w_ * other.w_ - x_ * other.x_ - y_ * other.y_ - z_ * other.z_;
    result.x_ = w_ * other.x_ + x_ * other.w_ + y_ * other.z_ - z_ * other.y_;
    result.y_ = w_ * other.y_ + y_ * other.w_ - x_ * other.z_ + z_ * other.x_;
    result.z_ = w_ * other.z_ + z_ * other.w_ + x_ * other.y_ - y_ * other.x_;
    return result;
}

void Quarternion::integrate(const Vec3D& omega, double timeStep)
{
    Quarternion deltaQ;
    Vec3D theta = 0.5 * omega * timeStep;
    double thetaMagSq = theta.getLengthSquared();
    double s;
    if (thetaMagSq * thetaMagSq / 24.0 < 1e-10)
    {
        std::cout << "Warning small rotation, needs additional checking" << std::endl;
        deltaQ.w_ = 1.0 - 0.5 * thetaMagSq;
        s = 1.0 - thetaMagSq / 6.0;
    }
    else
    {
        double thetaMag = std::sqrt(thetaMagSq);
        deltaQ.w_ = std::cos(thetaMag);
        s = std::sin(thetaMag) / thetaMag;
    }
    deltaQ.x_ = theta.X * s;
    deltaQ.y_ = theta.Y * s;
    deltaQ.z_ = theta.Z * s;
    ///\todo Are we sure this works? *this = deltaQ * *this \author weinhartt
    *this = deltaQ * *this;
}

Vec3D Quarternion::get3DAngles() const
{
    double rotY, rotZ, rotX;
    double sqrtX = x_ * x_;
    double sqrtY = y_ * y_;
    double sqrtZ = z_ * z_;
    double sqrtW = w_ * w_;
    double unit = sqrtX + sqrtY + sqrtZ + sqrtW;
    double test = x_ * y_ + z_ * w_;
    if (test > 0.499999 * unit) // singularity at north pole
    {
        rotY = 2.0 * std::atan2(x_, w_);
        rotZ = constants::pi / 2;
        rotX = 0;
    }
    else if (test < -0.499999 * unit) // singularity at south pole
    {
        rotY = -2.0 * atan2(x_, w_);
        rotZ = -constants::pi / 2;
        rotX = 0;
    }
    else
    {
        rotY = atan2(2.0 * y_ * w_ - 2.0 * x_ * z_, sqrtX - sqrtY - sqrtZ + sqrtW);
        rotZ = asin(2.0 * test / unit);
        rotX = atan2(2.0 * x_ * w_ - 2.0 * y_ * z_, -sqrtX + sqrtY - sqrtZ + sqrtW);
    }
    return Vec3D(rotX, rotY, rotZ);
}

std::ostream& operator<<(std::ostream& os, const Quarternion &q)
{
    os << q.x_ << " " << q.y_ << " " << q.z_ << " " << q.w_;
    return os;
}

std::istream& operator>>(std::istream& is, Quarternion &q)
{
    is >> q.x_ >> q.y_ >> q.z_ >> q.w_;
    return is;
}

