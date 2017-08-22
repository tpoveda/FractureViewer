SET( RAKNET_SEARCH_PATHS
	${RAKNET_ROOT_DIR}					# RAKNET!
	./lib/RakNet
	$ENV{PROGRAMFILES}/RakNet			# WINDOWS
	"$ENV{PROGRAMFILES(X86)}/RakNet"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( RAKNET_INCLUDE_DIR
	NAMES
	RakNetDefines.h
	PATHS
		${RAKNET_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where RakNet resides"
)


FIND_LIBRARY( RAKNET_LIBRARIES
	NAMES
		RakNet
	PATHS
		${RAKNET_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The RakNet library"
)

# Check if we found it!
IF ( RAKNET_INCLUDE_DIR AND RAKNET_LIBRARIES )
	SET( RAKNET_FOUND TRUE )
	MESSAGE(STATUS "Looking for RAKNET - found")
ELSE ( RAKNET_INCLUDE_DIR AND RAKNET_LIBRARIES )
	SET( RAKNET_FOUND FALSE )
	MESSAGE(STATUS "Looking for RAKNET - not found")
ENDIF ( RAKNET_INCLUDE_DIR AND RAKNET_LIBRARIES )