#version 330

uniform sampler2D texture0;
uniform vec2 lightPosition;

in vec2 fragTexCoord;
in vec2 fragPosition;

out vec4 finalColor;

// Adjustable attenuation parameters
const float LINEAR_TERM    = 0.00003; // linear term
const float QUADRATIC_TERM = 0.00003; // quadratic term
const float MIN_BRIGHTNESS = 0.003;    // avoid total darkness
const vec3 GLOW_COLOR = vec3(0.0, 1.0, 0.0);
const float GLOW_INTENSITY = 0.3;

float attenuate(float distance, float linearTerm, float quadraticTerm)
{
    float attenuation = 1.0 / (1.0 + 
                               linearTerm * distance + 
                               quadraticTerm * distance * distance);

    return max(attenuation, MIN_BRIGHTNESS);
}

void main()
{
    float distance = distance(lightPosition, fragPosition);
    float brightness = attenuate(distance, LINEAR_TERM, QUADRATIC_TERM);
    vec4 color = texture(texture0, fragTexCoord);
    finalColor = vec4(color.rgb + brightness * GLOW_COLOR * GLOW_INTENSITY, color.a);
}