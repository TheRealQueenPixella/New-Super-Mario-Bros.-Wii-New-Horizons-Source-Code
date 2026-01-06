.text

.extern DateFile_dispCurrentWorldNum

.global dispCurrentWorldNum
dispCurrentWorldNum:
	or r3, r31, r31
	b DateFile_dispCurrentWorldNum