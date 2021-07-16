/*
c program to test the fft software fftw
*/

#include <fftw3.h>
#include <math.h>

fftw_complex *generate_input_data(int N, fftw_complex *data)
{
    //  generate the input data for the test
    //  N: length of the array to return
    //  returns: a pointer to the fftw_complex array

    for (int i = 0; i < N; i++)
    {
        double tmp = sin(i) * 2;
        data[i][0] = tmp; // assign sin(i) to the real part of data
    }
    return data;
}

void print_data(fftw_complex *data, int N)
{
    // just prints the input fftw_complex struct to standard out

    // data: the data to print
    // N: the length of the data

    for (int i = 0; i < N; i++)
    {
        printf("%lf + %lf i\n", data[i][0], data[i][1]);
    }
}
int main(void)
{
    int N = 8; // length of sequences
    fftw_complex *in, *out;

    in = fftw_alloc_complex(sizeof(fftw_complex) * N);
    generate_input_data(N, in);

    out = fftw_alloc_complex((sizeof(fftw_complex) * N));
    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    printf("\nPrinting the input data\n");
    print_data(in, N);

    printf("\noutput before fft\n");
    print_data(out, N);

    fftw_execute(plan);

    printf("\noutput after fft\n");
    print_data(out, N);

    printf("\ninput after fft\n");
    print_data(in, N);

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
}
