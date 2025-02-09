cmake_minimum_required(VERSION 3.8)
project(ros2_bluerobotics_navigator)

# Enable stricter warnings if using GCC or Clang
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# --- 1) Standard ROS 2 ament_cmake boilerplate ---
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)     # <-- Add sensor_msgs

# --- 2) If you have a Rust output directory and library, set it here ---
set(RUST_OUT_DIR "/home/pi/navigator-lib/target/debug")
include_directories("${RUST_OUT_DIR}")

# --- 3) Create your node executable ---
add_executable(talker src/publisher_lambda_function.cpp)

#     Make sure to add sensor_msgs to the dependencies as well:
ament_target_dependencies(talker 
  rclcpp
  std_msgs
  sensor_msgs
)

# --- 4) Link the Rust library (if needed) ---
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(LIB_EXT "dylib")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(LIB_EXT "dll")
else()
  set(LIB_EXT "so")
endif()

target_link_libraries(
  talker
  "${RUST_OUT_DIR}/libbluerobotics_navigator.${LIB_EXT}"
)

# --- 5) Install your executable so ROS 2 can run it ---
install(TARGETS
  talker
  DESTINATION lib/${PROJECT_NAME}
)

# --- 6) (Optional) Build tests, add linting, etc ---
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_cpplint_FOUND TRUE)
  set(ament_cmake_uncrustify_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
