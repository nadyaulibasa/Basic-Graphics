// COMP3421 Assignment 1
// Nadya Ulibasa (z5286000)

// C++17 doesn't have std::pi
#define _USE_MATH_DEFINES
#define N_CIRCLES 7

#include <cmath>

#include <cstdlib>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include <chicken3421/chicken3421.hpp>

/* Helpers */ 
struct image_t {
    int width;
    int height;
    int n_channels;
    void *data;
};

image_t load_image(const std::string &filename) {
    image_t img;

    stbi_set_flip_vertically_on_load(true);
    img.data = stbi_load(filename.data(), &img.width, &img.height, &img.n_channels, 0);

    chicken3421::expect(img.data, "Could not read " + filename);

    return img;
}

void delete_image(image_t &img) {
    stbi_image_free(img.data);

    img.data = nullptr;
    img.width = 0;
    img.height = 0;
    img.n_channels = 0;
}

GLuint make_texture(const std::string &path) {
    image_t tex_img = load_image(path);

    GLint format = tex_img.n_channels == 3 ? GL_RGB : GL_RGBA; // assuming that if not 3 channels then 4

    GLuint tex;
    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, format, tex_img.width, tex_img.height, 0, format, GL_UNSIGNED_BYTE, tex_img.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void delete_texture(GLuint tex) {
    glDeleteTextures(1, &tex);
}

/* Circles */
struct circle_t {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizeiptr n_verts = 0;

    std::vector<glm::vec4> positions;
    std::vector<glm::vec2> tcs;
    std::vector<glm::vec3> color;

    glm::mat4 translate = glm::mat4(1);
    glm::mat4 rotate = glm::mat4(1);
    glm::mat4 scale = glm::mat4(1);
};

circle_t make_circle(float cx, float cy, float rad, float r, float b, float g, int detail = 32) {
    circle_t c;

    c.positions.emplace_back(cx, cy, 0, 1);
    c.tcs.emplace_back(0.5f, 0.5f);
    c.color.emplace_back(r/255.0F, b/255.0F, g/255.0F);
    c.n_verts++;

    float slice = 2 * M_PI/detail;
    for (int i = 0; i <= detail; ++i) {
        float theta = i * slice;
        float x = cx + rad * std::cos(theta);
        float y = cy + rad * std::sin(theta);

        float tx = (x/rad + 1) * 0.5f;
        float ty = (y/rad + 1) * 0.5f;

        c.positions.emplace_back(x, y, 0, 1);
        c.tcs.emplace_back(tx, ty);
        c.color.emplace_back(r/255.0F, b/255.0F, g/255.0F);
        c.n_verts++;
    }


    c.translate = glm::identity<glm::mat4>();
    c.rotate = glm::identity<glm::mat4>();
    c.scale = glm::identity<glm::mat4>();

    glGenVertexArrays(1, &c.vao);
    glGenBuffers(1, &c.vbo);

    glBindVertexArray(c.vao);
    glBindBuffer(GL_ARRAY_BUFFER, c.vbo);

    glBufferData(GL_ARRAY_BUFFER, c.n_verts * (sizeof(glm::vec4) + sizeof(glm::vec2) + sizeof(glm::vec3)), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, c.n_verts * sizeof(glm::vec4), c.positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, c.n_verts * sizeof(glm::vec4), c.n_verts * sizeof(glm::vec2), c.tcs.data());
    glBufferSubData(GL_ARRAY_BUFFER, c.n_verts * (sizeof(glm::vec4) + sizeof(glm::vec2)), c.n_verts * sizeof(glm::vec3), c.color.data());

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(c.n_verts * sizeof(glm::vec4)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(c.n_verts * (sizeof(glm::vec4) + sizeof(glm::vec2))));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return c;
}

void draw_circle(circle_t c, GLuint tex, GLint loc_tex, GLint loc_transform, int even) {
    glBindVertexArray(c.vao);
    glBindBuffer(GL_ARRAY_BUFFER, c.vbo);

    glBindTexture(GL_TEXTURE_2D, tex);

    glm::mat4 rotate;
    if (even == 1)
        c.rotate = glm::rotate(c.rotate, (GLfloat) glfwGetTime() * 2.0f, glm::vec3(0, 0, 1));
    else if (even == 0)
        c.rotate = glm::rotate(c.rotate, (GLfloat) glfwGetTime() * -3.0f, glm::vec3(0, 0, 1));
    glm::mat4 transform = c.translate * c.rotate * c.scale;
    glUniformMatrix4fv(loc_transform, 1, GL_FALSE, glm::value_ptr(transform));

    glDrawArrays(GL_TRIANGLE_FAN, 0, c.n_verts);
}

/* Transforms */
/**
 * On mouse click, the shapes return back to its initial place.
 */
void on_mouse_click(GLFWwindow *win, int button, int action, int mods) {
    circle_t *c = (circle_t *) glfwGetWindowUserPointer(win);

    c->rotate = glm::mat4(1);
}

/**
 * On the R key press, the circles rotate alternatingly clockwise and counterclockwise.
 */
void on_key_press(GLFWwindow *win, int key, int scancode, int action, int mods) {
    circle_t *c = (circle_t *) glfwGetWindowUserPointer(win);

    switch(key) {
        case GLFW_KEY_R:
            c->rotate = glm::rotate(c->rotate, (GLfloat) glfwGetTime() * 5.0f, glm::vec3(0, 0, 1));
            break;
        default:
            break;
    }
}


int main() {
    GLFWwindow *win = chicken3421::make_opengl_window(700, 700, "Assignment 1");

    // Use a texture as the background
    GLuint bg_tex = make_texture("res/img/background.jpeg");
    GLuint fboId = 0;
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bg_tex, 0); 
    
    // Make textures
    GLuint centre1_tex = make_texture("res/img/eye1.jpeg");
    GLuint centre2_tex = make_texture("res/img/eye2.jpeg");
    GLuint blue_tex = make_texture("res/img/blue.jpeg");
    GLuint green_tex = make_texture("res/img/greenb.jpeg");
    GLuint pink_tex = make_texture("res/img/pink.jpeg");
    GLuint orange_tex = make_texture("res/img/orange copy.jpeg");
    GLuint turq_tex = make_texture("res/img/turq.jpeg");
    GLuint yellow_tex = make_texture("res/img/yellow2.jpeg");
    
    // Make circles
    circle_t c1 = make_circle(0, 0, 0.1, 0, 0, 0);
    circle_t c2 = make_circle(0, 0, 0.2, 255, 148, 135);
    circle_t c3 = make_circle(0, 0, 0.3, 255, 33, 78);
    circle_t c4 = make_circle(0, 0, 0.4, 255, 183, 66);
    circle_t c5 = make_circle(0, 0, 0.5, 255, 235, 102);
    circle_t c6 = make_circle(0, 0, 0.6, 167, 255, 89);
    circle_t c7 = make_circle(0, 0, 0.7, 89, 89, 255);

    // Shaders and program
    GLuint vs = chicken3421::make_shader("res/shaders/vert.glsl", GL_VERTEX_SHADER);
    GLuint fs = chicken3421::make_shader("res/shaders/frag.glsl", GL_FRAGMENT_SHADER);
    GLuint program = chicken3421::make_program(vs, fs);

    // Setup for transformations
    glfwSetWindowUserPointer(win, &c1);
    glfwSetKeyCallback(win, on_key_press);
    glfwSetMouseButtonCallback(win, on_mouse_click);
    
    GLint loc_tex = glGetUniformLocation(program, "u_tex");
    GLint loc_transform = glGetUniformLocation(program, "u_transform");
    float f = 1, increment = 0.05;
    while (!glfwWindowShouldClose(win)) {
        using namespace std::chrono;
        long long now = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();

        glfwPollEvents();
        glUseProgram(program);

        glClear(GL_COLOR_BUFFER_BIT);

        // Set background
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, 1400, 1400, 0, 0, 1400, 1400, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // Draw circles
        draw_circle(c7, blue_tex, loc_tex, loc_transform, 0);
        draw_circle(c6, green_tex, loc_tex, loc_transform, 1);
        draw_circle(c5, pink_tex, loc_tex, loc_transform, 0);
        draw_circle(c4, orange_tex, loc_tex, loc_transform, 1);
        draw_circle(c3, turq_tex, loc_tex, loc_transform, 0);
        draw_circle(c2, yellow_tex, loc_tex, loc_transform, 1);

        if ((int)glfwGetTime() % 2 == 0) {
            draw_circle(c1, centre1_tex, loc_tex, loc_transform, -1);
        } else {
            draw_circle(c1, centre2_tex, loc_tex, loc_transform, -1);
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
        
        glfwSwapBuffers(win);
    }

    // Tear-down
    chicken3421::delete_shader(vs); chicken3421::delete_shader(fs);

    return EXIT_SUCCESS;
}