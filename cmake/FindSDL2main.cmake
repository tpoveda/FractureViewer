SET( SDL2MAIN_SEARCH_PATHS
	${SDL2MAIN_ROOT_DIR}				# SDL2!
	./lib/SDL2
	$ENV{PROGRAMFILES}/SDL2				# WINDOWS
	"$ENV{PROGRAMFILES(X86)}/SDL2"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_LIBRARY( SDL2MAIN_LIBRARIES
	NAMES
		SDL2main
	PATHS
		${SDL2MAIN_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The SDL2main library"
)

# Check if we found it!
IF ( SDL2MAIN_INCLUDE_DIRS AND SDL2MAIN_LIBRARIES )
	SET( SDL2MAIN_FOUND TRUE )
	MESSAGE(STATUS "Looking for SDL2main - found")
ELSE ( SDL2MAIN_INCLUDE_DIRS AND SDL2MAIN_LIBRARIES )
	SET( SDL2MAIN_FOUND FALSE )
	MESSAGE(STATUS "Looking for SDL2main - not found")
ENDIF ( SDL2MAIN_INCLUDE_DIRS AND SDL2MAIN_LIBRARIES )