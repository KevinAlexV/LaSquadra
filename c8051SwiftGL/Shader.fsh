#version 300 es

precision highp float;
in vec4 v_color;
// ### receive additional vertex attribs
in vec3 v_normal;
in vec2 v_texcoord;
out vec4 o_fragColor;

// add uniforms from vertex shader or others (e.g., texture sampler)
uniform sampler2D texSampler;

uniform mat3 normalMatrix;
uniform bool passThrough;   // in case this should be a simple pass through shader
uniform bool shadeInFrag;   // choose between doing more work in the vertex or fragment shader
uniform bool useFog;

void main()
{
    if (!passThrough && shadeInFrag) {
        vec3 eyeNormal = normalize(normalMatrix * v_normal);
        vec3 lightPosition = vec3(1.0, 1.0, 1.0);
        vec4 diffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
        
        float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
        
        o_fragColor = diffuseColor * nDotVP * texture(texSampler, v_texcoord);
    } else {
        o_fragColor = v_color;
    }
    if (useFog){
            // Fog effect added (linear)
            float fogDist = (gl_FragCoord.z / gl_FragCoord.w);
            vec4 fogColour = vec4(1.0, 1.0, 1.0, 1.0);
            float fogFactor = (4.0 - fogDist) / (4.0 - 1.0);
            fogFactor = clamp(fogFactor, 0.0, 1.0);
            o_fragColor = mix(fogColour, o_fragColor, fogFactor);
    }
}

