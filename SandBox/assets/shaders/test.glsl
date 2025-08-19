// ===========================================================================
#type vertex
#version 450 core

layout(location = 0) in
vec3 a_Position;
layout(location = 1) in
vec4 a_Color;

out
vec4 v_Color;

void main()
{
    v_Color = a_Color;
    gl_Position = vec4(a_Position, 1.0);
}

// ===========================================================================

#type geometry
#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in
vec4 v_Color[];
out
vec4 g_Color;

void main()
{
    g_Color = v_Color[0];
    float size = 0.2;

    vec4 offsets[4] = vec4[4]
    (
        vec4(-size, -size, 0.0, 0.0),
        vec4(size, -size, 0.0, 0.0),
        vec4(-size, size, 0.0, 0.0),
        vec4(size, size, 0.0, 0.0)
    );

    for (int i = 0; i < 4; i++)
    {
        gl_Position = gl_in[0].gl_Position + offsets[i];
        EmitVertex();
    }
    EndPrimitive();
}

// ===========================================================================

#type fragment
#version 450 core

in
vec4 g_Color;
out
vec4 o_Color;

void main()
{
    o_Color = g_Color;
}

// ===========================================================================