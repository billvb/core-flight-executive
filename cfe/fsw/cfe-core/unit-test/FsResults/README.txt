The FS Services unit test results are expected to have the
following results for each of the FS services source files:

==========================================================================

gcov: 'cfe_fs_api.c' 100.00% coverage

==========================================================================

gcov: 'cfe_fs_priv.c' 100.00% coverage

==========================================================================

cfe_fs_decompress.c - 87.91% coverage 

cfe_fs_decompress contains the code from a public domain copy of the gzip
utility that has been sightly modified to use the cFE variable types and 
return codes. Other than that, the decompress algorithm is very old and 
obtuse code. Most of what was not covered was because of macros that are
defined and use in the implementation. There are also lots of one letter
variables with no documentation.
==========================================================================


int32 FS_gz_eat_header( void )


        1:  215:		if ( (flags & CONTINUATION) != 0 ) {
        -:  216:			uint32 part;
    #####:  217:			part  = NEXTBYTE();
    #####:  218:			if( gGuzError != CFE_SUCCESS ) return CFE_FS_GZIP_READ_ERROR_HEADER; 
    #####:  219:			part |= NEXTBYTE() << 8;               /* <-- why is part used? -glw */
    #####:  220:			if( gGuzError != CFE_SUCCESS ) return CFE_FS_GZIP_READ_ERROR_HEADER;
        -:  221:		}
        -:  222:		
        1:  223:		if ( (flags & EXTRA_FIELD) != 0 ) {
        -:  224:			uint32 len;
    #####:  225:			len  = NEXTBYTE();
    #####:  226:			if( gGuzError != CFE_SUCCESS ) return CFE_FS_GZIP_READ_ERROR_HEADER;
    #####:  227:			len |= NEXTBYTE() << 8;
    #####:  228:			if( gGuzError != CFE_SUCCESS ) return CFE_FS_GZIP_READ_ERROR_HEADER;
    #####:  229:			while (len--) {
    #####:  230:				NEXTBYTE();
    #####:  231:				if( gGuzError != CFE_SUCCESS ) return CFE_FS_GZIP_READ_ERROR_HEADER;
        -:  232:			}
        -:  233:		}


        1:  252:		if ( (flags & COMMENT) != 0 ) {
    #####:  253:			keep_going = TRUE;
    #####:  254:			while ( keep_going ) {
        -:  255:
        -:  256:				/* Get the next byte */
    #####:  257:				thisByte = NEXTBYTE();
        -:  258:
        -:  259:				/* Header failure when end of file is reached or a read failure occurs */
    #####:  260:				if ((thisByte == EOF) || (gGuzError != CFE_SUCCESS)) return CFE_FS_GZIP_READ_ERROR_HEADER;
        -:  261:
        -:  262:				/* End of string was found */
    #####:  263:				if (thisByte == 0) keep_going = FALSE;
        -:  264:			}
        -:  265:		}



int32 FS_gz_huft_build( uint32 * b, uint32 n, uint32 s, uint16 * d, uint16 * e, int32 * m )


       20:  395:   if (c[0] == n) 
        -:  396:   {			/* null input--all zero length codes */
    #####:  397:		*m = 0;
    #####:  398:		return CFE_SUCCESS;
        -:  399:	}


     2197:  526:			if( (q == (HufTable *)NULL) && (j < z) )
        -:  527:			{
    #####:  528:				return CFE_FS_GZIP_BAD_CODE_BLOCK;
        -:  529:			}


int32 FS_gz_inflate( void )


        2:  574:	while ( gz_bk >= 8 ) {
    #####:  575:		gz_bk -= 8;
    #####:  576:		gz_inptr--;
        -:  577:	}


int32 FS_gz_inflate_block( int32 * e )


        -:  621:	/*  inflate that block type */
        6:  622:	if      ( t == 0 ) { res = FS_gz_inflate_stored();  trace[0]++; }  
        6:  623:	else if ( t == 1 ) { res = FS_gz_inflate_fixed();   trace[1]++; }
        6:  624:	else if ( t == 2 ) { res = FS_gz_inflate_dynamic(); trace[2]++; }
        -:  625:
    #####:  626:	else               res = CFE_FS_GZIP_BAD_CODE_BLOCK;    


int32 FS_gz_inflate_codes( HufTable * tl, HufTable * td, int32 bl, int32 bd )        


     2079:  678:				  if ( index >= 0  &&  index < gz_hufts ) 
     2079:  679:                 t = &( hufTable[index] );
        -:  680:				  else  
    #####:  681:                 return CFE_FS_GZIP_INDEX_ERROR;


    14203:  693:         if ( w == WSIZE ) 
        -:  694:         {
    #####:  695:            gz_outcnt = w;
    #####:  696:            FS_gz_flush_window();
    #####:  697:            w = 0;
        -:  698:         }


      337:  725:               if ( index >= 0  &&  index < gz_hufts ) 
      337:  726:                  t = &( hufTable[index] );
        -:  727:               else  
    #####:  728:                  return CFE_FS_GZIP_INDEX_ERROR;


        -:  918:      else 
        -:  919:      {                 /* j == 18: 11 to 138 zero length codes */
    #####:  920:         NEEDBITS(7);
    #####:  921:         j = 11 + ( (uint32)b & 0x7f );
    #####:  922:         DUMPBITS(7);
    #####:  923:         if ( (uint32)i + j > n ) return CFE_FS_GZIP_BAD_DATA;
    #####:  924:         while (j--) ll[i++] = 0;
    #####:  925:         l = 0;
        -:  926:		}

        
        1: 1012:	if ( error_code != CFE_SUCCESS && error_code != CFE_FS_GZIP_BAD_DATA ) return error_code;
        -: 1013:	/*  --- seems odd that BAD DATA is let through, but original did so --- */
        -: 1014:	/*  decompress until an end-of-block code  */
    #####: 1015:	error_code = FS_gz_inflate_codes( tl, td, bl, bd );
        -: 1016:
    #####: 1017:	return error_code;


int32 FS_gz_inflate_stored( void )


        -: 1043:	/*  get the length and its complement */
        1: 1044:	NEEDBITS(16);
    #####: 1045:	n = ( (uint32)b & 0xffff );
    #####: 1046:	DUMPBITS(16);
        -: 1047:
    #####: 1048:	NEEDBITS(16);
    #####: 1049:	if ( n != (uint32)( (~b) & 0xffff) )  return CFE_FS_GZIP_BAD_DATA;    /* error in compressed data */
    #####: 1050:	DUMPBITS(16);
        -: 1051:
        -: 1052:
        -: 1053:	/*  read and output the compressed data */
    #####: 1054:	while (n--) 
        -: 1055:   {
    #####: 1056:		NEEDBITS(8);
    #####: 1057:      gz_window[w++] = (uint8)b;
    #####: 1058:		if ( w == WSIZE ) 
        -: 1059:      {
    #####: 1060:	        gz_outcnt = w;
    #####: 1061:	        FS_gz_flush_window();
    #####: 1062:	        w = 0;
        -: 1063:		}
    #####: 1064:		DUMPBITS(8);
        -: 1065:	}
        -: 1066:
        -: 1067:	/*  restore the globals from the locals */
    #####: 1068:	gz_outcnt = w;					/* restore global window pointer */
    #####: 1069:	gz_bb     = b;					/* restore global bit buffer */
    #####: 1070:	gz_bk     = k;
        -: 1071:
    #####: 1072:	return CFE_SUCCESS;
        -: 1073:
        -: 1074:}
