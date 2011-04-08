#
# CernVM installer 
# Author: Artem Harutyunyan <hartem NOSPAM gmail.com>
#
# Bulid system is based on:
# CMakeQt: CMake build system template for Qt based projects
# Author: Rohit Yadav <rohityadav89 AT gmail.com>
#

#Put here CPPs
SET(CVMINSTALLER_SRCS
    main.cpp
)

SET(CVMINSTALLER_HDRS
)

#Put here QRC files to be Rcc-ed
SET(CVMINSTALLER_RCC
    CVMIDialog.qrc
    CVMIAboutPage.qrc
)

#include paths for vlmc
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/src
  .
)

#CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
set(CMAKE_CXX_FLAGS "-g -Wall")

LIST( APPEND CVMINSTALLER_SRCS
             CVMIDialog.cpp
             CVMIControlVBox.cpp
             CVMIConfigPage.cpp
             CVMIConfig.cpp
             CVMICreatePage.cpp
             CVMISelectPage.cpp
             CVMIInstallPage.cpp
             CVMIAboutPage.cpp
    )

LIST( APPEND CVMINSTALLER_HDRS
             CVMIDialog.h
             CVMIControlVM.h
             CVMIControlVBox.h
             CVMIConfigPage.h
             CVMIConfig.h
             CVMICreatePage.h
             CVMISelectPage.h
             CVMIInstallPage.h
             CVMIAboutPage.h
    )

QT4_WRAP_UI(CVMINSTALLER_UIS_H ${CVMINSTALLER_UIS})
QT4_ADD_RESOURCES(CVMINSTALLER_RCC_SRCS ${CVMINSTALLER_RCC})
QT4_WRAP_CPP(CVMINSTALLER_MOC_SRCS ${CVMINSTALLER_HDRS})
   
IF( MINGW )
    # resource compilation for MinGW
    ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icon.o
        COMMAND ${CMAKE_WINDRES} -I${CMAKE_SOURCE_DIR} -i${CMAKE_SOURCE_DIR}/cmake/icon.rc
        -o ${CMAKE_CURRENT_BINARY_DIR}/icon.o )
    SET(CVMINSTALLER_SRCS ${CVMINSTALLER_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/icon.o)  
    SET(LINK_FLAGS -Wl,-subsystem,windows)
ENDIF ( MINGW )

    #ENDIF( MINGW )

IF(WIN32)
    SET(GUI_TYPE WIN32)
ENDIF(WIN32)

IF(APPLE)
    SET(GUI_TYPE MACOSX_BUNDLE)
    SET(CVMINSTALLER_SRCS ${CVMINSTALLER_SRCS} ${CMAKE_SOURCE_DIR}/share/cernvm-installer.icns)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_SOURCE_DIR}/share/cernvm-installer.icns PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources)
    #SET(MACOSX_BUNDLE_ICON_FILE cernvm-installer.icns)
    SET_TARGET_PROPERTIES (PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/share/Info.plist")
ENDIF(APPLE)

ADD_EXECUTABLE(cernvm-installer ${CVMINSTALLER_SRCS} ${CVMINSTALLER_MOC_SRCS} ${CVMINSTALLER_UIS_H} ${CVMINSTALLER_RCC_SRCS} )

#SET(QT_USE_QTNETWORK TRUE)

#Link the target against the Qt libraries
TARGET_LINK_LIBRARIES(cernvm-installer ${CVMINSTALLER_LIBS})
SET_TARGET_PROPERTIES(cernvm-installer PROPERTIES COMPILE_FLAGS -m32 LINK_FLAGS -m32)
#Now we are done. Easy with CMake!!!

# --- Install
INSTALL(TARGETS cernvm-installer
        BUNDLE  DESTINATION ${CVMINSTALLER_BIN_DIR}
        RUNTIME DESTINATION ${CVMINSTALLER_BIN_DIR})
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_SOURCE_DIR}/bin/cernvm-installer
    COMMAND ${CMAKE_COMMAND} copy ${CMAKE_CURRENT_SOURCE_DIR}/cernvm-installer ${CMAKE_SOURCE_DIR}/bin/cernvm-installer
    DEPENDS ${CMAKE_BINARY_DIR}/src/cernvm-installer
)


ADD_DEFINITIONS(-W -Wall)

# Creates app bundle in /bin, uncomment #-dmg to create a dmg output 
IF( APPLE )
    SET(CERNVM_INSTALLER_BUNDLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/../bin/CernVM Installer.app/")

    ADD_CUSTOM_COMMAND(
        TARGET cernvm-installer
        POST_BUILD
        COMMAND mkdir -p ${CERNVM_INSTALLER_BUNDLE_DIR}/Contents/MacOS
        COMMAND mkdir -p ${CERNVM_INSTALLER_BUNDLE_DIR}/Contents/Resources
        #COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/cernvm-installer.app/Contents/MacOS
        COMMAND cp  ${CMAKE_SOURCE_DIR}/build/bin/cernvm-installer "${CERNVM_INSTALLER_BUNDLE_DIR}/Contents/MacOS/CernVM Installer"
        COMMAND cp  ${CMAKE_SOURCE_DIR}/share/Info.plist ${CERNVM_INSTALLER_BUNDLE_DIR}/Contents/
        COMMAND cp  ${CMAKE_SOURCE_DIR}/share/cernvm-installer.icns ${CERNVM_INSTALLER_BUNDLE_DIR}/Contents/Resources
)

    ADD_CUSTOM_COMMAND(
        TARGET cernvm-installer 
        POST_BUILD
        #COMMAND macdeployqt ${CMAKE_CURRENT_BINARY_DIR}/../bin/cernvm-installer.app -dmg #-no-plugins -dmg
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        #COMMAND macdeployqt '${CERNVM_INSTALLER_BUNDLE_DIR}' 
        #COMMAND macdeployqt "${CMAKE_SOURCE_DIR}/bin/CernVM\ Installer.app"  #CERNVM_INSTALLER_BUNDLE_DIR}' 
        #COMMENT "Deploying the Qt Framework onto the bundle"
    )
ENDIF( APPLE )


