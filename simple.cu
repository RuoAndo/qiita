#include <cuda_runtime.h>
#include <stdio.h>

void validateResult(float *hostRef, float *gpuRef, const int N)
{
    double epsilon = 1.0E-8;
    bool match = 1;

    for (int i = 0; i < N; i++)
    {
        if (abs(hostRef[i] - gpuRef[i]) > epsilon)
        {
            match = 0;
            printf("Arrays do not match!\n");
            printf("host %5.2f gpu %5.2f at current %d\n", hostRef[i], gpuRef[i], i);
            break;
        }
    }

    if (match) printf("Arrays match.\n\n");

    return;
}

void genData(float *ip, int size)
{
    time_t t;
    srand((unsigned) time(&t));

    for (int i = 0; i < size; i++)
    {
        ip[i] = (float)( rand() & 0xFF ) / 10.0f;
    }

    return;
}

void sumArraysOnHost(float *A, float *B, float *C, const int N)
{
    for (int idx = 0; idx < N; idx++)
    {
        C[idx] = A[idx] + B[idx];
    }
}
__global__ void sumArraysOnGPU(float *A, float *B, float *C, const int N)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < N) C[i] = A[i] + B[i];
}

int main(int argc, char **argv)
{
    struct timespec startTime, endTime, sleepTime;

    printf("%s Starting...\n", argv[0]);

    int dev = 0;
    cudaDeviceProp deviceProp;
    cudaGetDeviceProperties(&deviceProp, dev);
    printf("Using Device %d: %s\n", dev, deviceProp.name);
    cudaSetDevice(dev);

    int nElem = 1 << 28;
    printf("Vector size %d\n", nElem);

    size_t nBytes = nElem * sizeof(float);

    float *host_A, *host_B, *hostRef, *gpuRef;
    host_A     = (float *)malloc(nBytes);
    host_B     = (float *)malloc(nBytes);
    hostRef = (float *)malloc(nBytes);
    gpuRef  = (float *)malloc(nBytes);

    genData(host_A, nElem);
    genData(host_B, nElem);

    memset(hostRef, 0, nBytes);
    memset(gpuRef,  0, nBytes);

    clock_gettime(CLOCK_REALTIME, &startTime);
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 123;

    sumArraysOnHost(host_A, host_B, hostRef, nElem);

    clock_gettime(CLOCK_REALTIME, &endTime);
    if (endTime.tv_nsec < startTime.tv_nsec) {
	printf("%ld.%09ld", endTime.tv_sec - startTime.tv_sec - 1
	       ,endTime.tv_nsec + 1000000000 - startTime.tv_nsec);
    } else {
	printf("%ld.%09ld", endTime.tv_sec - startTime.tv_sec
	       ,endTime.tv_nsec - startTime.tv_nsec);
    }
    printf(" sec\n");

    float *device_A, *device_B, *device_C;
    cudaMalloc((float**)&device_A, nBytes);
    cudaMalloc((float**)&device_B, nBytes);
    cudaMalloc((float**)&device_C, nBytes);

    cudaMemcpy(device_A, host_A, nBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(device_B, host_B, nBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(device_C, gpuRef, nBytes, cudaMemcpyHostToDevice);

    int iLen = 1024;
    dim3 block (iLen);
    dim3 grid  ((nElem + block.x - 1) / block.x);

    clock_gettime(CLOCK_REALTIME, &startTime);
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 123;

    sumArraysOnGPU<<<grid, block>>>(device_A, device_B, device_C, nElem);
    cudaDeviceSynchronize();
    cudaGetLastError() ;

    clock_gettime(CLOCK_REALTIME, &endTime);
    if (endTime.tv_nsec < startTime.tv_nsec) {
	printf("%ld.%09ld", endTime.tv_sec - startTime.tv_sec - 1
	       ,endTime.tv_nsec + 1000000000 - startTime.tv_nsec);
    } else {
	printf("%ld.%09ld", endTime.tv_sec - startTime.tv_sec
	       ,endTime.tv_nsec - startTime.tv_nsec);
    }
    printf(" sec\n");

    cudaMemcpy(gpuRef, device_C, nBytes, cudaMemcpyDeviceToHost);

    validateResult(hostRef, gpuRef, nElem);

    cudaFree(device_A);
    cudaFree(device_B);
    cudaFree(device_C);

    free(host_A);
    free(host_B);
    free(hostRef);
    free(gpuRef);

    return(0);
}
