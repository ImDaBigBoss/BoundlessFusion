-- Bootstrap functions and global variables

-- boundless_fusion_root_path is defined by the loader

boundless_fusion = {
    not_implemented = function()
        if debug.error ~= nil then
            debug.error("Not implemented");
        else
            print("ERORR: Not implemented");
        end
    end,

    lib_init = function()
        debug.info("Boundless Fusuion v1.0.0");

        dofile("/game/main.lua");
        init();
    end,

    lib_loop = function()
        loop();
    end
}

-- io
dofile(boundless_fusion_root_path .. "io/debug.lua");
dofile(boundless_fusion_root_path .. "io/screen.lua");
