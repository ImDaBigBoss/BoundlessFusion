const luaconf = fengari.luaconf;
const lua = fengari.lua;
const lauxlib = fengari.lauxlib;
const lualib = fengari.lualib;

const L = lauxlib.luaL_newstate();
lualib.luaL_openlibs(L);

let canvas = null;
let canvas_ctx = null;

//This is used to speed up the process of getting the buffer pointer
let buffer_pointer = null;
let lua_buffer_pointer_idx = -1;

// --- Utils ---

function resize_canvas() {
  canvas_ctx.canvas.width = window.innerWidth;
  canvas_ctx.canvas.height = window.innerHeight;

  buffer_pointer = new Uint32Array(canvas_ctx.canvas.width * canvas_ctx.canvas.height);

  //Update Lua values
  lua.lua_getglobal(L, "screen");

  lua.lua_pushinteger(L, canvas_ctx.canvas.width);
  lua.lua_setfield(L, -2, "width");
  lua.lua_pushinteger(L, canvas_ctx.canvas.height);
  lua.lua_setfield(L, -2, "height");

  lua.lua_pushjsfunction(L, function(L) { //TODO: Optimize this, 200ms is too slow
    //Get the array of uint32s from the Lua variable screen.buffer
    lua.lua_rawgeti(L, lua.LUA_REGISTRYINDEX, lua_buffer_pointer_idx);
    for (let i = 0; i < buffer_pointer.length; i++) {
      lua.lua_rawgeti(L, -1, i + 1);
      buffer_pointer[i] = lua.lua_tointeger(L, -1);
      lua.lua_pop(L, 1);
    }
    lua.lua_pop(L, 1);

    //Put the buffer into the canvas
    //Each uint32 is a pixel in the format 0xRRGGBBAA
    const img_data = canvas_ctx.createImageData(canvas_ctx.canvas.width, canvas_ctx.canvas.height);
    for (let i = 0; i < buffer_pointer.length; i++) {
      img_data.data[i * 4 + 0] = (buffer_pointer[i] >> 24) & 0xFF;
      img_data.data[i * 4 + 1] = (buffer_pointer[i] >> 16) & 0xFF;
      img_data.data[i * 4 + 2] = (buffer_pointer[i] >> 8) & 0xFF;
      img_data.data[i * 4 + 3] = (buffer_pointer[i] >> 0) & 0xFF;
    }

    canvas_ctx.putImageData(img_data, 0, 0);
    return 0;
  });
  lua.lua_setfield(L, -2, "draw_buffer");

  //Call resized callback
  lua.lua_getfield(L, -1, "resized");
  lua.lua_call(L, 0, 0);

  //Remove the old buffer pointer & store the new one
  if (lua_buffer_pointer_idx != -1) {
    lauxlib.luaL_unref(L, lua.LUA_REGISTRYINDEX, lua_buffer_pointer_idx);
  }
  lua.lua_getfield(L, -1, "buffer");
  lua_buffer_pointer_idx = lauxlib.luaL_ref(L, lua.LUA_REGISTRYINDEX);

  lua.lua_pop(L, 1);
}

// --- Initialisation ---

function init_debug() {
  lua.lua_getglobal(L, "debug");

  lua.lua_pushjsfunction(L, function(L) {
    console.info(lua.lua_tojsstring(L, -1));
    return 0;
  });
  lua.lua_setfield(L, -2, "info");

  lua.lua_pushjsfunction(L, function(L) {
    console.warn(lua.lua_tojsstring(L, -1));
    return 0;
  });
  lua.lua_setfield(L, -2, "warn");

  lua.lua_pushjsfunction(L, function(L) {
    console.error(lua.lua_tojsstring(L, -1));
    return 0;
  });
  lua.lua_setfield(L, -2, "error");

  lua.lua_pop(L, 1);
}

function init_framebuffer() {
  canvas = document.getElementById("canvas");
  canvas_ctx = canvas.getContext("2d");

  resize_canvas();
  window.addEventListener("resize", (event) => {
    resize_canvas();
  });
}

// --- Backend ---

function backend_init() {
  init_debug();
  init_framebuffer();
}

// --- Main ---

console.info("Starting up...");

//Set the root path for the Lua code
lua.lua_pushliteral(L, "/libs/boundless_fusion/");
lua.lua_setglobal(L, "boundless_fusion_root_path");

//Setup the library
lauxlib.luaL_dofile(L, fengari.to_luastring("/libs/boundless_fusion/main.lua"));
backend_init(L); //Link JS functions

//Store Lua function pointers for lib_pre_init, lib_post_init and lib_loop
lua.lua_getglobal(L, "boundless_fusion");
lua.lua_getfield(L, -1, "lib_init");
const lib_init = lauxlib.luaL_ref(L, lua.LUA_REGISTRYINDEX);
lua.lua_getfield(L, -1, "lib_loop");
const lib_loop = lauxlib.luaL_ref(L, lua.LUA_REGISTRYINDEX);
lua.lua_pop(L, 1);

//Call lib_init
lua.lua_rawgeti(L, lua.LUA_REGISTRYINDEX, lib_init);
lua.lua_call(L, 0, 0);

console.info("Starting game loop...");

//Now we need to start a game loop that doesn't block the main thread so that the page can finish loading and the browser doesn't freeze
//This is done by using requestAnimationFrame
let game_running = true;
async function game_loop() {
  if (!game_running) return;

  let time_start = performance.now();

  lua.lua_rawgeti(L, lua.LUA_REGISTRYINDEX, lib_loop);
  lua.lua_call(L, 0, 0);

  let time_end = performance.now();
  console.info("Game at " + (1000 / (time_end - time_start)) + " FPS");

  window.requestAnimationFrame(game_loop);
}

//Wait for window load
window.addEventListener("load", (event) => {
  window.requestAnimationFrame(game_loop);
});
//Stop the loop when the window is closed, the page is reloaded or the user navigates away
window.addEventListener("beforeunload", (event) => {
  game_running = false;
});
