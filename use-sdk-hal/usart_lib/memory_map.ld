
MEMORY 
{
	osrom (rx)  : ORIGIN = 0x0, LENGTH = 0x2000
	librom (rx) : ORIGIN = 0x2000, LENGTH = 0x4000
	ram   (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

