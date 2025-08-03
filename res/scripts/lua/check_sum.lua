function data_handler(data_array, length)
    if not data_array or length <= 0 then
        return {}, 0
    end
    
    local checksum = 0
    local result_array = {}
    
    -- 复制原始数据并计算校验和
    for i = 1, length do
        if data_array[i] then
            result_array[i] = data_array[i]
            checksum = checksum + data_array[i]
        else
            result_array[i] = 0
        end
    end
    
    -- 添加校验和到数组末尾
    result_array[length + 1] = checksum
    
    return result_array, length + 1
end