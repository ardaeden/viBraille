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
             '16th'    :'0'     , '32th'   :'1',
             '64th'    :'2'     , '128th'  :'3'}

text_file = open("output.txt","w")         

if (text_file):
    print('File opened without any problem ...\n')


for measures in root.findall('./part/measure'):

    # Check for the measure attributes
    if (measures.find('attributes/time') is not None):

        tmpTimeCode = 't' + measures.find('attributes/time/beats').text + measures.find('attributes/time/beat-type').text
        print(tmpTimeCode + '_')
        text_file.write(tmpTimeCode + '_')

    for notes in measures.findall('note'):
        
        if (notes.find('type') is not None):
            noteType = noteTypes.get(notes.find('type').text)
        else:
            noteType = ''

        # If note is a rest, use "r" as the symbol
        if (notes.find('rest') is not None):
            print('r' + noteType + '__')
            text_file.write('r' + noteType + '__')
            
        #if not a rest keep on ...
        else:
            
            noteStep = noteSteps.get(notes.find('pitch/step').text)
            noteOct = int(notes.find('pitch/octave').text)

            #Get accidentals if exist.
            if (notes.find('accidental')) is not None:
                acc = notes.find('accidental').text
                
                if (acc=='flat-flat'):
                    for i in range(2):    
                        print('-___')
                        text_file.write('-' + '___')
                
                if (acc=='flat'):
                    print('-___')
                    text_file.write('-' + '___')

                if (acc=='sharp'):
                    print('+___')
                    text_file.write('+' + '___')

                if (acc=='double-sharp'):
                    for i in range(2):    
                        print('+___')
                        text_file.write('+' + '___')

                if (acc=='natural'):
                    print('n___')
                    text_file.write('n' + '___')


            # !!! Note interval and octave code decision is done here !!!
            # -----------------------------------------------------------
            
            # Create an octave code for the first note
            if (isFirstNote is True):
                oldOct = noteOct
                oldIndex = stepIndexes.get(noteStep)
                print('o' + str(noteOct) + '__')
                text_file.write('o' + str(noteOct) + '__')
                isFirstNote = False

            # Do the math and decide for octave codes :))
            else: 
                idx = stepIndexes.get(noteStep)
                o = noteOct - oldOct
                interval = abs((o * 7) + (idx - oldIndex)) + 1
                if ((interval >= 6) or ((interval in range(4,6)) and o != 0)):
                    print('o' + str(noteOct) + '__')
                    text_file.write('o' + str(noteOct) + '__')
                oldIndex = idx
                oldOct = noteOct


            # Create a note code in output
            print(noteStep + noteType + '__')
            text_file.write(noteStep + noteType + '__')

        # Get the dot count and create a dot
        # code in output for each <dot/> tag in note
        notedotcount = len(notes.findall('dot'))
        for i in range(notedotcount):
            print('.' + '___')
            text_file.write('.' + '___')

    print('|' + '___')
    text_file.write('|' + '___')
text_file.close()
