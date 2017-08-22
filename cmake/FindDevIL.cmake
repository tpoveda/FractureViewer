SET( DevIL_SEARCH_PATHS
	${DevIL_ROOT_DIR}					# DevIL!
	./lib/DevIL
	$ENV{PROGRAMFILES}/DevIL				# WINDOWS
	"$ENV{PROGRAMFILES(X86)}/DevIL"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( DevIL_INCLUDE_DIRS
	NAMES
	includeDevil.hpp
	PATHS
		${DevIL_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where Importer.h resides"
)

FIND_LIBRARY( DevIL_LIBRARIES
	NAMES
		DevIL
	PATHS
		${DevIL_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The DevIL library"
)

FIND_LIBRARY( DevIL_LIBRARIES2
	NAMES
		ILU
	PATHS
		${DevIL_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The DevIL library"
)

FIND_LIBRARY( DevIL_LIBRARIES3
	NAMES
		ILUT
	PATHS
		${DevIL_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The DevIL library"
)


# Check if we found it!
IF ( DevIL_INCLUDE_DIRS AND DevIL_LIBRARIES AND DevIL_LIBRARIES2 AND DevIL_LIBRARIES3)
	SET( DevIL_FOUND TRUE )
	MESSAGE(STATUS "Looking for DevIL - found")
ELSE ( DevIL_INCLUDE_DIRS AND DevIL_LIBRARIES AND DevIL_LIBRARIES2 AND DevIL_LIBRARIES3)
	SET( DevIL_FOUND FALSE )
	MESSAGE(STATUS "Looking for DevIL - not found")
ENDIF ( DevIL_INCLUDE_DIRS AND DevIL_LIBRARIES AND DevIL_LIBRARIES2 AND DevIL_LIBRARIES3)