cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(task)

option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#
# CRY stuff
#
set(CRY_PATH /home/beso/cry_v1.7)
set(CRY_LIB -L${CRY_PATH}/lib libCRY.a) 
include_directories(${CRY_PATH}/src)

# We presume the existence of three directories
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB_RECURSE macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)

set(macros
cmd.file
  init_vis.mac
  run2.mac
  vis.mac
  Pyramid_of_Khafre.stl
WLS_Ascii.stl
EJ200Scibar_Ascii.stl  
SiPM_Ascii.stl
EJ200Scibar_singlesipm_Ascii.stl
EJ200Scibar_singlewire_Ascii.stl
)

# Enable macros for out-of-source build
foreach(_file ${macros})
  configure_file(
    ${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
    )
endforeach()

add_executable(task main.cc ${sources} ${headers})
target_link_libraries(task ${Geant4_LIBRARIES} ${CRY_LIB})

install(TARGETS task DESTINATION bin)
