INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(plugins
	SHARED
	aliasPlugin.cpp
	quizPlugin.cpp
	timePlugin.cpp
)
