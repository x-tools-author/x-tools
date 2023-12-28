function(sak_3rd_setup_glog target)
  target_link_libraries(${target} PRIVATE glog::glog)
endfunction(sak_3rd_setup_glog)
