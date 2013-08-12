+--------------------------------------------+
|OS/2 UPSd for Trust 400/650 Energy Protector|
+--------------------------------------------+

Version 1.0 beta, 23rd of October 1997

Copyright (C) 1997 By Menno W. Pieters - The Netherlands
m.w.pieters@twi.tudelft.nl (until approx may/june 1998)

This program is a UPS daemon for OS/2 and the Trust UPS 400 and 650 Energy 
Protector. 
Source code is included. The program is compiled with Borland C++ for 
OS/2.

The information required to create this program has been obtained from the 
Linux UPS-HOWTO, available at sunsite.unc.edu via ftp and on the Infomagic 
Linux CD's (and several other sources).
Special thanks to Mikael Wahlgren.

This version of the program is donated to the public domain, under the 
conditions as stated in the General Public License of the Free Software 
Foundation, version 2 or above.

The program has only been tested with OS/2 Warp 4.0 (a.k.a. 'Merlin')

+----------+
|Disclaimer|
+----------+

THIS SOFTWARE IS PROVIDED "AS IS", WITH NO WARRANTY OF ANY KIND, NOR 
EXPRESSED OR IMPLIED. USE OF THE PROGRAM IS COMPLETELY AT YOUR OWN RISK.
THE AUTHOR OR ANYONE ELSE CANNOT BE HELD RESPONISBLE FOR ANY DAMAGE CAUSED 
BY OR WHILE USING THIS SOFTWARE.

OS/2 is a registered trademark of International Business Machines Corp.
Borland is a registered trademark of Borland International, Inc.
Trust is a registered trademark.

+-----------------------+
|Additional requirements|
+-----------------------+

First of all, the UPS itself, of course and a cable (do-it-yourself) to 
signal the computer in case of power failure and power low and to signal 
the UPS to turn off the power after system shutdown.

+------------+
|Installation|
+------------+

(1) Unpack the ZIP-file in a directory of your choice, if you haven't 
    already done so. 

(2) If you want to recompile the stuff, be my guest. Otherwise proceed 
    with the following.

(3) Copy the executable file UPSD.EXE to a suitable location, say 
    C:\OS2\SYSTEM.

(4) Add a line to CONFIG.SYS (at or near the end; assuming you put the 
    executable in the C:\OS2\SYSTEM directory):

    RUN=C:\OS2\SYSTEM\UPSD.EXE COMn

    (n can be any of 1, 2, 3, 4)

(5) Reboot and test your system.

-- If you would like to test it before installing, open an OS/2 command 
prompt (windowed) and type:

[C:\] START /PM UPSD COMn

(replace n by any of 1, 2, 3, 4, depending on what port the UPS-cable is 
connected to)

+----+
|ToDo|
+----+

(1) A nice graphical interface to the program, probably through a named 
    pipe.
(2) Make the code more readable!

