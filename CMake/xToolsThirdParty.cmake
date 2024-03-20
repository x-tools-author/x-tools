function(x_tools_3rd_setup_qt_aes target)
  target_link_libraries(${target} PRIVATE QtAES::QtAES)
endfunction()

function(x_tools_3rd_setup_glog target)
  target_link_libraries(${target} PRIVATE glog::glog)
endfunction()

function(x_tools_3rd_setup_nodeeditor target)
  target_link_libraries(${target} PRIVATE QtNodes::QtNodes)
endfunction()

function(x_tools_3rd_setup_qmdnsengine target)
  target_link_libraries(${target} PRIVATE qmdnsengine)
endfunction()
