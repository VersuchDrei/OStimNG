cmake_minimum_required(VERSION 3.21)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")


########################################################################################################################
## Define project
########################################################################################################################
project(
        OStim
        VERSION 7.3.0.0
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
    src/Core/Thread/ThreadClimax.cpp
    src/Core/Thread/ThreadNavigation.cpp
    src/Core/Thread/ThreadSound.cpp
    src/Core/ThreadActor/ThreadActorAnimation.cpp
    src/Core/ThreadActor/ThreadActorClimax.cpp
    src/Core/ThreadActor/ThreadActorEquipObjects.cpp
    src/Core/ThreadActor/ThreadActorExcitement.cpp
    src/Core/ThreadActor/ThreadActorExpression.cpp
    src/Core/ThreadActor/ThreadActorSound.cpp
    src/Core/ThreadStarter/NPCThreadStarter.cpp
    src/Core/ThreadStarter/PlayerThreadStarter.cpp
    src/Core/ThreadStarter/ThreadStarter.cpp
    src/Core/Thread.cpp
    src/Core/ThreadActor.cpp
    src/Core/ThreadInterface.cpp
    src/Core/ThreadManager.cpp
    src/Core/ThreadManager.h
    src/Events/EventListener.cpp
    src/Furniture/Furniture.cpp
    src/Furniture/FurnitureTable.cpp
    src/Furniture/FurnitureType.cpp
    src/Game/LocaleManager.cpp
	src/Game/LocaleManager.h
	src/Game/Locker.h
    src/GameAPI/Game.cpp
    src/GameAPI/GameActor.cpp
    src/GameAPI/GameCamera.cpp
    src/GameAPI/GameDialogue.cpp
    src/GameAPI/GameEvents.cpp
    src/GameAPI/GameFaction.cpp
    src/GameAPI/GameHooks.cpp
    src/GameAPI/GameObject.cpp
    src/GameAPI/GamePosition.cpp
    src/GameAPI/GameSex.cpp
    src/GameAPI/GameSound.cpp
    src/GameAPI/GameUtil.cpp
    src/GameAPI/GameTable.cpp
    src/Graph/GraphTable/GraphTableActions.cpp
    src/Graph/GraphTable/GraphTableEvents.cpp
    src/Graph/GraphTable/GraphTableNodes.cpp
    src/Graph/GraphTable/GraphTableSequences.cpp
    src/Graph/GraphTable/GraphTableSetupNodes.cpp
    src/Graph/GraphTable/GraphTableSetupOptions.cpp
    src/Graph/Node/NodeNavigation.cpp
    src/Graph/Action.cpp
    src/Graph/ActionAttributes.cpp
    src/Graph/Event.cpp
    src/Graph/GraphActor.cpp
    src/Graph/Node.cpp
    src/Graph/Sequence.cpp
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
    src/Sound/SoundUtil.cpp
    src/Trait/TraitTable/TraitTableActorProperties.cpp
    src/Trait/Condition.cpp
    src/Trait/FacialExpression.cpp
    src/Trait/TraitTable.cpp
    src/UI/Align/AlignMenu.cpp
    src/UI/Align/IncrementValue.cpp
    src/UI/Scene/SceneMenu.cpp
    src/UI/Scene/SceneOptions.cpp
    src/UI/Search/SearchMenu.cpp
    src/UI/OStimMenu.cpp
    src/UI/Settings.cpp
    src/UI/UIState.cpp
    src/Util/ActorUtil.cpp
    src/Util/APITable.cpp
    src/Util/CameraUtil.cpp
    src/Util/CheckSum.cpp
    src/Util/CompatibilityTable.cpp
    src/Util/ControlUtil.cpp
    src/Util/FormUtil.cpp
    src/Util/Globals.cpp
    src/Util/IDGenerator.cpp
    src/Util/Integrity.cpp
    src/Util/JsonUtil.cpp
    src/Util/LegacyUtil.cpp
    src/Util/LookupTable.cpp
    src/Util/MathUtil.cpp
    src/Util/ObjectRefUtil.cpp
    src/Util/SerializationUtil.cpp
    src/Util/StringUtil.cpp
    src/Util/VectorUtil.cpp
	src/Util.cpp
	src/main.cpp
        src/ModAPI/OstimNG-API-Main.cpp 
        src/ModAPI/OstimNG-API-Main.h
        src/ModAPI/OstimNG-API-Scene.h 
        src/ModAPI/OstimNG-API-Scene.cpp

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
find_package(OpenSSL REQUIRED)
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
        nlohmann_json::nlohmann_json
        OpenSSL::SSL)

target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        src/PCH.h)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/Sample"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(TARGETS ${PROJECT_NAME}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}")

set(OUTPUT_FOLDER "../../../data")

if(DEFINED OUTPUT_FOLDER)
    # If you specify an <OUTPUT_FOLDER> (including via environment variables)
    # then we'll copy your mod files into Skyrim or a mod manager for you!

    # Copy the SKSE plugin .dll files into the SKSE/Plugins/ folder
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )
    
    # Copy the SKSE plugin .pdb files into the SKSE/Plugins/ folder
    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )
endif()