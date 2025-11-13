// /engine/bindings_gfx.cpp
#include "raylib.h"
#include <iostream>

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
}

// clearBackground(r, g, b)
static int clearBackgroundLua(lua_State* luaState) {
    float red = (float)luaL_checknumber(luaState, 1);
    float green = (float)luaL_checknumber(luaState, 2);
    float blue = (float)luaL_checknumber(luaState, 3);

    ClearBackground({ (unsigned char)(red * 255.0f),
                      (unsigned char)(green * 255.0f),
                      (unsigned char)(blue * 255.0f),
                      255 });

    return 0;
}

// drawCircle(x, y, radius)
static int drawCircleLua(lua_State* luaState) {
    float x = (float)luaL_checknumber(luaState, 1);
    float y = (float)luaL_checknumber(luaState, 2);
    float radius = (float)luaL_checknumber(luaState, 3);

    DrawCircle((int)x, (int)y, radius, WHITE);
    return 0;
}

// endDrawing()
static int endDrawingLua(lua_State* luaState) {
    EndDrawing();
    return 0;
}

// beginDrawing()
static int beginDrawingLua(lua_State* luaState) {
    BeginDrawing();
    return 0;
}

void registerGfxBindings(lua_State* luaState) {
    // Create a 'gfx' table/namespace
    lua_newtable(luaState);
    
    // Register functions in the gfx table
    lua_pushcfunction(luaState, clearBackgroundLua);
    lua_setfield(luaState, -2, "clearBackground");
    
    lua_pushcfunction(luaState, drawCircleLua);
    lua_setfield(luaState, -2, "drawCircle");
    
    lua_pushcfunction(luaState, endDrawingLua);
    lua_setfield(luaState, -2, "endDrawing");
    
    lua_pushcfunction(luaState, beginDrawingLua);
    lua_setfield(luaState, -2, "beginDrawing");
    
    // Set the table as a global named 'gfx'
    lua_setglobal(luaState, "gfx");
}