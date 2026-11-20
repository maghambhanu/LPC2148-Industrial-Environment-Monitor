// Sets (forces to 1) a specific bit at 'BITPOS' in 'WORD' using bitwise OR
#define SETBIT(WORD,BITPOS) WORD|=(1<<BITPOS)

// Clears (forces to 0) a specific bit at 'BITPOS' in 'WORD' using bitwise AND with an inverted mask
#define CLRBIT(WORD,BITPOS) WORD&=~(1<<BITPOS)

// Complements (toggles 0->1 or 1->0) a specific bit at 'BITPOS' in 'WORD' using bitwise XOR
#define CPLBIT(WORD,BITPOS) WORD^=(1<<BITPOS)

// Writes a specific bit value ('BIT': 0 or 1) into 'WORD' at 'BITPOS' by clearing it first and ORing the new value
#define WRITEBIT(WORD,BITPOS,BIT) WORD=((WORD&~(1<<BITPOS))|(BIT<<BITPOS))

// Reads and returns the value (0 or 1) of a single bit at 'BITPOS' from 'WORD'
#define READBIT(WORD,BITPOS) ((WORD>>BITPOS)&1)

// Copies a bit from source position 'SBIT' to destination position 'DBIT' within the same 'WORD'
#define READWRITEBIT(WORD,DBIT,SBIT) WORD=((WORD&~(1<<DBIT))|((WORD>>SBIT)&1)<<DBIT)

// Copies a bit from 'SBIT' in 'SWORD' (source) and writes it into 'DBIT' in 'DWORD' (destination)
#define READWRITEBIT2(DWORD,DBIT,SWORD,SBIT) DWORD=((DWORD&~(1<<DBIT))|(((SWORD>>SBIT)&1)<<DBIT))

// Extracts and returns a 4-bit value (nibble) starting at 'STARTBIT' from 'WORD' (15 = 0xF mask)
#define READNIBBLE(WORD,STARTBIT) ((WORD>>STARTBIT)&15)

// Writes a 4-bit value ('VAL') into 'WORD' starting at 'STARTBIT' after clearing those 4 bits
#define WRITENIBBLE(WORD,STARTBIT,VAL) WORD=((WORD&~(15<<STARTBIT))|(VAL<<STARTBIT))

// Writes an 8-bit value ('VAL') into 'WORD' starting at 'STARTBIT' after clearing those 8 bits (255 = 0xFF mask)
#define WRITEBYTE(WORD,STARTBIT,VAL) WORD=((WORD&~((u32)255<<STARTBIT))|((u32)VAL<<STARTBIT))

// Extracts and returns an 8-bit value (byte) starting at 'STARTBIT' from 'WORD'
#define READBYTE(WORD,STARTBIT) ((WORD>>STARTBIT)&255)
