// /engine/main.cpp
#include "raylib.h"
#include <iostream>
#include <string>

// Luau / Lua headers – adjust the path based on how you vendor Luau
extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

// Forward declarations from other engine files
lua_State* createLuauState();
bool loadLuauScript(lua_State* luaState, const std::string& path);
bool callLuaUpdate(lua_State* luaState, float deltaTime);
void registerGfxBindings(lua_State* luaState);

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Luau + raylib engine");
    SetTargetFPS(60);

    lua_State* luaState = createLuauState();
    if (!luaState) {
        std::cerr << "Failed to create Luau state\n";
        CloseWindow();
        return 1;
    }

    // Register C++ → Luau bindings (gfx, etc.)
    registerGfxBindings(luaState);

    // Load main script
    if (!loadLuauScript(luaState, "scripts/main.luau")) {
        std::cerr << "Failed to load scripts/main.luau\n";
        lua_close(luaState);
        CloseWindow();
        return 1;
    }

    // Call optional init() in Luau, if defined
    lua_getglobal(luaState, "init");
    if (lua_isfunction(luaState, -1)) {
        if (lua_pcall(luaState, 0, 0, 0) != LUA_OK) {
            std::cerr << "Error in init(): " << lua_tostring(luaState, -1) << "\n";
            lua_pop(luaState, 1);
        }
    } else {
        lua_pop(luaState, 1); // pop non-function
    }

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Let Luau drive the frame logic
        if (!callLuaUpdate(luaState, deltaTime)) {
            std::cerr << "Error calling update(dt), stopping loop.\n";
            break;
        }
    }

    lua_close(luaState);
    CloseWindow();
    return 0;
}