void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    // Time varying pixel color
   // vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));
    float ar = iResolution.x / iResolution.y;
    vec2 unwarped_uv = vec2(uv.x * ar, uv.y);
    
    float len = length(unwarped_uv - 0.2) ;
    float len2 = length(unwarped_uv - 0.8) ;
    float r = ((sin(float(iFrame)/50.0) /2.0 ) + 0.5) * 0.3;
    float s = smoothstep(r, r-0.002, len);
    float z = smoothstep(r, r-0.2, len2);
    
    
    float c = s + z;
	vec3 col = vec3(c,c,c);
    // Output to screen
    fragColor = vec4(col,1.0);
}

