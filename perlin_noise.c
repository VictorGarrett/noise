#include "perlin_noise.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


typedef struct _vec2f{
    float x;
    float y;
}vec2f;


void fillMatrixWithPerlinNoise(float** matrix, int width, int height, int horizontalGridSize, int verticalGridSize, int outputType){
    
    
    
    int latticeWidth = horizontalGridSize + 1;
    int latticeHeight = verticalGridSize + 1;

    int horizontalCellSize = width/horizontalGridSize;
    int verticalCellSize = height/verticalGridSize;



    vec2f** gradientLattice = (vec2f**)malloc((latticeWidth)*sizeof(vec2f*));
    for(int i = 0; i < latticeWidth; i++){
        gradientLattice[i] = (vec2f*)malloc(latticeHeight*sizeof(vec2f));
    }

    for(int i = 0; i < latticeWidth; i++)
        for(int j = 0; j < latticeHeight; j++){
            gradientLattice[i][j].x = ((float)rand()/RAND_MAX)*2 - 1; //value in range [-1, 1]
            gradientLattice[i][j].y = ((float)rand()/RAND_MAX)*2 - 1; //value in range [-1, 1]
            float norm = sqrt(gradientLattice[i][j].x*gradientLattice[i][j].x + gradientLattice[i][j].y*gradientLattice[i][j].y);//normalize
            gradientLattice[i][j].x /= norm;
            gradientLattice[i][j].y /= norm;
        }
    
    float max = 0;
    for(int i = 0; i < width; i++)
        for(int j = 0; j < height; j++){

            vec2f distances[4]; //distances from the point to the nearest lattice nodes. counterclockwise from upper right

            distances[0].x = (float)(i%horizontalCellSize - horizontalCellSize + 1)/(horizontalCellSize - 1);
            distances[0].y = (float)(j%verticalCellSize)/(verticalCellSize - 1);

            distances[1].x = (float)(i%horizontalCellSize)/(horizontalCellSize - 1);
            distances[1].y = distances[0].y;

            distances[2].x = (float)(i%horizontalCellSize)/(horizontalCellSize - 1);
            distances[2].y = (float)(j%verticalCellSize - verticalCellSize + 1)/(verticalCellSize - 1);

            distances[3].x = distances[0].x;
            distances[3].y = distances[2].y;

            float dotProducts[4] = {distances[0].x*gradientLattice[i/horizontalCellSize+1][j/verticalCellSize].x +
                                    distances[0].y*gradientLattice[i/horizontalCellSize+1][j/verticalCellSize].y,

                                    distances[1].x*gradientLattice[i/horizontalCellSize][j/verticalCellSize].x +
                                    distances[1].y*gradientLattice[i/horizontalCellSize][j/verticalCellSize].y,

                                    distances[2].x*gradientLattice[i/horizontalCellSize][j/verticalCellSize+1].x + 
                                    distances[2].y*gradientLattice[i/horizontalCellSize][j/verticalCellSize+1].y,

                                    distances[3].x*gradientLattice[i/horizontalCellSize+1][j/verticalCellSize+1].x +
                                    distances[3].y*gradientLattice[i/horizontalCellSize+1][j/verticalCellSize+1].y,
                                    };
            
            float upperInterp = (6*pow(distances[1].x, 5) - 15*pow(distances[1].x, 4)+10*pow(distances[1].x, 3))*(dotProducts[0]-dotProducts[1]) + dotProducts[1];
            float lowerInterp = (6*pow(distances[2].x, 5) - 15*pow(distances[2].x, 4)+10*pow(distances[2].x, 3))*(dotProducts[3]-dotProducts[2]) + dotProducts[2];
            matrix[i][j] = (((6*pow(distances[1].y, 5) - 15*pow(distances[1].y, 4)+10*pow(distances[1].y, 3))*(lowerInterp-upperInterp) + upperInterp));
            if(fabs(matrix[i][j]) > max){
                max = fabs(matrix[i][j]);
            }
        }
        for(int i = 0; i < latticeWidth; i++)
            free(gradientLattice[i]);
        free(gradientLattice);

        if(outputType == OUTPUT_TYPE_NORMALIZED){
            max = 1/max;
            for(int i = 0; i < width; i++)
                for(int j = 0; j < height; j++){
                    matrix[i][j] *= max;
                }
        }
        else if(outputType == OUTPUT_TYPE_NORMALIZED_POSITIVE){
            max = 1/max;
            for(int i = 0; i < width; i++)
                for(int j = 0; j < height; j++){
                    matrix[i][j] *= max;
                    matrix[i][j] += 1;
                    matrix[i][j] /= 2;
                }

        }

}

void fillMatrixWithFractalNoise(float** matrix, int width, int height, int horizontalStartGridSize, int verticalStartGridSize, int octaves){
    float max;
    fillMatrixWithPerlinNoise(matrix, width, height, horizontalStartGridSize, verticalStartGridSize, OUTPUT_TYPE_NORMALIZED);
    horizontalStartGridSize *= 2;
    verticalStartGridSize *= 2;
    octaves--;

    float amp = 0.5;

    float** auxMatrix = (float**)malloc(width*sizeof(float*));
    for(int i = 0; i < width; i++){
        auxMatrix[i] = (float*)malloc(height*sizeof(float));
    }

    while(octaves && horizontalStartGridSize < width && verticalStartGridSize < height){
        fillMatrixWithPerlinNoise(auxMatrix, width, height, horizontalStartGridSize, verticalStartGridSize, OUTPUT_TYPE_NORMALIZED);
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                matrix[i][j] += amp*auxMatrix[i][j];
                matrix[i][j] /= 1+amp;
            }
        }
        
        horizontalStartGridSize *= 2;
        verticalStartGridSize *= 2;
        octaves--;
        amp /= 2;
    }

    
    for(int i = 0; i < width; i++){
        free(auxMatrix[i]);
    }
    free(auxMatrix);
}