#pragma once

//Common imports.
#include <Windows.h>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <psapi.h>
#include <dinput.h>
#include <concurrent_vector.h>
#include <libloaderapi.h>
#include <tchar.h>
#include <d3d9.h>
#include <time.h>

//Internal common utils.
#include "util/PanicUtil.hpp"
#include "util/CommonUtil.hpp"
#include "util/stuff/Singleton.hpp"
#include "util/stuff/DiscordRPC.hpp"
#include "util/accessors/LanguageAccessor.hpp"
#include "util/accessors/ConfigurationAccessor.hpp"

//Hooks reflection.
#include "../thirdparty/source/hooks/Detours.hpp"
#pragma comment (lib, "../thirdparty/assembled/hooks/detours.lib")

//Render.
#pragma comment (lib, "../thirdparty/assembled/gui/d3dx9.lib")
#pragma comment (lib, "../thirdparty/assembled/gui/xinput.lib")

//RPC.
#include "../thirdparty/source/discord/DiscordRPC.hpp"
#pragma comment (lib, "../thirdparty/assembled/discord/discord-rpc.lib")