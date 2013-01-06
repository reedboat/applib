import os


fileName='../remove'
myFile=open(fileName,"r")
for lines in myFile:
	lines=lines[0:len(lines)-1]
	os.remove(lines)
	print 'delete '+lines

myFile.close()
print 'finish deleting the bad traing files'
