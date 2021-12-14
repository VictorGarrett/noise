#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <time.h>
#include "perlin_noise.h"

#define IMAGE_HEIGHT    2048
#define IMAGE_WIDTH     2048

int main(){

    srand(time(NULL));

    float** perlinNoiseMatrix = (float**)malloc(IMAGE_WIDTH*sizeof(float*));
    for(int i = 0; i < IMAGE_WIDTH; i++){
        perlinNoiseMatrix[i] = (float*)malloc(IMAGE_HEIGHT*sizeof(float));
    }

    unsigned char* noiseImage = (unsigned char*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(unsigned char));

    fillMatrixWithPerlinNoise(perlinNoiseMatrix, IMAGE_WIDTH, IMAGE_HEIGHT, 16, 16, OUTPUT_TYPE_NORMALIZED);

    for(int i = 0; i < IMAGE_WIDTH; i++)
        for(int j = 0; j < IMAGE_HEIGHT; j++){
            //printf("out %f\n", perlinNoiseMatrix[i][j]);
            noiseImage[IMAGE_WIDTH*i+j] = (perlinNoiseMatrix[i][j]+1)/2*255;
        }
    
    
    stbi_write_png("./perlin.png", IMAGE_WIDTH, IMAGE_HEIGHT, 1, noiseImage, 0);

    fillMatrixWithFractalNoise(perlinNoiseMatrix, IMAGE_WIDTH, IMAGE_HEIGHT, 8, 8, 3);


    for(int i = 0; i < IMAGE_WIDTH; i++)
        for(int j = 0; j < IMAGE_HEIGHT; j++){
            //printf("out %f\n", perlinNoiseMatrix[i][j]);
            noiseImage[IMAGE_WIDTH*i+j] = (perlinNoiseMatrix[i][j]+1)/2*255;
        }
    
    stbi_write_png("./fractal.png", IMAGE_WIDTH, IMAGE_HEIGHT, 1, noiseImage, 0);


    for(int i = 0; i < IMAGE_WIDTH; i++)
        free(perlinNoiseMatrix[i]);
    free(perlinNoiseMatrix);
    free(noiseImage);
    return 0;
}
