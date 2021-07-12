/*
c program to test the fft software fftw
*/

#include <fftw3.h>
#include <math.h>

fftw_complex *generate_input_data(int N, fftw_complex *data)
{
    /* generate the input data for the test 
    @param: int N is length of the array
    */
    for (int i = 0; i < N; i++)
    {
        double tmp = sin(i);
        printf("%d", i);
        printf(" -> %f\n", tmp);
        data[i][0] = tmp; // assign sin(i) to the real part of data
    }
    return data;
}

void print_data(fftw_complex *data, int N)
{
    for (int i = 0; i < N; i++)
    {
        printf("%f + %f i\n", data[i][0], data[i][1]);
    }
}
int main(void)
{
    int N = 16; // length of sequences
    fftw_complex *in, *out;

    printf("Creating the data\n");
    in = fftw_alloc_complex(sizeof(fftw_complex) * N);
    generate_input_data(N, in);

    printf("\nPrinting the Input data\n");
    print_data(in, N);

}