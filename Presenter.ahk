#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

#SingleInstance force

CoordMode, Mouse, Screen

!l:: ; go to next slide
	send {Right down}
	sleep 100
	send {Right up}
return


!k:: ;go to prev slide
	send {Left down}
	sleep 100
	send {Left up}
return

!j:: ; open slides back up
	MouseClick, Left, 900, 500 ; assume calm music is playing
	
	sleep, 500
	
	send {RWin down}
	sleep 5
	send 2 ; open slides
	sleep 5
	send {RWin up}
return

!o:: ; play rock music
	send {RWin down} 
	sleep 5
	send 3 ; open firefox
	sleep 5
	send {RWin up}
	
	sleep 500
	
	MouseClick, Left, 900, 500
return

!p:: ; play calm music
	MouseClick, Left, 900, 500 ; assumes rokc music is playing
	
	sleep 500
	
	send {RWin down} 
	sleep 5
	send 4 ; open chrome work
	sleep 5
	send {RWin up}
	
	sleep 500
	
	MouseClick, Left, 900, 500 ; starts calm music
return

!i:: ;stop music by clicking
	MouseClick, Left, 900, 500
return