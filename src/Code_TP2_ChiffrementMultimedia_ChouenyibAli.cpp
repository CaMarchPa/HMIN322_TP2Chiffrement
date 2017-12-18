#include "CImg.h"
#include <iostream>
#include <cstdlib>
#include <bitset>
#include <vector>


using namespace cimg_library;


bool is_prime(int number);
int pgcd (int a, int b);
bool relativly_prime(int a, int b);
std::vector<int> possible_exponent_of_e(int  phi_n);
CImg<int> rsa_algorithm(CImg<int> img, int n, int e);
CImg<int> binarizeIMG(CImg<int> img);
double entropie (CImg<int> img);


int puissance(int x, int n);
int power_mod(int x, int e, int n);

int main(int argc, char const *argv[]) {
    CImg<> img_read;
    // CImg<> img_read2;
    if (argc != 4) {
        printf("ERROR : arguments not complete\n");
        return 1;
    }
    
    img_read.load(argv[1]);

    int number = atoi(argv[2]);
    int number2 = atoi(argv[3]);

    if (is_prime(number))
        std::cout << number << " est un nombre premier" << '\n';
    else
        std::cout << number << " n'est pas un nombre premier" << '\n';

    if (relativly_prime(number, number2))
        std::cout << number2 << " & " << number << " sont premiers entre eux " << '\n';
    else 
        std::cout << number2 << " & " << number << " ne sont pas premiers entre eux " << '\n';

    int phi_n = (number2 - 1) * (number2 - 1);
    int n = number2 * number;

    std::vector<int> v = possible_exponent_of_e(phi_n);

    std::cout << " phi(n) = " << phi_n << " et n = " << n <<'\n';
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << ", ";
    }
    std::cout << '\n';

    // on prend e = 17
    CImg<int> result = rsa_algorithm(img_read, n, 17);
    result.save_png("../img/rsa_result.png");
    img_read.save_png("../img/original.png");
    // std::cout << " puissance(3, 17, 253) : " << puissance(3, 17) % 253 << " et power_mod(3, 17, 253) : " << power_mod(3, 17, 253) << '\n';

    double entrop = entropie (result);
    std::cout << " L'entropie de l'image chiffrée est de : " << entrop << '\n';

    CImg<int> binarized = binarizeIMG(img_read);
    CImg<int> result_of_binarized = rsa_algorithm(binarized, n, 17);
    binarized.save_png("../img/binarized.png");
    result_of_binarized.save_png("../img/result_of_binarized.png");

    return 0;
}


bool is_prime(int number) {
    int half = number / 2;
    for (int i = 2; i < half; i++) {
        if (number % i == 0 && i != number) 
            return false;
    }
    return true;
}

int pgcd(int a, int b) {
    int t;
    while(b != 0){
        t = a;
        a = b;
        b = t%b;
    }
    return a;
}

bool relativly_prime(int a, int b) {
    return pgcd(a, b) == 1;
}

std::vector<int> possible_exponent_of_e(int phi_n) {
    std::vector<int> vect;

    for (int i = 1; i < phi_n; ++i) {
        if (pgcd(i, phi_n) == 1)
            vect.push_back(i);
    }
    return vect;
}

CImg<int> rsa_algorithm(CImg<int> img, int n, int e) {
    int width = img.width();
    int height = img.height();
    CImg<int> result(width, height, 1, 1, 0);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            result(i, j) = power_mod(img(i, j), e, n);
        }
    }

    return result;
}

int puissance (int x, int n) {
    int temp;
    if( n == 0)
        return 1;
    temp = puissance(x, n/2);
    if (n%2 == 0)
        return temp*temp;
    else
        return x*temp*temp;
}

int power_mod(int x, int e, int n) {

    int val = (x * x) % n;

    for (int i = 3; i <= e; i++) {
        val = (val * x) % n;
    }

    return val;
}

CImg<int> binarizeIMG(CImg<int> img) {
    int width = img.width();
    int height = img.height();

    CImg<int> result(width, height, 1, 1, 0);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (img(i, j) >= 128 )
                result(i, j) = 172;
        }
    } 

    return result;
}

double entropie (CImg<int> img) {
    float H = 0;
    std::vector<int> occurence(255, 0);

    int width = img.width();
    int height = img.height();

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int pixel = (int)img(i, j);
            if (pixel > 255 ) pixel = 255;
            if (pixel < 0) pixel = 0;
            // std::cout << pixel << ",";
            occurence[pixel] ++;
        }
    }

    for (int i = 0; i < occurence.size(); ++i) {
        double P = (double)occurence[i] / 255.0;
        std::cout << P << ", ";
        double logarithm = (P == 0) ? 0.0 : log2(P);
        H += (P * logarithm);
    }

    return H * (-1);
}