
/*
  -----------------------------------------------------------------------
  Copyright (c) 2001 Dr Brian Gladman <brg@gladman.uk.net>, Worcester, UK

  TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  This software is provided 'as is' with no guarantees of correctness or
  fitness for purpose.
  -----------------------------------------------------------------------
*/

/*
  Used to ensure table is generated in the right format
  depending on the internal byte order required.
*/

#define w0(p)          0x000000##p

/*
  Number of elements required in this table for different
  block and key lengths is:

  Rcon Table      key length (bytes)
  Length          16  20  24  28  32
               ---------------------
  block     16 |  10   9   8   7   7
  length    20 |  14  11  10   9   9
  (bytes)   24 |  19  15  12  11  11
            28 |  24  19  16  13  13
            32 |  29  23  19  17  14

  this table can be a table of bytes if the key schedule
  code is adjusted accordingly
*/

const word rijndael_enc_rcon_tab[ 29 ] = {
  w0( 01 ), w0( 02 ), w0( 04 ), w0( 08 ),
  w0( 10 ), w0( 20 ), w0( 40 ), w0( 80 ),
  w0( 1b ), w0( 36 ), w0( 6c ), w0( d8 ),
  w0( ab ), w0( 4d ), w0( 9a ), w0( 2f ),
  w0( 5e ), w0( bc ), w0( 63 ), w0( c6 ),
  w0( 97 ), w0( 35 ), w0( 6a ), w0( d4 ),
  w0( b3 ), w0( 7d ), w0( fa ), w0( ef ),
  w0( c5 )
};

#undef  w0

/*
  used to ensure table is generated in the right format
  depending on the internal byte order required
*/

#define r0(p,q,r,s) 0x##p##q##r##s
#define r1(p,q,r,s) 0x##q##r##s##p
#define r2(p,q,r,s) 0x##r##s##p##q
#define r3(p,q,r,s) 0x##s##p##q##r
#define w0(p)          0x000000##p
#define w1(p)        0x0000##p##00
#define w2(p)        0x00##p##0000
#define w3(p)        0x##p##000000

/*
  used to ensure table is generated in the right format
  depending on the internal byte order required
*/

/*  data for forward tables (other than last round) */

#define f_table \
    r(a5,63,63,c6), r(84,7c,7c,f8), r(99,77,77,ee), r(8d,7b,7b,f6), \
    r(0d,f2,f2,ff), r(bd,6b,6b,d6), r(b1,6f,6f,de), r(54,c5,c5,91), \
    r(50,30,30,60), r(03,01,01,02), r(a9,67,67,ce), r(7d,2b,2b,56), \
    r(19,fe,fe,e7), r(62,d7,d7,b5), r(e6,ab,ab,4d), r(9a,76,76,ec), \
    r(45,ca,ca,8f), r(9d,82,82,1f), r(40,c9,c9,89), r(87,7d,7d,fa), \
    r(15,fa,fa,ef), r(eb,59,59,b2), r(c9,47,47,8e), r(0b,f0,f0,fb), \
    r(ec,ad,ad,41), r(67,d4,d4,b3), r(fd,a2,a2,5f), r(ea,af,af,45), \
    r(bf,9c,9c,23), r(f7,a4,a4,53), r(96,72,72,e4), r(5b,c0,c0,9b), \
    r(c2,b7,b7,75), r(1c,fd,fd,e1), r(ae,93,93,3d), r(6a,26,26,4c), \
    r(5a,36,36,6c), r(41,3f,3f,7e), r(02,f7,f7,f5), r(4f,cc,cc,83), \
    r(5c,34,34,68), r(f4,a5,a5,51), r(34,e5,e5,d1), r(08,f1,f1,f9), \
    r(93,71,71,e2), r(73,d8,d8,ab), r(53,31,31,62), r(3f,15,15,2a), \
    r(0c,04,04,08), r(52,c7,c7,95), r(65,23,23,46), r(5e,c3,c3,9d), \
    r(28,18,18,30), r(a1,96,96,37), r(0f,05,05,0a), r(b5,9a,9a,2f), \
    r(09,07,07,0e), r(36,12,12,24), r(9b,80,80,1b), r(3d,e2,e2,df), \
    r(26,eb,eb,cd), r(69,27,27,4e), r(cd,b2,b2,7f), r(9f,75,75,ea), \
    r(1b,09,09,12), r(9e,83,83,1d), r(74,2c,2c,58), r(2e,1a,1a,34), \
    r(2d,1b,1b,36), r(b2,6e,6e,dc), r(ee,5a,5a,b4), r(fb,a0,a0,5b), \
    r(f6,52,52,a4), r(4d,3b,3b,76), r(61,d6,d6,b7), r(ce,b3,b3,7d), \
    r(7b,29,29,52), r(3e,e3,e3,dd), r(71,2f,2f,5e), r(97,84,84,13), \
    r(f5,53,53,a6), r(68,d1,d1,b9), r(00,00,00,00), r(2c,ed,ed,c1), \
    r(60,20,20,40), r(1f,fc,fc,e3), r(c8,b1,b1,79), r(ed,5b,5b,b6), \
    r(be,6a,6a,d4), r(46,cb,cb,8d), r(d9,be,be,67), r(4b,39,39,72), \
    r(de,4a,4a,94), r(d4,4c,4c,98), r(e8,58,58,b0), r(4a,cf,cf,85), \
    r(6b,d0,d0,bb), r(2a,ef,ef,c5), r(e5,aa,aa,4f), r(16,fb,fb,ed), \
    r(c5,43,43,86), r(d7,4d,4d,9a), r(55,33,33,66), r(94,85,85,11), \
    r(cf,45,45,8a), r(10,f9,f9,e9), r(06,02,02,04), r(81,7f,7f,fe), \
    r(f0,50,50,a0), r(44,3c,3c,78), r(ba,9f,9f,25), r(e3,a8,a8,4b), \
    r(f3,51,51,a2), r(fe,a3,a3,5d), r(c0,40,40,80), r(8a,8f,8f,05), \
    r(ad,92,92,3f), r(bc,9d,9d,21), r(48,38,38,70), r(04,f5,f5,f1), \
    r(df,bc,bc,63), r(c1,b6,b6,77), r(75,da,da,af), r(63,21,21,42), \
    r(30,10,10,20), r(1a,ff,ff,e5), r(0e,f3,f3,fd), r(6d,d2,d2,bf), \
    r(4c,cd,cd,81), r(14,0c,0c,18), r(35,13,13,26), r(2f,ec,ec,c3), \
    r(e1,5f,5f,be), r(a2,97,97,35), r(cc,44,44,88), r(39,17,17,2e), \
    r(57,c4,c4,93), r(f2,a7,a7,55), r(82,7e,7e,fc), r(47,3d,3d,7a), \
    r(ac,64,64,c8), r(e7,5d,5d,ba), r(2b,19,19,32), r(95,73,73,e6), \
    r(a0,60,60,c0), r(98,81,81,19), r(d1,4f,4f,9e), r(7f,dc,dc,a3), \
    r(66,22,22,44), r(7e,2a,2a,54), r(ab,90,90,3b), r(83,88,88,0b), \
    r(ca,46,46,8c), r(29,ee,ee,c7), r(d3,b8,b8,6b), r(3c,14,14,28), \
    r(79,de,de,a7), r(e2,5e,5e,bc), r(1d,0b,0b,16), r(76,db,db,ad), \
    r(3b,e0,e0,db), r(56,32,32,64), r(4e,3a,3a,74), r(1e,0a,0a,14), \
    r(db,49,49,92), r(0a,06,06,0c), r(6c,24,24,48), r(e4,5c,5c,b8), \
    r(5d,c2,c2,9f), r(6e,d3,d3,bd), r(ef,ac,ac,43), r(a6,62,62,c4), \
    r(a8,91,91,39), r(a4,95,95,31), r(37,e4,e4,d3), r(8b,79,79,f2), \
    r(32,e7,e7,d5), r(43,c8,c8,8b), r(59,37,37,6e), r(b7,6d,6d,da), \
    r(8c,8d,8d,01), r(64,d5,d5,b1), r(d2,4e,4e,9c), r(e0,a9,a9,49), \
    r(b4,6c,6c,d8), r(fa,56,56,ac), r(07,f4,f4,f3), r(25,ea,ea,cf), \
    r(af,65,65,ca), r(8e,7a,7a,f4), r(e9,ae,ae,47), r(18,08,08,10), \
    r(d5,ba,ba,6f), r(88,78,78,f0), r(6f,25,25,4a), r(72,2e,2e,5c), \
    r(24,1c,1c,38), r(f1,a6,a6,57), r(c7,b4,b4,73), r(51,c6,c6,97), \
    r(23,e8,e8,cb), r(7c,dd,dd,a1), r(9c,74,74,e8), r(21,1f,1f,3e), \
    r(dd,4b,4b,96), r(dc,bd,bd,61), r(86,8b,8b,0d), r(85,8a,8a,0f), \
    r(90,70,70,e0), r(42,3e,3e,7c), r(c4,b5,b5,71), r(aa,66,66,cc), \
    r(d8,48,48,90), r(05,03,03,06), r(01,f6,f6,f7), r(12,0e,0e,1c), \
    r(a3,61,61,c2), r(5f,35,35,6a), r(f9,57,57,ae), r(d0,b9,b9,69), \
    r(91,86,86,17), r(58,c1,c1,99), r(27,1d,1d,3a), r(b9,9e,9e,27), \
    r(38,e1,e1,d9), r(13,f8,f8,eb), r(b3,98,98,2b), r(33,11,11,22), \
    r(bb,69,69,d2), r(70,d9,d9,a9), r(89,8e,8e,07), r(a7,94,94,33), \
    r(b6,9b,9b,2d), r(22,1e,1e,3c), r(92,87,87,15), r(20,e9,e9,c9), \
    r(49,ce,ce,87), r(ff,55,55,aa), r(78,28,28,50), r(7a,df,df,a5), \
    r(8f,8c,8c,03), r(f8,a1,a1,59), r(80,89,89,09), r(17,0d,0d,1a), \
    r(da,bf,bf,65), r(31,e6,e6,d7), r(c6,42,42,84), r(b8,68,68,d0), \
    r(c3,41,41,82), r(b0,99,99,29), r(77,2d,2d,5a), r(11,0f,0f,1e), \
    r(cb,b0,b0,7b), r(fc,54,54,a8), r(d6,bb,bb,6d), r(3a,16,16,2c)

/* generate the required tables in the desired endian format */

#undef  r
#define r   r0

const word rijndael_enc_ft_tab[ 4 ][ 256 ] = {
  {   f_table },
#undef  r
#define r   r1
  {   f_table },
#undef  r
#define r   r2
  {   f_table },
#undef  r
#define r   r3
  {   f_table }
};

/* generate the required tables in the desired endian format */

#undef  r
#define r(p,q,r,s)  w0(q)
const word rijndael_enc_fl_tab[ 4 ][ 256 ] = {
  {   f_table    },
#undef  r
#define r(p,q,r,s)   w1(q)
  {   f_table    },
#undef  r
#define r(p,q,r,s)   w2(q)
  {   f_table    },
#undef  r
#define r(p,q,r,s)   w3(q)
  {   f_table    }
};

#define m_table \
    r(00,00,00,00), r(0b,0d,09,0e), r(16,1a,12,1c), r(1d,17,1b,12), \
    r(2c,34,24,38), r(27,39,2d,36), r(3a,2e,36,24), r(31,23,3f,2a), \
    r(58,68,48,70), r(53,65,41,7e), r(4e,72,5a,6c), r(45,7f,53,62), \
    r(74,5c,6c,48), r(7f,51,65,46), r(62,46,7e,54), r(69,4b,77,5a), \
    r(b0,d0,90,e0), r(bb,dd,99,ee), r(a6,ca,82,fc), r(ad,c7,8b,f2), \
    r(9c,e4,b4,d8), r(97,e9,bd,d6), r(8a,fe,a6,c4), r(81,f3,af,ca), \
    r(e8,b8,d8,90), r(e3,b5,d1,9e), r(fe,a2,ca,8c), r(f5,af,c3,82), \
    r(c4,8c,fc,a8), r(cf,81,f5,a6), r(d2,96,ee,b4), r(d9,9b,e7,ba), \
    r(7b,bb,3b,db), r(70,b6,32,d5), r(6d,a1,29,c7), r(66,ac,20,c9), \
    r(57,8f,1f,e3), r(5c,82,16,ed), r(41,95,0d,ff), r(4a,98,04,f1), \
    r(23,d3,73,ab), r(28,de,7a,a5), r(35,c9,61,b7), r(3e,c4,68,b9), \
    r(0f,e7,57,93), r(04,ea,5e,9d), r(19,fd,45,8f), r(12,f0,4c,81), \
    r(cb,6b,ab,3b), r(c0,66,a2,35), r(dd,71,b9,27), r(d6,7c,b0,29), \
    r(e7,5f,8f,03), r(ec,52,86,0d), r(f1,45,9d,1f), r(fa,48,94,11), \
    r(93,03,e3,4b), r(98,0e,ea,45), r(85,19,f1,57), r(8e,14,f8,59), \
    r(bf,37,c7,73), r(b4,3a,ce,7d), r(a9,2d,d5,6f), r(a2,20,dc,61), \
    r(f6,6d,76,ad), r(fd,60,7f,a3), r(e0,77,64,b1), r(eb,7a,6d,bf), \
    r(da,59,52,95), r(d1,54,5b,9b), r(cc,43,40,89), r(c7,4e,49,87), \
    r(ae,05,3e,dd), r(a5,08,37,d3), r(b8,1f,2c,c1), r(b3,12,25,cf), \
    r(82,31,1a,e5), r(89,3c,13,eb), r(94,2b,08,f9), r(9f,26,01,f7), \
    r(46,bd,e6,4d), r(4d,b0,ef,43), r(50,a7,f4,51), r(5b,aa,fd,5f), \
    r(6a,89,c2,75), r(61,84,cb,7b), r(7c,93,d0,69), r(77,9e,d9,67), \
    r(1e,d5,ae,3d), r(15,d8,a7,33), r(08,cf,bc,21), r(03,c2,b5,2f), \
    r(32,e1,8a,05), r(39,ec,83,0b), r(24,fb,98,19), r(2f,f6,91,17), \
    r(8d,d6,4d,76), r(86,db,44,78), r(9b,cc,5f,6a), r(90,c1,56,64), \
    r(a1,e2,69,4e), r(aa,ef,60,40), r(b7,f8,7b,52), r(bc,f5,72,5c), \
    r(d5,be,05,06), r(de,b3,0c,08), r(c3,a4,17,1a), r(c8,a9,1e,14), \
    r(f9,8a,21,3e), r(f2,87,28,30), r(ef,90,33,22), r(e4,9d,3a,2c), \
    r(3d,06,dd,96), r(36,0b,d4,98), r(2b,1c,cf,8a), r(20,11,c6,84), \
    r(11,32,f9,ae), r(1a,3f,f0,a0), r(07,28,eb,b2), r(0c,25,e2,bc), \
    r(65,6e,95,e6), r(6e,63,9c,e8), r(73,74,87,fa), r(78,79,8e,f4), \
    r(49,5a,b1,de), r(42,57,b8,d0), r(5f,40,a3,c2), r(54,4d,aa,cc), \
    r(f7,da,ec,41), r(fc,d7,e5,4f), r(e1,c0,fe,5d), r(ea,cd,f7,53), \
    r(db,ee,c8,79), r(d0,e3,c1,77), r(cd,f4,da,65), r(c6,f9,d3,6b), \
    r(af,b2,a4,31), r(a4,bf,ad,3f), r(b9,a8,b6,2d), r(b2,a5,bf,23), \
    r(83,86,80,09), r(88,8b,89,07), r(95,9c,92,15), r(9e,91,9b,1b), \
    r(47,0a,7c,a1), r(4c,07,75,af), r(51,10,6e,bd), r(5a,1d,67,b3), \
    r(6b,3e,58,99), r(60,33,51,97), r(7d,24,4a,85), r(76,29,43,8b), \
    r(1f,62,34,d1), r(14,6f,3d,df), r(09,78,26,cd), r(02,75,2f,c3), \
    r(33,56,10,e9), r(38,5b,19,e7), r(25,4c,02,f5), r(2e,41,0b,fb), \
    r(8c,61,d7,9a), r(87,6c,de,94), r(9a,7b,c5,86), r(91,76,cc,88), \
    r(a0,55,f3,a2), r(ab,58,fa,ac), r(b6,4f,e1,be), r(bd,42,e8,b0), \
    r(d4,09,9f,ea), r(df,04,96,e4), r(c2,13,8d,f6), r(c9,1e,84,f8), \
    r(f8,3d,bb,d2), r(f3,30,b2,dc), r(ee,27,a9,ce), r(e5,2a,a0,c0), \
    r(3c,b1,47,7a), r(37,bc,4e,74), r(2a,ab,55,66), r(21,a6,5c,68), \
    r(10,85,63,42), r(1b,88,6a,4c), r(06,9f,71,5e), r(0d,92,78,50), \
    r(64,d9,0f,0a), r(6f,d4,06,04), r(72,c3,1d,16), r(79,ce,14,18), \
    r(48,ed,2b,32), r(43,e0,22,3c), r(5e,f7,39,2e), r(55,fa,30,20), \
    r(01,b7,9a,ec), r(0a,ba,93,e2), r(17,ad,88,f0), r(1c,a0,81,fe), \
    r(2d,83,be,d4), r(26,8e,b7,da), r(3b,99,ac,c8), r(30,94,a5,c6), \
    r(59,df,d2,9c), r(52,d2,db,92), r(4f,c5,c0,80), r(44,c8,c9,8e), \
    r(75,eb,f6,a4), r(7e,e6,ff,aa), r(63,f1,e4,b8), r(68,fc,ed,b6), \
    r(b1,67,0a,0c), r(ba,6a,03,02), r(a7,7d,18,10), r(ac,70,11,1e), \
    r(9d,53,2e,34), r(96,5e,27,3a), r(8b,49,3c,28), r(80,44,35,26), \
    r(e9,0f,42,7c), r(e2,02,4b,72), r(ff,15,50,60), r(f4,18,59,6e), \
    r(c5,3b,66,44), r(ce,36,6f,4a), r(d3,21,74,58), r(d8,2c,7d,56), \
    r(7a,0c,a1,37), r(71,01,a8,39), r(6c,16,b3,2b), r(67,1b,ba,25), \
    r(56,38,85,0f), r(5d,35,8c,01), r(40,22,97,13), r(4b,2f,9e,1d), \
    r(22,64,e9,47), r(29,69,e0,49), r(34,7e,fb,5b), r(3f,73,f2,55), \
    r(0e,50,cd,7f), r(05,5d,c4,71), r(18,4a,df,63), r(13,47,d6,6d), \
    r(ca,dc,31,d7), r(c1,d1,38,d9), r(dc,c6,23,cb), r(d7,cb,2a,c5), \
    r(e6,e8,15,ef), r(ed,e5,1c,e1), r(f0,f2,07,f3), r(fb,ff,0e,fd), \
    r(92,b4,79,a7), r(99,b9,70,a9), r(84,ae,6b,bb), r(8f,a3,62,b5), \
    r(be,80,5d,9f), r(b5,8d,54,91), r(a8,9a,4f,83), r(a3,97,46,8d)

#undef r
#define r   r0

const word rijndael_enc_im_tab[ 4 ][ 256 ] = {
  {   m_table },
#undef  r
#define r   r1
  {   m_table },
#undef  r
#define r   r2
  {   m_table },
#undef  r
#define r   r3
  {   m_table }
};
