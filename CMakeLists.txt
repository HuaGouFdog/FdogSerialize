CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

add_definitions(-w)


PROJECT(main)
aux_source_directory(. ALL_SRCS) # 添加当前目录下所有的源文件
aux_source_directory(./example/ ALL_SRCS) # 添加当前目录下所有的源文件
add_executable(main ${ALL_SRCS})	# 指定生成目标
