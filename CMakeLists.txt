cmake_minimum_required(VERSION 3.17)

#Set path to compiler.
set(CMAKE_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.27.29110/bin/Hostx86/x86/cl.exe")

project(otv2-pl)

#Libs files.
set(
        THIRD_PARTY_FILES

        thirdparty/source/gui/imgui/ImGui.cpp
        thirdparty/source/gui/imgui/ImGuiDraw.cpp
        thirdparty/source/gui/imgui/ImGuiImplDx9.cpp
        thirdparty/source/gui/imgui/ImGuiImplWin32.cpp
        thirdparty/source/gui/imgui/ImGuiWidgets.cpp
)

#Project files.
set (
        SOURCE_FILES

        src/Bootstrap.cpp
        src/loader/runtime/RuntimeHandler.cpp
        src/loader/segment/Segment.cpp
        src/loader/segment/SegmentHeaderManager.cpp
        src/loader/ui/UIRenderer.cpp
        src/loader/ui/UIBridge.cpp
        src/util/CommonUtil.cpp
        src/util/PanicUtil.cpp
        src/util/accessors/LanguageAccessor.cpp
        src/util/accessors/ConfigurationAccessor.cpp
        src/util/stuff/DiscordRPC.cpp
        src/segment/framework/OneEngine.cpp
        src/segment/framework/sdk/game/memory/VirtualReflectionHandler.cpp
        src/loader/BootstrapTaskController.cpp
        src/loader/callback/CallbackManager.cpp)

#RU language dependency...
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#d3x9 includes.
include_directories(C:/DxSDK/Include)

#Compile as DLL.
add_library(otv2-pl SHARED ${THIRD_PARTY_FILES} ${SOURCE_FILES})