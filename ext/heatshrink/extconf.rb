require 'mkmf'
$CFLAGS += ' -Wno-declaration-after-statement -std=c99'
create_makefile 'heatshrink/heatshrink'
