## prtscrpp
Screenshot utility written in C++ and MFC for Windows.  

### Why?
Needed something for Windows that's lightweight and works the way I want. 
Also, needed this one for university.  

### Why MFC?
One-time exception. Never again.  
There is no actual reason as why I have chosen MFC. I have found an array of similar applications written using other toolkits, such as GTK or Qt, so I've decided to give it a try in MFC.  
Whilst doable it has been, the code is not so very pretty and I strongly advise anyone to stay off of this ancient technology as much as they can now that I've experience the suffering myself.  

![prtscrpp showing a screenshot of its own code](https://raw.githubusercontent.com/infyhr/prtscrpp/master/screenshot.png "prtscrpp showing a screenshot of its own github repository code")  
*prtscrpp showing a screenshot of its own github repository code*

### Requirements
* While not strictly required, a use of own Imgur API key would be desireable
* You'll need Windows, that's for sure, tested with >= 7
* x86 or x64 should do  

*In theory it should work even with Windows 2k, but let's not go into that shall we?*
### Features
* Support the following files: BMP, GIF, JP(E)G, PNG
* Open / Save (can convert files)
* Cut / Copy / Paste to and from clipboard
* Take a screenshot of a selected region, foreground window or the whole screen (region is keybound)
* ... and have it uploaded to Imgur with a direct link in your clipboard!
 
### To be added
* Settings (user customization)
* More upload providers and a custom server one (with a simple PHP/python script)
* Some kind of Bitmap manipulation, such as highlighting, drawing, or simply inputting text

### Limitations
* Keybind is currently hardcoded to `MOD_CONTROL+0x2C` which is equivalent to left control key (Ctrl) and "PrtScr"
* *... hence the name, prtscr++  :^)* 
#### P.S.
This README will be updated in the future. Maybe. Who knows?