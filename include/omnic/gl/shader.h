


uniform sampler2D pixeldata;
uniform int pixeldata_width;
uniform int pixeldata_height;

uniform sampler1D colorcorrection;

uniform sampler2D input_2d;
uniform sampler2DRect input_rect;
uniform bool input_use_rect;
uniform int input_width;
uniform int input_height;

vec3 apply_colorcorrection(vec3 color) {
  return vec3(
    texture1D(colorcorrection,texture1D(colorcorrection,color.r).r).a,
    texture1D(colorcorrection,texture1D(colorcorrection,color.g).g).a,
    texture1D(colorcorrection,texture1D(colorcorrection,color.b).b).a
  );
}

void main() {
  vec2 uv = gl_FragCoord.xy / vec2(pixeldata_width,pixeldata_height);
  vec4 pixeldata_pixel = texture2D(pixeldata,uv);

  vec4 color = input_use_rect ?
    texture2DRect(input_rect,calib.st * vec2(input_width,input_height)) :
    texture2D(input_rect,pixeldata_pixel.st);

  gl_FragColor = apply_colorcorrection(color.rgb)*pixeldata_pixel.a;
}
