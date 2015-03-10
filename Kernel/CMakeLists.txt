#Include directories where the source files and the libraries are stored
include_directories(
	${Mercury_SOURCE_DIR}/Kernel
	${Mercury_BINARY_DIR}/Kernel
)

#Basic library without hGrid
add_library(DPMBase STATIC
	Math/ExtendedMath.cc
	Math/Helpers.cc
	Math/Vector.cc
	Math/Matrix.cc
	Math/MatrixSymmetric.cc
	Math/RNG.cc
	Math/Quarternion.cc
	
	BaseObject.cc
    BaseInteractable.cc
	Logger.cc
	File.cc
	Files.cc
	STD_save.cc
	BoundaryHandler.cc
	InteractionHandler.cc
	ParticleHandler.cc
	SpeciesHandler.cc
	WallHandler.cc
	DPMBase.cc

	Boundaries/BaseBoundary.cc
	Boundaries/AngledPeriodicBoundary.cc
	Boundaries/ChuteInsertionBoundary.cc
	Boundaries/CircularPeriodicBoundary.cc
	Boundaries/CubeInsertionBoundary.cc	
	Boundaries/DeletionBoundary.cc
	Boundaries/PeriodicBoundary.cc
	Boundaries/HopperInsertionBoundary.cc
	Boundaries/InsertionBoundary.cc
	Boundaries/MaserBoundary.cc
	
	Walls/BaseWall.cc
	Walls/Coil.cc
	Walls/CylindricalWall.cc
	Walls/IntersectionOfWalls.cc
	Walls/AxisymmetricIntersectionOfWalls.cc
	Walls/InfiniteWall.cc
	Walls/InfiniteWallWithHole.cc
	Walls/Screw.cc

	Interactions/BaseInteraction.cc
	Interactions/LinearViscoelasticInteraction.cc
	Interactions/LinearPlasticViscoelasticInteraction.cc
	Interactions/HertzianViscoelasticInteraction.cc
	Interactions/TangentialForceInteractions/EmptyTangentialInteraction.cc
    Interactions/TangentialForceInteractions/SlidingFrictionInteraction.cc
    Interactions/TangentialForceInteractions/FrictionInteraction.cc
    Interactions/AdhesiveForceInteractions/EmptyAdhesiveInteraction.cc
    Interactions/AdhesiveForceInteractions/LinearReversibleAdhesiveInteraction.cc
    Interactions/AdhesiveForceInteractions/LinearIrreversibleAdhesiveInteraction.cc

	Species/BaseSpecies.cc
    Species/LinearViscoelasticSpecies.cc
    Species/LinearPlasticViscoelasticSpecies.cc
    Species/HertzianViscoelasticSpecies.cc
    Species/TangentialForceSpecies/EmptyTangentialSpecies.cc
    Species/TangentialForceSpecies/SlidingFrictionSpecies.cc
    Species/TangentialForceSpecies/FrictionSpecies.cc
    Species/AdhesiveForceSpecies/EmptyAdhesiveSpecies.cc
    Species/AdhesiveForceSpecies/LinearReversibleAdhesiveSpecies.cc
    Species/AdhesiveForceSpecies/LinearIrreversibleAdhesiveSpecies.cc

	Particles/BaseParticle.cc
	#The follow two should maybe removed from base at some point
	Particles/TangentialSpringParticle.cc
	Particles/DeltaMaxsParticle.cc
)

#Could later create a 2D and 3D library seperatly, but sure how many we want to create
add_library(MercuryBase STATIC
	HGrid.cc
	MercuryBase.cc
	Mercury2D.cc
	Mercury3D.cc
	HGridOptimiser.cc)

target_link_libraries(MercuryBase DPMBase)

#Required for Chute drivers
add_library(Chute STATIC
	ChuteBottom.cc
	ChuteWithHopper.cc
	#ChuteWithHopperAndInset_copy_of_ChuteWithHopper.cc
	Chute.cc
)

target_link_libraries(Chute MercuryBase)
