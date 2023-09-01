
screen = {
    width = 0,
    height = 0,
    buffer = {},

    draw_buffer = not_implemented,

    resized = function()
        debug.info("Screen resized to " .. screen.width .. "x" .. screen.height);

        screen.buffer = {};
        for i = 0, screen.width * screen.height do
            screen.buffer[i] = 0;
        end
    end
}
