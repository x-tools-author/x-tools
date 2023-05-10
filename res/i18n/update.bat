::lupdate -no-recursive -source-language en_US -target-language zh_CN ../../qml/common -ts common_zh_CN.ts
::lupdate -no-recursive -source-language en_US -target-language en_US ../../qml/common -ts common_en_US.ts
::lupdate -no-recursive ../../qml ../../src -ts easydebug_en_US.ts
::lupdate -no-recursive -source-language en_US -target-language zh_CN ../../qml ../../src -ts easydebug_zh_CN.ts


lupdate -no-recursive -source-language en -target-language en_US ../../qml/common    -ts common_en.ts
lupdate -no-recursive -source-language en -target-language zh_CN ../../qml/common    -ts common_zh_CN.ts
lupdate -no-recursive -source-language en -target-language en_US ../../qml ../../src -ts easydebug_en.ts
lupdate -no-recursive -source-language en -target-language zh_CN ../../qml ../../src -ts easydebug_zh_CN.ts