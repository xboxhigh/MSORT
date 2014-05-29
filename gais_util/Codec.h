#ifndef H_CODEC
#define H_CODEC

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#ifndef MAX_OFFSET_NO
#define MAX_OFFSET_NO	8
#endif

#define	MAX_TWO_BYTE_VALUE	32767

#define mTwoBDec(ucByteStr, iInputLength, iOutput)\
{\
	iOutput=(unsigned char)*ucByteStr;\
	if( iOutput<128 )\
		iInputLength=1;\
	else\
	{\
		iOutput= ((iOutput & 127) << 8) +  (unsigned char)*(ucByteStr+1);\
		iInputLength=2;\
	}\
}

#define mFiveBDec(ucByteStr, iInputLength, iOutput)\
{\
	iOutput=(unsigned char)*ucByteStr;\
	if( iOutput<128 )\
		iInputLength=1;\
	else if( iOutput<192 )\
	{\
		iOutput= ((iOutput & 63) << 8) +  (unsigned char)*(ucByteStr+1);\
		iInputLength=2;\
	}else if( iOutput<255 )\
	{\
		iOutput= ((iOutput & 63) << 8) +  (unsigned char)*(ucByteStr+1);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+2);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+3);\
		iInputLength=4;\
	}else\
	{\
		iOutput= (unsigned char)*(ucByteStr+1);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+2);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+3);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+4);\
		iInputLength=5;\
	}\
}

#define mTwoBDec2(ucByteStr, iOutput)\
{\
	iOutput=(unsigned char)*ucByteStr;\
	if( iOutput<128 )\
		ucByteStr++;\
	else\
	{\
		iOutput= ((iOutput & 127) << 8) +  (unsigned char)*(ucByteStr+1);\
		ucByteStr+=2;\
	}\
}

#define mFiveBDec2(ucByteStr, iOutput)\
{\
	iOutput=(unsigned char)*ucByteStr;\
	if( iOutput<128 )\
		ucByteStr++;\
	else if( iOutput<192 )\
	{\
		iOutput= ((iOutput & 63) << 8) +  (unsigned char)*(ucByteStr+1);\
		ucByteStr+=2;\
	}else if( iOutput<255 )\
	{\
		iOutput= ((iOutput & 63) << 8) +  (unsigned char)*(ucByteStr+1);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+2);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+3);\
		ucByteStr+=4;\
	}else\
	{\
		iOutput= (unsigned char)*(ucByteStr+1);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+2);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+3);\
		iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+4);\
		ucByteStr+=5;\
	}\
}


#define mOffsetDec(ucByteStr, iOutput)\
{\
	iOutput= (unsigned char)*(ucByteStr+1);\
	iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+2);\
	iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+3);\
	iOutput= (iOutput << 8) +  (unsigned char)*(ucByteStr+4);\
	ucByteStr+=5;\
}

#define mNineBDec(ucByteStr, iInputLength, iOutput)\
{\
	int		x, i;\
\
	bzero(iOutput, 8);\
	i=(int)(ucByteStr[0]);\
	if( i>9 )\
	{\
		iInputLength=1;\
		iOutput[0]=ucByteStr[0]-9;\
	}else\
	{\
		for( x=1; x<i; x++ )\
			iOutput[x-1]=ucByteStr[x];\
		iInputLength=i;\
	}\
}

#define mFiveBEncSize( uiInput, iSize)\
{\
	if(uiInput>=1040187393)\
		iSize=5;\
	else if(uiInput>=16384)\
		iSize=4;\
	else if(uiInput>=128)\
		iSize=2;\
	else\
		iSize=1;\
}

#define mTwoBEnc(iInput, iInputLength, ucByteStr)\
{\
	if( iInput>=128 )\
	{\
		ucByteStr[0]=(unsigned char)( ( (iInput|0x8000) & 0x0000ff00 ) >> 8 );\
		ucByteStr[1]=(unsigned char)( ( iInput & 0x000000ff ) );\
		iInputLength=2;\
	}\
	else\
	{\
		ucByteStr[0]=(unsigned char)iInput;\
		iInputLength=1;\
	}\
}

#define mFiveBEnc(iInput, iInputLength, ucByteStr)\
{\
	if( iInput>=1040187393 )\
	{\
		ucByteStr[0]=(unsigned char)0xff;\
		ucByteStr[1]=(unsigned char)( ( iInput & 0xff000000 ) >> 24 );\
		ucByteStr[2]=(unsigned char)( ( iInput & 0x00ff0000 ) >> 16 );\
		ucByteStr[3]=(unsigned char)( ( iInput & 0x0000ff00 ) >> 8 );\
		ucByteStr[4]=(unsigned char)( ( iInput & 0x000000ff ) );\
		iInputLength=5;\
	}\
	else if( iInput>=16384 )\
	{\
		ucByteStr[0]=(unsigned char)( ( (iInput|0xC0000000) & 0xff000000 ) >> 24 );\
		ucByteStr[1]=(unsigned char)( ( iInput & 0x00ff0000 ) >> 16 );\
		ucByteStr[2]=(unsigned char)( ( iInput & 0x0000ff00 ) >> 8 );\
		ucByteStr[3]=(unsigned char)( ( iInput & 0x000000ff ) );\
		iInputLength=4;\
	}\
	else if(iInput>=128)\
	{\
		ucByteStr[0]=(unsigned char)( ( (iInput|0x00008000) & 0x0000ff00 ) >> 8 );\
		ucByteStr[1]=(unsigned char)( ( iInput & 0x000000ff ) );\
		iInputLength=2;\
	}\
	else\
	{\
		ucByteStr[0]=(unsigned char)iInput;\
		iInputLength=1;\
	}\
}

#define mOffsetEnc( iInput, ucByteStr )\
{\
	ucByteStr[0]=(unsigned char)0xff;\
	ucByteStr[1]=(unsigned char)( ( iInput & 0xff000000 ) >> 24 );\
	ucByteStr[2]=(unsigned char)( ( iInput & 0x00ff0000 ) >> 16 );\
	ucByteStr[3]=(unsigned char)( ( iInput & 0x0000ff00 ) >> 8 );\
	ucByteStr[4]=(unsigned char)( ( iInput & 0x000000ff ) );\
}

#define mNineBEnc( ucpOffset, iInputLength, ucByteStr )\
{\
	int		x;\
\
	for( x=0; x<8; x++ )\
	{\
		if( ucpOffset[x]!=0 )\
			ucByteStr[x+1]=ucpOffset[x];\
		else\
			break;\
	}\
	if( x==1 && ucpOffset[0] < 246)\
	{\
		ucByteStr[0] = ucpOffset[0] + 9;\
		iInputLength=1;\
	}else\
	{\
		x++;\
		ucByteStr[0]=x;\
		iInputLength=x;\
	}\
}


/*#define mOffAryEnc( usaOffset, iInputLength, ucByteStr)\
{\
	int		x, iLen;\
	ushort	usPreOffset;\
\
	iInputLength=1;\
	usPreOffset=0;\
	for( x=0; x<8; x++ )\
	{\
		if( usaOffset[x]!=0 )\
		{\
			mTwoBEnc(usaOffset[x]-usPreOffset, iLen, (ucByteStr+iInputLength) );\
			usPreOffset=usaOffset[x];\
			iInputLength+=iLen;\
		}\
		else\
			break;\
	}\
	if( x==1 && usaOffset[0] < 255-MAX_OFFSET_NO*sizeof(ushort) )\
	{\
		ucByteStr[0] = (u_char)(usaOffset[0] + MAX_OFFSET_NO*sizeof(ushort));\
		iInputLength=1;\
	}else\
		ucByteStr[0]=iInputLength-1;\
}

#define mOffAryDec( ucByteStr, iInputLength, usaOutput )\
{\
	int		iOffNo, iCodeLen, iLen;\
	ushort	usPreOffset;\
\
	bzero(usaOutput, sizeof(ushort)*MAX_OFFSET_NO);\
	iCodeLen=(int)(ucByteStr[0]);\
	iInputLength=1;\
	if( iCodeLen>MAX_OFFSET_NO*sizeof(ushort) )\
		usaOutput[0]=(ushort)(iCodeLen-MAX_OFFSET_NO*sizeof(ushort));\
	else\
	{\
		iOffNo=0;\
		usPreOffset=0;\
		while( iCodeLen >= iInputLength )\
		{\
			mTwoBDec( (ucByteStr+iInputLength) , iLen, usaOutput[iOffNo] );\
			usaOutput[iOffNo]+=usPreOffset;\
			usPreOffset=usaOutput[iOffNo];\
			iInputLength+=iLen;\
			iOffNo++;\
		}\
	}\
}
*/

#define getPID(ptr, iDist, iPID)\
{\
	mFiveBDec2(ptr, iDist);\
	iPID+=iDist;\
}

#define getScore(ptr, iScore)\
{\
	mTwoBDec2(ptr, iScore);\
}

#define getOffset(ptr, iOffset)\
{\
	mOffsetDec(ptr, iOffset);\
}

#define getByteCnt(ptr, iByteCnt)\
{\
	mFiveBDec2(ptr, iByteCnt);\
}

#define	getOCnt(ptr, iOCnt)\
{\
	iOCnt = (u_char)*ptr; ptr++;\
}

#define getDocCnt(ptr, iDocCnt)\
{\
	mFiveBDec2(ptr, iDocCnt);\
}

#define getOccCnt(ptr, iDocCnt)\
{\
	mFiveBDec2(ptr, iDocCnt);\
}

#define getOffArray(ptr, usaOffsetArray)\
{\
	ptr+=OffsetArrayDecoder2(ptr, usaOffsetArray);\
}

// 肚 uiInput 竒筁 Encode 
int  SizeAfterFBEncoded(unsigned int uiInput);\

// Two byte  Decoder, 
// ucByteStr 琌肚 ByteString
// iInputLength 穦砆糶计砆 Encode Θ碭 Byte
// iOutput 穦砆糶 Decode ぇ 
void TwoByteDecoder(unsigned char *ucByteStr,int *iInputLength,uint *iOutput);

// Five byte  Encoder, 
// iInput 琌璶 Encode 计
// iInputLength 穦砆糶计砆 Encode Θ碭 Byte
// ucByteStr 琌璶糶 ByteString
void FiveByteEncoder(uint iInput,int *iInputLength,unsigned char *ucByteStr);

// Offset  Encoder,  Encode Θ 5 byte
// iInput 琌璶 Encode 计
// ucByteStr 琌璶糶 ByteString
void OffsetEncoder(uint iInput,unsigned char *ucByteStr);

// Two byte  Encoder, 
// iInput 琌璶 Encode 计
// iInputLength 穦砆糶计砆 Encode Θ碭 Byte
// ucByteStr 琌璶糶 ByteString
void TwoByteEncoder(uint iInput,int *iInputLength,unsigned char *ucByteStr);

void NineByteEncoder(unsigned char *ucpOffset,int *iInputLength,unsigned char *ucByteStr);
void NineByteDecoder(unsigned char *ucByteStr,int *iInputLength, unsigned char *iOutput);

// Five byte  Decoder, 
// ucByteStr 琌肚 ByteString
// iInputLength 穦砆糶计砆 Encode Θ碭 Byte
// iOutput 穦砆糶 Decode ぇ 
void FiveByteDecoder(unsigned char *ucByteStr,int *iInputLength,uint *iOuput);

void OffsetArrayEncoder(ushort *usaOffset, int *iInputLength, u_char *ucByteStr);
void OffsetArrayDecoder(u_char *ucByteStr, int *iInputLength, ushort *usaOutput);

#endif