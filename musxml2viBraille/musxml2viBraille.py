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

noteAlts = {'-2':'0', '-1':'1', '1':'3', '2':'4'}

text_file = open("output.txt","w")         

if (text_file):
    print('File opened without any problem ...\n')

for target in root.findall('./part/measure/note'):
    
    noteType = noteTypes.get(target.find('type').text)

    # If note is a rest, set noteStep='7' (reserved for a rest),
    # noteAlt='n' (nil) since it is meaningless for a rest.
    if (target.find('rest') is not None):
        print('r' + noteType)
        text_file.write('r' + noteType + '\n')
        #noteStep = 'r'
        #noteAlt = '_'
    else:
        noteStep = noteSteps.get(target.find('pitch/step').text)

        #If not a rest get octave
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
            if ((interval >= 6) or ((interval in range(4,6)) and o<>0)):
                print('o' + str(noteOct))
                text_file.write('o' + str(noteOct) + '\n')
            oldIndex = idx
            oldOct = noteOct

        #If not a rest, get accidental if exists. Else pick '2' for natural
        if (target.find('pitch/alter')) is not None:
            noteAlt  = noteAlts.get(target.find('pitch/alter').text)
        else:
            noteAlt = '2'


        # Create a note code in output
        print(noteStep + noteAlt + noteType)
        text_file.write(noteStep + noteAlt + noteType + '\n')



    # Get the dot count and create a dot
    # code in output for each <dot/> tag in note
    notedotcount = len(target.findall('dot'))
    for i in range(notedotcount):
        print('.')
        text_file.write('.' + '\n')

text_file.close()
