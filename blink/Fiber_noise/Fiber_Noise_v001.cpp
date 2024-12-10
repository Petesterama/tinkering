# define rand_modulus 2147483647
# define MAX_DISTANCE_ARRAY 12

// Static functions for random number generation
static int lcgRandom(int seed) {
    // Constants for random number generation
    const uint RAND_MULTIPLIER = 48271;
    const uint RAND_INCREMENT = 0;
    const uint RAND_MODULUS = 2147483647;

    int result = (seed * RAND_MULTIPLIER + RAND_INCREMENT) % RAND_MODULUS;
    return (result < 0) ? -result : result;
}

// another random function
inline float fract (float x) {return x-floor(x);}
inline float random(float co) { return fract(sin(co*(91.3458f)) * 47453.5453f); }

// Static function for FNV hash
static uint hash(uint i, uint j, uint k) {
    // Constants for FNV hash
    const uint FNV_PRIME = 16777619;
    const uint OFFSET_BASIS = 2166136261;
    return ((((((OFFSET_BASIS ^ i) * FNV_PRIME) ^ j) * FNV_PRIME) ^ k) * FNV_PRIME);
}



// Static function for probability lookup
static uint probLookup(uint value) {
    // Constants for Poisson distribution
    const uint one = 393325350;
    const uint two = 1022645910;
    const uint three = 1861739990;
    const uint four = 2700834071;
    const uint five = 3372109335;
    const uint six = 3819626178;
    const uint seven = 4075350088;
    const uint eight = 4203212043;
    if (value < one) return 1;
    if (value < two) return 2;
    if (value < three) return 3;
    if (value < four) return 4;
    if (value < five) return 5;
    if (value < six) return 6;
    if (value < seven) return 7;
    if (value < eight) return 8;
    return 9;
}

// Distance Function
static float ChebyshevDistanceFunc(float3 p1, float3 p2)
{
  float3 diff = p1 - p2;
  return max(max(fabs(diff.x), fabs(diff.y)), fabs(diff.z));
}

// Insertion Array
static void insert(float arr[], float value)
{
  float temp;
  for (int i = MAX_DISTANCE_ARRAY - 1; i >= 0; i--)
  {
    if (value > arr[i])
      break;
    temp = arr[i];
    arr[i] = value;
    if (i + 1 < MAX_DISTANCE_ARRAY)
    {
      arr[i + 1] = temp;
    }
  }
}

// Noise generation function
static float generateNoise(float3 input, int iRange, float fRange) {
    //Declare some values for later use
    int lastRandom, numberFeaturePoints;
    float3 randomDiff, featurePoint;
    int cubeX, cubeY, cubeZ;
    float distanceArray[MAX_DISTANCE_ARRAY];
    // Initialize values in distance array to large values
    for (int i = 0; i < MAX_DISTANCE_ARRAY; i++)
        distanceArray[i] = 6666;
    int evalCubeX = floor(input.x);
    int evalCubeY = floor(input.y);
    int evalCubeZ = floor(input.z);
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            for (int k = -1; k < 2; ++k) {
                cubeX = evalCubeX + i;
                cubeY = evalCubeY + j;
                cubeZ = evalCubeZ + k;

                // Generate a reproducible random number generator for the cube
                lastRandom = lcgRandom(hash(cubeX, cubeY, cubeZ));
                // Determine how many feature points are in the cube
                numberFeaturePoints = probLookup(lastRandom);
                // Check each feature point
                for (uint l = 0; l < numberFeaturePoints; ++l) {
                    lastRandom = lcgRandom(lastRandom);
                    randomDiff.x = float(lastRandom) / rand_modulus;

                    lastRandom = lcgRandom(lastRandom);
                    randomDiff.y = float(lastRandom) / rand_modulus;

                    lastRandom = lcgRandom(lastRandom);
                    randomDiff.z = float(lastRandom) / rand_modulus;

                    featurePoint = float3(randomDiff.x + float(cubeX), randomDiff.y + float(cubeY), randomDiff.z + float(cubeZ));

                    insert(distanceArray, ChebyshevDistanceFunc(input, featurePoint));
                }
            }
        }
    }

    float color = (distanceArray[iRange + 2] - distanceArray[iRange + 1]) * fmod(fRange, 1.0f) + distanceArray[iRange + 1] - distanceArray[0];
    return color;
}

// Define a function to rotate the gradient
float3 rotateGradient(float3 input, float angle) {
    // Convert angle from degrees to radians
    float radians = angle * (3.14159265359 / 180.0f);

    // Create the rotation matrix
    float cosAngle = cos(radians);
    float sinAngle = sin(radians);

    float3 rotatedInput;
    rotatedInput.x = input.x * cosAngle - input.y * sinAngle;
    rotatedInput.y = input.x * sinAngle + input.y * cosAngle;
    rotatedInput.z = input.z;  // Preserve the z component if needed

    return rotatedInput;
}

// Blink Kernel
kernel ChebyshevNoise : ImageComputationKernel<ePixelWise>
{
    Image<eWrite> dst;

    param:
        float range;
        float _spread;
        float _rotation;
        float _size;
        int _iterations;
        

    local:
        float fRange;
        int iRange;


    void define() {
        defineParam(range, "Range", 3.0f);
    }

    void init() {
        fRange = clamp(range, 0.0f, float(MAX_DISTANCE_ARRAY - 2));
        iRange = int(fRange);
	
    }

    void process(int2 pos) {

        float width = dst.bounds.x2;
        float height = dst.bounds.y2;
        float3 input = float3(pos.x, pos.y, 0)/_size;

        // Loop over generateNoise() 20 times and accumulate the results
        float accumulatedColor = 0.0f;
        float3 rotated_input;
        float angle = 0.0;
        for (int i = 0; i < _iterations; ++i) {
              angle = (i * _spread/float(_iterations)) + _rotation;
              rotated_input = rotateGradient(input, angle) * float3(100,1,1);
              accumulatedColor = max(accumulatedColor, generateNoise(rotated_input, iRange, fRange));
        }

        dst() = accumulatedColor;
        //dst() = pos.x/width;
    }
};