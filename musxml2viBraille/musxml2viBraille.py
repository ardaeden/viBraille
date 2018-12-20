#!/usr/bin/python3

###################################
# MusicXML to viBraille converter #
# by Arda Eden 2018               #
###################################

import sys
import xml.etree.ElementTree as ET

if (len(sys.argv)<2):
    print("Not enough arguments...")
    sys.exit()

isFirstNote = True
interval = 0
oldOct = 0
oldIndex = 0

tree = ET.parse(sys.argv[1])
root = tree.getroot()

#Necessary values for interval calculation
stepIndexes = {'c':0, 'd':1, 'e':2,
               'f':3, 'g':4, 'a':5, 'b':6}

#This is the dictionary for note steps convertion
noteSteps = {'C':'c', 'D':'d',
             'E':'e', 'F':'f',
             'G':'g', 'A':'a', 'B':'b'}

#This is the dictionary for note types conversion
noteTypes = {'whole'   :'0'     , 'half'   :'1',
             'quarter' :'2'     , 'eighth' :'3',
             '16th'    :'4'     , '32th'   :'5',
             '64th'    :'6'     , '128th'  :'7'}

text_file = open("output.txt","w")         

if (text_file):
    print('File opened without any problem ...\n')

for target in root.findall('./part/measure/note'):
    
    noteType = noteTypes.get(target.find('type').text)

    # If note is a rest, use "r" as the symbol
    if (target.find('rest') is not None):
        print('r' + noteType)
        text_file.write('r' + noteType + '\n')
        
    #if not a rest keep on ...
    else:
        
        noteStep = noteSteps.get(target.find('pitch/step').text)
        noteOct = int(target.find('pitch/octave').text)


        # !!! Note interval and octave code decision is done here !!!
        # -----------------------------------------------------------
        
        # Create an octave code for the first note
        if (isFirstNote is True):
            oldOct = noteOct
            oldIndex = stepIndexes.get(noteStep)
            print('o' + str(noteOct))
            text_file.write('o' + str(noteOct) + '\n')
            isFirstNote = False

        # Do the math and decide for octave codes :))
        else: 
            idx = stepIndexes.get(noteStep)
            o = noteOct - oldOct
            interval = abs((o * 7) + (idx - oldIndex)) + 1
            if ((interval >= 6) or ((interval in range(4,6)) and o!=0)):
                print('o' + str(noteOct))
                text_file.write('o' + str(noteOct) + '\n')
            oldIndex = idx
            oldOct = noteOct

        #Get accidentals if exist.
        if (target.find('accidental')) is not None:
            acc = target.find('accidental').text
            
            if (acc=='flat-flat'):
                for i in range(2):    
                    print('-')
                    text_file.write('-' + '\n')
            
            if (acc=='flat'):
                print('-')
                text_file.write('-' + '\n')

            if (acc=='sharp'):
                print('+')
                text_file.write('+' + '\n')

            if (acc=='double-sharp'):
                for i in range(2):    
                    print('+')
                    text_file.write('+' + '\n')

            if (acc=='natural'):
                print('n')
                text_file.write('n' + '\n')

        # Create a note code in output
        print(noteStep + noteType)
        text_file.write(noteStep + noteType + '\n')

    # Get the dot count and create a dot
    # code in output for each <dot/> tag in note
    notedotcount = len(target.findall('dot'))
    for i in range(notedotcount):
        print('.')
        text_file.write('.' + '\n')

text_file.close()
