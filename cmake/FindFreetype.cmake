SET( FREETYPE_SEARCH_PATHS
	${FREETYPE_ROOT_DIR}					# FREETYPE!
	./lib/freetype
	$ENV{PROGRAMFILES}/freetype			# WINDOWS
	"$ENV{PROGRAMFILES(X86)}/freetype"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( FREETYPE_INCLUDE_DIR1
	NAMES
	ft2build.h
	PATHS
		${FREETYPE_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where ft2build.h resides"
)

FIND_PATH( FREETYPE_INCLUDE_DIR2
	NAMES
		free.h
	PATHS
		${FREETYPE_SEARCH_PATHS}
	PATH_SUFFIXES
		include/freetype2/
	DOC
		"The directory where ftheader.h resides"
)

FIND_LIBRARY( FREETYPE_LIBRARIES
	NAMES
		freetype
	PATHS
		${FREETYPE_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The FREETYPE library"
)

# Check if we found it!
IF ( FREETYPE_INCLUDE_DIR1 AND FREETYPE_INCLUDE_DIR2 AND FREETYPE_LIBRARIES )
	SET( FREETYPE_FOUND TRUE )
	MESSAGE(STATUS "Looking for FREETYPE - found")
ELSE ( FREETYPE_INCLUDE_DIR1 AND FREETYPE_INCLUDE_DIR2 AND FREETYPE_LIBRARIES )
	SET( FREETYPE_FOUND FALSE )
	MESSAGE(STATUS "Looking for FREETYPE - not found")
ENDIF ( FREETYPE_INCLUDE_DIRS AND FREETYPE_LIBRARIES )