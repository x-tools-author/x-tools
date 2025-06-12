# https://github.com/wxWidgets/wxWidgets/releases/download/v3.3.0/wxWidgets-3.3.0.7z
set(package_version "3.3.0")
set(package_base_url "https://github.com/wxWidgets/wxWidgets/releases/download")
set(package_url "${package_base_url}/v${package_version}/wxWidgets-v${package_version}.7z")
set(package_name "wxWidgets-${package_version}")

# Download and extract wxWidgets
x_setup_3rd_library(${package_url} ${package_name})
