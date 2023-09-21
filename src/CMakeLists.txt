add_subdirectory(assistants)
add_subdirectory(modbusstudio)
add_subdirectory(canbusstudio)

if(QT_VERSION_MAJOR EQUAL 6)
    add_subdirectory(src/guiapp)
endif()
