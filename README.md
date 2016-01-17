# vt52

In 2015 I acquired a DEC VT52 video terminal (https://en.wikipedia.org/wiki/VT52).  Once plentiful, decent working examples are now becoming quite rare...  Over the 2105 winter holiday I did a hack project using an FPGA board I had on hand to scan out the contents on the VT52's character generator ROM -- the aim is to convert this into a modern font format for nostalgic use on modern hardware.

This repo currently contains the SystemVerilog code for the ROM scanner, the raw retrieved ROM data, and the data processed and formatted into a C header file.  I'll add font files here and possibly more code/scripts as the project progresses...
