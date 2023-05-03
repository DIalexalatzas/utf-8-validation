#include <stdio.h>
#include <stdlib.h>

int check1(int c) {
	if((c&0x80)==0) {  // Check if msb is 0
		c&=0x7F;  // Get code bits
		if(0<=c && c<=0x7F) return 1;  // Interval check
		else {
			printf("Invalid UTF-8 code point: U+%04X\n",c);
			return 0;
		}
	}
	else return 0;
}

int check234(int c1) {
	int c2, c3, c4, ch1, ch2, ch3, ch4;
	if((c1&0xE0)==0xC0) {  // Check if 3 msb is 110
		if((c2=getchar())==EOF) return -1;  // Read 2nd byte
		if((c2&0xC0)==0x80) {  // Check if 2 msb is 10
			ch1=c1, ch2=c2;
			ch1&=0x1F, ch2&=0x3F;
			ch1<<=6, ch1+=ch2;  // Get code bits
			if((c1&0x1E)==0) {  // Check if it fits in less bytes
				printf("Oversized UTF-8 code point: U+%04X\n",ch1);
				return 0;
			}
			if(0x80<=ch1 && ch1<=0x7FF) {  // Interval check
				if((c1&0x1E)==0) {
					printf("Oversized UTF-8 code point: U+%04X\n",ch1);
					return 0;
				}
				else return 1;
			}
			else {
				printf("Invalid UTF-8 code point: U+%04X\n",ch1);
				return 0;
			}
		}
		else {
			printf("Invalid UTF-8 tail byte: 0x%02X\n",c2);
			return 0;
		}
	}
	else {
		if((c1&0xF0)==0xE0) {  // Check if 4 msb is 1110
			if((c2=getchar())==EOF) return -1;  // Read 2nd byte
			if((c2&0xC0)==0x80) {  // Check if 2 msb is 10
				if((c3=getchar())==EOF) return -1;  // Read 3rd byte
				if((c3&0xC0)==0x80) {  // Check if 2 msb is 10
					ch1=c1, ch2=c2, ch3=c3;
					ch1&=0xF, ch2&=0x3F, ch3&=0x3F;
					ch2<<=6, ch1<<=12, ch1+=ch2+ch3;  // Get code bits
					if((c1&0xF)==0 && (c2&0x20)==0) {  //  Check if it fits in less bytes
						printf("Oversized UTF-8 code point: U+%04X\n",ch1);
						return 0;
					}
					if(0x800<=ch1 && ch1<=0xFFFF && !(0xD800<=ch1 && ch1<=0xDFFF)) {  // Interval check
						if((c1&0xF)!=0) return 1;
						else if((c2&0x20)!=0) return 1;
						else {
							printf("Oversized UTF-8 code point: U+%04X\n",ch1);
							return 0;
						}
					}
					else {
						printf("Invalid UTF-8 code point: U+%04X\n",ch1);
						return 0;
					}
				}
				else {
					printf("Invalid UTF-8 tail byte: 0x%02X\n",c3);
					return 0;
				}
			}
			else {
				printf("Invalid UTF-8 tail byte: 0x%02X\n",c2);
				return 0;
			}
		}
		else {
			if((c1&0xF8)==0xF0) {  // Check 5 msb is 11110
				if((c2=getchar())==EOF) return -1;  // Read 2nd byte
				if((c2&0xC0)==0x80) {  // Check if 2 msb is 10
					if((c3=getchar())==EOF) return -1;  // Read 3rd byte
					if((c3&0xC0)==0x80) {  // Check if 2 msb is 10
						if((c4=getchar())==EOF) return -1;  // Read 4th byte
						if((c4&0xC0)==0x80) {  // Check if 2 msb is 10
							ch1=c1, ch2=c2, ch3=c3, ch4=c4;
							ch1&=0x7, ch2&=0x3F, ch3&=0x3F, ch4&=0x3F;
							ch3<<=6, ch2<<=12, ch1<<=18, ch1+=ch2+ch3+ch4;  // Get code bits
							if((c1&0x7)==0 && (c2&0x30)==0) {  // Check if it fits in less bytes
								printf("Oversized UTF-8 code point: U+%04X\n",ch1);
								return 0;
							}
							if(0x10000<=ch1 && ch1<=0x10FFFF) {  // Interval check
								if((c1&0x7)!=0) return 1;
								else if((c2&0x30)!=0) return 1;
								else {
									printf("Oversized UTF-8 code point: U+%04X\n",ch1);
									return 0;
								}
							}
							else {
								printf("Invalid UTF-8 code point: U+%04X\n",ch1);
								return 0;
							}
						}
						else {
							printf("Invalid UTF-8 tail byte: 0x%02X\n",c4);
							return 0;
						}
					}
					else {
						printf("Invalid UTF-8 tail byte: 0x%02X\n",c3);
						return 0;
					}
				}
				else {
					printf("Invalid UTF-8 tail byte: 0x%02X\n",c2);
					return 0;
				}
			}
			else {
				printf("Invalid UTF-8 header byte: 0x%02X\n",c1);
				return 0;
			}
		}
	}
}

int main(void) {
	int c, cnt1=0, cnt234=0;
	while((c=getchar())!=EOF) {
		if(check1(c)==1) {  // Valid 1 byte encoding
			cnt1++;
		}
		else if(check234(c)==1) {  // Valid 2 or 3 or 4 bytes encoding
			cnt234++;
		}
		else return 0;  // Invalid encoding
	}
	printf("Found %d ASCII and %d multi-byte UTF-8 characters.\n",cnt1,cnt234);
	return 0;
}
