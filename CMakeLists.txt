cmake_minimum_required ( VERSION 2.8 )
project ( Lavie )

include_directories( plugins )
add_subdirectory( plugins )


add_executable( Lavie
	http.cpp
	irc.cpp
	main.cpp
	network.cpp
	plugins.cpp
	stringUtils.cpp
	main.cpp
)

target_link_libraries( Lavie plugins )
