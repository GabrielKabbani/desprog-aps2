#include <math.h>
#include <complex.h> 
#include "fourier.h"

void nft(double complex s[MAX_SIZE], double complex t[MAX_SIZE], int n, int sign) {
    for (int k = 0; k < n; k++) {
        t[k] = 0;

        for (int j = 0; j < n; j++) {
            t[k] += s[j] * cexp(sign * 2 * PI * k * j * I / n);
        }
    }
}

void nft_forward(double complex s[MAX_SIZE], double complex t[MAX_SIZE], int n) {
    nft(s, t, n, -1);
}

void nft_inverse(double complex t[MAX_SIZE], double complex s[MAX_SIZE], int n) {
    nft(t, s, n, 1);

    for (int k = 0; k < n; k++) {
        s[k] /= n;
    }
}

void fft(double complex s[MAX_SIZE], double complex t[MAX_SIZE], int n, int sign) {
    int cp=0;
    int ci=0;

    double complex sp[n/2];
    double complex si[n/2];
    double complex tp[n/2];
    double complex ti[n/2];
    
    if(n==1){
        t[0] = s[0];
        return;
    }
    for (int i=0; i<n; i++){
        if (i%2<0.000001){
            sp[cp]=s[i];
            cp++;
        }else{
            si[ci]=s[i];
            ci++;
        }
    }

    fft(sp, tp, (n/2), sign);
    fft(si, ti, (n/2), sign);

    for (int k=0; k<n/2; k++){
        t[k]=tp[k] + ti[k] * (cexp(sign*2*PI*k*I/n));
    }
    for (int k=0; k<n/2; k++){
        t[k + n/2]= tp[k] - ti[k] * (cexp(sign*2*PI*k*I/n));
    }  

}

void fft_forward(double complex s[MAX_SIZE], double complex t[MAX_SIZE], int n) {
    fft(s, t, n, -1);
}

void fft_inverse(double complex t[MAX_SIZE], double complex s[MAX_SIZE], int n) {
    fft(t, s, n, 1);

    for (int k = 0; k < n; k++) {
        s[k] /= n;
    }
}

void fft_forward_2d(double complex matrix[MAX_SIZE][MAX_SIZE], int width, int height) {
    for (int i=0; i<height; i++){
        double complex vetor[width];
        for (int j=0; j<width; j++){
            vetor[j]=matrix[i][j];
        }
        double complex t[width];
        fft_forward(vetor, t, width);
        for (int j=0; j<width; j++){
            matrix[i][j]=t[j];
        }
    }
    for (int i=0; i<width; i++){
        double complex vetor[height];
        for (int j=0; j<height; j++){
            vetor[j]=matrix[j][i];
        }
        double complex t[height];
        fft_forward(vetor, t, height);
        for (int j=0; j<height; j++){
            matrix[j][i]=t[j];
        }
    
    }
}

void fft_inverse_2d(double complex matrix[MAX_SIZE][MAX_SIZE], int width, int height) {
    for (int i=0; i<height; i++){
        double complex vetor[width];
        for (int j=0; j<width; j++){
            vetor[j]=matrix[i][j];
        }
        double complex t[width];
        fft_inverse(vetor, t, width);
        for (int j=0; j<width; j++){
            matrix[i][j]=t[j];
        }
    }
    for (int i=0; i<width; i++){
        
        double complex vetor[height];
        for (int j=0; j<height; j++){
            vetor[j]=matrix[j][i];
        }
        double complex t[height];
        fft_inverse(vetor, t, height);
        for (int j=0; j<height; j++){
            matrix[j][i]=t[j];
        }
     
    }
}



void filter(double complex input[MAX_SIZE][MAX_SIZE], double complex output[MAX_SIZE][MAX_SIZE], int width, int height, int flip) {
    int center_x = width / 2;
    int center_y = height / 2;

    double variance = -2 * SIGMA * SIGMA;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int dx = center_x - (x + center_x) % width;
            int dy = center_y - (y + center_y) % height;

            double d = dx * dx + dy * dy;

            double g = exp(d / variance);

            if (flip) {
                g = 1 - g;
            }

            output[y][x] = g * input[y][x];
        }
    }
}

void filter_lp(double complex input[MAX_SIZE][MAX_SIZE], double complex output[MAX_SIZE][MAX_SIZE], int width, int height) {
    filter(input, output, width, height, 0);
}

void filter_hp(double complex input[MAX_SIZE][MAX_SIZE], double complex output[MAX_SIZE][MAX_SIZE], int width, int height) {
    filter(input, output, width, height, 1);
}
