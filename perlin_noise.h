
#define OUTPUT_TYPE_NORMALIZED              0
#define OUTPUT_TYPE_NORMALIZED_POSITIVE     1


void fillMatrixWithPerlinNoise(float** matrix, int width, int height, int horizontalGridSize, int verticalGridSize, int outputType);

void fillMatrixWithFractalNoise(float** matrix, int width, int height, int horizontalStartGridSize, int verticalStartGridSize, int octaves);