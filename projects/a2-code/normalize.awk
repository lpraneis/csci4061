#!/usr/bin/awk -f

# Normalize output of a client transcript so that it has a standard
# format. Splits each line based on the \r carriage return character
# which should eliminate most of the control characters.

BEGIN{
# Full binary for control signals but gawk chokes on this
#  FS="\x1b\x5b\x32\x4b\x0d" 
# Hex code for ASCII Carriage Return
#  FS="\x0d"
  FS="\r" 
}
{
  print $NF
}
