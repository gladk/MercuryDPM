#Include directories where the source files and the libraries are stored
include_directories(
	${Mercury_SOURCE_DIR}/Kernel
	${Mercury_BINARY_DIR}/Kernel
)

#Basic library without hGrid
set (DPMBase_src
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
	FilesAndRunNumber.cc
	DPMBase.cc

	BoundaryHandler.cc
	InteractionHandler.cc
	ParticleHandler.cc
	SpeciesHandler.cc
	WallHandler.cc

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
	Boundaries/LeesEdwardsBoundary.cc
	
	Walls/BaseWall.cc
	Walls/Coil.cc
	Walls/CylindricalWall.cc
	Walls/IntersectionOfWalls.cc
	Walls/AxisymmetricIntersectionOfWalls.cc
	Walls/InfiniteWall.cc
	Walls/InfiniteWallWithHole.cc
	Walls/Screw.cc

	Interactions/BaseInteraction.cc
	Interactions/NormalForceInteractions/LinearViscoelasticInteraction.cc
	Interactions/NormalForceInteractions/LinearPlasticViscoelasticInteraction.cc
	Interactions/NormalForceInteractions/HertzianViscoelasticInteraction.cc
	Interactions/FrictionForceInteractions/EmptyFrictionInteraction.cc
    Interactions/FrictionForceInteractions/SlidingFrictionInteraction.cc
    Interactions/FrictionForceInteractions/FrictionInteraction.cc
    Interactions/AdhesiveForceInteractions/EmptyAdhesiveInteraction.cc
    Interactions/AdhesiveForceInteractions/ReversibleAdhesiveInteraction.cc
    Interactions/AdhesiveForceInteractions/IrreversibleAdhesiveInteraction.cc
    Interactions/AdhesiveForceInteractions/LiquidBridgeWilletInteraction.cc

	Species/BaseSpecies.cc
	Species/ParticleSpecies.cc
    Species/NormalForceSpecies/LinearViscoelasticNormalSpecies.cc
    Species/NormalForceSpecies/LinearPlasticViscoelasticNormalSpecies.cc
    Species/NormalForceSpecies/HertzianViscoelasticNormalSpecies.cc
    Species/FrictionForceSpecies/EmptyFrictionSpecies.cc
    Species/FrictionForceSpecies/SlidingFrictionSpecies.cc
    Species/FrictionForceSpecies/FrictionSpecies.cc
    Species/AdhesiveForceSpecies/EmptyAdhesiveSpecies.cc
    Species/AdhesiveForceSpecies/ReversibleAdhesiveSpecies.cc
    Species/AdhesiveForceSpecies/IrreversibleAdhesiveSpecies.cc
    Species/AdhesiveForceSpecies/LiquidBridgeWilletSpecies.cc

	Particles/BaseParticle.cc
    ${Mercury_BINARY_DIR}/Kernel/CMakeDefinitions.cc
)

add_library(DPMBase STATIC ${DPMBase_src})

if(Mercury_BACKTRACE_DEMANGLE)
  target_link_libraries(DPMBase dl)
endif()

#Could later create a 2D and 3D library seperatly, but sure how many we want to create
set(MercuryBase_src
	HGrid.cc
	MercuryBase.cc
	Mercury2D.cc
	Mercury3D.cc
	HGridOptimiser.cc)

add_library(MercuryBase STATIC ${MercuryBase_src})
target_link_libraries(MercuryBase DPMBase)

#Required for Chute drivers
set(Chute_src
	ChuteBottom.cc
	ChuteWithHopper.cc
	#ChuteWithHopperAndInset_copy_of_ChuteWithHopper.cc
	Chute.cc
)
add_library(Chute STATIC ${Chute_src})

target_link_libraries(Chute MercuryBase)

install(TARGETS MercuryBase DPMBase Chute DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(headersList "."
  "Boundaries"
  "Interactions"
  "Interactions/AdhesiveForceInteractions"
  "Interactions/FrictionForceInteractions"
  "Interactions/NormalForceInteractions"
  "Math"
  "Particles"
  "Species"
  "Species/AdhesiveForceSpecies"
  "Species/FrictionForceSpecies"
  "Species/FrictionForceSpecies"
  "Species/NormalForceSpecies"
  "Walls")

foreach(pathCur ${headersList})
  FILE(GLOB Head_LIB  "${pathCur}/*.h")
  INSTALL(FILES ${Head_LIB} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MercuryDPM/${pathCur}/")
endforeach(pathCur ${headersList})
