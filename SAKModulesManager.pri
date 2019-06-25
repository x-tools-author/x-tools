#*******************************************************************************
# The file is encoding with utf-8 (with BOM)
#
# I write the comment with English, it's not because that I'm good at English,
# but for "installing B".
#
# Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
#******************************************************************************/

# 域名解释工具
include($${PWD}/Modules/NSLookup/NSLookup.pri)

# 公网获取工具
include($${PWD}/Modules/GetPublicIP/GetPublicIP.pri)

# 软件更新工具
include($${PWD}/Modules/Update/Update.pri)

# 万能crc计算器
include($${PWD}/Modules/CRCCalculator/CRCCalculator.pri)

# 软件ui风格
include($${PWD}/Modules/QtAppStyleApi/QtAppStyleApi.pri)

# 软件ui样式
include($${PWD}/Modules/QtStyleSheetApi/QtStyleSheetApi.pri)

# 文件校验工具
include($${PWD}/Modules/QtCryptographicHash/QtCryptographicHash.pri)
