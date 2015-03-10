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

#include<iostream>
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "MercuryTime.h"

//not really useful functions as it does not return anything; simply for speed testing.

Mdouble passByReference(Vec3D& v)
{
    return v.X;
}

Mdouble passByConstReference(const Vec3D& v)
{
    return v.X;
}

Mdouble passByPointer(Vec3D* v)
{
    return v->X;
}

Mdouble passByValue(Vec3D v)
{
    return v.X;
}

Mdouble passByReference(Matrix3D& v)
{
    return v.XX;
}

Mdouble passByConstReference(const Matrix3D& v)
{
    return v.XX;
}

Mdouble passByPointer(Matrix3D* v)
{
    return v->XX;
}

Mdouble passByValue(Matrix3D v)
{
    return v.XX;
}

void testVec3D(unsigned int n)
{
    std::cout << "testing Vec3D performance" << std::endl;
    
    Vec3D v={1,0,0};
    Mdouble x=0;
    
    Time time;
    time.tic();
    for (unsigned int i = 0; i < n; i++)
    {
        x+=passByReference(v);
    }
    std::cout << "passByReference      clocks: " << time.toc()*CLOCKS_PER_SEC << std::endl;

    time.tic();
    for (unsigned int i = 0; i < n; i++)
    {
        x+=passByConstReference(v);
    }
    std::cout << "passByConstReference clocks: " << time.toc()*CLOCKS_PER_SEC << std::endl;

    Vec3D* p = &v;
    time.tic();
    for (unsigned int i = 0; i < n; i++)
    {
        x+=passByPointer(p);
    }
    std::cout << "passByPointer        clocks: " << time.toc()*CLOCKS_PER_SEC << std::endl;
    
    time.tic();
    for (unsigned int i = 0; i < n; i++)
    {
        x+=passByValue(v);
    }
    std::cout << "passByValue          clocks: " << time.toc()*CLOCKS_PER_SEC << std::endl;
    std::cout << x << std::endl;
}

void testMatrix3D(unsigned int n)
{
    std::cout << "testing Matrix3D performance" << std::endl;

    Matrix3D v={1,0,0,0,0,0,0,0,0};
    Mdouble x=0;
    
    Time time;
    time.tic();
    for (unsigned int i = 0; i < n; i++)
    {
        x+=passByReference(v);
    }
    std::cout << "passByReference      clocks: " << time.toc()*CLOCKS_PER_SEC << std::endl;

    time.tic();
    for (unsigned int i = 0; i < n; i++)
    {
        x+=passByConstReference(v);
    }
    std::cout << "passByConstReference clocks: " << time.toc()*CLOCKS_PER_SEC << std::endl;

    Matrix3D* p = &v;
    time.tic();
    for (unsigned int i = 0; i < n; i++)
    {
        x+=passByPointer(p);
    }
    std::cout << "passByPointer        clocks: " << time.toc()*CLOCKS_PER_SEC << std::endl;
    
    time.tic();
    for (unsigned int i = 0; i < n; i++)
    {
        x+=passByValue(v);
    }
    std::cout << "passByValue          clocks: " << time.toc()*CLOCKS_PER_SEC << std::endl;
    std::cout << x << std::endl;
}

int main()
{
    unsigned int n = 1e8;
    testVec3D(n);
    testMatrix3D(n);
    return 0;
}