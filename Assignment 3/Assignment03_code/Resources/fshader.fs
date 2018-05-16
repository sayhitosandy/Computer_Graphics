#version 330

in vec3 fColor;

//Comment these for Gauraud shading
in vec3 normal;
in vec3 Position;
uniform vec3 vLightPosition;
uniform vec3 vLightColor;
uniform vec3 viewPos;

out vec4 color;

void main(void) {
    //Gauraud Shading
	    //color = vec4(fColor, 1.0);

	//Phong Shading
	    //define constants
	        float amb = 0.3, diff = 0.5, spec = 0.5;

        //ambient
            vec3 lAmb = amb * vLightColor;

        //diffuse
            vec3 l = normalize(vLightPosition - Position);
            vec3 n = normalize(normal);
            float diffuse = max(dot(l, n), 0.0);
            vec3 lDiff = diffuse * vLightColor * diff;

        //specular
            vec3 v = normalize(viewPos - Position);
            vec3 h = normalize(l + v);
            float specular = pow(max(dot(n, h), 0.0), 64);
            vec3 lSpec = spec * specular * vLightColor;

        color = vec4(((lAmb + lDiff + lSpec)* fColor), 1.0);
}
