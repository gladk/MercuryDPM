include_directories(${Mercury_SOURCE_DIR}/src
		    ${Mercury_BINARY_DIR}/src)

#Basic library without hGrid
add_library(DPMBase STATIC
	STD_save.cc
	ExtendedMath.cc
	WallHandler.cc
	BoundaryHandler.cc
	ParticleHandler.cc
	Particles/BaseParticle.cc
	#The follow two should maybe removed from base at some point
	Particles/TangentialSpringParticle.cc
	Particles/DeltaMaxsParticle.cc
	MD.cc
	#HGRID.cc
)

#Could later create a 2D and 3D library seperatly, but sure how many we want to create
add_library(MercuryBase STATIC
	HGRID.cc
	HGRID_base.cc
	HGRID_2D.cc
	HGRID_3D.cc
	HGridOptimiser.cc)

target_link_libraries(MercuryBase DPMBase)

#Required for Chute drivers
add_library(Chute STATIC
	Chute.cc)
target_link_libraries(Chute MercuryBase)
