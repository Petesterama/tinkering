vec3 BG
vec3 PLATE
vec3 despill = keylight(color = {0,1,0})
vec3 backing_region_colour = sample(despill, {50,50})

despill_matte = saturation(Plate - despill,0)/whitepoint

processed_bg = BG - backing_region_colour

A = despill + processed_bg * despill_matte 
# with a being alpha

final_pixel = float4(Aa + B*(1-a), a)