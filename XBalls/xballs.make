# xballs was compiled successfully with the following commands
# and options
#
# Compile in IBM RS600 workstations
xlc -c  -O3 -I /usr/openwin/include   -I .      xballs.c
rm -f xballs; if [ -f xballs ]; then mv -f xballs xballs~; fi
xlc -o xballs xballs.o  -O3 -L /usr/openwin/lib    -lXt  -lX11 -lm  

# Compile on SUN worksations
gcc -c  -O3 -I /usr/openwin/include   -I .      xballs.c
rm -f xballs; if [ -f xballs ]; then mv -f xballs xballs~; fi
gcc -o xballs xballs.o  -O3 -L /usr/openwin/lib    -lXt  -lX11 -lm  

# Compile on DEC-alpha's
gcc -c  -O3 -I/usr/X11R6/include   -I.      xballs.c
rm -f xballs; if [ -f xballs ]; then mv -f xballs xballs~; fi
gcc -o xballs xballs.o  -O3 -L/usr/shlib    -lXt  -lX11 -lm  

# Compile on SGI (frida.pmmh.espci.fr)
gcc -c -O2 -I /usr/include -I . xballs.c
gcc -o xballs xballs.o  -O2 -L /usr/lib    -lXt  -lX11 -lm

# Compile on SuSE Linux
gcc -c -O3 -I /usr/include -I . xballs.c
gcc -o xballs xballs.o  -O3 -L /usr/X11R6/lib    -lXt  -lX11 -lm

# Compile on msm1-Linux
gcc -c -O3 -I /usr/include -I . xballs.c; \
gcc -o xballs.64linux xballs.o  -fast -O3 -L /usr/X11R6/lib    -lXt  -lX11 -lm

# Compile on Mac
gcc -c -O3 -I /usr/include -I . xballs.c
gcc -o xballs xballs.o -O3 -L/usr/X11R6/lib -lXt -lX11 -lm
