# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("scavenger")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
# 需要编辑的文件
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        scavenger.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/assertions.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/autolink.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/config.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/features.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/forwards.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/json.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/json_reader.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/json_tool.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/json_value.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/json_valueiterator.inl
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/json_writer.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/reader.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/value.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/version.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/json/writer.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/mmap/mmfile.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/mmap/mmfile.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/storage_handler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/storage/storage_handler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/utils/CoordinateUtils.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/utils/CoordinateUtils.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/utils/PathSmoothTool.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/utils/PathSmoothTool.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/model/cpoint.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/model/cpoint.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/model/sensor_value.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/model/sensor_value.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/handler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/handler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/kalman_handler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/kalman_handler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/shake_cheat_handler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/shake_cheat_handler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/default_cheat_handler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/default_cheat_handler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/pace_handler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/pace_handler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/climb_handler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/climb_handler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/calorie_handler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/calorie_handler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/kalman/KalmanTool.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/kalman/KalmanTool.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/cheat/CheatSensorHandler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/cheat/CheatSensorHandler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/mileage_handler.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/handler/mileage_handler.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/manager.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/manager.cpp
        ../../../../../ios-scavenger/Scaverger/CPP/src/model/result_point.h
        ../../../../../ios-scavenger/Scaverger/CPP/src/model/result_point.cpp
        )

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log)