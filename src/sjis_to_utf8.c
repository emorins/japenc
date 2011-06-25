void SijisToUtf8(char *te, char *ter)
{
  int cri = 0;
  int rcri = 0;
  int as;
  char smoji[3];
  int alen = strlen(te);
	while (cri < alen) {
     int nxt = 1;
	  if (te[cri] == 0) {break;}
	  if (te[cri] < 0x7f && te[cri] > 0) {
	    ter[rcri] = te[cri];
      rcri = rcri+1;
	    cri = cri+1;
	  } else {
		  smoji[0] = te[cri];
		  smoji[1] = te[cri+1];
            
		  //char to Shift-JIS
		  int sjt;
		  if (smoji[1] < 0) {
		    sjt = ((smoji[0] ^ 0xffffff00) << 8 ) | (smoji[1] ^ 0xffffff00);
		  }
		  else if(smoji[1] > 0) {
		    sjt = ((smoji[0] ^ 0xffffff00) << 8 ) | smoji[1];
		  }

		  //Shift-JIS > JIS
		  int sj = sjt >> 8;
		  int sjj = sjt ^ (sj << 8);
		  if (sj >= 0xe0) { sj = sj - 0x40; }
		  if (sjj >= 0x80) { sjj = sjj - 1; }
		  if (sjj >= 0x9e) {
		    sj = (sj - 0x70) * 2;
		    sjj = sjj - 0x7d;
		  } else {
		    sj = ((sj - 0x70) * 2) - 1;
		    sjj = sjj - 0x1f;
		  }
		  sj = (sj << 8) | sjj ;
		  
		  //JIS > UTF16
		  as=Jisx0208ToUnicode11(sj);
		  
		  //UTF16 > UTF8
		  int toU;
		  if(as > 0x07FF){
		    ter[rcri] = (char)(0xE0 | ((as >> 12) & 0x0F));
		    ter[rcri+1] = (char)(0x80 | ((as >>  6) & 0x3F));
		    ter[rcri+2] = (char)(0x80 | ((as >>  0) & 0x3F));
        rcri = rcri+3;
		  } else {
		    ter[rcri] = (char)(0xC0 | ((as >>  6) & 0x1F));
		    ter[rcri+1] = (char)(0x80 | ((as >>  0) & 0x3F));
        rcri = rcri+2;
		  }
		  cri = cri+2;
	  }
	}
  ter[cri] = '\0';
}

