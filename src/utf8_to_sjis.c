void Utf8ToSijis(char *te, char *ter)
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
      rcri = rcri + 1;
	    cri = cri + 1;
	  } else {
      int ucc;
      int uccc;
      int uc = te[cri];
      if (uc < 0) {
        uc = uc ^ 0xffffff00;
      }
		  int u16c;
		  switch (uc >> 4) { 
      case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        // 0xxxxxxx
        u16c = uc;
        break;
      case 12: case 13:
        // 110x xxxx   10xx xxxx
        ucc = te[cri + 1];
        if (ucc < 0) {
          ucc = ucc ^ 0xffffff00;
        }
        u16c = ((uc & 0x1F) << 6) | (ucc & 0x3F);
        nxt = 2;
        break;
		    case 14:
		      // 1110 xxxx  10xx xxxx  10xx xxxx
          ucc = te[cri + 1];
          uccc = te[cri + 2];
          if (ucc < 0) {
            ucc = ucc ^ 0xffffff00;
          }
          if (uccc < 0) {
            uccc = uccc ^ 0xffffff00;
          }
		      u16c = ((uc & 0x0F) << 12) | ((ucc & 0x3F) << 6) |	((uccc & 0x3F) << 0);
          nxt = 3;
		      break;
      }

		  //UTF16 > JIS
		  int rjis;
		  rjis=Unicode11ToJisx0208(u16c);

		  //JIS > Shift-JIS
		  char rsjis[4];
		  int rsjis1 = rjis >> 8;
		  int rsjis2 = rjis ^ (rsjis1 << 8);
		  if (rsjis1 % 2) {
		    rsjis1 = ((rsjis1 + 1) / 2) + 0x70;
		    rsjis2 = rsjis2 + 0x1f;
		  } else {
		    rsjis1 = (rsjis1 / 2) + 0x70;
		    rsjis2 = rsjis2 + 0x7d;
		  }
		  if (rsjis1 >= 0xa0) { rsjis1 = rsjis1 + 0x40; }
		  if (rsjis2 >= 0x7f) { rsjis2 = rsjis2 + 1; }
		  rsjis[0] = (char)rsjis1;
		  rsjis[1] = (char)rsjis2;
		  rsjis[2] = 0;

		  ter[rcri] = (char)rsjis1;
		  ter[rcri+1] = (char)rsjis2;
      
      rcri = rcri + 2;
		  cri = cri + nxt;
	  }
	}
  ter[cri] = '\0';
}

