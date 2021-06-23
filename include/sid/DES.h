#ifndef SIDDES_H
#define SIDDES_H


namespace DES {
	//#include <iostream>
	//#include <cstdlib>
	//#include <cmath>
	//#include <fstream>
	//#include <iomanip>
	#include <array>
	//#include <string>
	///////////////////////////////////////
	//                                   //
	//         HELPER FUNKCTION          //
	//                                   //
	///////////////////////////////////////

	constexpr uint64_t reverseBits(uint64_t value) noexcept {
		value = ((value & 0xFFFF'FFFF'0000'0000ull) >> 32) | ((value & 0x0000'0000'FFFF'FFFFull) << 32);
		value = ((value & 0xFFFF'0000'FFFF'0000ull) >> 16) | ((value & 0x0000'FFFF'0000'FFFFull) << 16);
		value = ((value & 0xFF00'FF00'FF00'FF00ull) >> 8) | ((value & 0x00FF'00FF'00FF'00FFull) << 8);
		value = ((value & 0xF0F0'F0F0'F0F0'F0F0ull) >> 4) | ((value & 0x0F0F'0F0F'0F0F'0F0Full) << 4);
		value = ((value & 0xCCCC'CCCC'CCCC'CCCCull) >> 2) | ((value & 0x3333'3333'3333'3333ull) << 2);
		value = ((value & 0xAAAA'AAAA'AAAA'AAAAull) >> 1) | ((value & 0x5555'5555'5555'5555ull) << 1);
		return value;
	}
	constexpr uint64_t reverseBytes(uint64_t value) noexcept {
		value = ((value & 0xFFFF'FFFF'0000'0000ull) >> 32) | ((value & 0x0000'0000'FFFF'FFFFull) << 32);
		value = ((value & 0xFFFF'0000'FFFF'0000ull) >> 16) | ((value & 0x0000'FFFF'0000'FFFFull) << 16);
		value = ((value & 0xFF00'FF00'FF00'FF00ull) >> 8) | ((value & 0x00FF'00FF'00FF'00FFull) << 8);
		return value;
	}



	///////////////////////////////////////
	//        ###   ###   ####           //
	//        #  #  #    #               //
	//        #  #  ##    ###            //
	//        #  #  #        #           //
	//        ###   ###  ####            //
	///////////////////////////////////////


	/**
	  Sbox[A][B][C]
	  A - S-Box number 0-7
	  B - row 0-3
	  C - column 0-15
	*/
	inline constexpr uint32_t SBoxLUT[8][4][16] = {
		{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7}, {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8}, {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0}, {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
		{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10}, {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5}, {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15}, {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
		{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8}, {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1}, {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7}, {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
		{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15}, {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9}, {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4}, {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
		{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9}, {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6}, {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14}, {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
		{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11}, {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8}, {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6}, {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
		{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1}, {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6}, {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2}, {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
		{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7}, {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2}, {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8}, {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}},
	};
	constexpr uint32_t SBox(uint32_t index, uint32_t in) {
		long row = (in & 1) | ((in >> 4) & 2); // get first and last from 6 input bits
											 // YxxxxX -> 00000X or 0000Y0 -> 0000YX
		in = (in >> 1) & 0xf; 				 // xXXXXx -> 0xXXXX -> 00XXXX
		return SBoxLUT[index][row][in];
	}
	constexpr uint32_t SubSBox(uint64_t in) {
		uint32_t out = 0;
		out = SBox(7, in);
		out |= SBox(6, in >> 6) << 4;
		out |= SBox(5, in >> 12) << 8;
		out |= SBox(4, in >> 18) << 12;
		out |= SBox(3, in >> 24) << 16;
		out |= SBox(2, in >> 30) << 20;
		out |= SBox(1, in >> 36) << 24;
		out |= SBox(0, in >> 42) << 28;
		return out;
	}

	// Initial Permutation
	inline constexpr uint32_t IPBox[64] = {
		57,49,41,33,25,17, 9,1, //00-07
		59,51,43,35,27,19,11,3, //08-15
		61,53,45,37,29,21,13,5, //16-23
		63,55,47,39,31,23,15,7, //24-31
		56,48,40,32,24,16, 8,0, //32-39
		58,50,42,34,26,18,10,2, //40-47
		60,52,44,36,28,20,12,4, //48-55
		62,54,46,38,30,22,14,6  //56-63
	};

	constexpr uint64_t InitPerm(uint64_t in) {
		uint64_t out = 0;
		for (uint32_t i = 0; i < 64; i++)
		{
			out |= ((in >> IPBox[i]) & 1) << i;
		}
		return out;
	}

	// Inverse of the Initial Permutation
	inline constexpr uint32_t IIPbox[64] = {
		39,7,47,15,55,23,63,31,
		38,6,46,14,54,22,62,30,
		37,5,45,13,53,21,61,29,
		36,4,44,12,52,20,60,28,
		35,3,43,11,51,19,59,27,
		34,2,42,10,50,18,58,26,
		33,1,41, 9,49,17,57,25,
		32,0,40, 8,48,16,56,24
	};
	constexpr uint64_t InvInitPerm(uint64_t in) {
		uint64_t out = 0;
		for (int i = 0; i < 64; i++)
		{
			out |= ((in >> IIPbox[i]) & 1) << i;
		}
		return out;
	}

	//E - 32b->48b
	inline constexpr uint32_t Ebox[48] = {
		31, 0, 1, 2, 3, 4,
		 3, 4, 5, 6, 7, 8,
		 7, 8, 9,10,11,12,
		11,12,13,14,15,16,
		15,16,17,18,19,20,
		19,20,21,22,23,24,
		23,24,25,26,27,28,
		27,28,29,30,31, 0
	};
	constexpr uint64_t Expand(uint64_t in) {
		uint64_t out = 0;
		for (int i = 0; i < 48; i++)
		{
			out |= ((in >> Ebox[i]) & 1) << i;
		}
		return out;
	}

	inline constexpr uint32_t Pbox[32] = {
		15, 6,19,20,
		28,11,27,16,
		 0,14,22,25,
		 4,17,30, 9,
		 1, 7,23,13,
		31,26, 2, 8,
		18,12,29, 5,
		21,10, 3,24
	};
	constexpr uint32_t DES_P(uint32_t in) {
		uint32_t out = 0;
		for (int i = 0; i < 32; i++)
		{
			out |= ((in >> (31 - Pbox[i])) & 1) << (31 - i);
		}
		return out;
	}

	/**
		R - 32b
		K - 48b
		return 32b
	*/
	constexpr uint32_t DES_F(uint32_t R, uint64_t K) {
		return DES_P(SubSBox((Expand(R) ^ K)));
	}

	inline constexpr uint64_t PC1box[56] = {
		56,48,40,32,24,16, 8,
		 0,57,49,41,33,25,17,
		 9, 1,58,50,42,34,26,
		18,10, 2,59,51,43,35,
		62,54,46,38,30,22,14,
		 6,61,53,45,37,29,21,
		13, 5,60,52,44,36,28,
		20,12, 4,27,19,11, 3
	};
	constexpr uint64_t DES_PC1(uint64_t in) {
		uint64_t out = 0;
		for (int i = 0; i < 56; i++)
		{
			out |= ((in >> (63 - PC1box[i])) & 1) << (55 - i);
		}
		return out;
	}
	inline constexpr const uint32_t PC2box[48] = {
		13,16,10,23, 0, 4,
		 2,27,14, 5,20, 9,
		22,18,11, 3,25, 7,
		15, 6,26,19,12, 1,
		40,51,30,36,46,54,
		29,39,50,44,32,47,
		43,48,38,55,33,52,
		45,41,49,35,28,31

	};
	constexpr uint64_t DES_PC2(uint64_t in) {
		uint64_t out = 0;
		for (int i = 0; i < 48; i++)
		{
			out |= ((in >> (55 - PC2box[i])) & 1) << (47 - i);
		}
		return out;
	}
	constexpr uint32_t Leftshift28b1s(int data) {
		data &= 0xFFFFFFF; // uci¹æ ostatnie 4 bity
		return ((data << 1) | (data >> 27)) & 0xFFFFFFF;
	}
	constexpr uint32_t Leftshift28b2s(int data) {
		data &= 0xFFFFFFF; // uci¹æ ostatnie 4 bity
		return ((data << 2) | (data >> 26)) & 0xFFFFFFF;
	}
	/**
		key - encription key, last bit of every byte is parity bit i.e.: 00101010, 11011001, etc
		subkeys[16] - place for 16 generated keys
	*/
	constexpr std::array<uint64_t, 16> KeyGenerator(uint64_t key) {
		uint64_t tmp = {};
		uint32_t C = {}, D = {};
		std::array<uint64_t, 16> subkeys = {};

		tmp = DES_PC1(key);
		//0
		C = tmp & 0xFFFFFFF;		D = tmp >> 28;
		//K1
		C = Leftshift28b1s(C);	D = Leftshift28b1s(D); 		//LS
		tmp = D;	subkeys[0] = DES_PC2((tmp << 28) | C);	//PC
		//K2
		C = Leftshift28b1s(C);	D = Leftshift28b1s(D); 		//LS
		tmp = D;	subkeys[1] = DES_PC2((tmp << 28) | C);	//PC
		//K3
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[2] = DES_PC2((tmp << 28) | C);	//PC
		//K4
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[3] = DES_PC2((tmp << 28) | C);	//PC
		//K5
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[4] = DES_PC2((tmp << 28) | C);	//PC
		//K6
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[5] = DES_PC2((tmp << 28) | C);	//PC
		//K7
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[6] = DES_PC2((tmp << 28) | C);	//PC
		//K8
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[7] = DES_PC2((tmp << 28) | C);	//PC
		//K9
		C = Leftshift28b1s(C);	D = Leftshift28b1s(D); 		//LS
		tmp = D;	subkeys[8] = DES_PC2((tmp << 28) | C);	//PC
		//K10
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[9] = DES_PC2((tmp << 28) | C);	//PC
		//K11
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[10] = DES_PC2((tmp << 28) | C);	//PC
		//K12
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[11] = DES_PC2((tmp << 28) | C);	//PC
		//K13
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[12] = DES_PC2((tmp << 28) | C);	//PC
		//K14
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[13] = DES_PC2((tmp << 28) | C);	//PC
		//K15
		C = Leftshift28b2s(C);	D = Leftshift28b2s(D); 		//LS
		tmp = D;	subkeys[14] = DES_PC2((tmp << 28) | C);	//PC
		//K16
		C = Leftshift28b1s(C);	D = Leftshift28b1s(D); 		//LS
		tmp = D;	subkeys[15] = DES_PC2((tmp << 28) | C);	//PC

		return subkeys;
	}


	/**
		encryption one prepared (padding, etc) 64-bit block
	*/
	constexpr uint64_t DES_Enc_Block(uint64_t data, const std::array<uint64_t, 16>& SubKeys) {
		uint64_t out{};
		uint32_t Left{}, Right{}, temp{};
		
		out = InitPerm(data);
		Right = out;
		Left = out >> 32;

		for (int i = 0; i < 16; i++)
		{
			temp = Right;
			Right = Left ^ DES_F(Right, SubKeys[i]);
			Left = temp;
		}
		out = Right;
		out = (out << 32) | Left;
		return InvInitPerm(out);
	}

	constexpr uint64_t DES_Dec_Block(uint64_t data, const std::array<uint64_t, 16>& SubKeys) {
		uint64_t out{};
		uint32_t Left{}, Right{}, temp{};

		out = InitPerm(data);
		Right = out;
		Left = out >> 32;

		for (int i = 15; i >= 0; i--)
		{
			temp = Right;
			Right = Left ^ DES_F(Right, SubKeys[i]);
			Left = temp;
		}
		out = Right;
		out = (out << 32) | Left;
		return InvInitPerm(out);
	}

	constexpr uint64_t DES_Enc_Block(uint64_t data, uint64_t key) {
		return DES_Enc_Block(data, KeyGenerator(key));
	}
	constexpr uint64_t DES_Dec_Block(uint64_t data, uint64_t key) {
		return DES_Dec_Block(data, KeyGenerator(key));
	}
}

#endif
