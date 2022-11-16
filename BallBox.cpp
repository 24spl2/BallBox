
#define _CRT_SECURE_NO_WARNINGS
#include "snail.cpp"
#include "cow.cpp"
#include <vector>
#include <iostream>


void ballBox() {

    const int N = 100000;
    vec3 *vertex_positions = (vec3 *) malloc(N * sizeof(vec3));
    vec3 *color_points = (vec3 *) malloc(N * sizeof(vec3));
    vec3 *velocity = (vec3 *) malloc(N * sizeof(vec3));


    int primitive_type[] = {POINTS, LINES, LINE_STRIP, LINE_LOOP, TRIANGLES, TRIANGLE_STRIP,
                                    TRIANGLE_FAN, QUADS, TRIANGLE_MESH, QUAD_MESH};

    //randomizes start positions
    for (int i = 0; i < N; i++) {
        vertex_positions[i] = {util_random_double(-1.0, 1.0), util_random_double(-1.0, 1.0),
                                                            util_random_double(-1.0, 1.0)};
        color_points[i] = {util_random_double(0.0, 1.0), util_random_double(0.0, 1.0),
                                                            util_random_double(0.0, 1.0)};
        velocity[i] = {util_random_double(-0.01, 0.01), util_random_double(-0.01, 0.01),
                                                            util_random_double(-0.01, 0.01)};
    }

    int N_slider = 12;
    int Primitive_slider = 0;
    int sizePoint = 100;
    int size_slider = 0;


    while(begin_frame()) {

        static Camera3D camera = { 5, RAD(45) };
        camera_move(&camera);
        mat4 PV = camera_get_PV(&camera);

        //make cube
        gl_PV(PV);


        //round one chair (1/2 of cube)
        gl_begin(LINE_LOOP);
        gl_color(1, 1, 1);

        gl_vertex(1, 1, 1);
        gl_vertex(-1, 1, 1);
        gl_vertex(-1, 1, -1);
        gl_vertex(-1, -1, -1);
        gl_vertex(-1, -1, 1);
        gl_vertex(1, -1, 1);
        gl_vertex(1, -1, -1);
        gl_vertex(1, 1, -1);
        gl_vertex(1, 1, 1);
        gl_end();

        //round two chair (1/2 of cube)
        gl_begin(LINE_LOOP);
        gl_color(1, 1, 1);

        gl_vertex(1, 1, -1);
        gl_vertex(-1, 1, -1);
        gl_vertex(-1, 1, 1);
        gl_vertex(-1, -1, 1);
        gl_vertex(-1, -1, -1);
        gl_vertex(1, -1, -1);
        gl_vertex(1, -1, 1);
        gl_vertex(1, 1, 1);
        gl_end();


        //UPDATE: sliders
        imgui_slider("N_slider", &N_slider, 4, N);
        imgui_slider("Primitive_slider", &Primitive_slider, 0, 9, 'j', 'k', true);
        imgui_slider("size_slider", &size_slider, 0, sizePoint);

        //velocity updates
        for (int i = 0; i < N; i++) {
            vec3 current = vertex_positions[i];
            vec3 update = velocity[i];

            //update position
            current[0] += update[0];
            current[1] += update[1];
            current[2] += update[2];

            //the bouncing off the walls part
            for (int j = 0; j < 3; j++) {
                if (current[j] <= -1 || current[j] >= 1) {
                    update[j] = -update[j];
                }
            }

            //send it back to the original array
            vertex_positions[i] = current;
            velocity[i] = update;
        }


        //DRAW
        //primitive type decision, camera, number of balls, where they are located, what color they are, and the size
        basic_draw(primitive_type[Primitive_slider], PV, N_slider, vertex_positions, color_points, size_slider, true);

    }

    free(vertex_positions);
    free(color_points);
    free(velocity);

}



int main() {
    init(false, "BALLBOX!");
    ballbox();
    return 0;
}
