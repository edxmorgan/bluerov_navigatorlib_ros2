# ROS2 Navigator

This project integrates ROS2 with Blue Robotics' Navigator library using Rust bindings. Follow the steps below to set up your BlueOS Raspberry Pi, build the necessary components, and run the project.

## Prerequisites

1. **BlueOS with ROS2**  
   Install ROS2 on your BlueOS Raspberry Pi by following the guide here:  
   [rpi-ros2](https://github.com/Ar-Ray-code/rpi-bullseye-ros2)

2. **Navigator Library Rust Bindings**  
   Build the Rust bindings for the Navigator library. Detailed instructions are available in the repository:  
   [navigator-lib C++/Rust binding examples](https://github.com/bluerobotics/navigator-lib/tree/master/examples/cpp)

## Setup Instructions

### 1. Build the Rust Bindings

Follow the steps in the [navigator-lib repository](https://github.com/bluerobotics/navigator-lib/tree/master/examples/cpp) to build the Rust bindings. This process will generate the required output in a directory (e.g., `/home/pi/navigator-lib/target/debug`).

### 2. Update the CMakeLists.txt

In this project's `CMakeLists.txt`, update the path to point to the built Rust binding directory:

```cmake
set(RUST_OUT_DIR "/home/pi/navigator-lib/target/debug")
```

### 3. Configure the Library Path

Ensure the system can locate the built libraries by updating the `LD_LIBRARY_PATH`. To make this change persistent, add the following line to your `.bashrc`:

```bash
export LD_LIBRARY_PATH="/home/pi/navigator-lib/target/debug:$LD_LIBRARY_PATH"
```

After editing `.bashrc`, load the changes with:

```bash
source ~/.bashrc
```

### 4. Build the Project with Colcon

Build the project using `colcon` from the root of your workspace:

```bash
colcon build
```

### 5. Run the Project

Run the project via SSH on your Raspberry Pi. Additionally, you can configure the project to run on startup using your preferred method (e.g., systemd or crontab).

## Additional Configuration

- **Startup Configuration**  
  To automatically start the project on boot, configure your startup scripts (such as creating a systemd service or adding a cron job).

- **Troubleshooting**  
  - Verify that the path in `CMakeLists.txt` matches the output directory from your Rust build.
  - Ensure that the `LD_LIBRARY_PATH` is correctly set and exported.
  - Check the build logs from `colcon` for any errors.

---