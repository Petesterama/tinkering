
int fastfloor( const float x ) { return x > 0 ? (int) x : (int) x - 1; }
inline float raw_noise_4d( const float x, const float y, const float z, const float w ) 
{
int simplex[64][4] = {
    {0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
    {0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
    {1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
    {2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}
};
    int perm[512] = {
        151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
        8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
        35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
        134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
        55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
        18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
        250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
        189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
        172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
        228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
        107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,

        151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
        8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
        35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
        134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
        55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
        18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
        250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
        189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
        172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
        228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
        107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };
   int grad4[32][4]= {
    {0,1,1,1},  {0,1,1,-1},  {0,1,-1,1},  {0,1,-1,-1},
    {0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
    {1,0,1,1},  {1,0,1,-1},  {1,0,-1,1},  {1,0,-1,-1},
    {-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
    {1,1,0,1},  {1,1,0,-1},  {1,-1,0,1},  {1,-1,0,-1},
    {-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
    {1,1,1,0},  {1,1,-1,0},  {1,-1,1,0},  {1,-1,-1,0},
    {-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}
   };
    float F4 = (sqrt(5.0f)-1.0)/4.0;
    float G4 = (5.0-sqrt(5.0f))/20.0;
    float n0, n1, n2, n3, n4;
    float s = (x + y + z + w) * F4;
    int i = fastfloor(x + s);
    int j = fastfloor(y + s);
    int k = fastfloor(z + s);
    int l = fastfloor(w + s);
    float t = (i + j + k + l) * G4;
    float X0 = i - t;
    float Y0 = j - t;
    float Z0 = k - t;
    float W0 = l - t;

    float x0 = x - X0;
    float y0 = y - Y0;
    float z0 = z - Z0;
    float w0 = w - W0;
    int c1 = (x0 > y0) ? 32 : 0;
    int c2 = (x0 > z0) ? 16 : 0;
    int c3 = (y0 > z0) ? 8 : 0;
    int c4 = (x0 > w0) ? 4 : 0;
    int c5 = (y0 > w0) ? 2 : 0;
    int c6 = (z0 > w0) ? 1 : 0;
    int c = c1 + c2 + c3 + c4 + c5 + c6;

    int i1, j1, k1, l1;
    int i2, j2, k2, l2;
    int i3, j3, k3, l3;
    i1 = simplex[c][0]>=3 ? 1 : 0;
    j1 = simplex[c][1]>=3 ? 1 : 0;
    k1 = simplex[c][2]>=3 ? 1 : 0;
    l1 = simplex[c][3]>=3 ? 1 : 0;
    i2 = simplex[c][0]>=2 ? 1 : 0;
    j2 = simplex[c][1]>=2 ? 1 : 0;
    k2 = simplex[c][2]>=2 ? 1 : 0;
    l2 = simplex[c][3]>=2 ? 1 : 0;
    i3 = simplex[c][0]>=1 ? 1 : 0;
    j3 = simplex[c][1]>=1 ? 1 : 0;
    k3 = simplex[c][2]>=1 ? 1 : 0;
    l3 = simplex[c][3]>=1 ? 1 : 0;
    float x1 = x0 - i1 + G4;
    float y1 = y0 - j1 + G4;
    float z1 = z0 - k1 + G4;
    float w1 = w0 - l1 + G4;
    float x2 = x0 - i2 + 2.0*G4;
    float y2 = y0 - j2 + 2.0*G4;
    float z2 = z0 - k2 + 2.0*G4;
    float w2 = w0 - l2 + 2.0*G4;
    float x3 = x0 - i3 + 3.0*G4;
    float y3 = y0 - j3 + 3.0*G4;
    float z3 = z0 - k3 + 3.0*G4;
    float w3 = w0 - l3 + 3.0*G4;
    float x4 = x0 - 1.0 + 4.0*G4;
    float y4 = y0 - 1.0 + 4.0*G4;
    float z4 = z0 - 1.0 + 4.0*G4;
    float w4 = w0 - 1.0 + 4.0*G4;
    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;
    int ll = l & 255;
    int gi0 = perm[ii+perm[jj+perm[kk+perm[ll]]]] % 32;
    int gi1 = perm[ii+i1+perm[jj+j1+perm[kk+k1+perm[ll+l1]]]] % 32;
    int gi2 = perm[ii+i2+perm[jj+j2+perm[kk+k2+perm[ll+l2]]]] % 32;
    int gi3 = perm[ii+i3+perm[jj+j3+perm[kk+k3+perm[ll+l3]]]] % 32;
    int gi4 = perm[ii+1+perm[jj+1+perm[kk+1+perm[ll+1]]]] % 32;
    float t0 = 0.6 - x0*x0 - y0*y0 - z0*z0 - w0*w0;
    if(t0<0) n0 = 0.0;
    else {
        t0 *= t0;
        n0 = t0 * t0 * dot(float4(grad4[gi0][0],grad4[gi0][2],grad4[gi0][3],grad4[gi0][3]), float4(x0, y0, z0, w0));
    }
    float t1 = 0.6 - x1*x1 - y1*y1 - z1*z1 - w1*w1;
    if(t1<0) n1 = 0.0;
    else {
        t1 *= t1;
        n1 = t1 * t1 * dot(float4(grad4[gi1][0],grad4[gi1][2],grad4[gi1][3],grad4[gi1][3]), float4(x1, y1, z1, w1));
    }
    float t2 = 0.6 - x2*x2 - y2*y2 - z2*z2 - w2*w2;
    if(t2<0) n2 = 0.0;
    else {
        t2 *= t2;
        n2 = t2 * t2 * dot(float4(grad4[gi2][0],grad4[gi2][2],grad4[gi2][3],grad4[gi2][3]), float4(x2, y2, z2, w2));
    }
    float t3 = 0.6 - x3*x3 - y3*y3 - z3*z3 - w3*w3;
    if(t3<0) n3 = 0.0;
    else {
        t3 *= t3;
        n3 = t3 * t3 * dot(float4(grad4[gi3][0],grad4[gi3][2],grad4[gi3][3],grad4[gi3][3]), float4(x3, y3, z3, w3));
    }
    float t4 = 0.6 - x4*x4 - y4*y4 - z4*z4 - w4*w4;
    if(t4<0) n4 = 0.0;
    else {
        t4 *= t4;
        n4 = t4 * t4 * dot(float4(grad4[gi4][0],grad4[gi4][2],grad4[gi4][3],grad4[gi4][3]), float4(x4, y4, z4, w4));
    }
    return 27.0 * (n0 + n1 + n2 + n3 + n4);
}
inline float octave_noise_4d( const float octaves, const float persistence, const float scale, const float x, const float y, const float z, const float w ) 
{
    float total = 0;
    float frequency = scale;
    float amplitude = 1;
    float maxAmplitude = 0;
    for( int i=0; i < octaves; i++ ) {
        total += (float)raw_noise_4d( x * frequency, y * frequency, z * frequency, w * frequency ) * amplitude;

        frequency *= 2;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }
    return float(total / maxAmplitude);
}

float3 gradientNoise(float3 P, float octaves, float persistence, float scale, float w, float epsilon){
   // float epsilon = grad_eps;
   // float3 P = float3(pos.x),pos.y, pos.z);
    float noise_x1 = (float)octave_noise_4d(float(octaves),persistence,scale,P.x + epsilon, P.y, P.z,w);
    float noise_x2 = (float)octave_noise_4d(float(octaves),persistence,scale,P.x - epsilon, P.y, P.z,w);
    float noise_y1 = (float)octave_noise_4d(float(octaves),persistence,scale,P.x, P.y + epsilon, P.z,w);
    float noise_y2 = (float)octave_noise_4d(float(octaves),persistence,scale,P.x, P.y - epsilon, P.z,w);
    float noise_z1 = (float)octave_noise_4d(float(octaves),persistence,scale,P.x, P.y, P.z + epsilon,w);
    float noise_z2 = (float)octave_noise_4d(float(octaves),persistence,scale,P.x, P.y, P.z - epsilon,w);

    float grad_x = (noise_x1 - noise_x2) / (2.0 * epsilon);
    float grad_y = (noise_y1 - noise_y2) / (2.0 * epsilon);
    float grad_z = (noise_z1 - noise_z2) / (2.0 * epsilon);
    return float3(grad_x, grad_y, grad_z);
}

kernel Curl_Noise : ImageComputationKernel<ePixelWise>
{
  // Declare the particle attributes as Images:
  Image<eReadWrite> p_velocity;
  Image<eReadWrite> p_position;
  Image<eReadWrite> p_color;

  // Declare our parameter storage
  param:
    float3 winddir;
    float windspeed;
    float maxSpeed;
    int octaves;
    float persistence;
    float scale;
    float _amount;
    float _dt;
    float w;
    float _frame;
    float grad_eps;
    float curl_eps;


  // Define the parameters to the outside world
  void define() {
    defineParam(maxSpeed, "paMaxSpeed", 1.0f);
    defineParam(octaves,      "octaves", 8);
    defineParam(persistence, "persistence",  0.5f);
    defineParam(scale, "scale", 0.001f);
    defineParam(_amount, "paAmount", 0.0f);
    defineParam(_dt, "_dt", 1.0f);
    defineParam(w, "w", 0.0f);

  }

  void process() {


    float3 pos = float3(p_position(0), p_position(1), p_position(2));
    float3 windPos = pos - (winddir*windspeed* _frame);
    float epsilon = curl_eps;

    float3 eps_dx = float3(epsilon, 0, 0);
    float3 eps_dy = float3(0,epsilon, 0);
    float3 eps_dz = float3(0, 0, epsilon);

    // Calculate gradients
    float3 dx = gradientNoise(windPos + eps_dx, octaves, persistence, scale, w, grad_eps) - gradientNoise(windPos-eps_dx, octaves, persistence, scale, w, grad_eps);
    float3 dy = gradientNoise(windPos + eps_dy, octaves, persistence, scale, w, grad_eps) - gradientNoise(windPos-eps_dy, octaves, persistence, scale, w, grad_eps);
    float3 dz = gradientNoise(windPos + eps_dz, octaves, persistence, scale, w, grad_eps) - gradientNoise(windPos-eps_dz, octaves, persistence, scale, w, grad_eps);

    // Normalize by the delta
    dx /= 2*epsilon;
    dy /= 2*epsilon;
    dz /= 2*epsilon;

    // Calculate the curl components
    float curlX = dy.z - dz.y;
    float curlY = dz.x - dx.z;
    float curlZ = dx.y - dy.x;

    // Create the curl vector
    float3 curl = float3(curlX, curlY, curlZ);

    
    float windNoise = (octave_noise_4d(float(octaves),persistence,scale,windPos.x, windPos.y, windPos.z ,w) +1.0 )/ 2.0;
    float3 wind = winddir * windNoise * windspeed;
    //curl *= windNoise;
    //curl += float3(0,0,_frame/10);
    p_color() = float4(windNoise, windNoise, windNoise, 1.0);
    p_velocity()+= ((curl * _amount ) +wind)*_dt;



  }
};
