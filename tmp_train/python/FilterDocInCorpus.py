import string
import sys
import time
import os



#read file list in one cat dir
def ReadFileList(rootDir):
	files=[]
	filelist=os.listdir(rootDir)                                                          
	for f in filelist:
		pathname=rootDir+'/'+f
		if os.path.isfile(pathname):
			files.append(f)

	return files


#whether contain one pattern
'''
def ContainPattern(fileName,pattern):
	for element in pattern:
		if ContainOnePattern(fileName,element)==True:
			return True

	return False
'''


	

def FilterPattern(fileName,pattern):
	indicator=True
	
	content=''
	
	myFile=open(fileName,"r")
	for lines in myFile:
		if Contain(lines,pattern)==False:#filter this sentence
			content=content+lines



	myFile.close()

	#if indicator==True:
	#	print 'find one filter pattern'

	return content


def Contain(content,pattern):
	for element in pattern:                                                       
		#print element                                                            
		if content.find(element)!=-1:# find                                       
		#print 'find '+element                                                
			return True                                                                 
		

	return False
			 
		


#filter pattern
def ReadFilterPattern(fileName):
	pattern=[]

	myFile=open(fileName,"r")
	for lines in myFile:
		lines=lines[0:len(lines)-1]

		#splitedList=lines.split('\t')
		pattern.append(lines)

		print lines
		#print splitedList

	myFile.close()
	return pattern

def MoveTo(sourceDir,destDir,fileName,pattern):

	#does this path exist?if not ,create it
	print destDir
	if os.path.exists(destDir)==False:
		commond='mkdir '+destDir
		os.system(commond)
		print 'making dir='+destDir
	
	#move
	path1=sourceDir+fileName;
	path2=destDir+fileName;
	commond='cp '+path1+' '+path2

	os.system(commond)
	

	filteredDoc=FilterPattern(path2,pattern)

	path3=destDir+'tmp'
	writer=open(path3,'w')
	writer.write(filteredDoc)
	writer.close()

	commond='mv '+path3+' '+path2
	os.system(commond)
	print 'after filtering one doc'


patternFile='../data/filterCorpusModel.txt'
pattern=ReadFilterPattern(patternFile)

rootDir='../data/traindata/'
targetDir='../data/deletedTrainData/'

mainDir='baby/'


rootDir=rootDir+mainDir;
sourceDir=rootDir
destDir=targetDir+mainDir


fileList=ReadFileList(sourceDir)
for element in fileList:
	#print element
	#print sourceDir+element

	
	MoveTo(sourceDir,destDir,element,pattern)
	print 'moving file:'+element



	
