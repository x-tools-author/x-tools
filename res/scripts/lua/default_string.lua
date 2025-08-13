function data_handler(data_string)
    -- 将输入字符串反转
    local reversed_string = data_string:reverse()
    return data_string .. "-" .. reversed_string
end
