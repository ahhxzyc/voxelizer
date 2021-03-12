# version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 36) out;;

uniform float voxResolution;

void main() {
    float halfres = voxResolution * 0.5;
    vec4 v[8] = {
        gl_in[0].gl_Position + vec4(-1, -1, -1, 0) * halfres,
        gl_in[0].gl_Position + vec4(-1, -1,  1, 0) * halfres,
        gl_in[0].gl_Position + vec4(-1,  1, -1, 0) * halfres,
        gl_in[0].gl_Position + vec4(-1,  1,  1, 0) * halfres,
        gl_in[0].gl_Position + vec4( 1, -1, -1, 0) * halfres,
        gl_in[0].gl_Position + vec4( 1, -1,  1, 0) * halfres,
        gl_in[0].gl_Position + vec4( 1,  1, -1, 0) * halfres,
        gl_in[0].gl_Position + vec4( 1,  1,  1, 0) * halfres
    };
    int t[12][3] = {
        {0, 1, 3}, {0, 3, 2}, {5, 3, 1}, {7, 3, 5},
        {7, 2, 3}, {6, 2, 7}, {6, 4, 2}, {2, 4, 0},
        {4, 7, 5}, {4, 6, 7}, {5, 1, 0}, {5, 0, 4}
    };
    // 12 triangle faces
    // this is actually meaningless since only 2 of them will be visible
    for (int i = 0; i < 12; i ++ ) {
        // get vertices of this face
        vec3 vs[3];
        for (int j = 0; j < 3; j ++ )
            vs[j] = v[t[i][j]].xyz;
        // find normal of this face
        vec3 normal = normalize( cross(vs[1] - vs[0], vs[2] - vs[0]) );
        // launch!
        for (int j = 0; j < 3; j ++ ) {
            gl_Position = v[t[i][j]];
            EmitVertex();
        }
        EndPrimitive();
    }
}