#include "AOC_Precompiled.h"

typedef union uwb {
	unsigned w;
	unsigned char b[4];
} MD5union;

typedef unsigned DigestArray[4];

unsigned func0(unsigned abcd[]) {
	return (abcd[1] & abcd[2]) | (~abcd[1] & abcd[3]);
}

unsigned func1(unsigned abcd[]) {
	return (abcd[3] & abcd[1]) | (~abcd[3] & abcd[2]);
}

unsigned func2(unsigned abcd[]) {
	return  abcd[1] ^ abcd[2] ^ abcd[3];
}

unsigned func3(unsigned abcd[]) {
	return abcd[2] ^ (abcd[1] | ~abcd[3]);
}

typedef unsigned(*DgstFctn)(unsigned a[]);

unsigned* calctable(unsigned* k)
{
	double s, pwr;
	int i;

	pwr = pow(2.0, 32);
	for (i = 0; i < 64; i++) {
		s = fabs(sin(1.0 + i));
		k[i] = (unsigned)(s * pwr);
	}
	return k;
}

unsigned rol(unsigned r, short N)
{
	unsigned mask1 = (1 << N) - 1;
	return ((r >> (32 - N)) & mask1) | ((r << N) & ~mask1);
}

unsigned Algorithms_Hash_MD5(const char* msg, int mlen)
{
	static const uint k[] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

	static const DgstFctn ff[] = { &func0, &func1, &func2, &func3 };
	static const short M[] = { 1, 5, 3, 7 };
	static const short O[] = { 0, 1, 5, 0 };
	static const short rot0[] = { 7, 12, 17, 22 };
	static const short rot1[] = { 5, 9, 14, 20 };
	static const short rot2[] = { 4, 11, 16, 23 };
	static const short rot3[] = { 6, 10, 15, 21 };
	static const short* rots[] = { rot0, rot1, rot2, rot3 };

	byte msg2[64] = { 0 };
	uint32* w = (uint32*)msg2;

	GC_Memcpy(msg2, msg, mlen);
	msg2[mlen] = 0x80;
	*((uint64*)&msg2[64 - 8]) = uint64(mlen) * 8;

	uint out[] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };

	uint abcd[] = { out[0], out[1], out[2], out[3] };

	for_range_v(p, 4)
	{
		DgstFctn fctn = ff[p];
		const short* rotn = rots[p];
		const short m = M[p];
		const short o = O[p];
		for_range_v(q, 16)
		{
			const short g = (m * q + o) % 16;
			unsigned f = abcd[1] + rol(abcd[0] + fctn(abcd) + k[q + 16 * p] + w[g], rotn[q % 4]);

			abcd[0] = abcd[3];
			abcd[3] = abcd[2];
			abcd[2] = abcd[1];
			abcd[1] = f;
		}
	}
	for_range(4)
		out[i] += abcd[i];

	return out[0];
}

static auto locPart1(char const* aFile, uint magic)
{
	GC_String data = GC_File::ReadAllText(aFile);
	uint len = data.Length() - 1;

	char input[32];
	GC_Memcpy(input, data, len);

	for_range(10000000)
	{
		auto itoa = GC_IntegerToString(i);
		GC_Memcpy(input + len, itoa, itoa.Length());
		if ((Algorithms_Hash_MD5(input, len + itoa.Length()) & magic) == 0)
		{
			return i;
		}
	}

	return 0;
}

DEFINE_TEST_G(Part1, Day4)
{
	TEST_EQ(locPart1("AOC_Day4Test.txt", 0x000f0ffff), 609043);
	TEST_EQ(locPart1("AOC_Day4Part1.txt", 0x000f0ffff), 346386);
}

DEFINE_TEST_G(Part2, Day4)
{
	TEST_EQ(locPart1("AOC_Day4Part1.txt", 0x000fFffff), 9958218);
}
