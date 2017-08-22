SET( ASSIMP_SEARCH_PATHS
	${ASSIMP_ROOT_DIR}					# ASSIMP!
	./lib/ASSIMP
	$ENV{PROGRAMFILES}/ASSIMP				# WINDOWS
	"$ENV{PROGRAMFILES(X86)}/ASSIMP"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( ASSIMP_INCLUDE_DIRS
	NAMES
	Importer.hpp
	PATHS
		${ASSIMP_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where Importer.h resides"
)

FIND_LIBRARY( ASSIMP_LIBRARIES
	NAMES
		assimp
	PATHS
		${ASSIMP_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The ASSIMP library"
)

# Check if we found it!
IF ( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )
	SET( ASSIMP_FOUND TRUE )
	MESSAGE(STATUS "Looking for ASSIMP - found")
ELSE ( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )
	SET( ASSIMP_FOUND FALSE )
	MESSAGE(STATUS "Looking for ASSIMP - not found")
ENDIF ( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )