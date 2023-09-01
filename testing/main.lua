
function init()
    debug.info("Hello from the init function!");
end

i = 0;
function loop()
    local buffer = screen.buffer;
    local width = screen.width;
    local y = 0;

    for sc_y = 0, screen.height - 1 do
        y = sc_y * width;

        for x = 0, width - 1 do
            buffer[x + y] = i;

            i = i + 1;
        end
    end

    screen.draw_buffer();
end
