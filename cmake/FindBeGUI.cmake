SET( BEGUI_SEARCH_PATHS
	${BEGUI_ROOT_DIR}					# BEGUI!
	./lib/begui
	$ENV{PROGRAMFILES}/begui				# WINDOWS
	"$ENV{PROGRAMFILES(X86)}/begui"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( BEGUI_INCLUDE_DIRS
	NAMES
	bgui.h
	PATHS
		${BEGUI_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where bgui.h resides"
)

FIND_LIBRARY( BEGUI_LIBRARY1
	NAMES
		bgui
	PATHS
		${BEGUI_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The bgui library"
)

FIND_LIBRARY( BEGUI_LIBRARY2
	NAMES
		bcore
	PATHS
		${BEGUI_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The bcore library"
)

# Check if we found it!
IF ( BEGUI_INCLUDE_DIRS AND BEGUI_LIBRARY1 AND  BEGUI_LIBRARY2)
	SET( BEGUI_FOUND TRUE )
	MESSAGE(STATUS "Looking for BEGUI - found")
ELSE ( BEGUI_INCLUDE_DIRS AND BEGUI_LIBRARY1 AND BEGUI_LIBRARY2 )
	SET( BEGUI_FOUND FALSE )
	MESSAGE(STATUS "Looking for BEGUI - not found")
ENDIF ( BEGUI_INCLUDE_DIRS AND BEGUI_LIBRARIES )