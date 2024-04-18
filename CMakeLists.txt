project(gitsandbox)

########
add_library(core
	core.cpp
)


########
add_library(foo
	moduleFoo.cpp
)
target_link_libraries(foo
	core
)

########
add_library(bar
	moduleBar.cpp
)
target_link_libraries(bar
	core
)

########
add_executable(a.out
	main.cpp
)
target_link_libraries(a.out
	foo
	bar
)
