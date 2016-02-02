set(SOURCES
    "src/pause_runtime.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/pause_runtime.h"
)

include_directories(include)
add_library(pause_runtime MODULE ${SOURCES} ${HEADERS})
target_link_libraries(pause_runtime PRIVATE lmscore)
