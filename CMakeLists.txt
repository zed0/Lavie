cmake_minimum_required ( VERSION 2.8 )
project ( Lavie )

# enable some warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra")

include_directories( plugins )

add_executable( Lavie
	http.cpp
	irc.cpp
	main.cpp
	network.cpp
	plugins.cpp
	stringUtils.cpp
	plugins/aliasPlugin.cpp
	plugins/quizPlugin.cpp
	plugins/timePlugin.cpp
	main.cpp
)
