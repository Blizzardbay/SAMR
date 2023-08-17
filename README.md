SAMR
====

Software Automatic Mouth Remastered

About
====
This is expansion of the original SAM github project done by Sebastian Macke. 
This is a C++ remake of the original code inorder to improve the functionalliy of the orginal project.
The extention adds full file reading of SAM commands and non-argument based input for the SAM application.

Usage
====
Run the provided debug SAMR.exe. 
Select the .sam file to use.
Follow instructions given by the console.

To make commands and .sam files all you have todo is follow the orginal project's syntax (shown below)
()
for SAM commands. Since the project has an option to output .wav files, there is no need for the -wav command.

In case you don't read through the code to find out how to execute the commands, here is the orginal print message
for using the code.

usage: sam [options] Word1 Word2 ....
		usage: sam [options] Word1 Word2 ....
options
	-phonetic 		enters phonetic mode. (see below)
	-pitch number		set pitch value (default=64)
	-speed number		set speed value (default=72)
	-throat number		set throat value (default=128)
	-mouth number		set mouth value (default=128)
	-wav filename		output to wav instead of libsdl
	-sing			special treatment of pitch
	-debug			print additional debug messages



	 VOWELS                            VOICED CONSONANTS	
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
ZH           plea(s)ure	
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


The Orginal Git License
=======

The software is a reverse-engineered version of a commercial software published more than 30 years ago.
The current copyright holder is SoftVoice, Inc. (www.text2speech.com)

Any attempt to contact the company failed. The website was last updated in the year 2009.
The status of the original software can therefore best described as Abandonware 
(http://en.wikipedia.org/wiki/Abandonware)

As long this is the case I cannot put my code under any specific open source software license
Use it at your own risk.



Contact The Orginal Git Creator
=======

If you have questions don' t hesitate to ask me.
If you discovered some new knowledge about the code please mail me.

Sebastian Macke
Email: sebastian@macke.de
