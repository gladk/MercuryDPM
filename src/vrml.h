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

/* Author: Vitaliy Ogarko, vogarko@gmail.com, 2009 */

void CylinderTransformer(Vec3D pointA, Vec3D pointB, Mdouble* length, Mdouble* angle, Vec3D* translation, Vec3D* axis)
{

	*length = GetDistance(pointA, pointB);
        
	// the translation (center of the cylinder) needed is
	*translation = (pointA + pointB) / 2.0f;

	//the initial orientation of the bond is in the y axis
	Vec3D init = Vec3D(0.0f, 1.0f, 0.0f);
        
	//the vector needed is the same as that from a to b
	Vec3D needed = Vec3D(pointB.X - pointA.X, pointB.Y - pointA.Y, pointB.Z - pointA.Z);
        

	Vec3D needed_n, init_n; // normilized

	needed_n	= needed / needed.GetLength();
	init_n		= init / init.GetLength();

	//so the angle to rotate the bond by is:
	*angle = acos(Dot(needed_n, init_n));

	
	//and the axis to rotate by is the cross product of the initial and
	//needed vectors - ie the vector orthogonal to them both
	Mdouble vx = init.Y * needed.Z - init.Z * needed.Y;
	Mdouble vy = init.Z * needed.X - init.X * needed.Z;
	Mdouble vz = init.X * needed.Y - init.Y * needed.X;

	*axis = Vec3D(vx, vy, vz);

}

// Export packing configuration to the VRML format.
void Export_to_VRML_format2(std::string fname)
{
	ofstream grout(fname.c_str());	

	grout << "\
#VRML V2.0 utf8 \n\n\
Group {\n\
  children [\n\
    WorldInfo {\n\
      title \"Granular Media\"\n\
      info [\n\
        \"Packing configuration.\"\n\
        \"Author: Vitaliy Ogarko; vogarko@gmail.com\"\n\
      ]\n\
    }\n\
    NavigationInfo {\n\
      type [ \"EXAMINE\", \"ANY\" ]\n\
    }\n\
    Background {\n\
      skyColor [ 1 1 1 ]\n\
    }\n\
  ]\n\
}\n\n";

/*
	grout << "\
Shape {\n\
    appearance Appearance {\n\
        material Material {\n\
            emissiveColor 0 0 0\n\
        }\n\
    }\n\
    geometry IndexedLineSet {\n\
        coord Coordinate {\n\
            point [\n\
            # Coordinates around the top of the cube\n\
                   0  1.0  1.0,\n\
                 1.0  1.0  1.0,\n\
                 1.0  1.0    0,\n\
                   0  1.0    0,\n\
            # Coordinates around the bottom of the cube\n\
                   0    0  1.0,\n\
                 1.0    0  1.0,\n\
                 1.0    0    0,\n\
                   0    0    0\n\
            ]\n\
        }\n\
        coordIndex [\n\
        # top\n\
            0, 1, 2, 3, 0, -1,\n\
        # bottom\n\
            4, 5, 6, 7, 4, -1,\n\
        # vertical edges\n\
            0, 4, -1,\n\
            1, 5, -1,\n\
            2, 6, -1,\n\
            3, 7\n\
        ]\n\
    }\n\
}\n\n";
*/


////// (+) DRAW CUBE -----------------------------------

	Vec3D translation, axis;
	Mdouble length, angle;

	Vec3D pointsA[12];
	Vec3D pointsB[12];

	pointsA[0] = Vec3D(0,0,0); pointsB[0] = Vec3D(1,0,0);
	pointsA[1] = Vec3D(0,0,0); pointsB[1] = Vec3D(0,1,0);
	pointsA[2] = Vec3D(0,0,0); pointsB[2] = Vec3D(0,0,1);

	pointsA[3] = Vec3D(1,0,0); pointsB[3] = Vec3D(1,0,1);
	pointsA[4] = Vec3D(1,0,0); pointsB[4] = Vec3D(1,1,0);

	pointsA[5] = Vec3D(0,1,0); pointsB[5] = Vec3D(0,1,1);
	pointsA[6] = Vec3D(0,1,0); pointsB[6] = Vec3D(1,1,0);

	pointsA[7] = Vec3D(0,0,1); pointsB[7] = Vec3D(0,1,1);
	pointsA[8] = Vec3D(0,0,1); pointsB[8] = Vec3D(1,0,1);

	pointsA[9]  = Vec3D(1,1,1); pointsB[9]  = Vec3D(0,1,1);
	pointsA[10] = Vec3D(1,1,1); pointsB[10] = Vec3D(1,1,0);
	pointsA[11] = Vec3D(1,1,1); pointsB[11] = Vec3D(1,0,1);


	for(int i=0; i<12; i++)
	{

		CylinderTransformer(pointsA[i], pointsB[i], &length, &angle, &translation, &axis);

		if (i == 11) { angle = 0.; } // without this a stick becomes black colored

	        grout << "\
Transform{\n\
    translation " << translation.X << " " << translation.Y << " " << translation.Z << "\n\
    rotation " << axis.X << " " << axis.Y << " " << axis.Z << " " << angle << "\n\
        children  Shape {\n\
        appearance Appearance {\n\
            material Material { diffuseColor 0.8 0.8 0.8 }\n\
        }\n\
        geometry Cylinder {\n\
            radius 0.005\n\
            height " << length << "\n\
        }}}\n\n";


	} // for


	grout << "\
Transform {\n\
	translation 0 0 0\n\
	children Shape {\n\
		geometry Sphere {\n\
		radius 0.005\n\
		}\n\
		appearance Appearance {\n\
			material Material { diffuseColor 0.8 0.8 0.8\n\
}}}}\n\n";

	grout << "\
Transform {\n\
	translation 1 0 0\n\
	children Shape {\n\
		geometry Sphere {\n\
		radius 0.005\n\
		}\n\
		appearance Appearance {\n\
			material Material { diffuseColor 0.8 0.8 0.8\n\
}}}}\n\n";

	grout << "\
Transform {\n\
	translation 0 1 0\n\
	children Shape {\n\
		geometry Sphere {\n\
		radius 0.005\n\
		}\n\
		appearance Appearance {\n\
			material Material { diffuseColor 0.8 0.8 0.8\n\
}}}}\n\n";

	grout << "\
Transform {\n\
	translation 0 0 1\n\
	children Shape {\n\
		geometry Sphere {\n\
		radius 0.005\n\
		}\n\
		appearance Appearance {\n\
			material Material { diffuseColor 0.8 0.8 0.8\n\
}}}}\n\n";

	grout << "\
Transform {\n\
	translation 1 1 0\n\
	children Shape {\n\
		geometry Sphere {\n\
		radius 0.005\n\
		}\n\
		appearance Appearance {\n\
			material Material { diffuseColor 0.8 0.8 0.8\n\
}}}}\n\n";

	grout << "\
Transform {\n\
	translation 1 0 1\n\
	children Shape {\n\
		geometry Sphere {\n\
		radius 0.005\n\
		}\n\
		appearance Appearance {\n\
			material Material { diffuseColor 0.8 0.8 0.8\n\
}}}}\n\n";

	grout << "\
Transform {\n\
	translation 0 1 1\n\
	children Shape {\n\
		geometry Sphere {\n\
		radius 0.005\n\
		}\n\
		appearance Appearance {\n\
			material Material { diffuseColor 0.8 0.8 0.8\n\
}}}}\n\n";

	grout << "\
Transform {\n\
	translation 1 1 1\n\
	children Shape {\n\
		geometry Sphere {\n\
		radius 0.005\n\
		}\n\
		appearance Appearance {\n\
			material Material { diffuseColor 0.8 0.8 0.8\n\
}}}}\n\n";

////// (-) DRAW CUBE -----------------------------------



// http://vrmlworks.crispen.org/orient.html >> calculate oriantation for a viewpoint

	grout << "\
Viewpoint {\n\
	 position    -0.9 1.6 3\n\
	 orientation -0.572612659264449 -0.806095467134217 -0.14941499361097152 0.6261167634082665\n\
	 fieldOfView 0.785398\n\
         description \"Viewpoint 1\"\n\
}\n\n";


	Mdouble r, c1, c2, c3;

	for(int i=0; i<Nmax; i++)
	{
	
		grout << "Transform {" << "\n";
		grout << "translation ";
		
		//grout << Particles[i]->Position << "\n";
		grout << Px[i] << ' ' << Py[i] << ' ' << Pz[i] << "\n";

		grout << "children Shape {" << "\n";
		grout << "geometry Sphere {" << "\n";
		grout << "radius ";

		//grout << Particles[i]->Radius << "\n";
		grout << Pr[i] << "\n";

		grout << "} \n";


		grout << "appearance Appearance {" << "\n";

		//grout << "material Material { diffuseColor 0.915064 0 0.0849365 }" << "\n";
		grout << "material Material { diffuseColor ";
		
		
		//Mdouble r = (Particles[i]->Radius - MinRadius) / (MaxRadius - MinRadius);

		Mdouble transparency = 0.2;

		if (PolyType == 1)
		{
			c1 = 0.5;
			c2 = 0.9;
			c3 = 0.9;
		} else
		if (PolyType == 2)
		{
			// small particle
			if (Pr[i] == MinRadius)
			{
				c1 = 0.5;
				c2 = 0.9;
				c3 = 0.9;
			} else
			{
				c1 = 0.9;
				c2 = 0.1;
				c3 = 0.9;
			}
		} else
		{
		
			r = (Pr[i] - MinRadius) / (MaxRadius - MinRadius);

			c3 = r;
			c2 = 4*r*(1-r);
			c1 = 1-r;


			// !!! (Big - blue, small - yellow)
			c3 = r;
			c1 = 1-r;
			c2 = 1-r;

			transparency = 0;

		}
		

		/*
		Mdouble r = pow(GetDistance(Particles[i]->Position, Vec3D(0.5, 0.5, 0.5)), 2./3.) / sqrt(0.75);
		
		Mdouble c3 = r;
		Mdouble c2 = 4*r*(1-r);
		Mdouble c1 = 1-r;
		*/

		//grout << c1 << " " << c2 << " " << c3 << " }\n";
		grout << c1 << " " << c2 << " " << c3 << "\n";

		grout << "	transparency " << transparency << "\n}\n";

		grout << "} \n";
		grout << "} \n";
		grout << "} \n";
		grout << "\n";

	} 


	grout.close();

}
