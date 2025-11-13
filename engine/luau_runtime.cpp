// /engine/luau_runtime.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Luau headers (C++ API, no extern "C" needed)
#include "lua.h"
#include "lualib.h"
#include "luacode.h"

static std::string loadFileToString(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Custom Luau require function
static int luauRequire(lua_State* L) {
    const char* module = lua_tostring(L, 1);
    if (!module) {
        luaL_error(L, "module name must be a string");
        return 0;
    }
    
    std::string modulePath = std::string(module) + ".luau";
    
    // Try to load the .luau file
    std::string source = loadFileToString(modulePath);
    if (source.empty()) {
        luaL_error(L, "module '%s' not found", module);
        return 0;
    }
    
    // Compile Luau source to bytecode using the C API
    lua_CompileOptions options = {};
    options.optimizationLevel = 1;
    options.debugLevel = 1;
    
    size_t bytecodeSize;
    char* bytecode = luau_compile(source.c_str(), source.size(), &options, &bytecodeSize);
    if (!bytecode) {
        luaL_error(L, "failed to compile module '%s'", module);
        return 0;
    }
    
    // Load and execute the module
    if (luau_load(L, modulePath.c_str(), bytecode, bytecodeSize, 0) != LUA_OK) {
        free(bytecode);
        lua_error(L);
        return 0;
    }
    
    free(bytecode);
    
    // Call the loaded function to get the module result
    if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
        lua_error(L);
        return 0;
    }
    
    // Return the module result
    return 1;
}

lua_State* createLuauState() {
    lua_State* luaState = luaL_newstate();
    if (!luaState) {
        return nullptr;
    }

    luaL_openlibs(luaState);
    
    // For now, skip the custom require and test basic functionality
    // lua_pushcclosurek(luaState, luauRequire, "require", 0, NULL);
    // lua_setglobal(luaState, "require");
    
    return luaState;
}

bool loadLuauScript(lua_State* luaState, const std::string& path) {
    std::string source = loadFileToString(path);
    if (source.empty()) {
        std::cerr << "Could not read file: " << path << "\n";
        return false;
    }

    // Compile Luau source to bytecode using the C API
    lua_CompileOptions options = {};
    options.optimizationLevel = 1;
    options.debugLevel = 1;
    
    size_t bytecodeSize;
    char* bytecode = luau_compile(source.c_str(), source.size(), &options, &bytecodeSize);
    if (!bytecode) {
        std::cerr << "Failed to compile Luau script: " << path << "\n";
        return false;
    }

    // Use luau_load instead of luaL_loadbuffer
    if (luau_load(luaState, path.c_str(), bytecode, bytecodeSize, 0) != LUA_OK) {
        std::cerr << "Luau load error: " << lua_tostring(luaState, -1) << "\n";
        lua_pop(luaState, 1);
        free(bytecode);
        return false;
    }

    free(bytecode);

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
