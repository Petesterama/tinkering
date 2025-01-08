kernel ParticleRenderKernel : ImageComputationKernel<ePixelWise>{
  Image<eRead, eAccessPoint> p_position;
  Image<eRead, eAccessPoint> p_color;
  Image<eRead, eAccessPoint> p_size;
  Image<eRead, eAccessPoint> p_velocity;
//   Image<eRead, eAccessRandom, eEdgeConstant> texture;
  Image<eWrite, eAccessRandom> output;
  param:
    float4x4 _worldToScreen;

    float _size;
    int _maxSteps;
    float spacing;
    float _shutter;
    float _shutter_offset;
    int _merge_operation;
    int _screen_space_scale;
    int _shutter_type;

    float haperture;
    float focal;

    float near;
    float near_falloff;
    float far;
    float far_falloff;

  local:
    int _w, _h;
    float2 out_res;
    // int tex_w, tex_h;
    float pixelsPerWorldUnit;
  void define() {
    defineParam(_worldToScreen, "_worldToScreen", float4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f) );
    // where the fuck did that spawn from
    defineParam(_size, "paSize", 3.0f);
    defineParam(_maxSteps, "Steps", 10);
    defineParam(spacing, "spacing", 1.0f);
    defineParam(_shutter, "shutter", 0.5f);
    defineParam(_shutter_offset, "shutter_offset", 0.0f);
    defineParam(_shutter_type, "shutter_type", 0);
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
    float2 xy = float2(r.x, r.y)/r.w;
    return xy;
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

  float lerp(float a, float b, float t){
    return a + t * (b-a);
  }
  float2 lerp(float2 a, float2 b, float t) {
     return a + t * (b - a);
  }



  void init()
  {
    _w = output.bounds.width();
    _h = output.bounds.height();
    out_res = float2(float(_w), float(_h));

    // tex_w = texture.bounds.width();
    // tex_h = texture.bounds.height();

    pixelsPerWorldUnit = (_w*0.5) / haperture;

  }
  void renderPoint( float2 xy, float4 pcolor, float pointSize, float falloff ){

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
//   void renderTex( float2 xy, float4 pcolor, float pointSize, float falloff ){

//     float sizeSquared = pointSize*pointSize;
//     float2 f = xy-floor(xy);                            //fractional part of screen coordinate?
//     float2 g = float2(1.0f) - f;                        // complement/invert of fractional part
//     //xy = floor(xy);
//     int size = ceil(pointSize);
//     int minX = max(0, int(xy.x)-size);                  // min x coord is clamped to image x=0
//     int maxX = min(_w-1, int(xy.x)+size);               // max x coord is clamped to max image x coord
//     int minY = max(0, int(xy.y)-size);                  // etc for y coord
//     int maxY = min(_h-1, int(xy.y)+size);
//     // for loop that runs over the expected size of the point to be rendered.
//     for ( int y = minY; y <= maxY; y++ ) {              // starting at min y until max y
//       for ( int x = minX ; x <= maxX; x++ ) {           // starting at min x until max x
//         float2 p = float2(x, y);                        // current position in the iteration space
//         //float2 uv = float2( (p.x/float(_w))*float(tex_w), ( p.y/float(_h))*float(tex_h));
//         float u = (float(x - minX) / size) * float(tex_w);
//         float v = (float(y - minY) / size) * float(tex_h);
//         //float2 uv = float2(u,v);
//         //float2 uv = float2((float(size)/p.x) * float(tex_w), (float(size)/p.y) * float(tex_h));
        
//         float4 texLookup = bilinear(texture, u, v);
//           if (_merge_operation == 0) {
//             output(p.x, p.y) += texLookup;
//           }
//           if (_merge_operation == 1){
//           float4 colourToAdd = texLookup;
//           float4 existingColour = output(p.x,p.y);
//           output(p.x, p.y) = srcOver(colourToAdd, existingColour);
//           }
//                            // after we calculate the pixels distance, and smooth it with smoothstep
//                                                         // we write it to the output buffer with a simple plus.
//         }
//       }
//     }

  float minMaxSmooth(float n,float ns,float f,float fs,float v){
      // input variables are near, near soft, far, far soft and value
      float output = 0.0;
      float near = min(n, f);
      float near_soft = max(ns, 0.0);
      float far = max(n, f);
      float far_soft = max(fs, 0.0);

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



    float screenSpaceSize(float focal, float pixelsPerWorldUnit, float p_size, float distance){
        float particleSizeOnImagePlane = (focal * p_size) / distance;
        float particleSizeInPixels = (particleSizeOnImagePlane * pixelsPerWorldUnit);
    return particleSizeInPixels;
  }  


  void process() {

    float3 P = p_position();
    float3 v = p_velocity();
    float3 xyz = transformz(P);

    float dist = xyz.z;

    // optimisation conditions, render only in frustum, and positive depth (in front of camera)
    if (dist>0.0f){

//xyz.x >0.0 && xyz.x < float(_w) && xyz.y >0.0 && xyz.y <float(_h)

        
        


        // size stuff
        /*
        float size = _size * p_size().x;
        if (_screen_space_scale){
        size = screenSpaceSize(focal, pixelsPerWorldUnit, size, dist);
        }
        size = min(size,100.0f);
        */

        // shutter stuff
        float3 shutter_open, shutter_close;
        if (_shutter_type == 0) {                   // start
            shutter_open = P;
            shutter_close = P+(v *_shutter);
        }
        else if (_shutter_type == 1) {              // centered
            shutter_open = P - (v *_shutter*.5);
            //shutter_open = P;
            shutter_close = P + (v *_shutter*.5);
        }
        else if (_shutter_type == 2) {              // end
            shutter_open = P;
            shutter_close = P-(v *_shutter);
        }

        float2 shutter_open_2d, shutter_close_2d;
        float size_start, size_end;
        float3 shutter_open_ndc = transformz(shutter_open);
        float3 shutter_close_ndc = transformz(shutter_close);

        shutter_open_2d = float2(shutter_open_ndc.x, shutter_open_ndc.y);
        shutter_close_2d = float2(shutter_close_ndc.x, shutter_close_ndc.y);
        //size_start = shutter_open_ndc.z;
        //size_end = shutter_close_ndc.z;
        size_start = min(100.0f, screenSpaceSize(focal, pixelsPerWorldUnit, _size * p_size().x, shutter_open_ndc.z));
        size_end = min(100.0f, screenSpaceSize(focal, pixelsPerWorldUnit, _size * p_size().x, shutter_close_ndc.z));


        float2 blur_vector = (shutter_close_2d-shutter_open_2d);
        float blur_length = length(blur_vector);

        // adaptive steps
        int steps = max(0 ,min(_maxSteps, ceil(blur_length/min(size_start, size_end)*2.0/spacing)));
        float2 step = blur_vector/float(steps);


        // colour stuff
        float depth_fade = minMaxSmooth(near, near_falloff, far, far_falloff, dist);
        float4 pcolor = p_color()/ float(steps);

        for (int i = 0; i <= steps; i++){
            float2 draw_xy = shutter_open_2d + (step * float(i));
            float size = lerp(size_start, size_end, float(i)/float(steps));
            if (draw_xy.x >-size && draw_xy.y>-size && draw_xy.x < out_res.x+size && draw_xy.y < out_res.y+size){
            renderPoint(draw_xy, pcolor, size, depth_fade);
            }
        }



    }
    

  }
};