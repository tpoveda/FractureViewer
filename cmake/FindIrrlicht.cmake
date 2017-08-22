
SET( IRRLICHT_SEARCH_PATHS
	${IRRLICHT_ROOT_DIR}					# IRRLICHT!
	./lib/irrlicht
	$ENV{PROGRAMFILES}/irrlicht				# WINDOWS
	"$ENV{PROGRAMFILES(X86)}/irrlicht"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( IRRLICHT_INCLUDE_DIRS
	NAMES
	irrlicht.h
	PATHS
		${IRRLICHT_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where Irrlicht resides"
)

FIND_LIBRARY( IRRLICHT_LIBRARIES
	NAMES
		Irrlicht
	PATHS
		${IRRLICHT_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
		lib/Win32-visualstudio
	DOC
		"The Irrlicht library"
)

# Check if we found it!
IF ( IRRLICHT_INCLUDE_DIRS AND IRRLICHT_LIBRARIES)
	SET( IRRLICHT_FOUND TRUE )
	MESSAGE(STATUS "Looking for IRRLICHT - found")
ELSE ( IRRLICHT_INCLUDE_DIRS AND IRRLICHT_LIBRARIES)
	SET( IRRLICHT_FOUND FALSE )
	MESSAGE(STATUS "Looking for IRRLICHT - not found")
ENDIF ( IRRLICHT_INCLUDE_DIRS AND IRRLICHT_LIBRARIES )
