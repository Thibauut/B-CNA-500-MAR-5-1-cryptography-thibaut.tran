/*
** EPITECH PROJECT, 2023
** B-CNA-500-MAR-5-1-cryptography-thibaut.tran
** File description:
** aes.cpp
*/

#include <iostream>
#include <string>
#include <getopt.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "../include/struct.h"

unsigned char expandedKeys[176];

void KeyExpansionCore(unsigned char *in, unsigned char i) {
    unsigned char t = in[0];

    in[0] = in[1];
    in[1] = in[2];
    in[2] = in[3];
    in[3] = t;

    in[0] = s_box[in[0]];
    in[1] = s_box[in[1]];
    in[2] = s_box[in[2]];
    in[3] = s_box[in[3]];

    in[0] ^= rcon[i];
}

void keyExpansion(const unsigned char* inputKey, unsigned char* expandedKeys) {
    for (int i = 0; i < 16; i++)
        expandedKeys[i] = inputKey[i];
    int bytesGenerated = 16;
    int rconCounter = 1;
    unsigned char temp[4];
    while (bytesGenerated < 176) {
        for (int i = 0; i < 4; i++)
            temp[i] = expandedKeys[i + bytesGenerated - 4];
        if (bytesGenerated % 16 == 0) {
            KeyExpansionCore(temp, rconCounter);
            rconCounter++;
        }
        for (unsigned char a = 0; a < 4; a++) {
            expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - 16] ^ temp[a];
            bytesGenerated++;
        }
    }
}

void mixColumns(unsigned char* state) {
	unsigned char tmp[16];

	tmp[0] = (unsigned char)(mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3]);
	tmp[1] = (unsigned char)(state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3]);
	tmp[2] = (unsigned char)(state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]]);
	tmp[3] = (unsigned char)(mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]]);

	tmp[4] = (unsigned char)(mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7]);
	tmp[5] = (unsigned char)(state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7]);
	tmp[6] = (unsigned char)(state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]]);
	tmp[7] = (unsigned char)(mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]]);

	tmp[8] = (unsigned char)(mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11]);
	tmp[9] = (unsigned char)(state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11]);
	tmp[10] = (unsigned char)(state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]]);
	tmp[11] = (unsigned char)(mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]]);

	tmp[12] = (unsigned char)(mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15]);
	tmp[13] = (unsigned char)(state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15]);
	tmp[14] = (unsigned char)(state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]]);
	tmp[15] = (unsigned char)(mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]]);

	for (int i = 0; i< 16; i++)
		state[i] = tmp[i];
}

void subBytes(unsigned char* state) {
    for (int i = 0; i < 16; ++i)
        state[i] = s_box[state[i]];
}

void shiftRows(unsigned char* state) {
	unsigned char tmp[16];
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];

	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];

	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i< 16; i++)
		state[i] = tmp[i];
}

void addRoundKey(unsigned char* state, const unsigned char* roundKey) {
	for (int i = 0; i< 16; i++) {
		state[i] ^= roundKey[i];
	}
}

void aesEncrypt(unsigned char* message, unsigned char* key, unsigned char* cipher) {
    unsigned char state[16];
    for (int i = 0; i < 16; i++)
        state[i] = message[i];
    int numberOfRounds = 9;
    keyExpansion(key, expandedKeys);
    addRoundKey(state, expandedKeys);
    for (int i = 0; i < numberOfRounds; i++) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, expandedKeys + (16 * (i + 1)));
    }
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, expandedKeys + 160);
    for (int i = 0; i < 16; i++)
        cipher[i] = state[i];
}

void invMixColumns(unsigned char* state) {
    unsigned char tmp[16];

    tmp[0] = (unsigned char)(mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]]);
    tmp[1] = (unsigned char)(mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]]);
    tmp[2] = (unsigned char)(mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]]);
    tmp[3] = (unsigned char)(mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]]);

    tmp[4] = (unsigned char)(mul14[state[4]] ^ mul11[state[5]] ^ mul13[state[6]] ^ mul9[state[7]]);
    tmp[5] = (unsigned char)(mul9[state[4]] ^ mul14[state[5]] ^ mul11[state[6]] ^ mul13[state[7]]);
    tmp[6] = (unsigned char)(mul13[state[4]] ^ mul9[state[5]] ^ mul14[state[6]] ^ mul11[state[7]]);
    tmp[7] = (unsigned char)(mul11[state[4]] ^ mul13[state[5]] ^ mul9[state[6]] ^ mul14[state[7]]);

    tmp[8] = (unsigned char)(mul14[state[8]] ^ mul11[state[9]] ^ mul13[state[10]] ^ mul9[state[11]]);
    tmp[9] = (unsigned char)(mul9[state[8]] ^ mul14[state[9]] ^ mul11[state[10]] ^ mul13[state[11]]);
    tmp[10] = (unsigned char)(mul13[state[8]] ^ mul9[state[9]] ^ mul14[state[10]] ^ mul11[state[11]]);
    tmp[11] = (unsigned char)(mul11[state[8]] ^ mul13[state[9]] ^ mul9[state[10]] ^ mul14[state[11]]);

    tmp[12] = (unsigned char)(mul14[state[12]] ^ mul11[state[13]] ^ mul13[state[14]] ^ mul9[state[15]]);
    tmp[13] = (unsigned char)(mul9[state[12]] ^ mul14[state[13]] ^ mul11[state[14]] ^ mul13[state[15]]);
    tmp[14] = (unsigned char)(mul13[state[12]] ^ mul9[state[13]] ^ mul14[state[14]] ^ mul11[state[15]]);
    tmp[15] = (unsigned char)(mul11[state[12]] ^ mul13[state[13]] ^ mul9[state[14]] ^ mul14[state[15]]);

    for (int i = 0; i < 16; i++)
        state[i] = tmp[i];
}

void invSubBytes(unsigned char* state) {
    for (int i = 0; i < 16; ++i)
        state[i] = inv_s_box[state[i]];
}

void invShiftRows(unsigned char* state) {
    unsigned char tmp[16];

    tmp[0] = state[0];
    tmp[1] = state[13];
    tmp[2] = state[10];
    tmp[3] = state[7];

    tmp[4] = state[4];
    tmp[5] = state[1];
    tmp[6] = state[14];
    tmp[7] = state[11];

    tmp[8] = state[8];
    tmp[9] = state[5];
    tmp[10] = state[2];
    tmp[11] = state[15];

    tmp[12] = state[12];
    tmp[13] = state[9];
    tmp[14] = state[6];
    tmp[15] = state[3];

    for (int i = 0; i < 16; i++)
        state[i] = tmp[i];
}

void aesDecrypt(unsigned char* cipher, unsigned char* key, unsigned char* decrypted) {
    unsigned char state[16];
    for (int i = 0; i < 16; i++)
        state[i] = cipher[i];

    int numberOfRounds = 9;
    keyExpansion(key, expandedKeys);

    addRoundKey(state, expandedKeys + 160);

    for (int i = numberOfRounds - 1; i >= 0; i--) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, expandedKeys + (16 * (i + 1)));
        invMixColumns(state);
    }

    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, expandedKeys);

    for (int i = 0; i < 16; i++)
        decrypted[i] = state[i];
}

std::string calcAes(Arguments args, std::string message) {
    std::string result;
    unsigned char messageBytes[16];
    unsigned char keyBytes[16];
    unsigned char ciphered[16];

    // Convert hex string to bytes
    for (int i = 0; i < 16; ++i) {
        sscanf(message.c_str() + 2 * i, "%2hhx", &messageBytes[i]);
        sscanf(args.key.c_str() + 2 * i, "%2hhx", &keyBytes[i]);
    }

    if (args.generate == true) {
        // Encrypt
        aesEncrypt(messageBytes, keyBytes, ciphered);

        std::ostringstream oss;
        for (int i = 0; i < 16; ++i)
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ciphered[i]);
        result = oss.str();
        return result;
    }
    if (args.generate == false) {
        // Decrypt
        aesDecrypt(messageBytes, keyBytes, ciphered);

        std::ostringstream oss;
        for (int i = 0; i < 16; ++i)
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ciphered[i]);
        result = oss.str();
        return result;
    }
    return result;
}