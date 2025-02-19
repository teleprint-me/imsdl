#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

GLuint vbo;

SDL_Window* create_window(const char* title, int width, int height) {
    SDL_Window* window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    }

    return window;
}

void init_opengl(void) {
    // Vertex data (X, Y)
    float vertices[] = {
        100.0f,
        100.0f, // Top-left
        300.0f,
        100.0f, // Top-right
        300.0f,
        250.0f, // Bottom-right
        100.0f,
        250.0f // Bottom-left
    };

    // Create Vertex Buffer Object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
}

void draw(SDL_Window* window) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind VBO and Draw
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_QUADS, 0, 4);

    SDL_GL_SwapWindow(window);
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = create_window("imsdl Window", 800, 600);
    if (!window) {
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        fprintf(stderr, "SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize GLEW (Must be after SDL_GL context is created)
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW Init Failed: %s\n", glewGetErrorString(err));
        exit(1);
    }

    // Print OpenGL Info
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLEW Version: %s\n", glewGetString(GLEW_VERSION));

    // Check if GLEW loaded necessary functions
    if (!GLEW_VERSION_2_0) {
        printf("OpenGL 2.0+ is required but not supported!\n");
        exit(1);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);

    init_opengl(); // Initialize VBO

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        draw(window);
    }

    // Cleanup
    glDeleteBuffers(1, &vbo);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
