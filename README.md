SAMR
====

Software Automatic Mouth Remastered

About
====
This is an expansion of the original SAM GitHub project done by Sebastian Macke. 
This is a C++ remake of the original code in order to improve the functionality of the original project.
The extension adds full file reading of SAM commands and non-argument-based input for the SAM application.
Use at your own risk.

Original Git
====
(https://github.com/vidarh/SAM)

Usage
====
Run the provided debug `SAMR.exe`. 

Select the `.sam` file to use.

Follow the instructions given by the console.

To make commands and `.sam` files all you have to do is follow the original project's syntax (shown below)

(https://github.com/vidarh/SAM)

Remove the `sam` prefix from the commands and put the commands into a file with
.sam at the end.

Example 1:
```
I am sam
```
Example 2:
```
-sing I am sam
```
Example 3:
```
-sing -pitch 60 I am sam
```
Example 4:
```
-sing -pitch 60 I am sam
-sing -pitch 255 I am sam
-sing -pitch 200 I am sam
-sing -pitch 1 I am sam
```
Since the project has the option to output `.wav` files, there is no need for the `-wav` command.
Note word lines should not exceed 99 characters to due API limitations. With spaces, it is now ok.
Just redo the options and put more text on the next line.

There is a known limitation for the length of the output. Try to keep outputs to less than 1 minute: 30 seconds.

In case you don't read through the code to find out how to execute the commands, here is the original print message
for using the code.
```
usage: sam [options] Word1 Word2 ....
options
-phonetic		    enters phonetic mode. (see below)
-pitch number		    set pitch value (default=64)
-speed number		    set speed value (default=72)
-throat number		    set throat value (default=128)
-mouth number		    set mouth value (default=128)
-sing			    special treatment of pitch
-debug			    print additional debug messages

    VOWELS                             VOICED CONSONANTS	
IY           f(ee)t                    R        red		
IH           p(i)n                     L        allow		
EH           beg                       W        away		
AE           Sam                       W        whale		
AA           pot                       Y        you		
AH           b(u)dget                  M        Sam		
AO           t(al)k                    N        man		
OH           cone                      NX       so(ng)		
UH           book                      B        bad		
UX           l(oo)t                    D        dog		
ER           bird                      G        again		
AX           gall(o)n                  J        judge		
IX           dig(i)t                   Z        zoo		
                                       ZH       plea(s)ure	
   DIPHTHONGS                          V        seven		
EY           m(a)de                    DH       (th)en		
AY           h(igh)						
OY           boy						
AW           h(ow)                     UNVOICED CONSONANTS	
OW           slow                      S         Sam		
UW           crew                      Sh        fish		
                                       F         fish		
                                       TH        thin		
 SPECIAL PHONEMES                      P         poke		
UL           sett(le) (=AXL)           T         talk		
UM           astron(omy) (=AXM)        K         cake		
UN           functi(on) (=AXN)         CH        speech		
Q            kitt-en (glottal stop)    /H        a(h)ead		
```
The Orginal Git License
=======

The software is a reverse-engineered version of commercial software published more than 30 years ago.
The current copyright holder is SoftVoice, Inc. (www.text2speech.com)

Any attempt to contact the company failed. The website was last updated in the year 2009.
The status of the original software can therefore best be described as Abandonware 
(http://en.wikipedia.org/wiki/Abandonware)

As long this is the case I cannot put my code under any specific open-source software license
Use it at your own risk.



Contact The Orginal Git Creator
=======

If you have questions don't hesitate to ask me.
If you discovered some new knowledge about the code please mail me.

Sebastian Macke
Email: sebastian@macke.de
