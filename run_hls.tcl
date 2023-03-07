
open_project -reset hdc_hls
set_top encode
add_files src/HDC_HLS.cc -cflags "-std=c++0x"

open_solution -reset "solution1"
set_part {xc7z020clg400-1}
create_clock -period 100.0MHz

# csim_design

csynth_design
export_design -format ip_catalog -version "1.0"

exit