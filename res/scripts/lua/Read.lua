print("Start 'Read' demo...")

function x_read(str)
    print("x_read:", str)
end

while true do
    ret = x_is_interruption_requested()
    if ret then
        print("Script interrupted.")
        break
    end

    x_sleep(1000) -- Sleep for 1000 milliseconds (1 second)
end
