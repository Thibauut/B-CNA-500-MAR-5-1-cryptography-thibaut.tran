#include <iostream>
#include <gmpxx.h>

mpz_class mod_pow(mpz_class base, mpz_class exp, mpz_class mod) {
    mpz_class result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

mpz_class mod_inverse(mpz_class a, mpz_class m) {
    mpz_class m0 = m, t, q;
    mpz_class x0 = 0, x1 = 1;

    if (m == 1) {
        return 0;
    }

    while (a > 1) {
        q = a / m;

        t = m;
        m = a % m;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    while (x1 < 0) {
        x1 += m0;
    }

    return x1;
}


int main() {
    std::string a1 = "d3";
    std::string a2 = "e3";
    // std::string a1 = "4b1da73924978f2e9c1f04170e46820d648edbee12ccf4d4462af89b080c86e1";
    // std::string a2 = "bb3ca1e126f7c8751bd81bc8daa226494efb3d128f72ed9f6cacbe96e14166cb";

    mpz_class decimal;
    mpz_set_str(decimal.get_mpz_t(), a1.c_str(), 16);

    mpz_class decimal2;
    mpz_set_str(decimal2.get_mpz_t(), a2.c_str(), 16);

    mpz_class p = decimal;
    mpz_class q = decimal2;
    // std::cout << "p: " << p << std::endl;
    // std::cout << "q: " << q << std::endl;

    mpz_class n = p * q;

    mpz_class totient = (p - 1) * (q - 1);
    mpz_class e = 65537;
    mpz_class d = mod_inverse(e, totient);

    std::string e_str = e.get_str(16);
    std::string n_str = n.get_str(16);
    std::string d_str = d.get_str(16);


    std::string test1 = "bb";
    std::string test2 = "19";

    mpz_class test;
    mpz_set_str(test.get_mpz_t(), test1.c_str(), 16);

    mpz_class test3;
    mpz_set_str(test3.get_mpz_t(), test2.c_str(), 16);

    std::cout << "test: " << mpz_sizeinbase(test.get_mpz_t(), 2) << " bits" << std::endl;
    std::cout << "test3: " << mpz_sizeinbase(test3.get_mpz_t(), 2) << " bits" << std::endl;


    std::cout << "Public Key: " << e_str << "-" << n_str << std::endl;
    std::cout << "Private Key: " << d_str << "-" << n_str << std::endl;

    return 0;
}
