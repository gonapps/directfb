/*
 * Copyright (C) 2006 Claudio Ciccani <klan@users.sf.net>
 *
 * Graphics driver for ATI R100 based chipsets written by
 *             Claudio Ciccani <klan@users.sf.net>.  
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __R100_MMIO_H__
#define __R100_MMIO_H__

#include <unistd.h>
#include <dfb_types.h>

#include "r100.h"


static __inline__ void
r100_out8( volatile __u8 *mmioaddr, __u32 reg, __u8 value )
{
     *((volatile __u8*)(mmioaddr+reg)) = value;
}

static __inline__ void
r100_out32( volatile __u8 *mmioaddr, __u32 reg, __u32 value )
{
#ifdef __powerpc__
    asm volatile( "stwbrx %0,%1,%2;eieio" 
                  :: "r" (value), "b"(reg), "r" (mmioaddr) : "memory" );
#else
    *((volatile __u32*)(mmioaddr+reg)) = value;
#endif
}

static __inline__ __u8
r100_in8( volatile __u8 *mmioaddr, __u32 reg )
{
     return *((volatile __u8*)(mmioaddr+reg));
}

static __inline__ __u32
r100_in32( volatile __u8 *mmioaddr, __u32 reg )
{
#ifdef __powerpc__
    __u32 value;
    asm volatile( "lwbrx %0,%1,%2;eieio"
                  : "=r" (value) : "b" (reg), "r" (mmioaddr) );
    return value;
#else
    return *((volatile __u32*)(mmioaddr+reg));
#endif
}


static __inline__ void
r100_outpll( volatile __u8 *mmioaddr, __u32 addr, __u8 value )
{
     r100_out8( mmioaddr, CLOCK_CNTL_INDEX, (addr & 0x3f) | PLL_WR_EN );
     r100_out8( mmioaddr, CLOCK_CNTL_DATA, value );
}

static __inline__ __u8
r100_inpll( volatile __u8 *mmioaddr, __u32 addr )
{
     r100_out8( mmioaddr, CLOCK_CNTL_INDEX, addr & 0x3f );
     return r100_in8( mmioaddr, CLOCK_CNTL_DATA );
}


static inline void 
r100_waitfifo( R100DriverData *rdrv,
               R100DeviceData *rdev,
               unsigned int    space )
{
     int waitcycles = 0;

     rdev->waitfifo_sum += space;
     rdev->waitfifo_calls++;

     if (rdev->fifo_space < space ) {
          do {
               rdev->fifo_space  = r100_in32( rdrv->mmio_base, RBBM_STATUS );
               rdev->fifo_space &= RBBM_FIFOCNT_MASK;
               if (++waitcycles > 10000000) {
                    D_BREAK( "FIFO timed out" );
                    r100_reset( rdrv, rdev );
                    return;
               }
          } while (rdev->fifo_space < space);
          
          rdev->fifo_waitcycles += waitcycles;
     } else
          rdev->fifo_cache_hits++;
	    
    rdev->fifo_space -= space;
}

static inline void
r100_waitidle( R100DriverData *rdrv, R100DeviceData *rdev )
{
     int waitcycles = 0;
     int status;

     r100_waitfifo( rdrv, rdev, 64 );
     
     do {
          status = r100_in32( rdrv->mmio_base, RBBM_STATUS );
          if (++waitcycles > 10000000) {
               D_BREAK( "Engine timed out" );
               r100_reset( rdrv, rdev );
               return;
          }
     } while (status & RBBM_ACTIVE);
     
     rdev->fifo_space = status & RBBM_FIFOCNT_MASK;
     rdev->idle_waitcycles += waitcycles;
}


#endif /* __R100_MMIO_H__ */
