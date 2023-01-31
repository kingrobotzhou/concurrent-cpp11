
add_executable( demo1
    main.cpp
)

target_compile_features (demo1
	PRIVATE 
		${PRJ_COMPILE_FEATURES}
)