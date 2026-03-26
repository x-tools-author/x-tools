function data_handler(data_string)
    -- reverse the input string and concatenate it with the original string
    -- such as "hello" becomes "hello-olleh"
    local reversed_string = data_string:reverse()
    return data_string .. "-" .. reversed_string
end
