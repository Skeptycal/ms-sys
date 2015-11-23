/******************************************************************
    Copyright (C) 2009  Henrik Carlqvist

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************/
#include <stdio.h>

#include "file.h"
#include "br.h"

int is_br(FILE *fp)
{
   /* A "file" is probably some kind of boot record if it contains the magic
      chars 0x55, 0xAA at position 0x1FE */
   unsigned char aucRef[] = {0x55, 0xAA};

   return contains_data(fp, 0x1FE, aucRef, sizeof(aucRef)); 
} /* is_br */

int is_lilo_br(FILE *fp)
{
   /* A "file" is probably a LILO boot record if it contains the magic
      chars LILO at position 0x6 or 0x2 for floppies */
   unsigned char aucRef[] = {'L','I','L','O'};

   return ( contains_data(fp, 0x6, aucRef, sizeof(aucRef)) ||
	    contains_data(fp, 0x2, aucRef, sizeof(aucRef)) ); 
} /* is_lilo_br */

int is_dos_mbr(FILE *fp)
{
   #include "mbr_dos.h"

   return
      contains_data(fp, 0x0, mbr_dos_0x0, sizeof(mbr_dos_0x0)) &&
      is_br(fp);
} /* is_dos_mbr */

int is_dos_f2_mbr(FILE *fp)
{
   #include "mbr_dos_f2.h"

   return
      contains_data(fp, 0x0, mbr_dos_f2_0x0, sizeof(mbr_dos_f2_0x0)) &&
      is_br(fp);
} /* is_dos_f2_mbr */

int is_95b_mbr(FILE *fp)
{
   #include "mbr_95b.h"

   return
      contains_data(fp, 0x0,   mbr_95b_0x0,   sizeof(mbr_95b_0x0)) &&
      contains_data(fp, 0x0e0, mbr_95b_0x0e0, sizeof(mbr_95b_0x0e0)) &&
      is_br(fp);
} /* is_95b_mbr */

int is_2000_mbr(FILE *fp)
{
   #include "mbr_2000.h"

   return
      contains_data(fp, 0x0, mbr_2000_0x0, sizeof(mbr_2000_0x0)) &&
      is_br(fp);
} /* is_2000_mbr */

int is_vista_mbr(FILE *fp)
{
   #include "mbr_vista.h"

   return
      contains_data(fp, 0x0, mbr_vista_0x0, sizeof(mbr_vista_0x0)) &&
      is_br(fp);
} /* is_vista_mbr */

int is_win7_mbr(FILE *fp)
{
   #include "mbr_win7.h"

   return
      contains_data(fp, 0x0, mbr_win7_0x0, sizeof(mbr_win7_0x0)) &&
      is_br(fp);
} /* is_win7_mbr */

int is_rufus_mbr(FILE *fp)
{
   #include "mbr_rufus.h"

   return
      contains_data(fp, 0x0, mbr_rufus_0x0, sizeof(mbr_rufus_0x0)) &&
      is_br(fp);
} /* is_rufus_mbr */

int is_reactos_mbr(FILE *fp)
{
   #include "mbr_reactos.h"

   return
      contains_data(fp, 0x0, mbr_reactos_0x0, sizeof(mbr_reactos_0x0)) &&
      is_br(fp);
} /* is_reactos_mbr */

int is_grub4dos_mbr(FILE *fp)
{
   #include "mbr_grub.h"

   return
      contains_data(fp, 0x0, mbr_grub_0x0, sizeof(mbr_grub_0x0)) &&
      is_br(fp);
} /* is_grub_mbr */

int is_grub2_mbr(FILE *fp)
{
   #include "mbr_grub2.h"

   return
      contains_data(fp, 0x0, mbr_grub2_0x0, sizeof(mbr_grub2_0x0)) &&
      is_br(fp);
} /* is_grub2_mbr */

int is_kolibrios_mbr(FILE *fp)
{
   #include "mbr_kolibri.h"

   return
      contains_data(fp, 0x0, mbr_kolibri_0x0, sizeof(mbr_kolibri_0x0)) &&
      is_br(fp);
} /* is_kolibri_mbr */

int is_syslinux_mbr(FILE *fp)
{
   #include "mbr_syslinux.h"

   return
      contains_data(fp, 0x0, mbr_syslinux_0x0, sizeof(mbr_syslinux_0x0)) &&
      is_br(fp);
} /* is_syslinux_mbr */

int is_syslinux_gpt_mbr(FILE *fp)
{
   #include "mbr_gpt_syslinux.h"

   return
      contains_data(fp, 0x0, mbr_gpt_syslinux_0x0,
		    sizeof(mbr_gpt_syslinux_0x0)) &&
      is_br(fp);
} /* is_syslinux_gpt_mbr */

int is_zero_mbr(FILE *fp)
{
   #include "mbr_zero.h"

   return
      contains_data(fp, 0x0, mbr_zero_0x0, sizeof(mbr_zero_0x0));
	/* Don't bother to check 55AA signature */
} /* is_zero_mbr */

/* Handle nonstandard sector sizes (such as 4K) by writing
   the boot marker at every 512-2 bytes location */
static int write_bootmark(FILE *fp)
{
   extern unsigned long ulBytesPerSector;
   unsigned char aucRef[] = {0x55, 0xAA};
   unsigned long pos = 0x1FE;

   for (pos = 0x1FE; pos < ulBytesPerSector; pos += 0x200) {
      if (!write_data(fp, pos, aucRef, sizeof(aucRef)))
		    return 0;
   }
   return 1;
}

int write_dos_mbr(FILE *fp)
{
   #include "mbr_dos.h"

   return
      write_data(fp, 0x0, mbr_dos_0x0, sizeof(mbr_dos_0x0)) &&
      write_bootmark(fp);
} /* write_dos_mbr */

int write_95b_mbr(FILE *fp)
{
   #include "mbr_95b.h"

   return
      write_data(fp, 0x0,   mbr_95b_0x0, sizeof(mbr_95b_0x0)) &&
      write_data(fp, 0x0e0, mbr_95b_0x0e0, sizeof(mbr_95b_0x0e0)) &&
      write_bootmark(fp);
} /* write_95b_mbr */

int write_2000_mbr(FILE *fp)
{
   #include "mbr_2000.h"

   return
      write_data(fp, 0x0, mbr_2000_0x0, sizeof(mbr_2000_0x0)) &&
      write_bootmark(fp);
} /* write_2000_mbr */

int write_vista_mbr(FILE *fp)
{
   #include "mbr_vista.h"

   return
      write_data(fp, 0x0, mbr_vista_0x0, sizeof(mbr_vista_0x0)) &&
      write_bootmark(fp);
} /* write_vista_mbr */

int write_win7_mbr(FILE *fp)
{
   #include "mbr_win7.h"

   return
      write_data(fp, 0x0, mbr_win7_0x0, sizeof(mbr_win7_0x0)) &&
      write_bootmark(fp);
} /* write_win7_mbr */

int write_rufus_mbr(FILE *fp)
{
   #include "mbr_rufus.h"

   return
      write_data(fp, 0x0, mbr_rufus_0x0, sizeof(mbr_rufus_0x0)) &&
      write_bootmark(fp);
} /* write_rufus_mbr */

int write_reactos_mbr(FILE *fp)
{
   #include "mbr_reactos.h"

   return
      write_data(fp, 0x0, mbr_reactos_0x0, sizeof(mbr_reactos_0x0)) &&
      write_bootmark(fp);
} /* write_reactos_mbr */

int write_kolibrios_mbr(FILE *fp)
{
   #include "mbr_kolibri.h"

   return
      write_data(fp, 0x0, mbr_kolibri_0x0, sizeof(mbr_kolibri_0x0)) &&
      write_bootmark(fp);
} /* write_kolibri_mbr */

int write_syslinux_mbr(FILE *fp)
{
   #include "mbr_syslinux.h"

   return
      write_data(fp, 0x0, mbr_syslinux_0x0, sizeof(mbr_syslinux_0x0)) &&
      write_bootmark(fp);
} /* write_syslinux_mbr */

int write_syslinux_gpt_mbr(FILE *fp)
{
   #include "mbr_gpt_syslinux.h"

   return
      write_data(fp, 0x0, mbr_gpt_syslinux_0x0, sizeof(mbr_gpt_syslinux_0x0)) &&
      write_bootmark(fp);
} /* write_syslinux_gpt_mbr */

int write_grub4dos_mbr(FILE *fp)
{
   #include "mbr_grub.h"

   return
      write_data(fp, 0x0, mbr_grub_0x0, sizeof(mbr_grub_0x0)) &&
      write_bootmark(fp);
}

int write_grub2_mbr(FILE *fp)
{
   #include "mbr_grub2.h"

   return
      write_data(fp, 0x0, mbr_grub2_0x0, sizeof(mbr_grub2_0x0)) &&
      write_bootmark(fp);
}

int write_zero_mbr(FILE *fp)
{
   #include "mbr_zero.h"

   return
      write_data(fp, 0x0, mbr_zero_0x0, sizeof(mbr_zero_0x0)) &&
      write_bootmark(fp);
} /* write_zero_mbr */
