#version 330 core
in vec3 posShifted;

uniform sampler3D textureVolume;
uniform sampler2D textureRender;
uniform vec2 windowSize;

out vec4 FragColor;

void main()
{
	vec4 fragEnd = vec4(vec3(0.0f),1.0f);
	vec2 coord = (gl_FragCoord.xy) / windowSize;
	vec3 rayDir = vec3(texture(textureRender,coord).xyz - posShifted);
	vec3 rayIn = posShifted;
	float max = length(rayDir);
	rayDir = normalize(rayDir);
    float step=1.0f/256;
    vec3 stepMove;

	for(float i=0.0f;i<max;i+=step){
		stepMove= texture(textureVolume,rayIn).r * vec3(texture(textureVolume,rayIn).r);
		stepMove *=fragEnd.a;
        fragEnd.rgb +=stepMove;
		fragEnd.a *= 1 - texture(textureVolume,rayIn).r;
		if(1-fragEnd.a >= 0.99f) break;
		rayIn += rayDir*step;
	}
	fragEnd.a = 1.0f;
	FragColor = fragEnd;
}