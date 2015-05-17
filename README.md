# SoftwareGroupProject_BashDefender
Group 6 Project

Bash defender is an interactive computer game in the ‘Tower Defence’ mould. 
Enemies will advance along a path towards the user’s base. The user will build 
and upgrade towers which attack these enemies. The commands will take the form 
of UNIX terminal, teaching the user basic computing concepts. The objective of 
the game is to survive long enough to complete the level, upgrading your towers
appropriately to deal with increasingly difficult waves of enemies.. 

This is the release for OSX. 

To install:
  1. Download Zip
  2. Open Terminal 
  3. Type cd Downloads/SoftwareGroupProject_BashDefender-OSXinstall/sources/
  4. Type make 

Then to run type:
  ./BASHdefender

You may have to add the SDL frameworks in sources/ to /Library/Frameworks.
To do this type:

  mv SDL2* /Library/Frameworks
