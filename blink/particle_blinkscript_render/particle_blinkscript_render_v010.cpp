kernel ParticleRenderKernel : ImageComputationKernel<ePixelWise>
{
  Image<eRead, eAccessPoint> p_position;
  Image<eRead, eAccessPoint> p_color;
  Image<eRead, eAccessPoint> p_size;
  Image<eRead, eAccessPoint> p_velocity;
  Image<eWrite, eAccessRandom> output;
  param:
    float4x4 _worldToScreen;

    float _size;
    int _steps;
    float _shutter;
    int _merge_operation;
    int _screen_space_scale;

    float haperture;
    float focal;

    float near;
    float near_falloff;
    float far;
    float far_falloff;

  local:
    int _w, _h;
    float pixelsPerWorldUnit;
  void define() {
    defineParam(_worldToScreen, "_worldToScreen", float4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f) );
    // where the fuck did that spawn from
    defineParam(_size, "paSize", 3.0f);
    defineParam(_steps, "Steps", 10);
    defineParam(_shutter, "shutter", 0.5f);
    defineParam(_merge_operation, "merge_operation", 0);
    defineParam(_screen_space_scale, "screen_space_scale", 0);
    defineParam(haperture, "haperture", 24.576f);
    defineParam(focal, "focal", 24.0f);
    defineParam(near, "near", 0.0f);
    defineParam(near_falloff, "near_falloff", 0.0f);
    defineParam(far, "far", 1000.0f);
    defineParam(far_falloff, "far_falloff", 0.0f);
    
  }
  float4 srcOver( float4 a, float4 b ) {
    return (1.0f-a.w)*b + a;
  }
  float2 transform( float3 p ){
    float4 r = _worldToScreen*float4(p.x, p.y, p.z, 1.0);
    return float2(r.x, r.y)/r.w;
  }
  float3 transformz( float3 p ){
    float4 r = _worldToScreen*float4(p.x, p.y, p.z, 1.0);
    float2 xy = float2(r.x, r.y)/r.w;
    float depth = r.w;
    return float3(xy, depth);
  }  
  float smoothstep( float a, float b, float x ) {
    float t = clamp((x - a) / (b - a), 0.0, 1.0);
    return t*t * (3.0f - 2.0f*t);
  }
  void init()
  {
    _w = output.bounds.width();
    _h = output.bounds.height();
    pixelsPerWorldUnit = (_w*0.5) / haperture;

  }
  void renderPoint( float2 xy, float4 pcolor, float pointSize, float falloff )
  {

    float sizeSquared = pointSize*pointSize;
    float2 f = xy-floor(xy);                            //fractional part of screen coordinate?
    float2 g = float2(1.0f) - f;                        // complement/invert of fractional part
    //xy = floor(xy);
    int size = ceil(pointSize);
    int minX = max(0, int(xy.x)-size);                  // min x coord is clamped to image x=0
    int maxX = min(_w-1, int(xy.x)+size);               // max x coord is clamped to max image x coord
    int minY = max(0, int(xy.y)-size);                  // etc for y coord
    int maxY = min(_h-1, int(xy.y)+size);
    // for loop that runs over the expected size of the point to be rendered.
    for ( int y = minY; y <= maxY; y++ ) {              // starting at min y until max y
      for ( int x = minX ; x <= maxX; x++ ) {           // starting at min x until max x
        float2 p = float2(x, y);                        // current position in the iteration space
        float2 d = p-xy;                                // vector from center point to the current pixel
        float r2 = dot(d, d);                           // squared distance to from p to xy
        // work with squares, ie squared distance and squared radius because it's faster to calculate.
        if ( r2 < sizeSquared ) {                       // if radius squared is smaller than size squared
         float t = 1.0f-smoothstep(0, sizeSquared, r2);
        t *= falloff;
        //t = max(t,0.0);


          if (_merge_operation == 0) {
            output(p.x, p.y) += pcolor*t;
          }
          if (_merge_operation == 1){
          float4 colourToAdd = pcolor * t;
          float4 existingColour = output(p.x,p.y);
          output(p.x, p.y) = srcOver(colourToAdd, existingColour);
          }
                           // after we calculate the pixels distance, and smooth it with smoothstep
                                                        // we write it to the output buffer with a simple plus.
        }
      }
    }
  }
  float minMaxSmooth(float n,float ns,float f,float fs,float v){
      // input variables are near, near soft, far, far soft and value
      float output = 0.0;
      float near = min(n, f);
      float near_soft = max(ns, 0);
      float far = max(n, f);
      float far_soft = max(fs, 0);

      float near_soft_absolute = near-near_soft;
      float far_soft_absolute = far+far_soft;

      if (v <= near){
        output = smoothstep(near_soft_absolute,near,v);
        }
      else{
        output = 1-smoothstep(far,far_soft_absolute,v);
        }
      return output;
  }


    float screenSpaceSizeGPT(float focal, float haperture, float p_size, float distance, float screenWidth){
        float particleSizeOnImagePlane = (focal * p_size) / distance;
        float pixelsPerWorldUnit = (screenWidth*0.5) / haperture;
        float particleSizeInPixels = (particleSizeOnImagePlane * pixelsPerWorldUnit);
    return particleSizeInPixels;
  }

    float screenSpaceSizeGPT_optimised(float focal, float pixelsPerWorldUnit, float p_size, float distance){
        float particleSizeOnImagePlane = (focal * p_size) / distance;
        float particleSizeInPixels = (particleSizeOnImagePlane * pixelsPerWorldUnit);
    return particleSizeInPixels;
  }  


  void renderUtility(float3 xyz, float pointSize, float depth_fade){
    
    int size = ceil(pointSize);
    int minX = max(0, int(xyz.x)-size);   
    int maxX = min(_w-1, int(xyz.x)+size);
    int minY = max(0, int(xyz.y)-size);   
    int maxY = min(_h-1, int(xyz.y)+size);
    for ( int y = minY; y <= maxY; y++ ) {              
        for ( int x = minX ; x <= maxX; x++ ) {  
            float2 p = float2(x, y);

            float4 colourToAdd = float4(depth_fade);
            float4 existingColour = output(p.x,p.y);
            output(p.x, p.y) = srcOver(colourToAdd, existingColour);

        
            //output(p.x, p.y) += depth_fade;
        }
    }
  }

  

  void process() {
    float3 P = p_position();
    float3 v = p_velocity();
    float3 xyz = transformz(P);
    //float screen_width = float(output.bounds.width());

    float dist = xyz.z;

    float depth_fade = minMaxSmooth(near, near_falloff, far, far_falloff, dist);

    float size = _size * p_size().x;

    if (_screen_space_scale){
    //size = screenSpaceSizeGPT(50.0, 24.576, size, dist, _w);
    //size = screenSpaceSizeGPT_squared(50.0, 24.576, size, dist, _w);
    size = screenSpaceSizeGPT_optimised(focal, pixelsPerWorldUnit, size, dist);
    }
    size = min(size,100.0f);
    float4 pcolor = p_color()/ float(_steps);

   for (int i=0; i <=_steps; i++){
     float3 P2 = P + ((v/float(_steps)) * float(i)*_shutter);
     float2 xy = transform(P2);
     renderPoint(xy, pcolor, size, depth_fade);        //interesting that it gets the x component of size, size is a float 3 :o
    }


//    float3 xyz = transformz(P);
//    float2 xy = float2(xyz.x, xyz.y);
//    float depth = xyz.z;
//    float depth_fade = minMaxSmooth(15.0, 5.0, 15.0, 5.0, depth);
//    renderUtility(xyz, size, depth_fade);
  }
};