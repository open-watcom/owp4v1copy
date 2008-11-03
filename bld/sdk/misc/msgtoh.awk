# Print preamble and set counter variable
BEGIN { 
    printf( "#define MSG_RC_BASE %d\n", rcbase )
    num = 1
}

# Process all lines beginning with 'pick'
/^ *pick/ {
   gsub( /,/, "" ) # Remove commas
   print "#define " $2 " (MSG_RC_BASE + " num ")"  
   num = num + 1   # Increment counter
}

# Slithglty different for lines beginning with 'wdpick'
/^ *wdpick/ {
   gsub( /,/, "" ) # Remove commas
   print "#define __" $2 " (MSG_RC_BASE + " num ")"  
   num = num + 1   # Increment counter
}
