#include <stdio.h>
#include <stdint.h>

// most significant digit in 1 byte char
#define TEC_MOST_SIG_BIT 128

/*tec_string_utf8:
	encodes a unicode codepoint into a utf8 encoded string
	uint32_t codepoint: the codepoint to be encoded into utf8
	char *str: the string where the encoded codepoint will be written to
	you need to ensure that str has at least 5 bytes
	4 bytes is the max length of a utf8 encoding plus NULL character
	returns number of bytes written (excluding NULL character)
*/
int			tec_string_utf8(uint32_t codepoint, char *str);

int tec_string_utf8(uint32_t n, char *str){

	if(n > 1114112 || ( n >= 0xd800 && n <= 0xdfff ) ){
		//encodes U+fffd; replacement character
		str[0] = 0xef;
		str[1] = 0xbf;
		str[2] = 0xbd;
		str[3] = 0;
		return 3;
	}

	// just like 7-bit ascii
	if(n < 128){
		str[0] = n;
		str[1] = '\0';
		return 1;
	}

	uint32_t len = 0;
	if(n < 2048){
		len = 2;
	}else{
		if( n < 65636){
			len = 3;
		}else{
			len = 4;
		}
	}
	str[len] = '\0';

	// set the bits at the start to indicate number of bytes
	unsigned char m = TEC_MOST_SIG_BIT;
	uint32_t i = 0;
	str[0] = 0;
	while(i < len){
		str[0] |= m;
		i += 1;
		m >>= 1;
	}

	//set the most significant bits in the other bytes
	i = 1;
	while(i < len){
		str[i] = TEC_MOST_SIG_BIT;
		i += 1;
	}

	//fill in the codepoint
	int j = len - 1;
	while(j >= 0){
		m = 1;
		i = 0;
		while(n && i < 6){
			if(n%2){
				str[j] |= m;
			}
			n >>= 1;
			m <<= 1;
			i += 1;
		}
		j -= 1;
	}

	return len;

}//tec_string_utf8*/

int main(){

	uint32_t cp = 0x1f300;
	char utf8[20];
	int i = 0;
	int cols = 50;
	char *tmp = 0;
	int n = 0;

	while(i < 1000){
		n = tec_string_utf8(cp, utf8);
//		tmp = utf8;
//		tmp += n;
//		tec_string_utf8(0xFE0E, tmp);
		printf("%5s ", utf8);
		cp+= 1;
		i+= 1;
		if( ! (i%cols) ){
			putchar('\n');
		}

	}

	putchar('\n');
	return 0;

}
