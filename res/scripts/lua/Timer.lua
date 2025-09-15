print("Start 'Timer' demo...")

while true do
    print("Current time: " .. os.date("%H:%M:%S"))
    x_write(os.date("%H:%M:%S") .. "\n")

    ret = x_is_interruption_requested()
    if ret then
        print("Script interrupted.")
        break
    end

    x_sleep(1000) -- Sleep for 1000 milliseconds (1 second)
end
