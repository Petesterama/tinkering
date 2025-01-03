kernel ParticleRenderKernel : ImageComputationKernel<ePixelWise>
{
  Image<eRead, eAccessPoint> p_position;
  Image<eRead, eAccessPoint> p_color;
  Image<eRead, eAccessPoint> p_size;
  Image<eRead, eAccessPoint> p_velocity;
  //Image<eWrite, eAccessPoint> test_attribute;
  Image<eWrite, eAccessRandom> output;
  param:
    float4x4 _worldToScreen;
    float _size;
    int _steps;
    float _shutter;
    int _merge_operation;

  local:
    int _w, _h;
  void define() {
    defineParam(_worldToScreen, "_worldToScreen", float4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f) );
    // where the fuck did that spawn from
    defineParam(_size, "paSize", 3.0f);
    defineParam(_steps, "Steps", 10);
    defineParam(_shutter, "shutter", 0.5f);
    defineParam(_merge_operation, "merge_operation", 0);
    
  }
  float4 srcOver( float4 a, float4 b ) {
    return (1.0f-a.w)*b + a;
  }
  float2 transform( float3 p )
  {
    float4 r = _worldToScreen*float4(p.x, p.y, p.z, 1.0);
    return float2(r.x, r.y)/r.w;
  }
  float smoothstep( float a, float b, float x ) {
    float t = clamp((x - a) / (b - a), 0.0, 1.0);
    return t*t * (3.0f - 2.0f*t);
  }
  void init()
  {
    _w = output.bounds.width();
    _h = output.bounds.height();
  }
  void renderPoint( float2 xy, float4 pcolor, float pointSize )
  {

    float sizeSquared = pointSize*pointSize;
    float2 f = xy-floor(xy);                            //fractional part of screen coordinate?
    float2 g = float2(1.0f) - f;                        // complement/invert of fractional part
    //xy = floor(xy);
    int size = ceil(pointSize);
    int sizex = size;
    int sizey = size*2;
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
  void process() {
    float3 P = p_position();
    float3 v = p_velocity();
    float4 pcolor = p_color()/ float(_steps);
    for (int i=0; i <=_steps; i++){
      float3 P2 = P + ((v/float(_steps)) * float(i)*_shutter);
      float2 xy = transform(P2);
      renderPoint(xy, pcolor, _size*p_size().x);        //interesting that it gets the x component of size, size is a float 3 :o
    }
  }
};
 