# * https://developer.apple.com/documentation/xcode/packaging-mac-software-for-distribution
# * 需要在xCode中配置好签名证书和描述文件（xCode->Settings->Accounts->Manage Certificates -> + Developer ID Application）
# * 需要在xCode中配置好签名证书和描述文件（xCode->Settings->Accounts->Manage Certificates -> + Developer ID Installer）
function(x_generate_pkg target dev_id_app dev_id_installer)
  # cmake-format: off
  add_custom_target(${target}_pkg VERBATIM
    COMMENT "Making pkg..."
    WORKING_DIRECTORY ${X_BINS_DIR}/${target}
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Application: ${dev_id_app}"
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Installer: ${dev_id_installer}"
    COMMAND codesign -f -o runtime -s "${dev_id_app}" -v --deep --entitlements ${X_ENTITLEMENTS} ${target}.app
    COMMAND productbuild --sign "${dev_id_installer}" --product ${target}.app/Contents/Info.plist --component ${target}.app /Applications ${target}-${X_VERSION}-${X_BUNDLE_VERSION}.pkg
  )
  # cmake-format: on
endfunction()