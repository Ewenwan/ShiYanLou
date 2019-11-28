#coding=utf-8
import os
import sys
import codecs
import chardet

 

def convert(filename,out_enc="UTF-8"): // "ASSIC"
	try:
		content=codecs.open(filename,'r').read()
		source_encoding=chardet.detect(content)['encoding']
		print source_encoding
		content=content.decode(source_encoding).encode(out_enc)
		codecs.open(filename,'w').write(content)
	except IOError as err:
		print("I/O error:{0}".format(err))

 

def explore(dir):
	for root,dirs,files in os.walk(dir):
		for file in files:
			if os.path.splitext(file)[1] in ['.cpp','.c','.h','.hpp']:
				print file
				path=os.path.join(root,file)
				convert(path)

def main():
	explore(os.getcwd())

if __name__=="__main__":

	main()
