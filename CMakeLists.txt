cmake_minimum_required(VERSION 2.8)
project(ObjectDetection)

find_package( OpenCV REQUIRED )
find_package(PCL 1.7 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})

link_directories(${PCL_LIBRARY_DIRS})

add_definitions(${PCL_DEFINITIONS})

add_executable(image_proc src/image_proc.cpp)
target_link_libraries(image_proc ${OpenCV_LIBS})

add_executable(frame_diff src/frame_diff.cpp)
target_link_libraries(frame_diff ${OpenCV_LIBS})


add_executable(pcl_segmentation src/pcl_segmentation.cpp)
target_link_libraries(pcl_segmentation ${PCL_LIBRARIES})

add_executable(openni_grabber src/openni_grabber.cpp)
target_link_libraries(openni_grabber ${PCL_LIBRARIES})

add_executable(openni_viewer_simple src/openni_viewer_simple.cpp)
target_link_libraries(openni_viewer_simple ${PCL_LIBRARIES})


add_executable(savePCD src/savePCD.cpp)
target_link_libraries(savePCD ${PCL_LIBRARIES})

set(PCL_BUILD_TYPE Release)
 
file(GLOB PCL_openni_viewer_SRC
    src/OpenniPclViewer.cpp
)
add_executable(openniViewer ${PCL_openni_viewer_SRC})
 target_link_libraries (openniViewer ${PCL_LIBRARIES})

add_executable(pcl_write src/pcl_write.cpp)
target_link_libraries (pcl_write ${PCL_LIBRARIES})

