cmake_minimum_required(VERSION 3.21)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")

########################################################################################################################
## Define project
########################################################################################################################
project(
        OStim
        VERSION 1.0.0
        DESCRIPTION ""
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

include(GNUInstallDirs)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.rc
        @ONLY)

set(headers
        include/Header/Header.h)

set(sources
    src/Alignment/ActorKey.cpp
    src/Alignment/Alignments.cpp
    src/Alignment/ThreadKey.cpp
    src/Core/Core.cpp
    src/Core/EquipObjectHandler.cpp
    src/Core/ExpressionUpdater.cpp
    src/Core/Singleton.h
    src/Core/Thread/ThreadAutoControl.cpp
    src/Core/Thread.cpp
    src/Core/ThreadActor.cpp
    src/Core/ThreadManager.cpp
    src/Core/ThreadManager.h
    src/Events/EventListener.cpp
    src/Furniture/Furniture.cpp
    src/Furniture/FurnitureTable.cpp
    src/Game/LocaleManager.cpp
	src/Game/LocaleManager.h
	src/Game/Locker.h
	src/Game/Patch.h
    src/GameAPI/Game.cpp
    src/GameAPI/GameActor.cpp
    src/GameAPI/GameCamera.cpp
    src/GameAPI/GameSex.cpp
    src/GameAPI/GameSound.cpp
    src/GameAPI/GameUtil.cpp
    src/GameAPI/GameTable.cpp
    src/Graph/Action.cpp
    src/Graph/GraphTable.cpp
    src/Graph/Node.cpp
    src/InterfaceSpec/PluginInterface.cpp
    src/MCM/MCMSetting.cpp
    src/MCM/MCMTable.cpp
    src/Messaging/MessagingRegistry.cpp
	src/PCH.h
	src/Papyrus/Papyrus.cpp
	src/Papyrus/Script.h
	src/Serial/Manager.cpp
	src/Sound/ActionSound/BoneDistanceSoundPlayer.cpp
	src/Sound/ActionSound/BoneDistanceSoundType.cpp
	src/Sound/ActionSound/LoopSoundPlayer.cpp
	src/Sound/ActionSound/LoopSoundType.cpp
	src/Sound/ActionSound/SingleBoneDistanceSoundPlayer.cpp
	src/Sound/ActionSound/MultiBoneDistanceSoundPlayer.cpp
	src/Sound/ActionSound/SoundPlayer.cpp
	src/Sound/ActionSound/SoundType.cpp
	src/Sound/SoundTable.cpp
    src/Trait/Condition.cpp
    src/Trait/FacialExpression.cpp
    src/Trait/TraitTable.cpp
    src/UI/Align/AlignMenu.cpp
    src/UI/Align/IncrementValue.cpp
    src/UI/UIState.cpp
    src/Util/ActorUtil.cpp
    src/Util/CameraUtil.cpp
    src/Util/CompatibilityTable.cpp
    src/Util/ControlUtil.cpp
    src/Util/FormUtil.cpp
    src/Util/LookupTable.cpp
    src/Util/ObjectRefUtil.cpp
    src/Util/SerializationUtil.cpp
    src/Util/StringUtil.cpp
    src/Util/VectorUtil.cpp
	src/Util.cpp
	src/main.cpp

        ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${headers}
        ${sources})

########################################################################################################################
## Configure target DLL
########################################################################################################################
find_package(pugixml REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CommonLibSSE CONFIG REQUIRED)
find_package(ryml CONFIG REQUIRED)
find_path(ARTICUNO_INCLUDE_DIRS "articuno/articuno.h")

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${headers} ${sources})
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_include_directories(${PROJECT_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
        $<INSTALL_INTERFACE:src>
        ${ARTICUNO_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ryml::ryml
        pugixml
        nlohmann_json::nlohmann_json)

target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        src/PCH.h)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/Sample"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(TARGETS ${PROJECT_NAME}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}")