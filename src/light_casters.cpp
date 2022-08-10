#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char * path);

// Settings
const unsigned int SCR_WIDTH {800};
const unsigned int SCR_HEIGHT {600};

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX {SCR_WIDTH/2}, lastY {SCR_HEIGHT/2};
bool firstMouse{true};

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Timing
float deltaTime {0.0f};
float lastFrame {0.0f};

int main()
{
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Tell GLFW to capture our mouse.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    // Shader pointlight_shader("../res/shaders/Materials-vertShader.vs", "../res/shaders/PointLight-fragShader.fs");
    // Shader dirlight_shader("../res/shaders/Materials-vertShader.vs", "../res/shaders/DirectionalLight-fragShader.fs");
    // Shader spotlight_shader("../res/shaders/Materials-vertShader.vs", "../res/shaders/Spotlight-fragShader.fs");
    Shader lighting_shader("../res/shaders/Materials-vertShader.vs", "../res/shaders/multiLights.fs");
    Shader lightcube_shader("../res/shaders/light-vertShader.vs", "../res/shaders/light_fragShader.fs");

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // World space positions of cubes
    glm::vec3 cubePositions[]{
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f) 
    };
    // Positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // Configure the cubes VAO and VBO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    
    // Configure the light's VAO, VBO stays the same but with different strides.
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute. Vertices now contains extra information, but we only want the vertex positions. Increase stride.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Load textures
    unsigned int diffuseMap = loadTexture("../res/textures/container2.png");
    unsigned int specularMap = loadTexture("../res/textures/container2_specular.png");

    // Shader config
    lighting_shader.use();
    lighting_shader.setInt("material.diffuse", 0);
    lighting_shader.setInt("material.specular", 1);

    while (!glfwWindowShouldClose(window))
    {
        // Per-frame time logic
        float currentFrame {static_cast<float>(glfwGetTime())};
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input handling
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lighting_shader.use();
        lighting_shader.setVec3("viewPos", camera.Position);
        lighting_shader.setFloat("material.shininess", 32.0f);

        // directional light
        lighting_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lighting_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lighting_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lighting_shader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lighting_shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lighting_shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.setFloat("pointLights[0].constant", 1.0f);
        lighting_shader.setFloat("pointLights[0].linear", 0.09f);
        lighting_shader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        lighting_shader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lighting_shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lighting_shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.setFloat("pointLights[1].constant", 1.0f);
        lighting_shader.setFloat("pointLights[1].linear", 0.09f);
        lighting_shader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        lighting_shader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lighting_shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lighting_shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.setFloat("pointLights[2].constant", 1.0f);
        lighting_shader.setFloat("pointLights[2].linear", 0.09f);
        lighting_shader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        lighting_shader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lighting_shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lighting_shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.setFloat("pointLights[3].constant", 1.0f);
        lighting_shader.setFloat("pointLights[3].linear", 0.09f);
        lighting_shader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        lighting_shader.setVec3("spotLight.position", camera.Position);
        lighting_shader.setVec3("spotLight.direction", camera.Front);
        lighting_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lighting_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lighting_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.setFloat("spotLight.constant", 1.0f);
        lighting_shader.setFloat("spotLight.linear", 0.09f);
        lighting_shader.setFloat("spotLight.quadratic", 0.032f);
        lighting_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lighting_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // View/projection properties
        glm::mat4 projection {glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f)};
        glm::mat4 view {camera.GetViewMatrix()};
        lighting_shader.setMat4("projection", projection); 
        lighting_shader.setMat4("view", view);

        // Model/normal matricies
        glm::mat4 model {glm::mat4(1.0f)};
        glm::mat3 normalMatrix = glm::inverse(glm::transpose(view * model));
        lighting_shader.setMat4("model", model);
        lighting_shader.setMat3("normalMatrix", normalMatrix);

        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Render the cube
        glBindVertexArray(cubeVAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lighting_shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object(s)
        lightcube_shader.use();
        lightcube_shader.setMat4("projection", projection);
        lightcube_shader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightcube_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Swap buffers and poll IO events.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;   // Necessary for callback to C implementation.
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    (void)window;   // Necessary for callback to C implementation.
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void)window;   // Necessary for callback to C implementation.
    (void)xoffset;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(const char * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format == GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
