#version 330
layout (location = 0)in vec3 vVertex;
layout (location = 1)in vec3 vColor;
layout (location = 2)in vec3 vNormal;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

//Uncomment for Gauraud Shading
//uniform vec3 vLightPosition;
//uniform vec3 vLightColor;
//uniform vec3 viewPos;

out vec3 fColor;

//Comment these for Gauraud Shading
out vec3 Position;
out vec3 normal;

void main(void) {

    //fColor = vColor;

    //Gauraud Shading
	    //gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
        //vec3 Position = vec3(vModel * vec4(vVertex, 1.0));

        //define constants
            //float amb = 0.2, diff = 0.5, spec = 1;

        //ambient
            //vec3 lAmb = amb * vLightColor;

        //diffuse
            //vec3 l = normalize(vLightPosition - Position);
            //vec3 n = normalize(vNormal);
            //float diffuse = max(dot(l, n), 0.0);
            //vec3 lDiff = diffuse * vLightColor * diff;

        //specular
            //vec3 v = normalize(viewPos - Position);
            //vec3 h = normalize(l + v);
            //float specular = pow(max(dot(n, h), 0.0), 128);
            //vec3 lSpec = spec * specular * vLightColor;

        //fColor = (lAmb + lDiff + lSpec)* vColor;

    //Phong Shading
        Position = vec3(vModel * vec4(vVertex, 1.0));
        gl_Position = vProjection * vView * vec4(Position, 1.0);

        normal = normalize(vNormal);
        fColor = vColor;
}
