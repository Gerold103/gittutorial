project(gitsandbox)

########
add_library(foo
	moduleFoo.cpp
)

########
add_library(bar
	moduleBar.cpp
)

########
add_executable(a.out
	main.cpp
)
target_link_libraries(a.out
	foo
	bar
)
