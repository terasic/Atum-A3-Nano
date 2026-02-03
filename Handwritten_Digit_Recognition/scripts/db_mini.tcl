# AXE5-Eagle MIPI demo.
# This script is used to only dump the content of the Video Frame Buffer when the 
#   NIOS-V C code is used to run the demo.

set version "MIPI dump VFB"
puts "--- $version ---"

#functions that can be called from the System Console Prompt:

##############################################################################################
#Update these Global Variables to match your system
##############################################################################################
set sysid 	0x00050970

################################################################################################
# Procedures defined below
################################################################################################
set mm [lindex [get_service_paths master] 0]
puts "JTAG Master : $mm"

open_service master $mm
set idreadback [master_read_32 $mm $sysid 1]
close_service master $mm	
puts "System ID: 0x[format %08X $idreadback]"


proc exportRGB { } {

global mm

set num_lines	128
set bytes_per_line	[expr {128 * 3}]

set vid_buff   0x00060000
set output_file_name vfb_content.rgb

	if {[info exists ::env(VFB_OUT)]} {
		set output_file_name $::env(VFB_OUT)
	} else {
		set output_file_name [file join [pwd] vfb_content.rgb]
	}

	open_service master $mm

	puts "Transferring the image"
	for {set i 0} {$i < $num_lines } {incr i} {
		set offset [expr $i * $bytes_per_line]
		lappend image [master_read_memory $mm [expr $vid_buff + $offset] $bytes_per_line ]
	}

	# Make the image one flat list of bytes instead of list of lines...
	package require struct::list
	set flat_image [struct::list flatten -full $image]
	puts "Image flattened"

	# Take away the 0x in front of each byte
	foreach i $flat_image {
		lappend hex_image [ string range [lindex $i 0] 2 3]
	}
	puts "Hex values stripped for '0x's"

	# Convert the 2-character hex strings to binary format
	set bin_image [binary decode hex $hex_image]
	puts "Hex image converted to binary"

	# Save to file
	#set file_pointer [open $output_file_name w+]
	#puts $file_pointer $bin_image
	#close $file_pointer
	#puts "File written successfully"

	set fh [open $output_file_name "w"]
  fconfigure $fh -translation binary -encoding binary
  puts -nonewline $fh $bin_image
  close $fh
  puts "EXPORT_DONE $output_file_name [string length $bin_image] bytes"

	close_service master $mm	
}

proc dump_vfb { startaddr {count 1}} {
  global mm
  
  set vid_buff   0x00060000

  open_service master $mm
  
  for {set oc 0} {$oc<$count} {incr oc} {
    set addr [ expr $vid_buff + $startaddr + [ expr $oc*4 ]]
    set rdata [master_read_32 $mm  0x[ format %x [ expr $vid_buff+[ expr $oc*4 ] ] ] 1 ]
    puts "VFB Memory: $rdata from 0x[ format %08x $addr ]"
  }
}
