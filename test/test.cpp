#include <iostream>
#include <gmpxx.h>
#include <string>
#include <sstream>

// Function for RSA encryption
mpz_class rsa_encrypt(const mpz_class& plaintext, const mpz_class& public_key_n, const mpz_class& public_key_e) {
    mpz_class ciphertext;
    mpz_powm(ciphertext.get_mpz_t(), plaintext.get_mpz_t(), public_key_e.get_mpz_t(), public_key_n.get_mpz_t());
    return ciphertext;
}

// Function for RSA decryption
mpz_class rsa_decrypt(mpz_class& ciphertext, mpz_class& public_key_n, mpz_class& private_key_d) {
    mpz_class plaintext;
    mpz_powm(plaintext.get_mpz_t(), ciphertext.get_mpz_t(), private_key_d.get_mpz_t(), public_key_n.get_mpz_t());
    return plaintext;
}

bool isLittleEndian(const mpz_class& number) {
    size_t sizeInBytes = mpz_sizeinbase(number.get_mpz_t(), 256);
    unsigned char* buffer = new unsigned char[sizeInBytes];
    mpz_export(buffer, nullptr, -1, 1, 0, 0, number.get_mpz_t());
    bool littleEndian = (buffer[0] == buffer[sizeInBytes - 1]);
    delete[] buffer;
    return littleEndian;
}

mpz_class convertLittleEndianToBigEndian(const mpz_class& littleEndianNumber) {
    size_t sizeInBytes = mpz_sizeinbase(littleEndianNumber.get_mpz_t(), 256);
    unsigned char* bigEndianBuffer = new unsigned char[sizeInBytes];
    mpz_export(bigEndianBuffer, nullptr, -1, 1, 0, 0, littleEndianNumber.get_mpz_t());
    std::reverse(bigEndianBuffer, bigEndianBuffer + sizeInBytes);
    mpz_class bigEndianNumber;
    mpz_import(bigEndianNumber.get_mpz_t(), sizeInBytes, -1, 1, 0, 0, bigEndianBuffer);
    delete[] bigEndianBuffer;
    return bigEndianNumber;
}


int main() {
    // std::string a1 = "d3";
    // std::string a2 = "e3";
    std::string a1 = "E1860C089BF82A46D4F4CC12EEDB8E640D82460E17041F9C2E8F972439A71D4B";
    std::string a2 = "CB6641E196BEAC6C9FED728F123DFB4E4926A2DAC81BD81B75C8F726E1A13CBB";
    // std::string a1 = "4b1da73924978f2e9c1f04170e46820d648edbee12ccf4d4462af89b080c86e1";
    // std::string a2 = "bb3ca1e126f7c8751bd81bc8daa226494efb3d128f72ed9f6cacbe96e14166cb";

    mpz_class value1;
    mpz_set_str(value1.get_mpz_t(), a1.c_str(), 16);
    // value1 = convertLittleEndianToBigEndian(value1);

    mpz_class value2;
    mpz_set_str(value2.get_mpz_t(), a2.c_str(), 16);
    // value2 = convertLittleEndianToBigEndian(value2);

    mpz_class p = value1;
    mpz_class q = value2;
    mpz_class n = p * q;
    mpz_class d;
    mpz_class totient = (p - 1) * (q - 1);

    mpz_class e = 65537;
    if (mpz_sizeinbase(value1.get_mpz_t(), 2) == 8 || mpz_sizeinbase(value2.get_mpz_t(), 2) == 8)
        e = 257;
    if (mpz_sizeinbase(value1.get_mpz_t(), 2) == 256 || mpz_sizeinbase(value2.get_mpz_t(), 2) == 256)
        e = 65537;

    mpz_invert(d.get_mpz_t(), e.get_mpz_t(), totient.get_mpz_t());

    std::cout << "Public Key: " << std::hex << e << "-" << n << std::endl;
    std::cout << "Private Key: " << std::hex << d << "-" << n << std::endl;

    mpz_class plaintext;
    mpz_set_str(plaintext.get_mpz_t(), "c1c2c3", 16);

    // Encryption
    mpz_class ciphertext = rsa_encrypt(plaintext, n, e);
    std::cout << "Ciphertext: " << std::hex << ciphertext << std::endl;

    // Decryption
    mpz_class decrypted_text = rsa_decrypt(ciphertext, n, d);
    std::cout << "Decrypted Text: " << std::hex << decrypted_text << std::endl;  // Use std::hex here
    return 0;
}
