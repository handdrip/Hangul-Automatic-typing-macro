#pragma once

#include <iostream>
#include <functional>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>
#include <locale>
#include <cassert>
#include <chrono>

#include <list>
#include <vector>
#include <map>
#include <unordered_map>

#include <conio.h>
#include <tchar.h>
#include <msctf.h>
#include <Windows.h>
#include <Winuser.h>

#include <nana/gui.hpp>
#include <nana/gui/wvl.hpp>
#include <nana/gui/place.hpp>

#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>

#include <nana/system/platform.hpp>

#include "matching_table.h"

#ifdef UNICODE
#pragma comment(linker, "/ENTRY:wWinMainCRTStartup /SUBSYSTEM:WINDOWS") 
#else
#pragma comment(linker, "/ENTRY:WinMainCRTStartup /SUBSYSTEM:WINDOWS") 
#endif