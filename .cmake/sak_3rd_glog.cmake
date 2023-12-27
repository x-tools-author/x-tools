function(sak_3rd_glog_setup target)
  target_link_libraries(${target} PRIVATE glog::glog)
endfunction(sak_3rd_glog_setup)
