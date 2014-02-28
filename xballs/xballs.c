/*
 * xballs - from reciprocating engine for X   
 * (you will not recognize the original ...)
 * 
 * Modified: Stefan Luding  1994 Dec.
 * Modified: Stefan Luding  1995 
 * Modified: Stefan Luding  1996 
 * Modified: Stefan Luding  1997 
 * Modified: Stefan Luding  1998 
 * Modified: Stefan Luding  1999 
 * Modified: Stefan Luding  2000 
 * Modified: Stefan Luding  2001 
 * Modified: Stefan Luding  2002 
 * Modified: Stefan Luding  2003 
 * Modified: Stefan Luding  2004 
 * Modified: Stefan Luding  2005 
 * Modified: Stefan Luding  2006 
 * Modified: Stefan Luding  2007 
 * Modified: Stefan Luding  2008 
 * 
 * Permission to use, copy, modify and distribute without charge this software,
 * documentation, images, etc. is granted, provided that this comment and the
 * author's name is retained.  The author assumes no responsibility for lost
 * sleep as a consequence of use of this software.
 * 
 * Send any comments, bug reports, etc. to: lui@ica1.uni-stuttgart.de
 * 
 */

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "patchlevel.h"
#include "functions.h"


#define XBPATH "/home/luding/XBALLS"
#define	PI 3.141592654

#define NKMAX 1300000
#define NFIELD 800

#define SKAL400 4

XtCallbackProc  redraw_callback();
XtTimerCallbackProc  X_Draw_Snap(char *cfile);

void  Draw_Contacts( float xy0, int h_offset, int v_offset,
                     float xskal, float yskal, float xleft, 
                     float xright, float xbottom, float xtop,
                     int NK, float *px, float *pz, float *prad );

void  Draw_Walls( float xy0, int h_offset, int v_offset,
                  float xskal, float yskal, float xleft, 
                  float xright, float xbottom, float xtop );

int ieyedist(const void *p1, const void *p2);

int read_header( int inread, FILE *fp, 
                 int *rNK, float *rtime, float *rxleft, float *rxrear, float *rxbottom, 
                                         float *rxright, float *rxfront, float *rxtop );

int read_line( int inread, FILE *fp, 
               float *xr1, float *xr2, float *xr3, float *xr4,
               float *xrrad, float *xr6, float *xr7, float *xrix,
               float *xr12, float *xr32, float *xr61, float *xr62,
               float *xr71, float *xr72 );

int             i0r,i0g,i0b,i0s,ietime,i0time,i0tposx,i0tposy,i03d,ialab;
int             width, height, hoffset, offset, noborder, i0text;
int             xcells,zcells; /* Anzahl der Zellen in x und y Richtung */
int             ich, icolmin, ivmode, incmax, idrmin, idrmax, incmin, idist;
int             center_of_mass, incr, inc, incdt, iferro, idraw, drwcont;
int             isolid, ifast, ifast0, ifast1, icmax, icmean, ladd, istdout;
int             icube, iwc1, iwc2, iphcm;
int             xtrace, xmtrace, ifformat, isort, iii, icb;
int             inread, iredraw, nredraw;
int             coll_hist, iscounter;
float           tmin, tmax, cskal, dskal, xdummy, r0rred, fpower, vpower; 
float           cdirmin, cdirmax, xtop0, xright0, inputmass12;
float           time_step, time_last, xyskal, coffset, drwmin, drwmax;
float           rotr, rotphi, rotphi0, rottheta, rottheta0, unidist, dist, distxz;
float           drotr, drotphi, drottheta, dunidist, eyepos, deyepos;
float           xellipse, trx, try, trz;

char            csystem[80], c1[1];
int             mmax101, ifilm, invert, ivector, ivv;
long int        ipause, xpause, jpause, jpnext, jpstep;
int             nodel, noddel, iwelo, ivelo, idie, iscale;
float           drfnskal;
float           vqskal,vvskal,tempx,ivskal;
int             isq, ixxxstep, istep, ilines, mfont, iwnum;
int             ix12, ifog;
float           xg12[NKMAX];
float           x12min, x12max;
char            *c2dfile, *c0file, *coutfile, *wallfile, *drfile;
int             ic0, ipixel;
int             iicmean[103], iiimean;
float           cccmean[103], xxxmean;

double 		piston_x2;

void            Standard_Parameters();
void            Create_Colors();

    Display     *dpy;                   /* X server connection            */
    Window	gwin1, andy, eng, lab, alab, blab;
    Pixmap      vgwin1, vandy, vlab, valab, vblab;
    Widget	toplevel;
    XEvent      event_xballs;

typedef struct {
        unsigned long *plane_masks;     /* points into dbpair.plane_masks */
        unsigned long enplanemask;      /* what we enable for drawing     */
        XColor        *colors;          /* size = 2 ** totalplanes        */
        unsigned long *pixels;          /* size = 2 ** planesperbuf       */
	GC gcCombustion, gcIntake, gcCrankcase, gcMechanicals, gcPins;
      } DBufInfo;

typedef struct {
        int planesperbuf;
        int pixelsperbuf;                 /* = 1<<planesperbuf  */
        int totalplanes;                  /* = 2*planesperbuf   */
        int totalpixels;                  /* = 1<<totalplanes   */
        unsigned long *plane_masks;       /* size = totalplanes */
        unsigned long pixels[1];
        int dbufnum;
        DBufInfo bufs[2];
        DBufInfo *drawbuf, *dpybuf;
      } DBufPair;

DBufPair dbpair;

GC gcEngine, gcBack, gcZero, gcCombust, gcIn, gcCase, gcMech, gcPin, gcFont, gcSFont;
GC gcBlack, gcRed, gcBlue, gcGreen, gcLGrey, gcGrey, gcWhite;
GC gcMap[103], cvalue[103];
GC gcMapGrey[103][100], gcColor;

#define NUM_BUFFERED_COLORS 6

String  colors[] = {"black", "red", "blue", "LightGrey", "Grey", "white"};

XFontStruct  *helvB24;

int main(argc, argv)
    int             argc;
    char          **argv;
{
    unsigned   long fg, bg;       /* Colors for foreground/background  */
    XSizeHints xsh;
    int        screen;
    XSetWindowAttributes  attributes;
    unsigned long         valuemask;

    int     count;

    XtTimerCallbackProc Cycle(); 
    XtTimerCallbackProc DummyCycle(); 

    toplevel = XtInitialize("xballs","XBalls",NULL,0,&argc,argv);

/* Set standard values for the parameters */
    Standard_Parameters();

    c1[0]=argv[argc-1][0];
    /* printf( " %d %s %1s ... ", argc-1, "-", c1 ); */
    if( strcmp("-", c1 ) == 1 ) argc--;

    for (count = 1; count < argc; count++) 
    { 
	if ((strcmp("help", argv[count]) == 0) && (count + 1 <= argc))
            {
                 sprintf(csystem,"more %s/xballs.txt",XBPATH);
                 system(csystem);
                 /* return; */
            }
	else if ((strcmp("-f", argv[count]) == 0) && (count + 1 <= argc))
		c2dfile = argv[++count];
	else if ((strcmp("-wf", argv[count]) == 0) && (count + 1 <= argc))
		wallfile = argv[++count];
	else if ((strcmp("-df", argv[count]) == 0) && (count + 1 <= argc))
		drfile = argv[++count];
	else if ((strcmp("-0f", argv[count]) == 0) && (count + 1 <= argc))
            {
		c0file = argv[++count];
                ic0=1;
            }
	else if ((strcmp("-nw", argv[count]) == 0) && (count + 1 <= argc))
		iwnum = uatoi(argv[++count]);
	else if ((strcmp("-of", argv[count]) == 0) && (count + 1 <= argc))
            {
		coutfile = argv[++count];
                ifilm = 1;
            }
	else if ((strcmp("-xof", argv[count]) == 0) && (count + 1 <= argc))
            {
		coutfile = argv[++count];
                ifilm = 2;
            }
	else if ((strcmp("-format", argv[count]) == 0) && (count + 1 <= argc))
                   inread = uatoi(argv[++count]);

	else if ((strcmp("-redraw", argv[count]) == 0) && (count + 1 <= argc))
                   nredraw = (uatoi(argv[++count]));
	else if ((strcmp("-rotphi", argv[count]) == 0) && (count + 1 <= argc))
                   rotphi = uatoi(argv[++count]);
	else if ((strcmp("-rottheta", argv[count]) == 0) && (count + 1 <= argc))
                   rottheta = uatoi(argv[++count]);
	else if ((strcmp("-rotr", argv[count]) == 0) && (count + 1 <= argc))
                   rotr = uatoi(argv[++count]);
	else if ((strcmp("-unidist", argv[count]) == 0) && (count + 1 <= argc))
                   unidist = uatoi(argv[++count]);
	else if ((strcmp("-drotphi", argv[count]) == 0) && (count + 1 <= argc))
                   drotphi = uatoi(argv[++count]);
	else if ((strcmp("-drottheta", argv[count]) == 0) && (count + 1 <= argc))
                   drottheta = uatoi(argv[++count]);
	else if ((strcmp("-drotr", argv[count]) == 0) && (count + 1 <= argc))
                   drotr = uatoi(argv[++count]);
	else if ((strcmp("-dunidist", argv[count]) == 0) && (count + 1 <= argc))
                   dunidist = uatoi(argv[++count]);
	else if ((strcmp("-translate", argv[count]) == 0) && (count + 1 <= argc))
                  {
                   trx = uatoi(argv[++count]);
                   try = uatoi(argv[++count]);
                   trz = uatoi(argv[++count]);
                   printf( " %f %f %f \n ",trx,try,trz );
                  }
	else if ((strcmp("-eyepos", argv[count]) == 0) && (count + 1 <= argc))
                   eyepos = uatoi(argv[++count]);
	else if ((strcmp("-deyepos", argv[count]) == 0) && (count + 1 <= argc))
                   deyepos = uatoi(argv[++count]);

	else if ((strcmp("-pixel", argv[count]) == 0) && (count + 1 <= argc))
                   ipixel = uatoi(argv[++count]);
	else if ((strcmp("-e", argv[count]) == 0) && (count + 1 <= argc))
                   xellipse = uatoi(argv[++count]);
	else if ((strcmp("-s", argv[count]) == 0) && (count + 1 <= argc))
                   xyskal = uatoi(argv[++count])*100.;
	else if ((strcmp("-ss", argv[count]) == 0) && (count + 1 <= argc))
                   xyskal = uatoi(argv[++count])*1.;
	else if ((strcmp("-sss", argv[count]) == 0) && (count + 1 <= argc))
                   xyskal = uatoi(argv[++count])/100.;
	else if ((strcmp("-p", argv[count]) == 0) && (count + 1 <= argc))
                   xpause = jpstep * uatoi(argv[++count]);
	else if ((strcmp("-w", argv[count]) == 0) && (count + 1 <= argc))
                   width = uatoi(argv[++count]);
	else if ((strcmp("-xcells", argv[count]) == 0) && (count + 1 <= argc))
                   xcells = uatoi(argv[++count]);
	else if ((strcmp("-zcells", argv[count]) == 0) && (count + 1 <= argc))
                   zcells = uatoi(argv[++count]);
	else if ((strcmp("-h", argv[count]) == 0) && (count + 1 <= argc))
                   height = uatoi(argv[++count]);
	else if ((strcmp("-noborder", argv[count]) == 0) && (count + 1 <= argc))
                   noborder = uatoi(argv[++count]);
	else if ((strcmp("-o", argv[count]) == 0) && (count + 1 <= argc))
                   offset = uatoi(argv[++count]);
	else if ((strcmp("-oh", argv[count]) == 0) && (count + 1 <= argc))
                   hoffset = uatoi(argv[++count]);
	else if ((strcmp("-mo", argv[count]) == 0) && (count + 1 <= argc))
                   offset = - uatoi(argv[++count]);
	else if ((strcmp("-moh", argv[count]) == 0) && (count + 1 <= argc))
                   hoffset = - uatoi(argv[++count]);
	else if ((strcmp("-c", argv[count]) == 0) && (count + 1 <= argc))
                   cskal = (float) uatoi(argv[++count]);
	else if ((strcmp("-cd", argv[count]) == 0) && (count + 1 <= argc))
                   dskal = (float) uatoi(argv[++count])/10.;
	else if ((strcmp("-c1", argv[count]) == 0) && (count + 1 <= argc))
                   cskal = uatoi(argv[++count]) / 1.e-6 ;
	else if ((strcmp("-ch", argv[count]) == 0) && (count + 1 <= argc))
                   ich = uatoi(argv[++count]);
	else if ((strcmp("-tmin", argv[count]) == 0) && (count + 1 <= argc))
                   tmin = 1.e-6 * (float) uatoi(argv[++count]);
	else if ((strcmp("-tmax", argv[count]) == 0) && (count + 1 <= argc))
                   tmax = 1.e-6 * (float) uatoi(argv[++count]);
	else if ((strcmp("-ncoll", argv[count]) == 0) && (count + 1 <= argc))
                   coll_hist = uatoi(argv[++count]);
	else if ((strcmp("-v", argv[count]) == 0) && (count + 1 <= argc))
                   xtrace = uatoi(argv[++count]);
	else if ((strcmp("-mv", argv[count]) == 0) && (count + 1 <= argc))
                   xmtrace = uatoi(argv[++count]);
	else if ((strcmp("-rgbr", argv[count]) == 0) && (count + 1 <= argc))
                   i0r = uatoi(argv[++count]);
	else if ((strcmp("-rgbg", argv[count]) == 0) && (count + 1 <= argc))
                   i0g = uatoi(argv[++count]);
	else if ((strcmp("-rgbb", argv[count]) == 0) && (count + 1 <= argc))
                   i0b = uatoi(argv[++count]);
	else if ((strcmp("-rgbs", argv[count]) == 0) && (count + 1 <= argc))
                   i0s = uatoi(argv[++count]);
	else if ((strcmp("-stime", argv[count]) == 0) && (count + 1 <= argc))
                   i0time=-uatoi(argv[++count]);
	else if ((strcmp("-ltime", argv[count]) == 0) && (count + 1 <= argc))
                   i0time=uatoi(argv[++count]);
	else if ((strcmp("-etime", argv[count]) == 0) && (count + 1 <= argc))
                   ietime=uatoi(argv[++count]);
	else if ((strcmp("-tposx", argv[count]) == 0) && (count + 1 <= argc))
                   i0tposx=uatoi(argv[++count]);
	else if ((strcmp("-tposy", argv[count]) == 0) && (count + 1 <= argc))
                   i0tposy=uatoi(argv[++count]);
	else if ((strcmp("-text", argv[count]) == 0) && (count + 1 <= argc))
                   i0text=1;
	else if ((strcmp("-rmult", argv[count]) == 0) && (count + 1 <= argc))
                   r0rred=1./uatoi(argv[++count]);
	else if ((strcmp("-rred", argv[count]) == 0) && (count + 1 <= argc))
                   r0rred=uatoi(argv[++count]);
	else if ((strcmp("-3dturn", argv[count]) == 0) && (count + 1 <= argc))
                   i03d=uatoi(argv[++count]);
	else if ((strcmp("-crange", argv[count]) == 0) && (count + 1 <= argc))
        {
                   incmin = uatoi(argv[++count]);
                   incmax = uatoi(argv[++count]);
        }
	else if ((strcmp("-drrange", argv[count]) == 0) && (count + 1 <= argc))
        {
                   idrmin = uatoi(argv[++count]);
                   idrmax = uatoi(argv[++count]);
        }
	else if ((strcmp("-cpower", argv[count]) == 0) && (count + 1 <= argc))
                   vpower=uatoi(argv[++count])/100.;
	else if ((strcmp("-fpower", argv[count]) == 0) && (count + 1 <= argc))
                   fpower=uatoi(argv[++count])*1.;
	else if ((strcmp("-cmax", argv[count]) == 0) && (count + 1 <= argc))
                   icmax = 1;
	else if ((strcmp("-cminset", argv[count]) == 0) && (count + 1 <= argc))
                   {
                      icmax = 3;
                      cdirmin = uatoi(argv[++count])/100.0;
                   }
	else if ((strcmp("-cmaxset", argv[count]) == 0) && (count + 1 <= argc))
                   {
                      icmax = 2;
                      cdirmax = uatoi(argv[++count])/100.0;
                   }
	else if ((strcmp("-cmaxset5", argv[count]) == 0) && (count + 1 <= argc))
                   {
                      icmax = 2;
                      cdirmax = uatoi(argv[++count])/10000000.0;
                   }
	else if ((strcmp("-cmean", argv[count]) == 0) && (count + 1 <= argc))
                   icmean = 1;
	else if ((strcmp("-a", argv[count]) == 0) && (count + 1 <= argc))
                   ladd = 1;
	else if ((strcmp("-x", argv[count]) == 0) && (count + 1 <= argc))
                   istdout = 1;
	else if ((strcmp("-m", argv[count]) == 0) && (count + 1 <= argc))
                   center_of_mass = 1;
	else if ((strcmp("-solid", argv[count]) == 0) && (count + 1 <= argc))
                   isolid = 1;
	else if ((strcmp("-solidf", argv[count]) == 0) && (count + 1 <= argc))
                   isolid = 2;
	else if ((strcmp("-solidb", argv[count]) == 0) && (count + 1 <= argc))
                   isolid = 3;
	else if ((strcmp("-fast", argv[count]) == 0) && (count + 1 <= argc))
                   ifast = 1;
	else if ((strcmp("-fast0", argv[count]) == 0) && (count + 1 <= argc))
                   ifast0 = 1;
	else if ((strcmp("-drwmin", argv[count]) == 0) && (count + 1 <= argc))
                   drwmin = uatoi(argv[++count]);
	else if ((strcmp("-drwmax", argv[count]) == 0) && (count + 1 <= argc))
                   drwmax = uatoi(argv[++count]);
	else if ((strcmp("-draw", argv[count]) == 0) && (count + 1 <= argc))
                   idraw = uatoi(argv[++count]);
/*	else if ((strcmp("-nc", argv[count]) == 0) && (count + 1 <= argc))
                   inc = 1; */
	else if ((strcmp("-nct", argv[count]) == 0) && (count + 1 <= argc))
                   incdt = 1;
	else if ((strcmp("-rc", argv[count]) == 0) && (count + 1 <= argc))
                   incr = 1;
	else if ((strcmp("-ferro", argv[count]) == 0) && (count + 1 <= argc))
                   iferro = 1;
  	else if ((strcmp("-cm", argv[count]) == 0) && (count + 1 <= argc))
                   icb = 1; 
	else if ((strcmp("-px", argv[count]) == 0) && (count + 1 <= argc))
                {
                   mmax101 = 20;
                   mfont=0;
                }
	else if ((strcmp("-p0", argv[count]) == 0) && (count + 1 <= argc))
                   mmax101 = 20;
	else if ((strcmp("-p1", argv[count]) == 0) && (count + 1 <= argc))
                   mmax101 = 101;
	else if ((strcmp("-step", argv[count]) == 0) && (count + 1 <= argc))
                   {
                    istep = uatoi(argv[++count]);
		    if( istep < 1 ) istep=1;
		   }
	else if ((strcmp("-sort", argv[count]) == 0) && (count + 1 <= argc))
                   isort = 1;
	else if ((strcmp("-lines", argv[count]) == 0) && (count + 1 <= argc))
                   {
                      ilines = uatoi(argv[++count]);
                      cskal = 200;
                   }
	else if ((strcmp("-cmode", argv[count]) == 0) && (count + 1 <= argc))
                  {
                   idist=0;
                   ivmode = uatoi(argv[++count]);
                  }
	else if ((strcmp("-mass12", argv[count]) == 0) && (count + 1 <= argc))
                   inputmass12 = uatoi(argv[++count]);
	else if ((strcmp("-coffset", argv[count]) == 0) && (count + 1 <= argc))
                   coffset = uatoi(argv[++count]);
	else if ((strcmp("-ncoffset", argv[count]) == 0) && (count + 1 <= argc))
                   coffset = -uatoi(argv[++count]);
	else if ((strcmp("-squares", argv[count]) == 0) && (count + 1 <= argc))
                   isq = uatoi(argv[++count]);
	else if ((strcmp("-vvec", argv[count]) == 0) && (count + 1 <= argc))
                   {
                    ivv = 1;
                    ifast = 1;
                   }
	else if ((strcmp("-vscale", argv[count]) == 0) && (count + 1 <= argc))
                   ivskal = uatoi(argv[++count]);
	else if ((strcmp("-vq", argv[count]) == 0) && (count + 1 <= argc))
                   vqskal = uatoi(argv[++count]);
	else if ((strcmp("-vector", argv[count]) == 0) && (count + 1 <= argc))
                    ivector = 1;
	else if ((strcmp("-v0", argv[count]) == 0) && (count + 1 <= argc))
                    ivelo = 0;
	else if ((strcmp("-cdist", argv[count]) == 0) && (count + 1 <= argc))
                    idist = 1;
	else if ((strcmp("-col0", argv[count]) == 0) && (count + 1 <= argc))
                    icolmin = 2;
	else if ((strcmp("-w0", argv[count]) == 0) && (count + 1 <= argc))
                    iwelo = 0;
	else if ((strcmp("-invert", argv[count]) == 0) && (count + 1 <= argc))
                   invert = 1;
	else if ((strcmp("-noinvert", argv[count]) == 0) && (count + 1 <= argc))
                   invert = 0;
	else if ((strcmp("-scala", argv[count]) == 0) && (count + 1 <= argc))
                   iscale = uatoi(argv[++count]);
	else if ((strcmp("-nodel", argv[count]) == 0) && (count + 1 <= argc))
                   nodel = 1;
	else if ((strcmp("-noddel", argv[count]) == 0) && (count + 1 <= argc))
                   noddel = 0;
	else if ((strcmp("-nodrwcont", argv[count]) == 0) && (count + 1 <= argc))
                   drwcont = 0;
	else if ((strcmp("-dcskal", argv[count]) == 0) && (count + 1 <= argc))
                   drfnskal = uatoi(argv[++count]);
	else if ((strcmp("-die", argv[count]) == 0) && (count + 1 <= argc))
                   idie = 1;
	else if ((strcmp("-phcm", argv[count]) == 0) && (count + 1 <= argc))
                   iphcm = 1;
	else if ((strcmp("-cube", argv[count]) == 0) && (count + 1 <= argc))
                   icube = 1;
	else if ((strcmp("-fog", argv[count]) == 0) && (count + 1 <= argc))
                   ifog = uatoi(argv[++count]);
        else
        {
	   fprintf(stderr, 
              "Usage : xballs [-f filename|-] +Options (see xballs.txt) \n");
	   exit(0);
	}
    }

    dpy = XtDisplay(toplevel);
    screen = DefaultScreen(dpy);
    if( iscale > 0 ) ialab=99;
    if ( invert == 0 )
    {
      bg = BlackPixel(dpy, screen);
      fg = WhitePixel(dpy, screen);
    }
    else
    {
      fg = BlackPixel(dpy, screen);
      bg = WhitePixel(dpy, screen);
    }

    xsh.flags = (PPosition | PSize);

/* width and height of XWindow (600,400) */
    xsh.width = width+100;
    xsh.height = height+100; 

/* x and y from left and from top (10,1) */
    xsh.x = 10;
    xsh.y = 1;

    valuemask =  CWBackPixel;

    attributes.override_redirect = False;
    attributes.background_pixel = fg;

/* Create main Window */
    eng = XCreateWindow(dpy, DefaultRootWindow(dpy),
                        xsh.x, xsh.y, xsh.width+ialab, xsh.height,
                        1, 0, InputOutput, CopyFromParent,
                        valuemask, &attributes);


/* Do error-handling for eng */
    switch (eng) 
    {
        case BadAlloc : printf("Window create: BadAlloc\n");
                        exit(1);
                        break;
        case BadMatch : printf("Window create: BadMatch\n");
                        exit(1);
                        break;
        case BadValue : printf("Window create: BadValue\n");
                        exit(1);
                        break;
        case BadWindow : printf("Window create: BadWindow\n");
                        exit(1);
                        break;
    }

/* Create child windows */
    gwin1 = XCreateSimpleWindow(dpy, eng, 0, 0, width-1, height-1, 1, fg, bg);
    andy = gwin1;
    lab  = XCreateSimpleWindow(dpy, eng, width, height, 99+ialab, 99, 1, bg, fg);
    alab = XCreateSimpleWindow(dpy, eng, width, 0, 99+ialab, height, 1, bg, fg);
    blab = XCreateSimpleWindow(dpy, eng, 0, height, width, 99, 1, bg, fg);

/* Create virtual Windows for buffered output */
    if( mmax101 > 50 )
    {
       vgwin1 = XCreatePixmap(dpy, eng,
                            xsh.width, xsh.height,
                            DefaultDepthOfScreen(XtScreen(toplevel)) );
       vandy = vgwin1;
       vlab = XCreatePixmap(dpy, eng,
                         xsh.width, xsh.height,
                         DefaultDepthOfScreen(XtScreen(toplevel)) );
       valab = XCreatePixmap(dpy, eng,
                          xsh.width, xsh.height,
                          DefaultDepthOfScreen(XtScreen(toplevel)) );
       vblab = XCreatePixmap(dpy, eng,
                          xsh.width, xsh.height,
                          DefaultDepthOfScreen(XtScreen(toplevel)) );
    }
    else
    {
       vandy = andy;
       vlab = lab;
       valab = alab;
       vblab = blab;
    }

    Create_Colors();
    printf("Create_Color_Map %d %p \n",1,gcMap[1]); 

    if (i03d == 1)
    {
       printf("RH    Right   Up  ^  Out   \n");
       printf("View: x: |->, y:  |, z: (o)\n");
    }
    else if (i03d == 2)
    {
       printf("RH    Out     Right   Up  ^\n");
       printf("View: x: (o), y: |->, z:  |\n");
    }
    else if (i03d == 3)
    {
       printf("LH    Right   Out     Up  ^\n");
       printf("View: x: |->, y: (o), z:  |\n");
    }
    else if (i03d == 4)
    {
       printf("RH    Right   In      Up  ^\n");
       printf("View: x: |->, y: (x), z:  |\n");
    }
    else if (i03d == 5)
    {
       printf("LH    Left    Up  ^   Out   \n");
       printf("View: x: <-|, y:  | , z: (o)\n");
    }
    else if (i03d == 6)
    {
       printf("LH    Out     Right   Down |\n");
       printf("View: x: (o), y: |->, z:   v\n");
    }
    else
       printf("View not defined ... check -3dturn option! \n");

/* Define Font */
    if( mmax101 > 50 )
    {
       gcFont = XCreateGC(dpy, lab, 0, 0);
       XSetForeground(dpy, gcFont, bg);

/* Font-File : helvB24 = XLoadQueryFont(dpy,"xeng_iso");*/
/* Use Standard X-Font from directory /usr/lib/X11/fonts/iso_8859.1/75dpi  */
       helvB24 = XLoadQueryFont(dpy,"timR18");     
       if( helvB24 != NULL )
           XSetFont(dpy,gcFont,helvB24->fid);
       else
           printf("Error: Font timR18 not found \n");

       gcSFont = XCreateGC(dpy, lab, 0, 0);
       XSetForeground(dpy, gcSFont, bg);
       helvB24 = XLoadQueryFont(dpy,"timR12");     
       if( helvB24 != NULL )
           XSetFont(dpy,gcSFont,helvB24->fid);
       else
           printf("Error: Font timR12 not found \n");
    }
    else
    {
     if( mfont > 0 )
     {
       gcFont = XCreateGC(dpy, lab, 0, 0);
       XSetForeground(dpy, gcFont, bg);
       helvB24 = XLoadQueryFont(dpy,"/isr/lib/X11/fonts/misc/stan");     
       XSetFont(dpy,gcFont,helvB24->fid);

       gcSFont = XCreateGC(dpy, lab, 0, 0);
       XSetForeground(dpy, gcSFont, bg);
     }
    }

/* Write the Name in the Top-Bar */
    XStoreName (dpy, eng, "XBalls v2.01 (3D) luii - 16.09.08");

/* Select the window, from which Input is expected */
    XSelectInput (dpy, eng, ExposureMask);
    XSelectInput (dpy, eng, ButtonPressMask);
    XSelectInput (dpy, eng, KeyPressMask);
    XMapWindow(dpy, andy);

/*  Map All Windows */
    XMapWindow(dpy, eng);
    XMapWindow(dpy, lab);
    XMapWindow(dpy, alab);
    XMapWindow(dpy, blab);

/* Infinite Event-Loop */
    for (;;)
    {
      XNextEvent(dpy, &event_xballs);
      if (event_xballs.type == Expose) break;
      if (event_xballs.type == KeyPress) break;
    }

    XFlush(dpy);


/* Interval timer start	 */
    XtAddTimeOut(1u, Cycle(), NULL );
    printf("finished drawing loop ... \n");
    printf("press any key to quit ... \n");

    if( idie == 0 )
    {
      for (;;) 
      {
        XNextEvent(dpy,&event_xballs);
        /* printf("event.type %d %d \n",event_xballs.type,event_xballs.xkey); */
        if (event_xballs.type == Expose) redraw_callback();
        if (event_xballs.type == KeyPress) break;
      }
    }

}
/*** end of main procedure ***/

XtCallbackProc redraw_callback()
{
	Window root;
	int xrt,yrt;
	unsigned int wrt,hrt,brt,drt;

	XGetGeometry(dpy,andy,&root,&xrt,&yrt,&wrt,&hrt,&brt,&drt);
	width = wrt;
	height = hrt;

        XCopyArea ( dpy, vandy, andy, gcBlack, 0, 0, width, height, 0, 0 );
        XCopyArea ( dpy, vlab, lab, gcBlack, 0, 0, width, height, 0, 0 );
        XCopyArea ( dpy, valab, alab, gcBlack, 0, 0, width, height, 0, 0 );
        XCopyArea ( dpy, vblab, blab, gcBlack, 0, 0, width, height, 0, 0 );

        XFlush(dpy);
        return NULL;
}

XtTimerCallbackProc DummyCycle()
{
        return NULL;
}

XtTimerCallbackProc Cycle()
{
        return X_Draw_Snap(c2dfile);
}

void Draw_Contacts( float xy0, int h_offset, int v_offset,
                    float xskal, float yskal, float xleft, 
                    float xright, float xbottom, float xtop,
                    int NK, float *px, float *pz, float *prad )
{
  FILE *fpw;
  int ierr, icolor;
  int drnc,drnw,ic1,ic2,ip1,ip2;
  float drtime,drk1,drT,drTm,drda;
  float linex0,linez0,linex1,linez1;
  float drdelta,drdeltamax,drchi1,drchi2,drfn,drft,drf0;
  float muf0,p12dist;

  fpw = fopen(drfile,"r");
  if ( fpw == 0 ) 
  {
     printf("Error opening wall data file `%s' %f \n", drfile,px[0]);
     return;
  }

  /* read in first line of contact data file */
  ierr = fscanf(fpw,"%f %d %d %f %f %f %f",
                    &drtime,&drnc,&drnw,&drk1,&drT,&drTm,&drda);
  if( ierr == 7 )
  {
      printf("%f %d %d %f %f %f %f \n",drtime,drnc,drnw,drk1,drT,drTm,drda);

      for( ic1=1; ic1<=drnc; ic1++ )
      {
         for( ic2=1; ic2<=drnc; ic2++ )
         {
             ierr = fscanf(fpw,"%d %d %f %f %f %f %f %f %f",&ip1,&ip2,
                     &drdelta,&drdeltamax,&drchi1,&drchi2,&drfn,&drft,&drf0);
             if( ierr == 9 )
             {
                 /* printf("%d %d %f %f %f %f %f %f %f \n",ip1,ip2,
                     drdelta,drdeltamax,drchi1,drchi2,drfn,drft,drf0); */
                 if( ip2 >= 0 )
                 {
                   linex0 = - h_offset + (int)(xy0 + xskal*px[ip1]);
                   linez0 = - v_offset + (int)(height - xy0 - yskal*pz[ip1]);
                   linex1 = - h_offset + (int)(xy0 + xskal*px[ip2]);
                   linez1 = - v_offset + (int)(height - xy0 - yskal*pz[ip2]);

                   icolor=mmax101/2 +drfn*drfnskal;
                   if( icolor < 2 ) icolor = 2;
                   if( icolor > mmax101 ) icolor = mmax101-1;

                   XDrawLine(dpy, vandy, gcMap[icolor], linex0, linez0, linex1, linez1 ); 
                   /* 
                   printf("%f %f %f %d \n", xy0, xskal, yskal, height );
                   printf("%f %f %f %f \n", px[ip1], pz[ip1], px[ip2], pz[ip2] );
                   printf("%f %f %f %f \n", linex0, linez0, linex1, linez1 );
                   */

                   muf0=drft/(drfn-drf0);
                   p12dist=sqrt((px[ip2]-px[ip1])*(px[ip2]-px[ip1])+(pz[ip2]-pz[ip1])*(pz[ip2]-pz[ip1]));
                   linex1 = 1+(int)(xskal*p12dist*muf0);
                   linez1 = 1+(int)(yskal*p12dist*muf0);

                   linex0 = - h_offset + (int)(xy0          + xskal*(px[ip1] + prad[ip1]/(prad[ip1]+prad[ip2]) * (px[ip2]-px[ip1])) -linex1/2.);
                   linez0 = - v_offset + (int)(height - xy0 - yskal*(pz[ip1] + prad[ip1]/(prad[ip1]+prad[ip2]) * (pz[ip2]-pz[ip1])) -linez1/2.);

                   icolor=(int)(mmax101*2*muf0);
                   if( icolor < 2 ) icolor = 2;
                   if( icolor > mmax101 ) icolor = mmax101-1;
                   /* printf("%d %d %f %d \n", ip1,ip2,muf0,icolor ); */

                   XFillArc(dpy, vandy, gcMap[icolor], linex0, linez0, linex1, linez1, 0, 360*64);

                 }
                 else
                 {
                  if((drwcont==1) && (ip2>=-4) && (ip2<=-1))
                  {
                   linex0 = - h_offset + (int)(xy0 + xskal*px[ip1]);
                   linez0 = - v_offset + (int)(height - xy0 - yskal*pz[ip1]);
                   if(-ip2==1)
                   {
                     linex1 = - h_offset + (int)(xy0          + xskal*(px[ip1]));
                     linez1 = - v_offset + (int)(height - xy0 - yskal*(pz[ip1]-prad[ip1]));
                   }
                   if(-ip2==2)
                   {
                     linex1 = - h_offset + (int)(xy0          + xskal*(px[ip1]));
                     linez1 = - v_offset + (int)(height - xy0 - yskal*(pz[ip1]+prad[ip1]));
                   }
                   if(-ip2==3)
                   {
                     linex1 = - h_offset + (int)(xy0          + xskal*(px[ip1]-prad[ip1]));
                     linez1 = - v_offset + (int)(height - xy0 - yskal*(pz[ip1]));
                   }
                   if(-ip2==4)
                   {
                     linex1 = - h_offset + (int)(xy0          + xskal*(px[ip1]+prad[ip1]));
                     linez1 = - v_offset + (int)(height - xy0 - yskal*(pz[ip1]));
                   }
                   icolor=mmax101/2 +drfn*drfnskal;
                   if( icolor < 2 ) icolor = 2;
                   if( icolor > mmax101 ) icolor = mmax101-1;

                   XDrawLine(dpy, vandy, gcMap[icolor], linex0, linez0, linex1, linez1 ); 
                  }
                 }
             }
             else
             {
                 ic1=drnc+1;
                 ic2=drnc+1;
             }
         }
      }
  }
  else
  {
      printf("Error drfile read %d \n",ierr);
  }

  fclose( fpw );

}


void Draw_Walls( float xy0, int h_offset, int v_offset, 
                 float xskal, float yskal, float xleft, 
                 float xright, float xbottom, float xtop )
{
#define IWMAX 1000

  FILE *fpw;
  int iwall, ierr, ierr2;
  float xwtyp,nx,ny,nz,x0,y0,z0;
  float linex0, linex1, linez0, linez1;
  float xwleft,xwright,xwbottom,xwtop,xwrear,xwfront;
  float xL=xright-xleft; 
  float yL=xtop-xbottom;
  float xyL=xL/yL;
  float sx, sz, sx0, sx1, sz0, sz1, an, anx, any, anz;
  short xa, ya;
  short a1, a2;
  unsigned short ixrad, iyrad;
  char comment[250];

  fpw = fopen(wallfile, "r");

  if ( fpw == 0 ) 
  {
     printf("Error opening wall data file `%s' \n", wallfile);
     return;
  }

  /* printf("opening wall data file %s \n", wallfile); */

  for( iwall=1; iwall<=iwnum; iwall++)
  {
     /* ierr = fscanf( fpw, "%f %f %f %f %f %f %f %f %f %f %f %f %f ",&xwtyp,&nx,&ny,&nz,&x0,&y0,&z0,&xwleft,&xwright,&xwrear,&xwfront,&xwbottom,&xwtop); */
     ierr = fscanf( fpw, "%f %f %f %f %f %f %f ",&xwtyp,&nx,&ny,&nz,&x0,&y0,&z0);
     /* accept only seven numbers as straight wall format */
     if( ierr == 7 ) 
     {
      /* Wall-type < 100 is a straight wall within the system boundaries */
      /* Wall-type = 2 is a front/back wall with normal out of plane --
                       therefore don't draw it */
      if( xwtyp < 100 )
      {
      /* Wall-type = 2 is a front/back wall with normal out of plane --
                       therefore don't draw it */
      if( xwtyp != 2 )
      {
       an = sqrt(nx*nx+ny*ny+nz*nz);
       anx = nx/an;
       any = ny/an;
       anz = nz/an;
       sx0 = xleft;
       sz0 = xbottom;
       sx1 = xright;
       sz1 = xtop;
       if( (anx) == 0 ){
          sz0 = z0;
          sz1 = z0;
          sx0 = xleft;
          sx1 = xright;
       }
       if( (anz) == 0 ){
          sx0 = x0;
          sx1 = x0;
          sz0 = xbottom;
          sz1 = xtop;
       }
       if(( (anx) != 0 ) && ( (anz) != 0 )){
          sx0 = x0+anz/anx*(z0-xbottom);
          if( sx0 > xright){
             sx0=xright;
             sz0=z0-anx/anz*(xright-x0);
          }
          if( sx0 < xleft){
             sx0=xleft;
             sz0=z0-anx/anz*(xleft-x0);
          }
          sx1 = x0+anz/anx*(z0-xtop);
          if( sx1 > xright){
             sx1=xright;
             sz1=z0-anx/anz*(xright-x0);
          }
          if( sx1 < xleft){
             sx1=xleft;
             sz1=z0-anx/anz*(xleft-x0);
          }
       }
       /* printf(" %f %f %f %f %f %f \n", anx, anz, sx0, sx1, sz0, sz1 ); */

       linex0 = - h_offset + (int)(xy0 + xskal*sx0);
       linez0 = - v_offset + (int)(height - xy0 - yskal*sz0);
       linex1 = - h_offset + (int)(xy0 + xskal*sx1);
       linez1 = - v_offset + (int)(height - xy0 - yskal*sz1);

       /* printf("box-boundary: %f %f %f %f \n", linex0, linez0, linex1, linez1 ); */

       XDrawLine(dpy, vandy, gcBlack, linex0, linez0, linex1, linez1 );
      }
      }
      else if(( xwtyp > 99 ) && ( xwtyp < 1000 ))
      {
       ixrad=(int)(nx*xskal*2);
       iyrad=(int)(nz*yskal*2);
       xa=-h_offset + (int)( xy0+(x0)*xskal ) - ixrad/2;
       ya=height - v_offset + (int)(- xy0-(z0)*yskal ) - iyrad/2;
       a1=0;
       a2=360*64;
       XDrawArc(dpy, vandy, gcBlack, xa, ya, ixrad, iyrad, a1, a2);
      }
      else
      {
         printf("Warning: Unknown wall-type %f \n",xwtyp);
      }
     }
     else 
     {
       printf(" %f %f %f %d ",xwtyp,xwleft,xwright,ierr);
       if( ierr == 9 )
       {
          ierr = fscanf( fpw, "%f %f ",&xwleft,&xwright);
          printf(" : %f %f %f %d ",xwtyp,xwleft,xwright,ierr);
          if( xwtyp == 1001 )
          {
            sx0=x0;
            sx1=x0;
            sz0=xwleft;
            sz1=xwright;
            linex0 = - h_offset + (int)(xy0 + xskal*sx0);
            linex1 = - h_offset + (int)(xy0 + xskal*sx1);
            linez0 = - v_offset + (int)(height - xy0 - yskal*sz0);
            linez1 = - v_offset + (int)(height - xy0 - yskal*sz1);
            XDrawLine(dpy, vandy, gcBlack, linex0, linez0, linex1, linez1 );
            printf(" (1001) %f %f %f %f \n",sx0,sx1,sz0,sz1 );
            printf(" (1001) %f %f %f %f \n",linex0,linex1,linez0,linez1 );
          }
          if( xwtyp == 1003 )
          {
            sx0=xwleft;
            sx1=xwright;
            sz0=z0;
            sz1=z0;
            linex0 = - h_offset + (int)(xy0 + xskal*sx0);
            linex1 = - h_offset + (int)(xy0 + xskal*sx1);
            linez0 = - v_offset + (int)(height - xy0 - yskal*sz0);
            linez1 = - v_offset + (int)(height - xy0 - yskal*sz1);
            XDrawLine(dpy, vandy, gcBlack, linex0, linez0, linex1, linez1 );
            printf(" (1003) %f %f %f %f \n",sx0,sx1,sz0,sz1 );
            printf(" (1003) %f %f %f %f \n",linex0,linex1,linez0,linez1 );
          }
       }
       else
       {
         iwall--;
         printf(" > %d | %d < ",ierr,iwall);
         if( ierr >= 0 )
         {
           ierr = fscanf( fpw, "%s", comment);
           printf(" %s",comment);
         }
         else
           iwall = iwnum+1;
       }
     }
  }

  fclose( fpw );

}


XtTimerCallbackProc  X_Draw_Snap(char *cfile )
{

   FILE  *fp, *fpp;
   int   NK, NK0, NK1, NK2, NKdy, ierr, ierr6, i, j, j0, iistep;
   int   v_offset, h_offset;
   int   *ipp;
   float *x1,*x2,*x3,*xrad,*unif,*unig,*x4;
   float *x61,*x6,*x62,*x71,*x7,*x72;
   float *x10, *x12, *x20, *x32;
   float *vabs,*va0,*wabs,*v1,*v2;
   float *ix, *ccode, x1dumy, x2dumy;
   float R11, R12, R13, R21, R22, R23, R31, R32, R33;
   float xskal, yskal, xthick, dx, dy;
   float xy0, xleft, xright, xrear, xfront, xbottom, xtop;
   float vabs000, vabsmax,vabsmean,vabsmin;
   float xhcm, vhcm, vzcm, whcm, logvcm, logxhcm, logwhcm;
   float dens[NFIELD+1], densc[NFIELD+1], velo[NFIELD+1], wrad[NFIELD+1];
   float hdens[NFIELD+1], hvelo[NFIELD+1], hwrad[NFIELD+1];
   float densmax, velomax, wradmax;
   float hdensmax, hvelomax, hwradmax;
   float xradius, time, timedy, time0f, xnewrad;
   float xd, xx1, xx2, xx12, xx3, xx4;
   float xy1, xy2, xy3, xy4;
   float cmaxskal, srp, crp;
   int   ichnext, ichcolor;
   int   ixcenter, ixradius;
   int   icolor, jcolor, isqcolor;
   int   sum_add, vsum_add, wsum_add;
   int   ifperr;
   float ixmax, ixmean, ncoll_max;
   float pmax, pmin, pval, pval0, pvalj, pvalk;

   int   ixrad, iyrad;
   int   bx1, by1; 
   int   bx2, by2;
   int   nr; /* Schleifenvariable zum Zeichnen des grids */
   int   NKOLD = -1;
   int   icount = 0;
   float radmax, radmin;
   char	 rpm[18],repm[40];

   void  fill_dens();
   void  fill_velo();
   void  fill_wrad();

         NK = -1;
         NK0 = 0;
         NK1 = 0;
         NK2 = 0;
         cmaxskal=1.;
         ixmax=0.0;
         ixmean=0.0;
         vabsmin=1.e10;
         vabsmax=-1.10;
         vabsmean=0.;

         densmax = 1.0e-10;
         velomax = 1.0e-10;
         wradmax = 1.0e-10;
         hdensmax = 1.0e-10;
         hvelomax = 1.0e-10;
         hwradmax = 1.0e-10;
         sum_add = 1;
         vsum_add = 1;
         wsum_add = 1;
         iistep = 0;
         for( i=1; i<=mmax101; i++)
         {
            cccmean[i]=0.0;
            iicmean[i]=0;
         }

         printf("# xballs v1.97 3D (c) by lui \n");

         if ( !strcmp(cfile,"-" ))
         {
            fp = stdin;
         }
         else
         {
            fp = fopen(cfile, "r");
         }

         if ( fp == 0 ) 
         {
            printf("Error opening File %s \n", cfile);
         }
         else
         {
           printf("# Reading File %s \n", cfile);

           while (( ierr6 = read_header( inread, fp, &NK, &time, &xleft, &xrear, &xbottom, 
                                                                 &xright, &xfront, &xtop )) >= 5 )
           {
             /* printf("read %d %f %d %d %f %f\n",iistep,time,NK,ierr6,xright,xtop); */

             if(xtop0<=0) xtop0=xtop;
             if(xright0<=0) xright0=xright;
             time_step=time-time_last;
             time_last=time;
             ixmax=0.0;
             ixmean=0.0;
             vabsmin=1.e10;
             vabsmax=-1.10;
             vabsmean=0.;
             x12min=1.e10;
             x12max=-1.e10;
             radmax=0.;
             radmin=1.e10;
             NK0=NK;

             if( ic0 > 0 )
             {
               fpp = fopen(c0file, "r");
               if( fpp == 0 )
               {
                  printf("Error opening File %s \n", c0file);
                  NK1=0;
               }
               else
               {
                  ierr=read_header( inread, fpp, &NK1, &time0f, &xleft, &xrear, &xbottom,
                                                                &xright, &xfront, &xtop );
                  /* printf("fpp read %d %d %d %d %d \n",ierr,NK,NK0,NK1,NKOLD); */
               } 
             }
             if(NK1<1) NK1=0;
             NK0=NK+NK1;

        /* test maximum fieldlength */
             if( NK0 >= NKMAX )
                printf("Too many paricles %d %d > %d ! ",NK,NK0,NKMAX);
             else
             {
                if( icube > 0 ) NK2=8;
                else            NK2=0;

                if( NK0 > NKOLD )
                {
                  if(NKOLD != -1)
                  {
                   printf("Free memory from %d particles \n",NKOLD);
                   free(x1);
                   free(x2);
                   free(x3);
                   free(xrad);
                   free(unif);
                   free(unig);
                   free(x4);
                   free(x6);
                   free(x7);
                   free(x61);
                   free(x62);
                   free(x71);
                   free(x72);
                   free(x10);
                   free(x20);
                   free(x12);
                   free(x32);
                   free(vabs);
                   free(wabs);
                   free(va0);
                   free(v1);
                   free(v2);
                   free(ix);
                   free(ipp);
                   free(ccode);
                  }
                  NKOLD=NK0;

                  printf("Allocate memory for %d particles: %d moving, %d fixed \n",NK0,NK,NK1);
                  x1=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x2=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x3=(float*) calloc(NK0+NK2+1,sizeof(float));
                  xrad=(float*) calloc(NK0+NK2+1,sizeof(float));
                  unif=(float*) calloc(NK0+NK2+1,sizeof(float));
                  unig=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x4=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x6=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x7=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x61=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x62=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x71=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x72=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x10=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x20=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x12=(float*) calloc(NK0+NK2+1,sizeof(float));
                  x32=(float*) calloc(NK0+NK2+1,sizeof(float));
                  vabs=(float*) calloc(NK0+NK2+1,sizeof(float));
                  va0=(float*) calloc(NK0+NK2+1,sizeof(float));
                  wabs=(float*) calloc(NK0+NK2+1,sizeof(float));
                  v1=(float*) calloc(NK0+NK2+1,sizeof(float));
                  v2=(float*) calloc(NK0+NK2+1,sizeof(float));
                  ix=(float*) calloc(NK0+NK2+1,sizeof(float));
                  ccode=(float*) calloc(NK0+NK2+1,sizeof(float));

                  ipp=(int*) calloc(NK0,sizeof(int));
                }
             }

             if( icube > 0 )
             {
        /* put the system boundaries to the eight last particles NK+NK1+NK2-7 to NK+NK1+NK2 */
              x1[NK+NK1+NK2-7]=xleft;
              x12[NK+NK1+NK2-7]=xrear;
              x2[NK+NK1+NK2-7]=xbottom;
              x1[NK+NK1+NK2-6]=xleft;
              x12[NK+NK1+NK2-6]=xrear;
              x2[NK+NK1+NK2-6]=xtop;
              x1[NK+NK1+NK2-5]=xright;
              x12[NK+NK1+NK2-5]=xrear;
              x2[NK+NK1+NK2-5]=xbottom;
              x1[NK+NK1+NK2-4]=xleft;
              x12[NK+NK1+NK2-4]=xfront;
              x2[NK+NK1+NK2-4]=xbottom;
              x1[NK+NK1+NK2-3]=xleft;
              x12[NK+NK1+NK2-3]=xfront;
              x2[NK+NK1+NK2-3]=xtop;
              x1[NK+NK1+NK2-2]=xright;
              x12[NK+NK1+NK2-2]=xfront;
              x2[NK+NK1+NK2-2]=xbottom;
              x1[NK+NK1+NK2-1]=xright;
              x12[NK+NK1+NK2-1]=xrear;
              x2[NK+NK1+NK2-1]=xtop;
              x1[NK+NK1+NK2]=xright;
              x12[NK+NK1+NK2]=xfront;
              x2[NK+NK1+NK2]=xtop;
             }

             while( time < -1e10 )
	     {
        /* read the data from File */

              for ( i=1; i<=NK; i++)
              {
        /* read x,y,vx,rad,vy */
                ierr=read_line( inread, fp, 
                             &x1[i], &x2[i], &x3[i], &x4[i], &xrad[i], &x6[i], &x7[i], &ix[i],
                             &x12[i], &x32[i], &x61[i], &x62[i], &x71[i], &x72[i] );

        /* read x,y,vx,vy,rad,r,w,ncoll */
                if ( ierr < 1 )
                {
                   printf("Error 1 %d reading File %s in Line %d at time %f \n", ierr, cfile, i, time);
                   i=NK+1;
                   return DummyCycle();
                }

              }
              ierr = read_header( inread, fp, &NKdy, &timedy, &xleft, &xrear, &xbottom,
                                                              &xright, &xfront, &xtop );
              time_step=time-time_last;
              time_last=time;
              if ( ierr < 1 )
              {
                   printf("Error 2 %d reading File %s in Line %d\n", ierr, cfile, i);
                   i=NK+1;
                   return DummyCycle();
              }
              if( NKdy > NK )
              {
                   printf("Error 21 %d > %d reading File %s \n", NKdy, NK, cfile);
                   i=NK+1;
                   return DummyCycle();
              }

             }


             for( iistep=0; iistep<istep; iistep++  )
	     {
        /* read the data from File */

              for ( i=1; i<=NK; i++)
              {
                if( ivector > 0 )
                {
                 if( iistep==0 )
                 {
                   x10[i]=x1[i]; 
                   x20[i]=x2[i]; 
                 }
                }
        /* read x,y,vx,rad,vy */
                if( iistep>0 )
                {
                  ierr=read_line( inread, fp,
                             &x1[i], &x2[i], &x3[i], &x4[i], &xrad[i], &x6[i], &x7[i], &ix[i],
                             &x12[i], &x32[i], &x61[i], &x62[i], &x71[i], &x72[i] );

        /* read x,y,vx,vy,rad,r,w,ncoll */
                 if ( ierr < 1 )
                 {
                   printf("Error 3 %d reading File %s in Line %d\n", ierr, cfile, i);
                   i=NK+1;
                   return DummyCycle();
                 }
                }
               }
               if( iistep>0 )
               {
                 ierr = read_header( inread, fp, &NKdy, &timedy, &xleft, &xrear, &xbottom,
                                                                 &xright, &xfront, &xtop );
                 time_step=time-time_last;
                 time_last=time;
		 printf("dummy read %d %f\n",iistep,time);
                 if ( ierr < 1 )
                 {
                   printf("Error 4 %d reading File %s in Line %d\n", ierr, cfile, i);
                   i=NK+1;
                   return DummyCycle();
                 }
                 if( NKdy > NK )
                 {
                      printf("Error 41 %d > %d reading File %s - use: -step 1 ! \n", NKdy, NK, cfile);
                      i=NK+1;
                      return DummyCycle(); 
                 }
               }
             }

        /* scale picture */
             xskal = xyskal*SKAL400;
             yskal = xyskal*SKAL400;
             xy0 = (width+height)/29.; 

        /* start output */

        /* write text to lab-window */
            if( mfont > 0 )
            {
             XFillRectangle(dpy, vlab, gcWhite, 0, 0,
                            100+ialab, 100 );
             sprintf(rpm,"N = %d ",(NK));
             XDrawString( dpy, vlab, gcFont, 10, 20, rpm, strlen(rpm));
             sprintf(rpm,"t = %f s",time);
             XDrawString( dpy, vlab, gcFont, 10, 40, rpm, strlen(rpm));
            }

        /* do nothing if t < t_minimal */
            if( time >= tmin )
            {
        /* clear graphics-windows */
               if( nodel > 0 )
               {
                  XFillRectangle(dpy, vandy, gcWhite, 0, 0,
                                 width-1, height-1 );
                  nodel -= 1;
                  if( nodel == 1 ) nodel = 2;
               }
               XFillRectangle(dpy, valab, gcWhite, 5, 0,
                              94+ialab, height );
               XFillRectangle(dpy, vblab, gcWhite, 0, 0,
                              width, 100 ); 

        /* draw walls */
               v_offset = offset;
               h_offset = hoffset;
               if( strcmp(wallfile, "-") )
                 Draw_Walls( xy0, h_offset, v_offset, xskal, yskal,
                             xleft, xright, xbottom, xtop );

        /* draw box */
               xthick = (xright-xleft)/10.;
               if( noborder < 4 )
               {
                bx1=0;
                by1=height -v_offset - (int) (xy0+(xbottom)*yskal);
                if(by1 < 0) by1=0;
                bx2=(int) (width);
                by2=(int) (height);
                XFillRectangle(dpy, vandy, gcGrey, bx1, by1, bx2, by2);       /* lower */

                bx1=0;
                by1=0;
                bx2=(int) (xy0+(xleft)*xskal)  - h_offset;
                if(bx2 < 0) bx2=0;
                by2=(int) (height); 
                XFillRectangle(dpy, vandy, gcGrey, bx1, by1, bx2, by2);       /* left */

                bx1=0;
                by1=0;
                bx2=(int) (width);
                by2=height -v_offset - (int) (xy0+(xtop)*yskal);
                if(by2 < 0) by2=0; 
                XFillRectangle(dpy, vandy, gcGrey, bx1, by1, bx2, by2);        /* top */

                bx1=(int) (xy0+(xright)*xskal)  - h_offset;
                if(bx1 > (int)(width)) bx1=(int)(width);
                by1=0;
                bx2=(int) (width);
                by2=(int) (height); 
                XFillRectangle(dpy, vandy, gcGrey, bx1, by1+1, bx2, by2);         /* right */ 
               }
               /* printf("nob %d ",noborder); */
               if( noborder == 5 )
               {
                bx1=0;
                by1=height -v_offset - (int) (xy0+(xbottom)*yskal);
                if(by1 < 0) by1=0;
                bx2=(int) (width);
                by2=(int) (height);
                XFillRectangle(dpy, vandy, gcBlue, bx1, by1, bx2, by2);       /* lower */
               }
               if( noborder < 3 )
               {
                if(( noborder < 1 ) || ( noborder == 2 ))
                { 
                 XFillRectangle(dpy, vandy, gcBlue, 
                   (int) (xy0+(xleft)*xskal) - h_offset, 
                   height -v_offset - (int) (xy0+(xbottom)*yskal), 
                   1+(int) ((xright-xleft)*xskal), 1+(int) (xthick*yskal)-1); /* lower boundary */
                 XFillRectangle(dpy, vandy, gcBlue, 
                   (int) (xy0+(xleft)*xskal) - h_offset, 
                   height -v_offset - (int) (xy0+(xtop+xthick)*yskal-1.), 
                   1+(int) ((xright-xleft)*xskal), (int) (xthick*yskal)); /* upper boundary */
                }
                if( noborder < 2 )
                { 
                 XFillRectangle(dpy, vandy, gcBlue, 
                   (int) (xy0+(xleft-xthick)*xskal)  - h_offset, 
                   height -v_offset - (int) (xy0+(xtop+xthick)*yskal), 
                   (int) (xthick*xskal+1.), (int) ((xtop-xbottom+2.*xthick)*yskal)); /* left boundary */
                 XFillRectangle(dpy, vandy, gcBlue, 
                   (int) (xy0+xright*xskal) - h_offset, 
                   height -v_offset - (int) (xy0+(xtop+xthick)*yskal), 
                   (int) (xthick*xskal), (int) ((xtop-xbottom+2.*xthick)*yskal)); /* right boundary */
                }
               }

               /* insert the time in the data window */
               if( mfont > 0 )
               {
                if( ietime > 0 )
                {
                  sprintf(repm,"ezz =%8.4f   exx =%8.4f   eV =%8.4f",1-xtop/xtop0,1-xright/xright0,1-xright*xtop/(xright0*xtop0));
                  XDrawString( dpy, vandy, gcFont, ietime, ietime, repm, strlen(repm));
                }
                if( i0time > 0 )
                  XDrawString( dpy, vandy, gcFont, i0time, i0time, rpm, strlen(rpm));
                if( i0time < 0 )
                {
                  sprintf(rpm,"t =%7.3f s",time);
                  XDrawString( dpy, vandy, gcSFont, -i0time, -i0time, rpm, strlen(rpm));
                }
                if(( i0tposx > 0 ) || ( i0tposy > 0 ))
                {
                  sprintf(rpm,"c");
                  XDrawString( dpy, vandy, gcFont, i0tposx+3, i0tposy-2, rpm, strlen(rpm));
                  sprintf(rpm,"O S.Luding 1999");
                  XDrawString( dpy, vandy, gcFont, i0tposx, i0tposy, rpm, strlen(rpm));
                }
               }

	       /* draw Grid */
	       if (xcells>0)
               {
		 for (nr=0; nr<=xcells; nr++){
                        XDrawLine(dpy, vandy, gcBlack,
				  (int) ( xy0+(xleft+nr*(xright-xleft)*xskal/xcells )) - h_offset,
					  height -v_offset - (int)( xy0+(xtop)*yskal), 
				  (int) ( xy0+(xleft+nr*(xright-xleft)*xskal/xcells )) - h_offset,
					  height -v_offset - (int) (xy0+(xbottom)*yskal) );
		   
		 };
	       };
	       if (zcells>0)
               {
		 for (nr=0; nr<=zcells; nr++)
                 {
                    XDrawLine(dpy, vandy, gcBlack,
     (int) ( xy0+(xleft )) - h_offset,
     height -v_offset - (int)( xy0+(xbottom+nr*(xtop-xbottom)/zcells)*yskal), 
     (int) ( xy0+(xleft+(xright-xleft)*xskal )) - h_offset,
     height -v_offset - (int) (xy0+(xbottom+nr*(xtop-xbottom)/zcells)*yskal) );
		 };
	       };

        /* initialize fields and variables */
               {
                 xhcm = 0.;
                 vhcm = 0.;
                 vzcm = 0.;
                 whcm = 0.;
                 ncoll_max = 0.0;
                 densmax = 1.0e-10;
                 velomax = 1.0e-10;
                 wradmax = 1.0e-10;
                 hdensmax = 1.0e-10;
                 hvelomax = 1.0e-10;
                 hwradmax = 1.0e-10;
                 if( ( ladd == 0 ) || ( sum_add == 1 ) )
                 {
                    for ( i=1; i<=NFIELD; i++)
                    {
                      dens[i] = 0.;
                      velo[i] = 0.;
                      wrad[i] = 0.;
                      densc[i] = 0.;
                      hdens[i] = 0.;
                      hvelo[i] = 0.;
                      hwrad[i] = 0.;
                    }
                 }
               } /* end ini-fields */
             }
             /* start redraw loop */
             for( iredraw=1; iredraw<=nredraw; iredraw++ )
             {
              if( iredraw > 1 )
              {
        /* clear graphics-windows */
                if( nodel > 0 )
                {
                   XFillRectangle(dpy, vandy, gcWhite, 0, 0,
                                  width-1, height-1 );
                   nodel -= 1;
                   if( nodel == 1 ) nodel = 2;
                }
                XFillRectangle(dpy, valab, gcWhite, 5, 0,
                               94+ialab, height );
                XFillRectangle(dpy, vblab, gcWhite, 0, 0,
                               width, 100 );


              }
              if( iscale > 0 ) 
              {
                 for ( i=1; i<=NK+NK1; i++) ccode[i]=0.0;
                 for( i=1; i<=mmax101; i++)
                 {
                   cccmean[i]=0.0;
                   iicmean[i]=0;
                 }
              }

        /* read the data from File and draw to output */
              if( iredraw == 1 )
               for ( i=1; i<=NK+NK1; i++)
               {
                if( i <= NK )
                   ierr=read_line( inread, fp,
                        &x1[i],&x2[i],&x3[i],&x4[i],&xrad[i],&x6[i],&x7[i],&ix[i],
                        &x12[i],&x32[i],&x61[i],&x62[i],&x71[i],&x72[i] );
                else
                {
                   ierr=read_line( inread, fpp,
                        &x1[i], &x2[i], &x3[i], &x4[i], &xrad[i], &x6[i], &x7[i], &ix[i],
                        &x12[i], &x32[i], &x61[i], &x62[i], &x71[i], &x72[i] );
                   xrad[i]=xrad[i]*r0rred;
                }
                  /* printf(" ierr=%d ", ierr); */

               /* read x,y,vx,vy,rad,r,w,ncoll */
                if ( ierr < 1 )
                {
                  printf("Error 5 %d reading File %s in Line %d\n", ierr, cfile, i);
                  printf("%f %f %f %f %f %f %f %f \n",
                   x1[i], x2[i], x3[i], x4[i], xrad[i], x6[i], x7[i], ix[i]);
                  i=NK+NK1+1;
                  /* return DummyCycle(); */
                }
                else
                {
            /* set radial scale factor to unity */
                  unif[i]=1.0;
                  unig[i]=1.0;
            /* center the coordinates with respect to the box */
                  if( icb > 0 )
                  {
                      x1[i]=x1[i]-(xright-xleft)/2.;
                      x2[i]=x2[i]-(xtop-xbottom)/2.;
                  }
            /* exchange coordinates if requested */
                  if( i03d > 0 )
                  {
                   if(( inread == 7 ) || ( inread == 4 ) || (inread == 14))
                   {
                    if( i03d == 1 )
                    {
                       xdummy = x2[i];
                       x2[i] = x12[i];
                       x12[i] = xdummy;
                       xdummy = x4[i];
                       x4[i] = x32[i];
                       x32[i] = xdummy;
                       xdummy = x62[i];
                       x62[i] = x61[i];
                       x61[i] = xdummy;
                       xdummy = x72[i];
                       x72[i] = x71[i];
                       x71[i] = xdummy;
                    }
                    else if( i03d == 2 )
                    {
                       xdummy = x1[i];
                       x1[i] = x12[i];
                       x12[i] = xdummy;
                       xdummy = x3[i];
                       x3[i] = x32[i];
                       x32[i] = xdummy;
                       xdummy = x6[i];
                       x6[i] = x61[i];
                       x61[i] = xdummy;
                       xdummy = x7[i];
                       x7[i] = x71[i];
                       x71[i] = xdummy;
                    }
                    else if( i03d == 4 )
                    {
                       x12[i] = xright-x12[i];
                       x32[i] = -x32[i];
                    }
                    else if( i03d == 5 )
                    {
                       xdummy = x2[i];
                       x2[i] = x12[i];
                       x12[i] = xdummy;
                       x1[i] = xright-x1[i];
                       xdummy = x4[i];
                       x4[i] = x32[i];
                       x32[i] = xdummy;
                       x3[i] = -x3[i];
                       xdummy = x62[i];
                       x62[i] = x61[i];
                       x61[i] = xdummy;
                       xdummy = x72[i];
                       x72[i] = x71[i];
                       x71[i] = xdummy;
                    }
                    else if( i03d == 6 )
                    {
                       xdummy = x1[i];
                       x1[i] = x12[i];
                       x12[i] = xdummy;
                       x2[i] = xtop-x2[i];
                       xdummy = x3[i];
                       x3[i] = x32[i];
                       x32[i] = xdummy;
                       x4[i] = -x4[i];
                       xdummy = x6[i];
                       x6[i] = x61[i];
                       x61[i] = xdummy;
                       xdummy = x7[i];
                       x7[i] = x71[i];
                       x71[i] = xdummy;
                    }
                   }
                  }
                /* printf("%f %f %f %f %f %f %f %f \n",
                   x1[i], x12[i], x2[i], x3[i], x32[i], x4[i], xrad[i], ix[i]); */

            /* averages */
                  if( xrad[i] > radmax ) radmax = xrad[i];
                  if( xrad[i] < radmin ) radmin = xrad[i];

                  if( inread == 8 || inread == 5 )
                  {
                   xhcm = xhcm + x2[i];
                   if( ivmode == 0 )
                      vabs[i] = sqrt(x3[i]*x3[i]+x4[i]*x4[i])-coffset;
                   else if(ivmode == 1 )
                      vabs[i] = x1[i]-coffset;
                   else if(ivmode == 2 )
                      vabs[i] = x2[i]-coffset;
                   else if(ivmode == 3 )
                      vabs[i] = x3[i]-coffset;
                   else if(ivmode == 4 )
                      vabs[i] = x4[i]-coffset;
                   else if(ivmode == 5 )
                      vabs[i] = xrad[i]-coffset;
                   else if(ivmode == 6 )
                      vabs[i] = x6[i]-coffset;
                   else if(ivmode == 7 )
                      vabs[i] = x7[i]-coffset;
                   else if(ivmode == 8 )
                      { 
                        if( incdt == 1 ) vabs[i] = ix[i]/time_step;
                        else             vabs[i] = ix[i];
                      }
                   else if(ivmode == 9 )
                      vabs[i] = x1[i]+x2[i]-coffset;
                   else if(ivmode == 10 )
                      vabs[i] = sqrt(x3[i]*x3[i]+x4[i]*x4[i])-coffset;
                   else if(ivmode == 11 )
                      vabs[i] = (xrad[i]*x7[i])-coffset;
                   else if(ivmode == 12 )
                      vabs[i] = sqrt(x1[i]*x1[i]+x2[i]*x2[i]);
                   else if(ivmode == 17 )
                      vabs[i] = xrad[i]*xrad[i]*x7[i]*x7[i]-coffset;
                   else if(ivmode == 20 )
                      vabs[i] = i;
                  }
                  else if(( inread == 7 ) || (inread == 4 ) || ( inread == 14 ))
                  {
                   xhcm = xhcm + x3[i];
                   if( ivmode == 0 )
                      vabs[i] = sqrt(x3[i]*x3[i]+x32[i]*x32[i]+x4[i]*x4[i])-coffset;
                   else if(ivmode == 1 )
                      vabs[i] = (x1[i])-coffset;
                   else if(ivmode == 2 )
                      vabs[i] = (x12[i])-coffset;
                   else if(ivmode == 3 )
                      vabs[i] = (x2[i])-coffset;
                   else if(ivmode == 21 )
                     {
                      vabs[i] = (x1[i])-coffset;
                      if( vabs[i] < 0 ) vabs[i]=-vabs[i];
                     }
                   else if(ivmode == 22 )
                     {
                      vabs[i] = (x12[i])-coffset;
                      if( vabs[i] < 0 ) vabs[i]=-vabs[i];
                     }
                   else if(ivmode == 23 )
                     {
                      vabs[i] = (x2[i])-coffset;
                      if( vabs[i] < 0 ) vabs[i]=-vabs[i];
                     }
                   else if(ivmode == 4 )
                      vabs[i] = x3[i]-coffset;
                   else if(ivmode == 5 )
                      vabs[i] = x32[i]-coffset;
                   else if(ivmode == 6 )
                      vabs[i] = x4[i]-coffset;
                   else if(ivmode == 7 )
                      vabs[i] = xrad[i]-coffset;
                   else if(ivmode == 110 )
                   {
                      vabs[i] = x71[i]*x71[i]+x7[i]*x7[i]+x72[i]*x72[i]-coffset;
                      /* printf(" %d %f %f %f \n", i, x71[i], x7[i], x72[i] ); */
                   }
                   else if(ivmode == 111 )
                      vabs[i] = x71[i]-coffset;
                   else if(ivmode == 112 )
                      vabs[i] = x7[i]-coffset;
                   else if(ivmode == 113 )
                      vabs[i] = x72[i]-coffset;
                   else if(ivmode == 120 )
                   {
                      vabs[i] = x61[i]*x61[i]+x6[i]*x6[i]+x62[i]*x62[i]-coffset;
                      /* printf(" %d %f %f %f \n", i, x71[i], x7[i], x72[i] ); */
                   }
                   else if(ivmode == 121 )
                      vabs[i] = x61[i]-coffset;
                   else if(ivmode == 122 )
                      vabs[i] = x6[i]-coffset;
                   else if(ivmode == 123 )
                      vabs[i] = x62[i]-coffset;
                   else if(ivmode == -8 )
                   {
                      vabs[i] = ix[i]+inputmass12*
                                sqrt(x3[i]*x3[i]+x4[i]*x4[i])-coffset;
                   }
                   else if(ivmode == 8 )
                      { 
                        if( incdt == 1 ) vabs[i] = ix[i]/time_step;
                        else             vabs[i] = ix[i];
                      }
                   else if(ivmode == 9 )
                      vabs[i] = x1[i]+x12[i]+x2[i]-coffset;
                   else if(ivmode == 10 )
                      vabs[i] = sqrt(x3[i]*x3[i]+x32[i]*x32[i]+x4[i]*x4[i])-coffset;
                   else if(ivmode == 12 )
                      vabs[i] = sqrt(x1[i]*x1[i]+x12[i]*x12[i]+x2[i]*x2[i]);
                   else if(ivmode == 13 )
                     {
                      xdummy = sqrt(x1[i]*x1[i]+x12[i]*x12[i]);
                      if(xdummy != 0 ) vabs[i] = ( x2[i]/xdummy );
                      else             vabs[i] = 0;
                      if( vabs[i] < 0 ) vabs[i] = -vabs[i];
                     }
                   else if(ivmode == 14 )
                     {
                      xdummy = x1[i];
                      if(xdummy != 0 ) vabs[i] = ( x12[i]/xdummy );
                      else             vabs[i] = 0;
                      if( vabs[i] < 0 ) vabs[i] = -vabs[i];
                     }
                   else if(ivmode == 15 )
                     {
                      xdummy = sqrt(x1[i]*x1[i]+x12[i]*x12[i]+x2[i]*x2[i]);
                      if(xdummy != 0)
                        vabs[i]=-(-x2[i]/xdummy*x3[i]+x1[i]/xdummy*x4[i]);
                      else
                        vabs[i]=0;
                     }
                   else if(ivmode == 16 )
                     {
                      xdummy = sqrt(x1[i]*x1[i]+x12[i]*x12[i]+x2[i]*x2[i]);
                      if(xdummy != 0)
                        vabs[i]=+(-x2[i]/xdummy*x3[i]+x1[i]/xdummy*x4[i]);
                      else
                        vabs[i]=0;
                     }
                   else if(ivmode == 17 )
                     {
                      xdummy = sqrt(x1[i]*x1[i]+x12[i]*x12[i]+x2[i]*x2[i]);
                      if(xdummy != 0)
                      {
                        vabs[i]=(-x2[i]/xdummy*x3[i]+x1[i]/xdummy*x4[i]);
                        if(vabs[i] < 0) vabs[i]=-vabs[i];
                      }
                      else
                        vabs[i]=0;
                     }
                   else if(ivmode == 18 )
                     {
                      vabs[i] = sqrt(x1[i]*x1[i]+x12[i]*x12[i]);
                     }
                   else if(ivmode == 20 )
                      vabs[i] = i;
                  }

                  ccode[i]=vabs[i];

                  if(( vpower >= 1.0001 ) || ( vpower <= 0.9999 ))
                      vabs[i] = pow( vabs[i], vpower );

                  if((i >= incmin ) && (i <= incmax)) 
                  {
                      if( vabs[i] > vabsmax ) vabsmax = vabs[i];
                      if( vabs[i] < vabsmin ) vabsmin = vabs[i];

                      vabsmean = vabsmean + vabs[i]/NK;
                      if( ix[i] > ixmax ) ixmax = ix[i];
                      ixmean = ixmean + ix[i]/NK;
                  }

                  if( inread == 8 )
                  {
                      wabs[i] = sqrt(xrad[i]*x7[i]*xrad[i]*x7[i]);
                  }
                  else
                  {
                      wabs[i] = 0.0;
                  }
                  vhcm = vhcm + vabs[i];
                  vzcm = vzcm + x4[i];
                  whcm = whcm + wabs[i];

            /* test if v == 0 */
               /* if( veloabs[i] > 0.0 ) */
                  {
                     if( ivskal == 0.0 )
                     {
                      if(vabs[i]==0.0)
                      {
                        v1[i]=x1[i];
                        v2[i]=x2[i];
                      }
                      else
                      {
                       v1[i]=x1[i]+xrad[i]*x3[i]/vabs[i];
                       v2[i]=x2[i]+xrad[i]*x4[i]/vabs[i];
                      }
                      va0[i]=vabs[i];
                     }
                     else
                     {
                      v1[i]=x1[i]+xrad[i]*x3[i]*ivskal;
                      v2[i]=x2[i]+xrad[i]*x4[i]*ivskal;
                      va0[i]=1./ivskal;
                     }
                  }
               /* else
                  {
                      v1[i]=x1[i];
                      v2[i]=x2[i];
                      va0[i]=1.e10;
                  } */
            /* maximum-collision-number-test */
                  if( ( ncoll_max < ix[i] ) && ( ix[i] > 0. ) )
                      ncoll_max=ix[i];
                }
             }
             if((ic0>0) && (iredraw==1)) fclose(fpp);  /* close second input file */

        /* do nothing if t < t_minimal */
             if( time >= tmin )
             {

         /* Set data after calculation of averages */
               if( ifast == 0 )
               {
                 v_offset = offset - center_of_mass * xhcm/NK * yskal;
               }
               /*printf("cc: %d %d %f \n",i,icolor,vabs[i]);*/

         /* Calculate connections - if necessary */
               if( ilines > 0 )
               {
                   for ( i=1; i<=NK; i++)
                     {
                       for ( j=i+1; j<=NK; j++)
                          {
                             xx1=x1[i]-x1[j];
                             xx2=x2[i]-x2[j];
                             if( ( inread == 7 ) || ( inread == 14 ) )
                                xx12=x12[i]-x12[j];
                             else
                                xx12=0.;
                             xd=sqrt(xx1*xx1+xx2*xx2+xx12*xx12);
                             if ( xd < (float) (ilines)*(xrad[i]+xrad[j])/1000. )
                             {
                               vabs000=xd/(xrad[i]+xrad[j])/ilines*1000.;

                               icolor = - (int) (50.*cskal*log10((double)(vabs000)));

                               if( icolor < 2 ) icolor = 2;
                               if( icolor > mmax101 ) icolor = mmax101-1;

                         XDrawLine(dpy, vandy, gcMap[icolor],
                                (int) ( xy0+x1[i]*xskal ) - h_offset, height -v_offset - (int) ( xy0+x2[i]*yskal ),
                                (int) ( xy0+x1[j]*xskal ) - h_offset, height -v_offset - (int) ( xy0+x2[j]*yskal ));
                              }
                          }
                      }
               }

         /* draw displacement vectors */
              if(( ivector > 0 ) && ( iistep == istep ))
              {
                if( idraw <= 0 ) idraw = 1;
                for( i=1; i<=NK+NK1; i+=idraw )
                {
                     vabs000=x20[i]-x2[i];
                     if ( inc > 0 ) vabs000 = 0.1* (ix[i]);
                     if( vabs000 > 0.0 )
                     {
                        icolor = 10*mmax101/101-(int) (60.*mmax101/101+2.*cskal*log10((double)(vabs000)));
                        if( icolor < 2 ) icolor = 2;
                        if( icolor > (int)(mmax101*0.15) ) icolor = (int)(mmax101*0.35);

                     }
                     else if( vabs000 < 0.0 )
                     {
                        icolor = 90*mmax101/101+(int) (60.*mmax101/101+2.*cskal*log10((double)(-vabs000)));
                        if( icolor < (int)(mmax101*0.85) ) icolor = (int)(mmax101*0.65);
                        if( icolor > mmax101-1 ) icolor = mmax101-1;
                     }
                     else
                     {
                        icolor = icolmin;
                     }

                  XFillArc(dpy, vandy, gcBlue,
                    (int) ( xy0+x1[i]*xskal ) - h_offset, 
                    height -v_offset - (int) ( xy0+x2[i]*yskal ),
                    (int) ( .2*xrad[i] * xskal ), (int) ( .2*xrad[i] * yskal ),
                    0 * 64, 360 * 64);
                  dx=(x1[i]-x10[i])*(ivskal/100.);
                  dy=(x2[i]-x20[i])*(ivskal/100.);
                  if(( (fabs)(x1[i]-x10[i]) < (xright-xleft)/2. ) && 
                     ( (fabs)(x2[i]-x20[i]) < (xtop-xbottom)/2. ))
                     XDrawLine(dpy, vandy, gcMap[icolor],
                      (int) ( xy0+x1[i]*xskal ) - h_offset, height -v_offset - (int) ( xy0+x2[i]*yskal ),
                      (int) ( xy0+(x1[i]-dx)*xskal ) - h_offset, height -v_offset - (int) ( xy0+(x2[i]-dy)*yskal ));
                }
               }

               ichnext=0;
               ichcolor=1;

         /* Plot data i=1,N loop */
               if( idraw <= 1 )
                 idraw = 1;
               else
                 printf(" draw just every %d th particle ... \n", idraw);
               
         /* sort particles according to depth */
               /* if(( inread == 7 ) || ( inread == 13 ) || ( inread == 8 )) */
               {
                if((rotr!=1.0)||(rottheta!=0.0)||(rotphi!=0.0)||(unidist!=0.0)||
                   (drotr!=0.0)||(drottheta!=0.0)||(drotphi!=0.0)||(dunidist!=0.0))
                {
              /* printf("Start rotating ... \n"); */
                 R11=rotr*cos(rotphi)*cos(rottheta);
                 R22=rotr*cos(rottheta);
                 R33=rotr*cos(rotphi);

                 R12=rotr*cos(rotphi)*sin(rottheta);
                 R21=-rotr*sin(rottheta);

                 R13=-rotr*sin(rotphi);
                 R31=rotr*sin(rotphi)*cos(rottheta);

                 R23=0.0;
                 R32=rotr*sin(rotphi)*sin(rottheta);
                 /* printf(" %f %f %f \n", R11, R12, R13 );
                    printf(" %f %f %f \n", R21, R22, R23 );
                    printf(" %f %f %f \n", R31, R32, R33 ); */
                 for ( i=1; i<=NK+NK1+NK2; i++)
                 {
                   /* printf(" -- %d ",i); */
                   if((iredraw>1)&&(unig[i]>0))
                   {
                     x1[i]=x1[i]/unig[i];
                     x2[i]=x2[i]/unig[i];
                   }

                   if( unidist != 0.0 )
                   {
                     distxz=eyepos-x12[i];
                     dist=sqrt(x1[i]*x1[i]+x2[i]*x2[i]+distxz*distxz);
                     if(dist>1.e-12*unidist) 
                        unif[i]=unidist/xrad[i]*tan(asin(xrad[i]/dist));
                     else
                        unif[i]=1;
                     unig[i]=1;
                     if(distxz<=0)
                        unif[i]=1;
                     else
                        unig[i]=(unidist/distxz);
                  /* printf(" %d %f %f %f %f %f %f %f \n",i,unidist,dist,unif[i],unidist-x12[i],x1[i],x2[i],x12[i]); */
                   }

                   x1[i]=x1[i]+trx;
                   x2[i]=x2[i]+trz;
                   x12[i]=x12[i]+try;
                   x1dumy=R11*x1[i]+R12*x2[i]+R13*x12[i];
                   x2dumy=R21*x1[i]+R22*x2[i]+R23*x12[i];
                   x12[i]=R31*x1[i]+R32*x2[i]+R33*x12[i];
                   if(x12[i]<x12min) x12min=x12[i];
                   if(x12[i]>x12max) x12max=x12[i];
                   x1[i]=(x1dumy);
                   x2[i]=(x2dumy);
                   if((nredraw>=1)&&(unig[i]>0))
                   {
                     x1[i]=x1[i]*unig[i];
                     x2[i]=x2[i]*unig[i];
                   }
                   xrad[i]=xrad[i]*rotr;
                /* printf("%f %f %f %f %f %f %f %f \n",
                   x1[i], x12[i], x2[i], x3[i], x32[i], x4[i], xrad[i], ix[i]); 
                 */

                 /* if(( idraw > 1 )&&( i<= NK+NK1-idraw+1 )) i+=idraw-1; */
                 }

                 if( nredraw > 1 )
                 {
                  rotr=rotr*(1.0+drotr);
                  rottheta=drottheta;
                  rottheta0=rottheta0+drottheta;
                  rotphi=drotphi;
                  rotphi0=rotphi0+drotphi;
                  unidist=unidist+dunidist;
                  trx=0.0;
                  try=0.0;
                  trz=0.0;
                  eyepos=eyepos+deyepos;
                 }
                 else
                 {
                  rotr=rotr+drotr;
                  rottheta=rottheta+drottheta;
                  rottheta0=rottheta;
                  rotphi=rotphi+drotphi;
                  rotphi0=rotphi;
                  unidist=unidist+dunidist;
                  eyepos=eyepos+deyepos;
                 }
                 printf("3d-info: t=%g | *=%g p=%g h=%g | u=%g e=%g \n",time,rotr,rotphi0,rottheta0,unidist,eyepos);

                }

                /* x12min=1.e10;
                   x12max=-1.e10; */

                for(iii=1; iii<=NK+NK1; iii++)
                {
                   ipp[iii-1]=iii;
                   xg12[iii]=eyepos-x12[iii];
                   /* if(xg12[iii]>x12max) x12max=xg12[iii];
                      if(xg12[iii]<x12min) x12min=xg12[iii]; */
                }
                /* printf(" %d %g %g \n",iredraw,x12min,x12max); */

                if(isort == 1)    /* sort particles with respect to 
                                     their distance from viewer */
                {
                   qsort( ipp, NK+NK1, sizeof(int), ieyedist);
                }
               }

         /* begin drawing loop */
               for ( iii=0; iii<NK+NK1; iii+=idraw)
               {
                i=ipp[iii];    /* particle range from 1 to NK+NK1 */
                if( i>=idrmin && i<=idrmax )
                {
                   {
                        if ( icmax > 1 ) vabsmax = cdirmax;
                        if ( icmax > 1 ) vabsmin = cdirmin;
                        if ( inc > 0 )
                        {
                            vabs000 = 0.1* (ix[i]);
                            if ( icmax > 0 ) vabsmax = 0.1 * (ixmax);
                            if ( icmean > 0 ) vabsmean = 0.1 * (ixmean);
                        }
                        else
                        {
                            vabs000 = vabs[i]*(1.0+1.0e-10) - vabsmin;
                        }

                        if( vabs000 > 0.0 )
                        {
                          if( icmean > 0 )
                          {
                            icolor = (int) (mmax101*(1.+cskal/10.*log10((double)(vabs000/vabsmean/4.))));
                            if( icolor < 2 ) icolor = 2;
                            if( icolor > mmax101 ) icolor = mmax101;
                          }
                          else
                          {
                            if ( icmax > 0 )
                            {
                              if ( icmax > 1 ) vabsmax = cdirmax;
                              if ( icmax > 1 ) vabsmin = cdirmin;
                              icolor = (int) (mmax101*(1.+cskal/10.*log10((double)(vabs000/(vabsmax-vabsmin)))));
                              if( icolor < 2 ) icolor = 2;
                              if( icolor > mmax101 ) icolor = mmax101;
                            }
                            else
                            {
                              icolor = (int) (60.*mmax101/101+2.*cskal*log10((double)(vabs000)));
                              if( icolor < 2 ) icolor = 2;
                              if( icolor > mmax101 ) icolor = mmax101;
                            }
                          }
                        }
                        else
                        {
                           icolor = icolmin;
                        }


                 /* change color after ich particles */
                        if( ich > 0 )
                        {
                           if( i >= ichnext )
                           {
                               ichnext += ich;
                               ichcolor += 1;
                           }

                           if( ichcolor > mmax101 ) ichcolor = 2;
                           icolor = ichcolor;
                        }

                 /* particle color == size */
                        if( incr > 0 )
                        {
                            if( radmax-radmin != 0. )
                               icolor = 1 + mmax101 * (int) (xrad[i]/(radmax-radmin));
                            else
                               icolor = 2;
                            if( icolor > mmax101 ) icolor = mmax101;
                            if( icolor < 2 ) icolor = 2;
                     }

            /* w-colorcode with log_10 v  */
                     if( wabs[i] > 1.e-10 )
                     {
                        jcolor = (int) (60.*mmax101/101+2.*cskal*log10((double)(wabs[i])));
                        if( jcolor < 2 ) jcolor = 2;
                        if( jcolor > mmax101 ) jcolor = mmax101+1;
                     }
                     else
                        jcolor = 1;

            /* ferro colorcode */
                    if ( iferro == 1 )
                    {
                       if( x7[i] > 1.e-6 )
                       {
                         jcolor = 0.5*cskal*log10((double)(x7[i]));
                         icolor = 7-jcolor;
                         if( icolor > (int) mmax101/4. ) 
                           icolor = (int) mmax101/4.;
                       }
                       else
                       {
                         if( x7[i] < -1.e-6 )
                         {
                           jcolor = 0.5*cskal*log10((double)(-x7[i]));
                           icolor = mmax101-5+jcolor;
                           if( icolor < (int) mmax101*3./4. ) 
                             icolor = (int) mmax101*3./4.;
                         }
                         else
                         {
                           icolor = icolmin;
                         }
                       }
                       if( icolor < 2 ) icolor = 2;
                       if( icolor > mmax101 ) icolor = mmax101;
                    }

                    /* printf("ic2: %d %d %f %f %f\n",i,icolor,x12[i],xright,x12[i]/xright);*/
                    if( idist == 1 )
                    {
                     if(( inread == 7 ) || ( inread == 3 ) || ( inread == 14 ))
                     {
                        icolor = mmax101*x12[i]/xright;
                        /*printf("ic2: %d %d %f %f %f\n",i,icolor,x12[i],xright,x12[i]/xright);*/
                        if( icolor < 2 ) icolor = 2;
                        if( icolor > mmax101 ) icolor = mmax101;
                     }
                    }

                    if (isq < 0)
                    {
            /* Draw particles as circles */
                     if (( ilines == 0 ) && ( ivector == 0 ))
                     {
            /* Fill Circle in slow-mode*/
                       if(unif[i] == -1)
                       {
                        ixrad=0;
                        iyrad=0;
                        unif[i]=0;
                       }
                       else
                       {
                        ixrad = (int)(2.*xrad[i] * xskal *unif[i]);
                        iyrad = (int)(2.*xrad[i] * yskal *unif[i]);
                        if( ixrad < 2 ) 
                        {
                          if( ipixel >= 2 ) ixrad=1;
                          else              ixrad=2;
                        }
                        if( iyrad < 2 )
                        {
                          if(( ipixel == 1 ) || ( ipixel >= 3 )) 
                                            iyrad=1;
                          else              iyrad=2;
                        }
                       }
            /* Draw an ellipse instead of a circle */
                       if(( xellipse > 0 ) && ( i>NK ))
                       {
                          srp=sin(rotphi0);
                          crp=cos(rottheta0);
                          if( srp < 0 ) srp=-srp;
                          if( crp < 0 ) crp=-crp;
                          ixrad = (int)(2.*xrad[i] * xskal *unif[i] *srp);
                          iyrad = (int)(2.*xrad[i] * yskal *unif[i] *crp);
                          if( ixrad < 2 ) ixrad=2;
                          if( iyrad < 2 ) iyrad=2;
                          /* printf(" ellipse: %d %g %g %d %d %g \n",i,rotphi0,rottheta0,ixrad,iyrad,xrad[i]); */
                       }
                       else
                       {
                          srp=1.0;
                          crp=1.0;
                       }

                       /*printf("1: %f %d \n",ccode[i],icolor); */
                       cccmean[icolor] += ccode[i];
                       iicmean[icolor] += 1;

                      /*printf(" %g %g %g \n", drwmin, vabs[iii], drwmax );*/
                      if( ( drwmax > vabs[i] ) && ( drwmin < vabs[i] ) )
                      {
                       if( i > NK ) ifast1=ifast0;
                       else         ifast1=ifast;

                       if( ifast1 == 0 )
                       {
                        /*printf("ic: %d %d %d \n",iii,icolor,gcMap[icolor]);*/
                        if( ifog==0 )
                           gcColor=gcMap[icolor];
                        else
                        {  /* set greyscales on the color as function of distance from the viewer */
                           ix12=(int)(25.0+5.*ifog*pow((x12max-x12[i])/(x12max-x12min),fpower));
                           if( ix12 < 0 )  ix12=0;
                           if( ix12 > 99 ) ix12=99;
                           gcColor=gcMapGrey[icolor][ix12];
                           /* printf("%d %d %f %f %f \n",i,ix12,x12[i],x12min,x12max); */
                        }

                        XFillArc(dpy, vandy, gcColor,
                            (int) ( xy0+(x1[i]-xrad[i]*unif[i]*srp)*xskal ) - h_offset, 
                            height -v_offset - (int) ( xy0+(x2[i]+xrad[i]*unif[i]*crp)*yskal ),
                            ixrad, iyrad, 0 * 64, 360 * 64);
                        if( isolid == 1 )
                         XDrawArc(dpy, vandy, gcMap[icolor],
                          (int) ( xy0+(x1[i]-xrad[i]*unif[i]*srp)*xskal ) - h_offset, 
                          height -v_offset - (int) ( xy0+(x2[i]+xrad[i]*unif[i]*crp)*yskal ),
                          ixrad, iyrad, 0 * 64, 360 * 64);
                        else if( isolid == 2 )
                         XDrawArc(dpy, vandy, gcBlack,
                          (int) ( xy0+(x1[i]-xrad[i]*unif[i]*srp)*xskal ) - h_offset, 
                          height -v_offset - (int) ( xy0+(x2[i]+xrad[i]*unif[i]*crp)*yskal ),
                          ixrad, iyrad, 0 * 64, 360 * 64);
                        else if( isolid == 3 )
                         XDrawArc(dpy, vandy, gcWhite,
                          (int) ( xy0+(x1[i]-xrad[i]*unif[i]*srp)*xskal ) - h_offset, 
                          height -v_offset - (int) ( xy0+(x2[i]+xrad[i]*unif[i]*crp)*yskal ),
                          ixrad, iyrad, 0 * 64, 360 * 64);
                       }
                       else
                       {
            /* Draw Circle */
                       if( ivv == 0 )
                        XDrawArc(dpy, vandy, gcMap[icolor],
                         (int) ( xy0+(x1[i]-xrad[i]*srp)*xskal ) - h_offset, 
                         height -v_offset - (int) ( xy0+(x2[i]+xrad[i]*crp)*yskal ),
                         ixrad, iyrad, 0 * 64, 360 * 64);
                        if( isolid == 1 )
                         XFillArc(dpy, vandy, gcMap[icolor],
                          (int) ( xy0+(x1[i]-xrad[i]*srp)*xskal ) - h_offset, 
                          height -v_offset - (int) ( xy0+(x2[i]+xrad[i]*crp)*yskal ),
                          ixrad, iyrad, 0 * 64, 360 * 64);
                        else if( isolid == 2 )
                         XFillArc(dpy, vandy, gcBlack,
                          (int) ( xy0+(x1[i]-xrad[i]*srp)*xskal ) - h_offset, 
                          height -v_offset - (int) ( xy0+(x2[i]+xrad[i]*crp)*yskal ),
                          ixrad, iyrad, 0 * 64, 360 * 64);
                        else if( isolid == 3 )
                         XFillArc(dpy, vandy, gcWhite,
                          (int) ( xy0+(x1[i]-xrad[i]*srp)*xskal ) - h_offset, 
                          height -v_offset - (int) ( xy0+(x2[i]+xrad[i]*crp)*yskal ),
                          ixrad, iyrad, 0 * 64, 360 * 64);
                       }
                      }
                     }
                    }
                    else    /* draw squares */
                    {
                      isqcolor=1;
                      if(isq==1) 
                      {
                        isqcolor=(int) (dskal*(60.*mmax101/101+cskal*log10(x6[i])));
                        if( isqcolor < 2 ) isqcolor = 2;
                        if( isqcolor > mmax101 ) isqcolor = mmax101+1;
                      }
                      else if(isq==2)
                      {
                        if( x7[i] < 0. ) x7[i]=-x7[i];
                        if( x7[i] == 0. ) x7[i]=1.e-20;
                        isqcolor=(int) (dskal*(60.*mmax101/101+cskal*log10(x7[i])));
                        if( isqcolor < 2 ) isqcolor = 2;
                        if( isqcolor > mmax101 ) isqcolor = mmax101+1;
                      }
                      else if(isq==3)
                      {
                        if( ix[i] < 0. ) ix[i]=-ix[i];
                        if( ix[i] == 0. ) ix[i]=1.e-20;
                        isqcolor=(int) (dskal*(60.*mmax101/101+cskal*log10(ix[i])));
                        if( isqcolor < 2 ) isqcolor = 2;
                        if( isqcolor > mmax101 ) isqcolor = mmax101+1;
                      }
                      else if(isq==36)
                      {
                        tempx=x6[i]-x3[i]*x3[i];
                        if( tempx < 0. ) tempx=-tempx;
                        if( tempx == 0. ) tempx=1.e-20;
                        isqcolor=(int) (dskal*(60.*mmax101/101+cskal*log10(tempx)));
                        if( isqcolor < 2 ) isqcolor = 2;
                        if( isqcolor > mmax101 ) isqcolor = mmax101+1;
                      }
                      else if(isq==47)
                      {
                        tempx=x7[i]-x4[i]*x4[i];
                        if( tempx < 0. ) tempx=-tempx;
                        if( tempx == 0. ) tempx=1.e-20;
                        isqcolor=(int) (dskal*(60.*mmax101/101+cskal*log10(tempx)));
                        if( isqcolor < 2 ) isqcolor = 2;
                        if( isqcolor > mmax101 ) isqcolor = mmax101+1;
                      }
                      else if(isq==3647)
                      {
                        tempx=x6[i]-x3[i]*x3[i]+x7[i]-x4[i]*x4[i];
                        if( tempx < 0. ) tempx=-tempx;
                        if( tempx == 0. ) tempx=1.e-20;
                        isqcolor=(int) (dskal*(60.*mmax101/101+cskal*log10(tempx)));
                        if( isqcolor < 2 ) isqcolor = 2;
                        if( isqcolor > mmax101 ) isqcolor = mmax101+1;
                      }
                      else
                      {
                        if( vabs[i] == 0. ) vabs[i]=1.e-20;
                        isqcolor=(int) (dskal*(60.*mmax101/101+cskal*log10(vabs[i])));
                        if( isqcolor < 2 ) isqcolor = 2;
                        if( isqcolor > mmax101 ) isqcolor = mmax101+1;
                      }

                      XFillRectangle(dpy, vandy, gcMap[isqcolor],
                         (int) ( xy0+(x1[i])*xskal ) - h_offset, 
                         height -v_offset - (int) ( xy0+(x2[i]+2.*xrad[i])*yskal ),
                         (int) ( 2*xrad[i] * xskal ), (int) ( 2*xrad[i] * yskal ) );
                    }

            /* Black Circle for collision-history Particle */
                     if ( ix[i] > ( ncoll_max - (float)coll_hist ) )
                     {
                       ixrad=xrad[i] * xskal;
                       iyrad=xrad[i] * yskal;
                       if( ixrad < 2 ) ixrad=2;
                       if( iyrad < 2 ) iyrad=2;

                       XDrawArc(dpy, vandy, gcBlack,
                        (int) ( xy0+(x1[i]-xrad[i]/2)*xskal ) - h_offset, 
                        height -v_offset - (int) ( xy0+(x2[i]+xrad[i]/2)*yskal ),
                        ixrad, iyrad, 0 * 64, 360 * 64);
                     }

            /* Black Dot for traced Particle */
                     if ( i == xtrace )
                     {
                       xnewrad = xrad[i]*xskal;
                       if( xnewrad < 5. ) 
                          xnewrad = 5.;
                       XFillArc(dpy, vandy, gcBlack,
                        (int) ( xy0+(x1[i]-xrad[i]/2)*xskal ) - h_offset, 
                        height -v_offset - (int) ( xy0+(x2[i]+xrad[i]/2)*yskal ),
                        (int) ( xnewrad ), (int) ( xnewrad ),
                        0 * 64, 360 * 64);
                     }
            /* Large Black Dot for mtraced Particle */
                     if ( i == xmtrace )
                     {
                       xnewrad = xrad[i]*xskal;
                       if( xnewrad < 5. ) 
                          xnewrad = 15.;
                       XFillArc(dpy, vandy, gcBlack,
                        (int) ( xy0+(x1[i]-xrad[i]/2)*xskal ) - h_offset, 
                        height -v_offset - (int) ( xy0+(x2[i]+xrad[i]/2)*yskal ),
                        (int) ( xnewrad ), (int) ( xnewrad ),
                        0 * 64, 360 * 64);
                     }
                  }
                }
               }

               if( iscale > 0 )
               {
                 xxxmean = 0.0;
                 iiimean = 0;
                 for( i=1; i<=mmax101; i++)
                 {
                   if(iicmean[i]>0)
                      xxxmean += cccmean[i]/iicmean[i];
                   iiimean += iicmean[i];
                 }
                 if( iiimean > 0 ) xxxmean = xxxmean / iiimean;
                 sprintf(rpm,"x %f",xxxmean);
                 if(( mfont > 0 ) && ( iscale == 3 ))
                    XDrawString( dpy, valab, gcSFont, 
                        54, height-2-(int) ((0.1*height)/(mmax101+1)), 
                        rpm, strlen(rpm));
                 if(( mfont > 0 ) && ( iscale == 1 ))
                    XDrawString( dpy, valab, gcSFont, 
                        20, height-2-(int) ((0.1*height)/(mmax101+1)), 
                        rpm, strlen(rpm));

                 ixxxstep=1;
                 if(1.0*height/(mmax101+1) < 12) ixxxstep=2;
                 if(1.0*height/(mmax101+1) < 6) ixxxstep=4;
                 if(1.0*height/(mmax101+1) < 3) ixxxstep=8;

                 for( i=1; i<=mmax101+1; i+=ixxxstep)
                 {
                   if(( iicmean[i]>0 ) && ( xxxmean != 0.0 ))
                   {
                     /* printf("%d %d %f %f \n",i,iicmean[i],cccmean[i]/iicmean[i],xxxmean); */
                     if(iscale == 1)       sprintf(rpm,"%-15.9g",cccmean[i]/iicmean[i]/xxxmean);
                     else if(iscale == 2 ) sprintf(rpm,"%-15.9g",cccmean[i]/iicmean[i]);
                     else if(iscale == 3 ) sprintf(rpm,"%-5.5d  %-12.6g",iicmean[i],cccmean[i]/iicmean[i]/xxxmean);
                     else if(iscale == 4 ) sprintf(rpm,"%-5.5d  %-12.6g",iicmean[i],cccmean[i]/iicmean[i]);
                     else                  sprintf(rpm,"%-16.9g",cccmean[i]/iicmean[i]);
                     if ( mfont > 0 )
                     {
                        XDrawString( dpy, valab, gcSFont, 
                            11, height-2-(int) (((float)(i)-0.9)*height/(mmax101+1)),
                            rpm, strlen(rpm));
                     }

                   }
                 }
               }

            /* plot the system boundaries with lines */
               if( icube != 0 )
               {
                 for( i=1; i<=12; i++ )
                 {
                    switch (i)
                    {
                      case 1:
                        iwc1=NK+NK1+NK2-6;
                        iwc2=NK+NK1+NK2-7;
                        break;
                      case 2:
                        iwc1=NK+NK1+NK2-5;
                        iwc2=NK+NK1+NK2-7;
                        break;
                      case 3:
                        iwc1=NK+NK1+NK2-4;
                        iwc2=NK+NK1+NK2-7;
                        break;
                      case 4:
                        iwc1=NK+NK1+NK2-6;
                        iwc2=NK+NK1+NK2-3;
                        break;
                      case 5:
                        iwc1=NK+NK1+NK2-4;
                        iwc2=NK+NK1+NK2-3;
                        break;
                      case 6:
                        iwc1=NK+NK1+NK2-0;
                        iwc2=NK+NK1+NK2-3;
                        break;
                      case 7:
                        iwc1=NK+NK1+NK2-5;
                        iwc2=NK+NK1+NK2-2;
                        break;
                      case 8:
                        iwc1=NK+NK1+NK2-4;
                        iwc2=NK+NK1+NK2-2;
                        break;
                      case 9:
                        iwc1=NK+NK1+NK2-0;
                        iwc2=NK+NK1+NK2-2;
                        break;
                      case 10:
                        iwc1=NK+NK1+NK2-6;
                        iwc2=NK+NK1+NK2-1;
                        break;
                      case 11:
                        iwc1=NK+NK1+NK2-5;
                        iwc2=NK+NK1+NK2-1;
                        break;
                      case 12:
                        iwc1=NK+NK1+NK2-0;
                        iwc2=NK+NK1+NK2-1;
                        break;
                    }
                    XDrawLine(dpy, vandy, gcBlack,
                      (int) ( xy0+x1[iwc1]*xskal ) - h_offset, height -v_offset - (int) ( xy0+x2[iwc1]*yskal ),
                      (int) ( xy0+x1[iwc2]*xskal ) - h_offset, height -v_offset - (int) ( xy0+x2[iwc2]*yskal ));
                    /* printf("walls: %d %d %f %f %f %f \n",iwc1,iwc2,x1[iwc1],x2[iwc1],x1[iwc2],x2[iwc2]); */
                 }
               }


            /* Draw direction of velocity */
               if( idraw <= 0 ) idraw = 1;
               for( i=1; i<=NK; i+=idraw )
               {
                    if (( ilines == 0 ) && ( ivector == 0 ) && ( ivelo > 0 ))
                    {
                     if( isq < 0 )
                     {
                       XDrawLine(dpy, vandy, gcBlack,
                         (int) ( xy0+x1[i]*xskal ) - h_offset, height -v_offset - (int) ( xy0+x2[i]*yskal ),
                         (int) ( xy0+v1[i]*xskal ) - h_offset, height -v_offset - (int) ( xy0+v2[i]*yskal ));
                     }
                     else
                     {
                       vvskal=vqskal*.05;
                       printf(" .2. %d %d %f %f %f %f \n", i, inread, x1[i], x2[i], x3[i], x4[i] );
                       /* use different vectors in different dimensions - if(true) -> 2D */
                       if( inread  == 8 || inread  == 5 || inread  == 6 )
                          XDrawLine(dpy, vandy, gcBlack,
                            (int) ( xy0+(x1[i]+xrad[i])*xskal ) - h_offset, 
         height -v_offset - (int) ( xy0+(x2[i]+xrad[i])*yskal ),
                            (int) ( xy0+(x1[i]+xrad[i]+x3[i]*vvskal)*xskal ) - h_offset, 
         height -v_offset - (int) ( xy0+(x2[i]+xrad[i]+x4[i]*vvskal)*yskal ));
                       else   /* else -> 3D */
                          XDrawLine(dpy, vandy, gcBlack,
                            (int) ( xy0+(x1[i]+xrad[i])*xskal ) - h_offset, 
         height -v_offset - (int) ( xy0+(x2[i]+xrad[i])*yskal ),
                            (int) ( xy0+(x1[i]+xrad[i]+x3[i]*vvskal)*xskal ) - h_offset, 
         height -v_offset - (int) ( xy0+(x2[i]+xrad[i]+x4[i]*vvskal)*yskal ));

                       xnewrad = 4.;
                     }
                    }
                    if(( ilines == 0 ) && ( ivector == 0 ))
                    {
                    /*   XDrawArc(dpy, vandy, gcBlack,
                        (int) ( xy0+(x1[i]+xrad[i])*xskal-2. ) - h_offset, 
                        height -v_offset - (int) ( xy0+(x2[i]+xrad[i])*yskal +2.),
                        (int) ( xnewrad ), (int) ( xnewrad ),
                        0 * 64, 360 * 64);   */

            /* Draw Rotation Arc */
                     if( iwelo == 0 ) wabs[i]=0.0;
                     if( ( inread == 8 ) && ( wabs[i] > 1.e-10 ) && ( isq < 0 ) && (iwelo > 0))
                     {

                      {
                       if(( ifast == 0 ) && ( iferro == 0 ))
                       {
            /* w-colorcode with log_10 v  */
                         if( wabs[i] > 1.e-10 )
                         {
                           jcolor = (int) (60.*mmax101/101+2.*cskal*log10((double)(wabs[i])));
                           if( jcolor < 2 ) jcolor = 2;
                           if( jcolor > mmax101 ) jcolor = mmax101+1;
                         }
                         else
                           jcolor = 1;

                        /* printf("ij: %d %d %d %d \n",iii,i,jcolor,gcMap[jcolor]); */
                        XFillArc(dpy, vandy, gcMap[jcolor],
                         (int) ( xy0+(x1[i]-0.7*xrad[i])*xskal ) - h_offset, 
                         height -v_offset - (int) ( xy0+(x2[i]+0.7*xrad[i])*yskal ),
                         (int) ( 1.4* xrad[i] * xskal ), (int) ( 1.4* xrad[i] * yskal ),
                         (int) (-x6[i]/2/PI*360) * 64, 90 * 64);

                        XDrawArc(dpy, vandy, gcLGrey,
                         (int) ( xy0+(x1[i]-0.7*xrad[i])*xskal ) - h_offset, 
                         height -v_offset - (int) ( xy0+(x2[i]+0.7*xrad[i])*yskal ),
                         (int) ( 1.4* xrad[i] * xskal ), (int) ( 1.4* xrad[i] * yskal ),
                         (int) ( -x6[i]/2/PI*360) * 64, 90 * 64);
                       }
                      }
                     }
                    }

            /* perform density-calculation */
                     if( ifast == 0 )
                     {
                        ixcenter = (int) ( (float)(height-v_offset-xy0-x2[i]*yskal) 
                                                   * NFIELD / height );
                        xradius = ( xrad[i]*yskal * NFIELD / height );
                        ixradius = (int) xradius;
                        if( ixradius < 1 ) ixradius = 1;
                        fill_dens( ixcenter, ixradius, dens );
                        if( ( x1[i] > xleft + xthick*3.33 ) & ( x1[i] < xleft + xthick*6.66 ) )
                        fill_dens( ixcenter, ixradius, densc );
                        fill_velo( ixcenter, ixradius, velo, vabs[i] );
                        if( wabs[i] > 1.e-10 )
                           fill_wrad( ixcenter, ixradius, wrad, wabs[i] );

             /* horizontal density calculation */
                        ixcenter = (int) ( (float)(xy0+x1[i]*xskal) 
                                                   * NFIELD / width ) - h_offset;
                        xradius = ( xrad[i]*xskal * NFIELD / width );
                        ixradius = (int) xradius;
                        if( ixradius < 1 ) ixradius = 1;
                        fill_dens( ixcenter, ixradius, hdens );
                        fill_velo( ixcenter, ixradius, hvelo, vabs[i] );
                        if( wabs[i] > 1.e-10 )
                           fill_wrad( ixcenter, ixradius, hwrad, wabs[i] );
                     }
               }

               {
        /* plot h_cm */
                 logxhcm = (float) ( (int) (xhcm/NK*100000) / 100000. );
                 if( iphcm == 1 )
                   XFillRectangle(dpy, vandy, gcRed, 
                    (int) (xy0+(xleft-xthick)*xskal) - h_offset, 
                    height -v_offset - (int) (xy0+(xhcm/NK)*yskal), 
                    (int) (xthick*xskal), 1); 
                 sprintf(rpm,"h = %f m",logxhcm);
                 if ( mfont > 0 ) 
                   XDrawString( dpy, vlab, gcSFont, 10, 60, rpm, strlen(rpm));

        /* plot v_cm */
                 if( vhcm > 0.0 )
                    icolor = (int) (60.*mmax101/101+2.*cskal*log10((double)(vhcm/NK)));
                 else
                    icolor = icolmin;

                 logvcm = pow( (double) 10., (double) ((float)(icolor-60*mmax101/101)/2./cskal) );
                 logvcm = (float) ( (int) (logvcm*100000) / 100000. );
                 XFillRectangle(dpy, valab, gcBlue, 
                  5, height-2-(int) ((float) (icolor*height)/(mmax101+1)),
                  3, 1 + (int) ((float) height / (mmax101+1)));
                 sprintf(rpm,"v = %f m/s",vhcm/NK);
                 if ( mfont > 0 ) 
                   XDrawString( dpy, vlab, gcSFont, 10, 75, rpm, strlen(rpm));

        /* plot w_cm */
                 if( whcm > 1.e-10 )
                    jcolor = (int) (60.*mmax101/101+2.*cskal*log10((double)(whcm/NK)));
                 else
                    jcolor = 1;
                 logwhcm = pow( (double) 10., (double) ((float)(jcolor-60*mmax101/101)/2./cskal) );
                 logwhcm = (float) ( (int) (logwhcm*100000) / 100000. );
                 XFillRectangle(dpy, valab, gcGreen, 
                  7, height-2-(int) ((float) (jcolor*height)/(mmax101+1)),
                  3, 1 + (int) ((float) height / (mmax101+1)));
                 sprintf(rpm,"wr = %f m/s",whcm/NK);
                 if ( mfont > 0 )
                    XDrawString( dpy, vlab, gcSFont, 10, 90, rpm, strlen(rpm));

        /* plot density */
                 for ( i=1; i<=NFIELD; i++)
                 {
                    if( dens[i]/sum_add > densmax ) densmax = dens[i]/sum_add;
                    if( hdens[i]/sum_add > hdensmax ) hdensmax = hdens[i]/sum_add;
                    if(dens[i] > 0. )
                       if( velo[i]/dens[i]/vsum_add > velomax ) 
                          velomax = velo[i]/dens[i]/vsum_add;
                    if(hdens[i] > 0. )
                       if( hvelo[i]/hdens[i]/vsum_add > hvelomax ) 
                          hvelomax = hvelo[i]/hdens[i]/vsum_add;
                    if(dens[i] > 0. )
                       if( wrad[i]/dens[i]/wsum_add > wradmax ) 
                          wradmax = wrad[i]/dens[i]/wsum_add;
                    if(hdens[i] > 0. )
                       if( hwrad[i]/hdens[i]/wsum_add > hwradmax ) 
                          hwradmax = hwrad[i]/hdens[i]/wsum_add;
                 }
                 sprintf(rpm,"density");
                 if ( mfont > 0 )
                 XDrawString( dpy, valab, gcSFont, 20+ialab, 20, rpm, strlen(rpm));
                 XDrawLine(dpy, valab, gcRed, 20+ialab, 22, 90+ialab, 22);
                 XDrawLine(dpy, valab, gcGrey, 20+ialab, 24, 90+ialab, 24);
                 sprintf(rpm,"lin.-energy");
                 if ( mfont > 0 )
                 XDrawString( dpy, valab, gcSFont, 20+ialab, 34, rpm, strlen(rpm));
                 XDrawLine(dpy, valab, gcBlue, 20+ialab, 36, 90+ialab, 36);
                 sprintf(rpm,"rot.-energy");
                 if ( mfont > 0 )
                 XDrawString( dpy, valab, gcSFont, 20+ialab, 46, rpm, strlen(rpm));
                 XDrawLine(dpy, valab, gcGreen, 20+ialab, 48, 90+ialab, 48);

                 XDrawLine(dpy, valab, gcBlack, 10+ialab, 0, 10+ialab, height);
                 XDrawLine(dpy, valab, gcBlack, 10+ialab, height, 100+ialab, height);

                 sprintf(rpm,"%f",densmax);
                 if ( mfont > 0 )
                 XDrawString( dpy, valab, gcSFont, 90-6*strlen(rpm)+ialab, height-10, rpm, strlen(rpm));
                 XDrawLine(dpy, valab, gcRed, 90+ialab, height-8, 90+ialab, height);

                 sprintf(rpm,"%f",velomax);
                 if ( mfont > 0 )
                 XDrawString( dpy, valab, gcSFont, 93-6*strlen(rpm)+ialab, height-20, rpm, strlen(rpm));
                 XDrawLine(dpy, valab, gcBlue, 93+ialab, height-18, 93+ialab, height);

                 sprintf(rpm,"%f",wradmax);
                 if ( mfont > 0 )
                 XDrawString( dpy, valab, gcSFont, 96-6*strlen(rpm)+ialab, height-30, rpm, strlen(rpm));
                 XDrawLine(dpy, valab, gcGreen, 96+ialab, height-28, 96+ialab, height);

                 XDrawLine(dpy, vblab, gcBlack, 0, 90, width, 90 );
                 XDrawLine(dpy, vblab, gcBlack, width-10, 90, width-10, 0 );

                 sprintf(rpm,"%f",hdensmax);
                 if ( mfont > 0 )
                 XDrawString( dpy, vblab, gcSFont, width-10-6*strlen(rpm), 10, rpm, strlen(rpm));
                 XDrawLine(dpy, vblab, gcRed, width-10, 10, width-5, 10 );

                 sprintf(rpm,"%f",hvelomax);
                 if ( mfont > 0 )
                 XDrawString( dpy, vblab, gcSFont, width-10-6*strlen(rpm), 20, rpm, strlen(rpm));
                 XDrawLine(dpy, vblab, gcBlue, width-10, 20, width-5, 20 );

                 sprintf(rpm,"%f",hwradmax);
                 if ( mfont > 0 )
                 XDrawString( dpy, vblab, gcSFont, width-10-6*strlen(rpm), 30, rpm, strlen(rpm));
                 XDrawLine(dpy, vblab, gcGreen, width-10, 30, width-5, 30 );

                 for ( i=1; i<NFIELD-1; i++)
                 {
                   if ( densmax <= 1.0e-10 ) densmax = 1.0e-10;
                   if ( velomax <= 1.0e-10 ) velomax = 1.0e-10;
                   if ( wradmax <= 1.0e-10 ) wradmax = 1.0e-10;
           /* Plot vertical density */
                   if ( ivector == 0 )
                   {
                   XDrawLine(dpy, valab, gcRed,
                       10 + (int) (dens[i]/densmax/sum_add * 80.) + ialab,
                       (int) ( (float)(i*height) / NFIELD ), 
                       10 + (int) (dens[i+1]/densmax/sum_add * 80.) + ialab,
                       (int) ( (float)((i+1)*height) / NFIELD )); 
                   XDrawLine(dpy, valab, gcGrey,
                       10 + (int) (3.*densc[i]/densmax/sum_add * 80.) + ialab,
                       (int) ( (float)(i*height) / NFIELD ), 
                       10 + (int) (3.*densc[i+1]/densmax/sum_add * 80.) + ialab,
                       (int) ( (float)((i+1)*height) / NFIELD )); 
                   if( ( dens[i] > 0. ) & ( dens[i+1] > 0. ) )
                     XDrawLine(dpy, valab, gcBlue,
                       10 + (int) (velo[i]/dens[i]/velomax/vsum_add * 80.) + ialab,
                       (int) ( (float)(i*height) / NFIELD ), 
                       10 + (int) (velo[i+1]/dens[i+1]/velomax/vsum_add * 80.) + ialab,
                       (int) ( (float)((i+1)*height) / NFIELD )); 
                   if( ( dens[i] > 0. ) & ( dens[i+1] > 0. ) )
                     XDrawLine(dpy, valab, gcGreen,
                       10 + (int) (wrad[i]/dens[i]/wradmax/wsum_add * 80.) + ialab,
                       (int) ( (float)(i*height) / NFIELD ), 
                       10 + (int) (wrad[i+1]/dens[i+1]/wradmax/wsum_add * 80.) + ialab,
                       (int) ( (float)((i+1)*height) / NFIELD )); 

            /* Plot Horizontal density */
                   if ( hdensmax <= 1.0e-10 ) hdensmax = 1.0e-10;
                   if ( hvelomax <= 1.0e-10 ) hvelomax = 1.0e-10;
                   if ( hwradmax <= 1.0e-10 ) hwradmax = 1.0e-10;
                   XDrawLine(dpy, vblab, gcRed,
                       (int) ( (float)(i*width) / NFIELD ), 
                       90 - (int) (hdens[i]/hdensmax/sum_add * 80.),
                       (int) ( (float)((i+1)*width) / NFIELD ), 
                       90 - (int) (hdens[i+1]/hdensmax/sum_add * 80.));
                   if( ( hdens[i] > 0. ) & ( hdens[i+1] > 0. ) )
                     XDrawLine(dpy, vblab, gcBlue,
                       (int) ( (float)(i*width) / NFIELD ), 
                       90 - (int) (hvelo[i]/hdens[i]/hvelomax/vsum_add * 80.),
                       (int) ( (float)((i+1)*width) / NFIELD ),
                       90 - (int) (hvelo[i+1]/hdens[i+1]/hvelomax/vsum_add * 80.));
                   if( ( hdens[i] > 0. ) & ( hdens[i+1] > 0. ) )
                     XDrawLine(dpy, vblab, gcGreen,
                       (int) ( (float)(i*width) / NFIELD ), 
                       90 - (int) (hwrad[i]/hdens[i]/hwradmax/wsum_add * 80.),
                       (int) ( (float)((i+1)*width) / NFIELD ),
                       90 - (int) (hwrad[i+1]/hdens[i+1]/hwradmax/wsum_add * 80.));
                   }
                 }
                 if( ladd == 1 )
                    sum_add = sum_add + 1;

               if (( istdout == 1 ) && ( ifast == 0 ))
               {
                 if ( densmax <= 1.0e-10 ) densmax = 1.0e-10;
                 if ( velomax <= 1.0e-10 ) velomax = 1.0e-10;
                 if ( wradmax <= 1.0e-10 ) wradmax = 1.0e-10;
                 if ( hdensmax <= 1.0e-10 ) hdensmax = 1.0e-10;
                 if ( hvelomax <= 1.0e-10 ) hvelomax = 1.0e-10;
                 if ( hwradmax <= 1.0e-10 ) hwradmax = 1.0e-10;
                 for ( i=1; i<NFIELD-1; i++)
                 {
                      xy1= velo[i]/dens[i]/velomax/vsum_add;
                      if( dens[i] == 0 ) 
                       xy1=-1.;
                      xy2= wrad[i]/dens[i]/wradmax/wsum_add;
                      if( dens[i] == 0 )
                       xy2=-1.;
                      xy3= hvelo[i]/hdens[i]/hvelomax/vsum_add;
                      if( hdens[i] == 0 )
                       xy3=-1.;
                       xy4= hwrad[i]/hdens[i]/hwradmax/wsum_add;
                      if( hdens[i] == 0 )
                       xy4=-1;

                      printf("%f %f %f %f %f %f %f %f %f %f\n",
                             -((float)(i*height)/NFIELD+(float)(-height+v_offset+xy0))/yskal,
                             dens[i]/densmax/sum_add,xy1,xy2,
                             -((float)(i*width)/NFIELD+(float)(-width+xy0))/xskal,
                             hdens[i]/hdensmax/sum_add,xy3,xy4,
                             dens[i]/sum_add,hdens[i]/sum_add);
                    }
                  }
               }

        /* draw contacts */
               if( strcmp(drfile, "-") )
               {
                 /* clear graphics-windows */
                 if( noddel > 0 )
                 {
                   XFillRectangle(dpy, vandy, gcWhite, 0, 0,
                                  width-1, height-1 );
                 }
                 printf("Draw contacts ... from file: %s \n",drfile);
                 Draw_Contacts( xy0, h_offset, v_offset, xskal, yskal,
                                xleft, xright, xbottom, xtop, 
                                NK, x1, x2, xrad );
               }

        /* draw text-time into the window */
               if( mfont > 0 )
               {
                if( ietime > 0 )
                {
                  sprintf(repm,"ezz =%8.4f   exx =%8.4f   eV =%8.4f",1-xtop/xtop0,1-xright/xright0,1-xright*xtop/(xright0*xtop0));
                  XDrawString( dpy, vandy, gcFont, ietime, ietime, repm, strlen(repm));
                }
                if( i0time > 0 )
                  XDrawString( dpy, vandy, gcFont, i0time, i0time, rpm, strlen(rpm));
                if( i0time < 0 )
                {
                  sprintf(rpm,"t =%7.3f s",time);
                  XDrawString( dpy, vandy, gcSFont, -i0time, -i0time, rpm, strlen(rpm));
                }
                if(( i0tposx > 0 ) || ( i0tposy > 0 ))
                {
                  sprintf(rpm,"c");
                  XDrawString( dpy, vandy, gcFont, i0tposx+3, i0tposy-2, rpm, strlen(rpm));
                  sprintf(rpm,"O S.Luding 1999");
                  XDrawString( dpy, vandy, gcFont, i0tposx, i0tposy, rpm, strlen(rpm));
                }
               }


        /* swap buffers */
               if( mmax101 > 50 )
               {
                  XCopyArea ( dpy, vandy, andy, gcBlack, 0, 0, width, height, 0, 0 );
                  XCopyArea ( dpy,  vlab,  lab, gcBlack, 0, 0, width, height, 0, 0 );
                  XCopyArea ( dpy, valab, alab, gcBlack, 0, 0, width, height, 0, 0 );
                  XCopyArea ( dpy, vblab, blab, gcBlack, 0, 0, width, height, 0, 0 );
               }
               if( iistep >= istep ) XFlush(dpy);
               if( ifilm >= 1 )
               {
                if( ifilm == 1 )
                  sprintf(csystem,"import +screen -window %lu -scene %d %6.6d.%s \n",andy,icount,icount,coutfile); 
                if( ifilm == 2 )
                  sprintf(csystem,"import +screen -window %lu -border -scene %d %6.6d.%s \n",eng,icount,icount,coutfile); 
                printf(csystem,"%s");
                system(csystem); 
                icount++;
               }

        /* make loop for pause between frames */
               jpause=0;
               jpnext=jpstep;
               for ( ipause=0; ipause<xpause; ipause++ )
               {
                  jpause++;
                  if(jpause>jpnext)
                  {
                     printf(".");
                     jpnext=jpause*2;
                  }
               }
             } /* end redraw loop */
            } /* end t>tmin */

            if( time >= tmax ) 
            {
                printf("Reached maximum time %f > %f \n",time,tmax);
                return DummyCycle();
            }

           }
           printf("%d %d --- Closing File %s \n", ierr, ierr6, cfile);
           fclose(fp);
           return DummyCycle();
         }

    return DummyCycle();
}


void Standard_Parameters()
{
   c2dfile = " ";
   c0file = " ";
   ic0 = 0;
   wallfile = "-";
   drfile = "-";
   iwnum = 0;
   coutfile = " ";
   width = 600;
   height = 600;
   xcells = 0;
   zcells = 0;
   offset = 0;
   hoffset = 0;
   noborder = 0;

   xellipse = 0.0;

   nredraw = 1;
   unidist = 0.0;
   rotr = 1.0;
   rotphi = 0.0;
   rotphi0 = 0.0;
   rottheta = 0.0;
   rottheta0 = 0.0;
   dunidist = 0.0;
   eyepos = 1.0;
   deyepos = 0.0;
   drotr = 0.0;
   drotphi = 0.0;
   drottheta = 0.0;
   trx = 0.0;
   try = 0.0;
   trz = 0.0;
   xyskal = 10000;
   vqskal = 100.;
   ivskal = 0.0;
   xpause = 0;
   jpstep = 1000000;
   xtrace = 0;
   xmtrace = 0;

   tmin   = -1.0e20;
   tmax   = 1.e20;

   drwmin = -1.e20;
   drwmax = 1.e20;
   vpower = 1.0;
   fpower = 1.0;
   cskal  = 10;
   dskal  = 1.;
   ich    = 0;
   ivmode = 0;
   coffset= 0.0;
   incmin = 0;
   idrmin = 0;
   incmax = 1000000;
   idrmax = 1000000;

   ladd = 0;

   icmax = 0;
   icmean = 0;
   icb = 0;
   cdirmin = 0.0;
   cdirmax = 100.0;

   icube = 0;
   ifog = 0;
   iphcm = 0;
   isq = -1;
   istep = 1;
   ilines = 0;
   isort = 0;
   ivector = 0;
   ivv = 0;
   invert = 0;
   idie = 0;
   nodel = 2;

   drwcont=1;
   drfnskal=1.;
   noddel = 1;

   iscale = 0;
   ifilm = 0;
   inc = 0;
   incdt = 0;
   incr = 0;
   iferro = 0;
   iwelo = 1;
   ivelo = 1;
   r0rred = 1.0;
   i0time = 0;
   ietime = 0;
   i0tposx = 0;
   i0tposy = 0;
   i0text = 0;
   i03d = 4;

   xtop0=-1.e10;
   xright0=-1.e10;

   time_step=1.;
   time_last=-1.;

   mmax101 = 65;
   mfont = 1;
   center_of_mass = 0;
   ifast = 0;
   ifast0 = 0;
   ifast1 = 0;
   isolid = 0;
   idraw = 0;
   istdout = 0;

   inread = 8;
   coll_hist = 0;
   icolmin=1;

 
   i0r = (int)(90*mmax101/101);
   i0g = (int)(50*mmax101/101);
   i0b = (int)(10*mmax101/101);
   i0s = (int)(80*mmax101/101);
   printf("# Color: R G B S %d %d %d %d \n",i0r,i0g,i0b,i0s);

}
    
void Create_Colors()
{
    int i, ifc, i0smax;

    XColor srcColor, dummyColor;
    unsigned int ired, igreen, iblue;
    float fgauss();

    XSelectInput (dpy, andy, ExposureMask);
    XMapWindow(dpy, andy);

    gcBlack = XCreateGC(dpy, andy,
                       (unsigned long) 0, NULL);
    XAllocNamedColor(dpy,
                     DefaultColormap(dpy, 0), "Black",
                     &srcColor, &dummyColor);
    XSetForeground(dpy, gcBlack, srcColor.pixel);

    gcRed = XCreateGC(dpy, andy,
                       (unsigned long) 0, NULL);
    XAllocNamedColor(dpy,
                     DefaultColormap(dpy, 0), "Red",
                     &srcColor, &dummyColor);
    XSetForeground(dpy, gcRed, srcColor.pixel);

    gcBlue = XCreateGC(dpy, andy,
                       (unsigned long) 0, NULL);
    XAllocNamedColor(dpy,
                     DefaultColormap(dpy, 0), "Blue",
                     &srcColor, &dummyColor);
    XSetForeground(dpy, gcBlue, srcColor.pixel);

    gcGreen = XCreateGC(dpy, andy,
                        (unsigned long) 0, NULL);
    XAllocNamedColor(dpy,
                     DefaultColormap(dpy, 0), "Green",
                     &srcColor, &dummyColor);
    XSetForeground(dpy, gcGreen, srcColor.pixel);

    gcLGrey = XCreateGC(dpy, andy,
                       (unsigned long) 0, NULL);
    XAllocNamedColor(dpy,
                     DefaultColormap(dpy, 0), "LightGrey",
                     &srcColor, &dummyColor);
    XSetForeground(dpy, gcLGrey, srcColor.pixel);

    gcGrey = XCreateGC(dpy, andy,
                       (unsigned long) 0, NULL);
    XAllocNamedColor(dpy,
                     DefaultColormap(dpy, 0), "Grey",
                     &srcColor, &dummyColor);
    XSetForeground(dpy, gcGrey, srcColor.pixel);

    gcWhite = XCreateGC(dpy, andy,
                       (unsigned long) 0, NULL);
    XAllocNamedColor(dpy,
                     DefaultColormap(dpy, 0), "white",
                     &srcColor, &dummyColor);
    XSetForeground(dpy, gcWhite, srcColor.pixel);

    if ( invert == 1 )
    {
     gcWhite = XCreateGC(dpy, andy,
                       (unsigned long) 0, NULL);
     XAllocNamedColor(dpy,
                     DefaultColormap(dpy, 0), "Black",
                     &srcColor, &dummyColor);
     XSetForeground(dpy, gcWhite, srcColor.pixel);
     gcBlack = XCreateGC(dpy, andy,
                       (unsigned long) 0, NULL);
     XAllocNamedColor(dpy,
                     DefaultColormap(dpy, 0), "white",
                     &srcColor, &dummyColor);
     XSetForeground(dpy, gcBlack, srcColor.pixel);
    }

/* Zero velocity ==> Black / large velocity ==> White */
    gcMap[1] = gcBlack;
    for( ifc=0; ifc<100; ifc++ )
      gcMapGrey[1][ifc] = XCreateGC(dpy, andy,
                                (unsigned long) 0, NULL);
    gcMap[mmax101+1] = gcWhite;
    for( ifc=0; ifc<100; ifc++ )
      gcMapGrey[mmax101+1][ifc] = XCreateGC(dpy, andy,
                                        (unsigned long) 0, NULL);
    XFillRectangle(dpy, valab, gcMap[1], 
           0, height-2-(int) ((float) (height)/(mmax101+1)),
           5, 1 + (int) ((float) height / (mmax101+1)));
    XFillRectangle(dpy, valab, gcMap[mmax101+1], 
           0, height-2-(int) ((float) ((mmax101+1.)*height)/(mmax101+1)),
           5, 1 + (int) ((float) height / (mmax101+1)));

/* Velocity Scale 100 values */
    for(i=2;i<=mmax101;i++)
    {
      gcMap[i] = XCreateGC(dpy, andy,
                           (unsigned long) 0, NULL);
      srcColor.pixel = 1;
     /* create map ... */
      ired   = (int) ( 65535. * fgauss(i-1, i0r, i0s));
      igreen = (int) ( 65535. * fgauss(i-1, i0g, i0s));
      iblue  = (int) ( 65535. * fgauss(i-1, i0b, i0s));
     /* printf("%d %d %d %d\n",i,ired,igreen,iblue); */
      srcColor.red   = ired;
      srcColor.green = igreen;
      srcColor.blue  = iblue;
      XAllocColor(dpy,
                  DefaultColormap(dpy, 0), 
                  &srcColor);
      XSetForeground(dpy, gcMap[i], srcColor.pixel);

      XFillRectangle(dpy, valab, gcMap[i], 
           0, height-2-(int) ((float) (i*height)/(mmax101+1)),
           5, 1 + (int) ((float) height / (mmax101+1)));

      for( ifc=0; ifc<100; ifc++ )
      {
         gcMapGrey[i][ifc] = XCreateGC(dpy, andy,
                                   (unsigned long) 0, NULL);
         srcColor.pixel = 1;
        /* create map ... */
        /* i0smax=(int)(i0s*pow(ifc/5.,fpower)); */
         i0smax=(int)(i0s*pow(ifc/25.,2));
         if(i0smax<1) i0smax=1;
        /* if(i0smax>mmax101) i0smax=mmax101+1; */
        /* printf("MapGrey: %d %d %d \n",ifc,i0s,i0smax); */
         ired   = (int) ( 65535. * fgauss(i-1, i0r, i0smax));
         igreen = (int) ( 65535. * fgauss(i-1, i0g, i0smax));
         iblue  = (int) ( 65535. * fgauss(i-1, i0b, i0smax));
         if( ired   > 65535 )   ired=65535;
         if( igreen > 65535 ) igreen=65535;
         if( iblue  > 65535 )  iblue=65535;
         /* printf("%d %d %d %d\n",i,ired,igreen,iblue); */
         srcColor.red   = ired;
         srcColor.green = igreen;
         srcColor.blue  = iblue;
         XAllocColor(dpy,
                     DefaultColormap(dpy, 0), 
                     &srcColor);
         XSetForeground(dpy, gcMapGrey[i][ifc], srcColor.pixel);
      }
    }
  XFlush(dpy);
}

float fgauss(i,i0,is)
   int i, i0, is;
{
   float xval;
   if( i>i0 )
      xval = 1. - (float) (i-i0) / (float) is;
   else
      xval = 1. - (float) (i0-i) / (float) is;

   if( xval > 1. )
      xval = 1.;
   if( xval < 0. )
      xval = 0.;

   return xval;
}

void fill_dens( ixcenter, ixradius, dens )
   int ixcenter, ixradius;
   float dens[];
{
   float xtmp1, xtmp2, xx0;
   int   j, jj;

   xx0 = sqrt((float)(ixradius*ixradius))+
         sqrt((float)(ixradius*ixradius - 1));

   for ( j=ixcenter-ixradius; j<ixcenter+ixradius; j++ )
   {
      jj = j;
      if( jj > NFIELD-1 ) jj = 0;
      if( jj < 1 ) jj = 0;
      xtmp1 = sqrt((float)(ixradius*ixradius - (j-ixcenter)*(j-ixcenter)));
      if( jj > 0 )
      {
         xtmp2 = sqrt((float)(ixradius*ixradius - (j+1-ixcenter)*(j+1-ixcenter)));
         dens[jj] = dens[jj] + (xtmp1 + xtmp2)/xx0;
         xtmp1 = xtmp2;
      }
   }
}

void fill_velo( ixcenter, ixradius, vdens, vaver )
   int ixcenter, ixradius;
   float vdens[];
   float vaver;
{
   float xtmp1, xtmp2, xx0;
   int   j, jj;

   xx0 = sqrt((float)(ixradius*ixradius))+
         sqrt((float)(ixradius*ixradius - 1));

   for ( j=ixcenter-ixradius; j<ixcenter+ixradius; j++ )
   {
      jj = j;
      if( jj > NFIELD-1 ) jj = 0;
      if( jj < 1 ) jj = 0;
      xtmp1 = sqrt((float)(ixradius*ixradius - (j-ixcenter)*(j-ixcenter)));
      if( jj > 0 )
      {
         xtmp2 = sqrt((float)(ixradius*ixradius - (j+1-ixcenter)*(j+1-ixcenter)));
         vdens[jj] = vdens[jj] + vaver*(xtmp1 + xtmp2)/xx0;
         xtmp1 = xtmp2;
      }
   }
}

void fill_wrad( ixcenter, ixradius, vdens, vaver )
   int ixcenter, ixradius;
   float vdens[];
   float vaver;
{
   float xtmp1, xtmp2, xx0;
   int   j, jj;

   xx0 = sqrt((float)(ixradius*ixradius))+
         sqrt((float)(ixradius*ixradius - 1));

   for ( j=ixcenter-ixradius; j<ixcenter+ixradius; j++ )
   {
      jj = j;
      if( jj > NFIELD-1 ) jj = 0;
      if( jj < 1 ) jj = 0;
      xtmp1 = sqrt((float)(ixradius*ixradius - (j-ixcenter)*(j-ixcenter)));
      if( jj > 0 )
      {
         xtmp2 = sqrt((float)(ixradius*ixradius - (j+1-ixcenter)*(j+1-ixcenter)));
         vdens[jj] = vdens[jj] + vaver*(xtmp1 + xtmp2)/xx0;
         xtmp1 = xtmp2;
      }
   }
}

int read_line( int inread, FILE *fp, 
               float *xr1, float *xr2, float *xr3, float *xr4, 
               float *xrrad, float *xr6, float *xr7, float *xrix,
               float *xr12, float *xr32, float *xr61, float *xr62,
               float *xr71, float *xr72 )
{
  int i, ierr = 0;
  int clength = 450;
  char cline[450];
  int icomp=0;
  float xcont=0.;

while( icomp == 0 )
{
  fgets( cline, clength, fp );
  if( (icomp = strncmp( "#", cline, 1 )) != 0 )
  {
    if( inread == 6 )
    {
      ierr = sscanf( cline, "%f %f %f %f %f ", xr1, xr2, xr3, xrrad, xr4);
      *xr6 = 0.0;
      *xr7 = 0.0;
      *xrix = 0.0;
    }
    else if( inread == 5 )
    {
      ierr = sscanf(cline, "%f %f %f %f %f ", xr1, xr2, xr3, xr4, xrrad);
      *xr6 = 0.0;
      *xr7 = 0.0;
      *xrix = 0.0;
    }
    else if( inread == 7 )
    {
      ierr = sscanf(cline, "%f %f %f %f %f %f %f %f \n ", 
                    xr1, xr12, xr2, xr3, xr32, xr4, xrrad, xrix);
      *xr6 = 0.0;
      *xr7 = 0.0;
    }
    else if(( inread == 3 ) || ( inread == 13 ))
    {
      ierr = sscanf(cline, "%f %f %f %f %f %f %f %f %f %f %f %f %f\n",
                    xr1, xr12, xr2, xr3, xr32, xr4, xrrad,
                    xr61, xr6, xr62, xr71, xr7, xr72);
      *xrix = 0.0;
    }
    else if(( inread == 4 ) || ( inread == 14 ))
    {
      ierr = sscanf(cline, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
                    xr1, xr12, xr2, xr3, xr32, xr4, xrrad,
                    xr61, xr6, xr62, xr71, xr7, xr72, xrix);
    }
    else if( inread == 8 )
    {
      ierr = sscanf(cline, "%f %f %f %f %f %f %f %f\n",
                    xr1, xr2, xr3, xr4, xrrad, xr6, xr7, xrix );
    }
    else
    {
      printf("Unknown data-file format: %d \n ",inread);
      ierr=-2;
    }
  }
  else
  {
     ierr=-3;
  }

}
*xrrad=*xrrad/r0rred;
return ierr;
}

int read_header( int inread, FILE *fp, 
                 int *rNK, float *rtime, float *rxleft, float *rxrear,
                 float *rxbottom, float *rxright, float *rxfront, float *rxtop )
{
  int ierr = 0;
  int clength = 240;
  char cline[240];
  int icomp=0;

  *rxrear=0;
  *rxfront=0;
 
while( icomp == 0 )
{
  fgets( cline, clength, fp );
  if( (icomp = strncmp( "#", cline, 1 )) != 0 )
  {
    if(( inread == 3 ) || ( inread == 7 ) || ( inread == 14 ))
    {
      ierr = sscanf( cline, "%d %f %f %f %f %f %f %f ", 
                     rNK, rtime, rxleft, rxrear, rxbottom, rxright, rxfront, rxtop); 
      /* printf(" ie = %d cl = %d %d ...", ierr, cline, clength );  */
      printf(".rh7.t=%f \n",*rtime); 
    }
    else
    {
      if( inread != 6 ){
         ierr = sscanf( cline, "%d %f %f %f %f %f ", 
                        rNK, rtime, rxleft, rxbottom, rxright, rxtop);
         /* printf(" %d %d %f \n", inread, rNK, rxright ); */
         printf(".rh8.t=%f \n",*rtime); 
      }
      else
      {
         ierr = sscanf( cline, "%d %f %f %f %f ",
                        rNK, rtime, rxleft, rxbottom, rxright);
         *rxtop = 1.0;
         printf(".rh6.t=%f \n",*rtime); 
      }
    }
  }
  else
      ierr=-2;
  /* printf(" %d %d %f \n", inread, rNK, rxright ); */
}
return ierr;
}

int ieyedist(const void *p1, const void *p2)
{
   int icomp, i1, i2;

   i1=*(const int*)p1;
   i2=*(const int*)p2; 
   
   if( xg12[i1] < xg12[i2] ) { icomp=1; }
   else
   {
      if( xg12[i1] > xg12[i2] ) { icomp=-1; }
      else                      { icomp=0;  }
   }
   /* printf(" %d %d %d %g %g \n", i1, i2, icomp, xg12[i1], xg12[i2] ); */

   return icomp;
}
