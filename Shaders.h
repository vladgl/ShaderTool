#define SHADER_VERSION \
    "#version 330 core\n"

#define BASIC_VERTEX_SRC \
    "in vec3 st_Position;\n" \
    "void main()\n" \
    "{\n"\
    "    gl_Position = vec4(st_Position, 1.0);\n" \
    "}\n"
#define SHADER_INPUT \
    "uniform vec2 iResolution; //viewport resolution (in pixels) \n" \
    "uniform float iTime; //shader playback time (in seconds)\n" \
    "uniform float iTimeDelta; //render time (in seconds) \n" \
    "uniform int iFrame; //shader playback frame \n" \
    "uniform vec4 iMouse; //mouse pixel coords. xy: current, zw: click \n" \
    "//input channels \n" \
    "uniform sampler2D iChannel0; \n"\
    "uniform sampler2D iChannel1; \n"\
    "uniform sampler2D iChannel2; \n"\
    "uniform sampler2D iChannel3; \n"\
    "uniform vec4 iDate; //(year, month, day, time in seconds)"

#define MAIN_IMAGE_DECL \
    "void mainImage(out vec4 fragColor, in vec2 fragCoord); \n"

#define MAIN_IMAGE_BASIC_DEF \
    "void mainImage(out vec4 fragColor, in vec2 fragCoord) \n" \
    "{\n" \
    "    fragColor = vec4(0.1, 0.7, 0.2, 1.0);\n" \
    "}\n"

#define FRAGMENT_MAIN \
    "void main() \n" \
    "{ \n" \
    "    vec2 fragCoord = gl_FragCoord.xy;\n" \
    "    vec4 fragColor; \n" \
    "    mainImage(fragColor, fragCoord); \n" \
    "    gl_FragColor = fragColor; \n"\
    "} \n"


#define VERTEX_SHADER \
    SHADER_VERSION \
    BASIC_VERTEX_SRC

#define FRAGMENT_SHADER_HEADER \
    SHADER_VERSION \
    SHADER_INPUT \
    MAIN_IMAGE_DECL
