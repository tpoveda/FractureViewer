
SET( BULLET_SEARCH_PATHS
	${BULLET_ROOT_DIR}					# BULLET!
	./lib/bullet
	$ENV{PROGRAMFILES}/bullet				# WINDOWS
	"$ENV{PROGRAMFILES(X86)}/bullet"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( BULLET_INCLUDE_DIR1
	NAMES
		btBulletCollisionCommon.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	DOC
		"The directory where Bullet Collision resides"
)

FIND_PATH( BULLET_INCLUDE_DIR11
	NAMES
		btDispatcher.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletCollision/BroadphaseCollision
	DOC
		"The directory where Bullet Collision resides"
)

FIND_PATH( BULLET_INCLUDE_DIR12
	NAMES
		btBoxBoxDetector.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletCollision/CollisionDispatch
	DOC
		"The directory where Bullet Collision resides"
)
FIND_PATH( BULLET_INCLUDE_DIR13
	NAMES
		btBoxShape.h
	PATHS
		${BULLET_SEARCH_PATHS}
	PATH_SUFFIXES
		BulletCollision/CollisionShapes
	DOC
		"The directory where Bullet Collision resides"
)
FIND_PATH( BULLET_INCLUDE_DIR14
	NAMES
		btGImpactShape.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletCollision/Gimpact
	DOC
		"The directory where Bullet Collision resides"
)
FIND_PATH( BULLET_INCLUDE_DIR15
	NAMES
		btGjkEpa2.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletCollision/NarrowPhaseCollision
	DOC
		"The directory where Bullet Collision resides"
)

FIND_PATH( BULLET_INCLUDE_DIR2
	NAMES
		btBulletDynamicsCommon.h	
	PATHS
		${BULLET_SEARCH_PATHS}
	DOC
		"The directory where Bullet Dynamics resides"
)
FIND_PATH( BULLET_INCLUDE_DIR21
	NAMES
		btCharacterControllerInterface.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletDynamics/Character
	DOC
		"The directory where Bullet Collision resides"
)
FIND_PATH( BULLET_INCLUDE_DIR22
	NAMES
		btConstraintSolver.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletDynamics/ConstraintSolver
	DOC
		"The directory where Bullet Collision resides"
)
FIND_PATH( BULLET_INCLUDE_DIR23
	NAMES
		btDynamicsWorld.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletDynamics/Dynamics
	DOC
		"The directory where Bullet Collision resides"
)
FIND_PATH( BULLET_INCLUDE_DIR24
	NAMES
		btMultiBody.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletDynamics/Featherstone
	DOC
		"The directory where Bullet Collision resides"
)
FIND_PATH( BULLET_INCLUDE_DIR25
	NAMES
		btDantzigSolver.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletDynamics/MLCPSolvers
	DOC
		"The directory where Bullet Collision resides"
)
FIND_PATH( BULLET_INCLUDE_DIR26
	NAMES
		btRaycastVehicle.h
	PATHS
		${BULLET_SEARCH_PATHS}	
	PATH_SUFFIXES
		BulletDynamics/Vehicle
	DOC
		"The directory where Bullet Collision resides"
)

FIND_PATH( BULLET_INCLUDE_DIR3
	NAMES
		btList.h	
	PATHS
		${BULLET_SEARCH_PATHS}
	PATH_SUFFIXES
		LinearMath		
	DOC
		"The directory where Bullet Linear Math resides"
)

FIND_LIBRARY( BULLET_LIBRARY1
	NAMES
		BulletCollision
	PATHS
		${BULLET_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
		lib/Win32-visualstudio
	DOC
		"The Bullet Collision module lib"
)

FIND_LIBRARY( BULLET_LIBRARY2
	NAMES
		BulletDynamics
	PATHS
		${BULLET_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
		lib/Win32-visualstudio
	DOC
		"The Bullet Dynamics module lib"
)

FIND_LIBRARY( BULLET_LIBRARY3
	NAMES
		LinearMath
	PATHS
		${BULLET_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
		lib/Win32-visualstudio
	DOC
		"The Bullet Linear Math module lib"
)

# Check if we found it!
IF ( BULLET_INCLUDE_DIR1 AND BULLET_INCLUDE_DIR2 AND BULLET_INCLUDE_DIR3 AND BULLET_LIBRARY1 AND BULLET_LIBRARY2 AND BULLET_LIBRARY3)
	SET( BULLET_FOUND TRUE )
	MESSAGE(STATUS "Looking for BULLET - found")
ELSE (  BULLET_INCLUDE_DIR1 AND BULLET_INCLUDE_DIR2 AND BULLET_INCLUDE_DIR3 AND BULLET_LIBRARY1 AND BULLET_LIBRARY2 AND BULLET_LIBRARY3)
	SET( BULLET_FOUND FALSE )
	MESSAGE(STATUS "Looking for BULLET - not found")
ENDIF (  BULLET_INCLUDE_DIR1 AND BULLET_INCLUDE_DIR2 AND BULLET_INCLUDE_DIR3 AND BULLET_LIBRARY1 AND BULLET_LIBRARY2 AND BULLET_LIBRARY3 )
