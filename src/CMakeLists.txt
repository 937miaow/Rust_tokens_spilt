# 添加源文件
set(SOURCES
    tokens.cpp
)

# 添加头文件目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 创建库
add_library(tokenizer STATIC ${SOURCES})

# 设置输出文件名
set_target_properties(tokenizer PROPERTIES
    OUTPUT_NAME "tokenizer"
)

# 添加编译选项
if(MSVC)
    target_compile_options(tokenizer PRIVATE /W4 /utf-8)
endif()

# 安装目标
install(TARGETS tokenizer
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
) 