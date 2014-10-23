##############################################################################
## Filename:          /homes/ugrad/alanachtenberg/Lab3/drivers/multiply_v1_00_a/data/multiply_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Thu Oct  2 20:04:37 2014 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "multiply" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
