// /engine/luau_runtime.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

lua_State* createLuauState() {
    lua_State* luaState = luaL_newstate();
    if (!luaState) {
        return nullptr;
    }

    // For Luau you may not need all standard libs, but this is fine to start
    luaL_openlibs(luaState);
    return luaState;
}

static std::string loadFileToString(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool loadLuauScript(lua_State* luaState, const std::string& path) {
    std::string source = loadFileToString(path);
    if (source.empty()) {
        std::cerr << "Could not read file: " << path << "\n";
        return false;
    }

    if (luaL_loadbuffer(luaState, source.c_str(), source.size(), path.c_str()) != LUA_OK) {
        std::cerr << "Luau load error: " << lua_tostring(luaState, -1) << "\n";
        lua_pop(luaState, 1);
        return false;
    }

    if (lua_pcall(luaState, 0, 0, 0) != LUA_OK) {
        std::cerr << "Luau runtime error: " << lua_tostring(luaState, -1) << "\n";
        lua_pop(luaState, 1);
        return false;
    }

    return true;
}

bool callLuaUpdate(lua_State* luaState, float deltaTime) {
    lua_getglobal(luaState, "update");
    if (!lua_isfunction(luaState, -1)) {
        lua_pop(luaState, 1);
        // No update defined, just keep going
        return true;
    }

    lua_pushnumber(luaState, deltaTime);

    if (lua_pcall(luaState, 1, 0, 0) != LUA_OK) {
        std::cerr << "Error in update(dt): " << lua_tostring(luaState, -1) << "\n";
        lua_pop(luaState, 1);
        return false;
    }

    return true;
}
