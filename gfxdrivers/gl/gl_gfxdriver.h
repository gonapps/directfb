/*
   (c) Copyright 2012-2013  DirectFB integrated media GmbH
   (c) Copyright 2001-2013  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Shimokawa <andi@directfb.org>,
              Marek Pikarski <mass@directfb.org>,
              Sven Neumann <neo@directfb.org>,
              Ville Syrjälä <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __GL_GFXDRIVER_H__
#define __GL_GFXDRIVER_H__

#include <GL/glx.h>


typedef enum {
     GLBF_UPDATE_TARGET  = 0x00000001,
     GLBF_UPDATE_TEXTURE = 0x00000002,
} GLBufferFlags;

typedef struct {
     int                      magic;

     /* Update flags for OpenGL driver */
     GLBufferFlags            flags;

     /* Texture object bound to buffer */
     GLuint                   texture;
} GLBufferData;


typedef struct {
    DFBSurfaceBlittingFlags  supported_blittingflags;

     /* validation flags */
     int                      v_flags;

     /* GLSL related */
     bool                     has_glsl;
     bool                     glsl_probed;
     GLhandleARB              shader_colorkey;
     GLint                    location_colorkey;

     /** Add shared data here... **/
} GLDeviceData;


typedef struct {
     DFBSurfaceBlittingFlags  blittingflags;

     /* Flush every bunch of commands to avoid issue with the XServer... */
     unsigned int             calls;

     /** Add local data here... **/
} GLDriverData;


#endif
