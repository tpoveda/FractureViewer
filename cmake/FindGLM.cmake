SET( GLM_SEARCH_PATHS
	${GLM_ROOT_DIR}						# GLM!
	./lib/glm
	$ENV{PROGRAMFILES}/glm				# WINDOWS
	"$ENV{PROGRAMFILES(X86)}/glm"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( GLM_INCLUDE_DIRS
	NAMES
	glm.hpp
	PATHS
		${GLM_SEARCH_PATHS}
	PATH_SUFFIXES
		glm
	DOC
		"El directorio donde glm.hpp reside"
)

# Check if we found it!
IF ( GLM_INCLUDE_DIRS )
	SET( GLM_FOUND TRUE )
	MESSAGE(STATUS "Looking for glm - found")
ELSE ( GLM_INCLUDE_DIRS )
	SET( GLM_FOUND FALSE )
	MESSAGE(STATUS "Looking for glm - not found")
ENDIF ( GLM_INCLUDE_DIRS AND GLM_LIBRARIES )