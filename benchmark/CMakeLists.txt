get_filename_component(MONOLITH_BENCH_SOURCE_PATH "./${PROJECT_NAME}" ABSOLUTE)

set(MONOLITH_BENCH_TARGET_BASE "${PROJECT_NAME}-bench")

# ффффффффф
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0") # Для отладки
# set(CMAKE_CXX_FLAGS_RELEASE "-O0")
# set(CMAKE_CXX_FLAGS_RELEASE "-O2")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -funroll-loops -ffast-math") 


# Создать файл прав для подписи (entitlements)
set(ENTITLEMENTS_PATH "${CMAKE_BINARY_DIR}/entitlements.plist")
file(WRITE ${ENTITLEMENTS_PATH} "
<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">
<plist version=\"1.0\">
<dict>
    <key>com.apple.security.get-task-allow</key>
    <true/>
</dict>
</plist>
")

macro(MONOLITH_BENCH_DECLARE suffix file)
    add_executable(
        ${MONOLITH_BENCH_TARGET_BASE}-${suffix}
        ${MONOLITH_BENCH_SOURCE_PATH}/${file}
        ${MONOLITH_BENCH_SOURCE_PATH}/Fun.cpp
    )

    target_link_libraries(
        ${MONOLITH_BENCH_TARGET_BASE}-${suffix}
        ${PROJECT_NAME}
        benchmark::benchmark
    )

    # отладочные флаги для компиляции
    target_compile_options(
        ${MONOLITH_BENCH_TARGET_BASE}-${suffix}
        PRIVATE
        -g -O0
    )

    # шаг для подписи бинарного файла
    add_custom_command(
        TARGET ${MONOLITH_BENCH_TARGET_BASE}-${suffix} POST_BUILD
        COMMAND codesign --force --sign - --entitlements ${ENTITLEMENTS_PATH} $<TARGET_FILE:${MONOLITH_BENCH_TARGET_BASE}-${suffix}>
        COMMENT "Signing ${MONOLITH_BENCH_TARGET_BASE}-${suffix} for Instruments"
    )
endmacro()

MONOLITH_BENCH_DECLARE(binary-search BinarySearchBench.cpp)
MONOLITH_BENCH_DECLARE(search-name SearchByNameBench.cpp)
MONOLITH_BENCH_DECLARE(multi MultiThreadedBench.cpp)
