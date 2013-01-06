import string


def ReadFile(fileName):
	myfile=open(fileName)
	content=''
	for Line in myfile.readlines():
		Line=Line[0:len(Line)-1]#delete the last \n
		content=content+Line

	

	myfile.close()
	content=content+'\n'
	return content


fileList='../etc/filelist.txt'
myfile=open(fileList)

outFile='../data/allnews.txt'
fileHandle=open(outFile,'w')

counter=1
for Line in myfile.readlines():
	Line=Line[0:len(Line)-1]#delete the last \n
	part=Line.split()
	if len(part)==2:
		filename='../'+part[1]
		content=ReadFile(filename)

	if counter%1==0:#only get 1/3 of whole corpus
		fileHandle.writelines(content)
		print 'finish file:'+filename

	counter=counter+1

myfile.close()
fileHandle.close()
