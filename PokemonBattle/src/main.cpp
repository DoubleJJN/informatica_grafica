#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera.h>

void configScene();
void renderScene();
void setLights (glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos      (GLFWwindow* window, double xposIn, double yposIn);
void flotarYGirar(float times);

unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);

// Shaders
Shaders shaders;
Shaders skyboxShaders;

// Modelos
Model sphere;
Model plane;
Model cube;
Model grass;
Model Voltorb;
Model Mimikyu;
Model Pokeball;

// Imagenes (texturas)
Texture imgNoEmissive;
Texture imgRuby;
Texture imgGold;
Texture imgEarth;
Texture imgChess;
Texture imgCubeDiffuse;
Texture imgCubeSpecular;
Texture imgWindow;
Texture imgWallDiffuse;
Texture imgWallSpecular;
Texture imgWallNormal;
Texture imgGrassDiffuse;
Texture imgVoltorbDiffuse, imgVoltorbNormal, imgVoltorbSpecular;
Texture imgMimikyu;
Texture imgPokeball,imgwhite;

// Luces y materiales
#define   NLD 1
#define   NLP 2
#define   NLF 3
Light     lightG;
Light     lightD[NLD];
Light     lightP[NLP];
Light     lightF[NLF];
Material  mluz;
Material  ruby;
Material  gold;
Textures  texRuby;
Textures  texGold;
Textures  texEarth;
Textures  texChess;
Textures  texCube;
Textures  texWindow;
Textures  texWall;
Textures  texGrass;
Textures texVoltorb;
Textures texMimikyu;
Textures texPokeball;

// Viewport
int w = 700;
int h = 700;

// Animaciones
float rotX = 0.0;
float rotY = 0.0;
float desZ = 0.0;

// Movimiento de camara
float fovy   = 60.0;
float alphaX =  0.0;
float alphaY =  0.0;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)w / 2.0;
float lastY = (float)h / 2.0;
bool firstMouse = true;

// Movimiento de Voltorb
float flotar = 1.0;
float girar = 0.0;
bool subir = true;
bool palante = true;

// Tiempo
float milisecond = 0.07;

unsigned int skyboxVAO, skyboxVBO;
unsigned int cubeTexture, cubemapTexture;

float x, y, z;


float cubeVertices[] = {
   // positions          // texture Coords
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
   
float skyboxVertices[] = {
   // positions          
   -1.0f,  1.0f, -1.0f,
   -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   -1.0f,  1.0f, -1.0f,

   -1.0f, -1.0f,  1.0f,
   -1.0f, -1.0f, -1.0f,
   -1.0f,  1.0f, -1.0f,
   -1.0f,  1.0f, -1.0f,
   -1.0f,  1.0f,  1.0f,
   -1.0f, -1.0f,  1.0f,

   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,

   -1.0f, -1.0f,  1.0f,
   -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
   -1.0f, -1.0f,  1.0f,

   -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   -1.0f,  1.0f,  1.0f,
   -1.0f,  1.0f, -1.0f,

   -1.0f, -1.0f, -1.0f,
   -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f
};

int main()
{
   // Inicializamos GLFW
   if (!glfwInit())
      return -1;
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

   // Creamos la ventana
   GLFWwindow *window;
   window = glfwCreateWindow(w, h, "Pokemon Battle", NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSwapInterval(1);

   // Inicializamos GLEW
   glewExperimental = GL_TRUE;
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      std::cout << "Error: " << glewGetErrorString(err) << std::endl;
      return -1;
   }
   std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
   const GLubyte *oglVersion = glGetString(GL_VERSION);
   std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

   // Configuramos los CallBacks
   glfwSetFramebufferSizeCallback(window, funFramebufferSize);
   glfwSetKeyCallback(window, funKey);
   glfwSetScrollCallback(window, funScroll);
   glfwSetCursorPosCallback(window, funCursorPos);

   // Entramos en el bucle de renderizado
   configScene();

   while (!glfwWindowShouldClose(window))
   {
      renderScene();
      glfwSwapBuffers(window);
      glfwPollEvents();
      flotarYGirar(milisecond);
   }

   glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
   glfwDestroyWindow(window);
   glDeleteVertexArrays(1, &skyboxVAO);
   glDeleteBuffers(1, &skyboxVBO);
   // Reponer si es necesario el borrado de cube
   glfwTerminate();

   return 0;
}

void configScene() {

 // Test de profundidad
    glEnable(GL_DEPTH_TEST);

 // Transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 // Shaders
   shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");
   skyboxShaders.initShaders("resources/shaders/skybox_vshader.glsl","resources/shaders/skybox_fshader.glsl");

 // Modelos
    sphere.initModel("resources/models/sphere.obj");
    plane.initModel("resources/models/plane.obj");
    cube.initModel("resources/models/cube.obj");
   grass.initModel("resources/models/grass.obj");
   Voltorb.initModel("resources/models/voltorb.obj");
   Mimikyu.initModel("resources/models/mimikyu.obj");
   Pokeball.initModel("resources/models/Pokeball_Obj.obj");

   // Imagenes (texturas)
   imgNoEmissive.initTexture("resources/textures/imgNoEmissive.png");
   imgRuby.initTexture("resources/textures/imgRuby.png");
   imgGold.initTexture("resources/textures/imgGold.png");
   imgEarth.initTexture("resources/textures/imgEarth.png");
   imgChess.initTexture("resources/textures/imgChess.png");
   imgCubeDiffuse.initTexture("resources/textures/imgCubeDiffuse.png");
   imgCubeSpecular.initTexture("resources/textures/imgCubeSpecular.png");
   imgWindow.initTexture("resources/textures/imgWindow.png");
   imgWallDiffuse.initTexture("resources/textures/imgWallDiffuse.png");
   imgWallSpecular.initTexture("resources/textures/imgWallSpecular.png");
   imgWallNormal.initTexture("resources/textures/imgWallNormal.png");
   imgGrassDiffuse.initTexture("resources/textures/grass_difuse.jpg");
   imgVoltorbDiffuse.initTexture("resources/textures/voltorb_diffuse.png");
   imgVoltorbNormal.initTexture("resources/textures/voltorb_normal.png");
   imgVoltorbSpecular.initTexture("resources/textures/voltorb_specular.png");
   imgMimikyu.initTexture("resources/textures/mimikyu.png");

   imgPokeball.initTexture("resources/textures/p.png");
   imgwhite.initTexture("resources/textures/GTex.png");
   glGenVertexArrays(1, &skyboxVAO);
   glGenBuffers(1, &skyboxVBO);
   glBindVertexArray(skyboxVAO);
   glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

   std::vector<std::string> faces{
       "resources/textures/skybox/px.png",
       "resources/textures/skybox/nx.png",
       "resources/textures/skybox/py.png",
       "resources/textures/skybox/ny.png",
       "resources/textures/skybox/pz.png",
       "resources/textures/skybox/nz.png"};
   cubemapTexture = loadCubemap(faces);

   skyboxShaders.useShaders();
   skyboxShaders.setFloat("skybox", 0.0);

   // Luz ambiental global
   lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

   // Luces direccionales
   lightD[0].direction = glm::vec3(-1.0, 0.0, 0.0);
   lightD[0].ambient = glm::vec3(0.1, 0.1, 0.1); //0.1
   lightD[0].diffuse = glm::vec3(0.7, 0.7, 0.7); //0.7
   lightD[0].specular = glm::vec3(0.7, 0.7, 0.7); //0.7

   // Luces posicionales
   lightP[0].position = glm::vec3(0.0, 3.0, 3.0);
   lightP[0].ambient = glm::vec3(0.0, 0.0, 0.0); //0.2
   lightP[0].diffuse = glm::vec3(0.0, 0.0, 0.0); //0.9
   lightP[0].specular = glm::vec3(0.0, 0.0, 0.0); //0.9
   lightP[0].c0 = 1.00;
   lightP[0].c1 = 0.22;
   lightP[0].c2 = 0.20;

   // Luces focales
   lightF[0].position = glm::vec3(-2.0, 2.0, 5.0);
   lightF[0].direction = glm::vec3(2.0, -2.0, -5.0);
   lightF[0].ambient = glm::vec3(0.0, 0.0, 0.0); //0.2
   lightF[0].diffuse = glm::vec3(0.0, 0.0, 0.0); //0.9
   lightF[0].specular = glm::vec3(0.0, 0.0, 0.0); //0.9
   lightF[0].innerCutOff = 10.0;
   lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
   lightF[0].c0 = 1.000;
   lightF[0].c1 = 0.090;
   lightF[0].c2 = 0.032;
   lightF[1].position = glm::vec3(2.0, 2.0, 5.0);
   lightF[1].direction = glm::vec3(-2.0, -2.0, -5.0);
   lightF[1].ambient = glm::vec3(0.0, 0.0, 0.0); //0.2
   lightF[1].diffuse = glm::vec3(0.0, 0.0, 0.0); //0.9
   lightF[1].specular = glm::vec3(0.0, 0.0, 0.0); //0.9
   lightF[1].innerCutOff = 5.0;
   lightF[1].outerCutOff = lightF[1].innerCutOff + 1.0;
   lightF[1].c0 = 1.000;
   lightF[1].c1 = 0.090;
   lightF[1].c2 = 0.032;

   //de pokeball
   lightF[2].position = glm::vec3(-2.0, 0.0, 3.0);
   lightF[2].direction = glm::vec3(2.0, -2.0, -5.0);
   lightF[2].ambient = glm::vec3(0.2, 0.2, 0.2);
   lightF[2].diffuse = glm::vec3(0.9, 0.9, 0.9);
   lightF[2].specular = glm::vec3(1.0, 1.0, 1.0);
   lightF[2].innerCutOff = 50.0;
   lightF[2].outerCutOff = lightF[0].innerCutOff + 5.0;
   lightF[2].c0 = 1.000;
   lightF[2].c1 = 0.090;
   lightF[2].c2 = 0.032;

   // Materiales
   mluz.ambient = glm::vec4(0.0, 0.0, 0.0, 1.0);
   mluz.diffuse = glm::vec4(0.0, 0.0, 0.0, 1.0);
   mluz.specular = glm::vec4(0.0, 0.0, 0.0, 1.0);
   mluz.emissive = glm::vec4(1.0, 1.0, 1.0, 1.0);
   mluz.shininess = 1.0;

   ruby.ambient = glm::vec4(0.174500, 0.011750, 0.011750, 0.55);
   ruby.diffuse = glm::vec4(0.614240, 0.041360, 0.041360, 0.55);
   ruby.specular = glm::vec4(0.727811, 0.626959, 0.626959, 0.55);
   ruby.emissive = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
   ruby.shininess = 76.8;

   gold.ambient = glm::vec4(0.247250, 0.199500, 0.074500, 1.00);
   gold.diffuse = glm::vec4(0.751640, 0.606480, 0.226480, 1.00);
   gold.specular = glm::vec4(0.628281, 0.555802, 0.366065, 1.00);
   gold.emissive = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
   gold.shininess = 51.2;

   texRuby.diffuse = imgRuby.getTexture();
   texRuby.specular = imgRuby.getTexture();
   texRuby.emissive = imgNoEmissive.getTexture();
   texRuby.normal = 0;
   texRuby.shininess = 76.8;

   texGold.diffuse = imgGold.getTexture();
   texGold.specular = imgGold.getTexture();
   texGold.emissive = imgNoEmissive.getTexture();
   texGold.normal = 0;
   texGold.shininess = 51.2;

   texEarth.diffuse = imgEarth.getTexture();
   texEarth.specular = imgEarth.getTexture();
   texEarth.emissive = imgNoEmissive.getTexture();
   texEarth.normal = 0;
   texEarth.shininess = 10.0;

   texChess.diffuse = imgChess.getTexture();
   texChess.specular = imgChess.getTexture();
   texChess.emissive = imgNoEmissive.getTexture();
   texChess.normal = 0;
   texChess.shininess = 10.0;

   texCube.diffuse = imgCubeDiffuse.getTexture();
   texCube.specular = imgCubeSpecular.getTexture();
   texCube.emissive = imgNoEmissive.getTexture();
   texCube.normal = 0;
   texCube.shininess = 10.0;

   texWindow.diffuse = imgWindow.getTexture();
   texWindow.specular = imgWindow.getTexture();
   texWindow.emissive = imgWindow.getTexture();
   texWindow.normal = 0;
   texWindow.shininess = 10.0;

   texWall.diffuse = imgWallDiffuse.getTexture();
   texWall.specular = imgWallSpecular.getTexture();
   texWall.emissive = imgNoEmissive.getTexture();
   texWall.normal = imgWallNormal.getTexture();
   texWall.shininess = 51.2;

   texGrass.diffuse   = imgGrassDiffuse.getTexture();
   texGrass.normal    = 0;
   texGrass.shininess = 10.0;

   texVoltorb.diffuse = imgVoltorbDiffuse.getTexture();
   texVoltorb.specular = imgVoltorbSpecular.getTexture();
   texVoltorb.normal = imgVoltorbNormal.getTexture();
   texVoltorb.emissive = 1;
   texVoltorb.shininess = 60.0;

   texMimikyu.diffuse = imgMimikyu.getTexture();
   texMimikyu.specular = imgMimikyu.getTexture();
   texMimikyu.normal = 0;
   texMimikyu.emissive = 0;
   texMimikyu.shininess = 50.0;

   texPokeball.diffuse = imgPokeball.getTexture();
   texPokeball.specular = imgwhite.getTexture();
   texPokeball.emissive = imgGold.getTexture();
   texPokeball.normal = imgPokeball.getTexture();
   texPokeball.shininess = 50.0;

   x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
   y = 10.0f*glm::sin(glm::radians(alphaY));
   z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
   camera.Position = glm::vec3(2.0f, 2.5f, 11.0f);
   camera.Front = glm::vec3(-0.2f, -0.1f, -1.0f);
}

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
   shaders.useShaders();

 // Matriz P
   float nplane =  0.1;
   float fplane = 25.0;
   float aspect = (float)w/(float)h;
   glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

   // Matriz V
   /*float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
   float y = 10.0f*glm::sin(glm::radians(alphaY));
   float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));*/
   //glm::vec3 eye   (  x,   y,   z);
   //glm::vec3 center(0.0, 0.0,  0.0);
   //glm::vec3 up    (0.0, 1.0,  0.0);
   //glm::mat4 V = glm::lookAt(eye, center, up);
   glm::mat4 V = camera.GetViewMatrix();
   camera.Zoom = fovy;
   //shaders.setVec3("ueye",eye);
   shaders.setMat4("uV",V);

   // Fijamos las luces
   setLights(P,V);

   // Plataformas
   glm::mat4 S = glm::scale    (I, glm::vec3(0.015, 0.015, 0.015));
   glm::mat4 T = glm::translate(I, glm::vec3(4.0, 0.0, -3.0));
   glm::mat4 R = glm::rotate   (I, glm::radians(-90.0f), glm::vec3(1,0,0));
   drawObjectTex(grass, texGrass, P, V, T * S * R);
   T = glm::translate(I, glm::vec3(-2.0, 0.0, 3.0));
   drawObjectTex(grass, texGrass, P, V, T * S * R);

   //Pokemon Voltorb
   glm::mat4 SV = glm::scale(I, glm::vec3(0.3, 0.3, 0.3));
   glm::mat4 TV = glm::translate(I, glm::vec3(4.0, flotar + 0.7, -3.0));
   glm::mat4 RVy = glm::rotate(I, glm::radians(-30.0f), glm::vec3(0, 1, 0));
   glm::mat4 RVx = glm::rotate(I, glm::radians(girar), glm::vec3(1, 0, 0));
   drawObjectTex(Voltorb, texVoltorb, P, V, RVx * TV * SV * RVy);

   // Mimikyu
   glm::mat4 SM = glm::scale(I, glm::vec3(1.5, 1.5, 1.5));
   glm::mat4 TM = glm::translate(I, glm::vec3(-2.0, flotar - 0.4, 3.0));
   glm::mat4 RMy = glm::rotate(I, glm::radians(30.0f), glm::vec3(0, 1, 0));
   glm::mat4 RMx = glm::rotate(I, glm::radians(girar), glm::vec3(1, 0, 0));
   drawObjectTex(Mimikyu, texMimikyu, P, V, RMx * TM * SM * RMy);

   //Pokeball
   glm::vec3 cesped_scale = glm::vec3(0.01, 0.01, 0.01);
   glm::mat4 S_cesped = glm::scale(glm::mat4(1.0f), cesped_scale);

   glm::vec3 figura_scale = glm::vec3(11.54 / 7236.07, 286.81 / 8000.0, 286.81 / 7608.45);
   glm::mat4 S_figura = glm::scale(I, figura_scale);
   glm::mat4 TP = glm::translate(I, glm::vec3(-2.0, 0.2, 3.0));
   glm::mat4 RP = glm::rotate(I, glm::radians(-90.0f), glm::vec3(0, 0, 1));
   glm::mat4 RP2 = glm::rotate(I, glm::radians(-30.0f), glm::vec3(1, 0, 0));
   glm::mat4 figura_transform = TP * RP * RP2 * S_cesped * S_figura;

   glm::mat4 Tx = glm::translate(I, glm::vec3(4.0, 0.2, -3.0));
   glm::mat4 figura_transform2 = Tx * RP * RP2 * S_cesped * S_figura;
   drawObjectTex(Pokeball, texPokeball, P, V, figura_transform);
   drawObjectTex(Pokeball, texPokeball, P, V, figura_transform2);

   //Shadow ball
   glm::mat4 S1 = glm::scale(I, glm::vec3(4, 4, 4));
   glm::mat4 T1 = glm::translate(I, glm::vec3(3.5, -2.0, -3.0));
   glm::mat4 R1 = glm::rotate(I, glm::radians(0.0f), glm::vec3(1, 0, 0));
   drawObjectTex(sphere, texGold, P, V, R * T * S);

   // draw skybox as last
   glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
   skyboxShaders.useShaders();
   V = glm::mat4(glm::mat3(camera.GetViewMatrix()));
   skyboxShaders.setMat4("view", V);
   skyboxShaders.setMat4("projection", P);
   // skybox cube
   glBindVertexArray(skyboxVAO);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
   glDrawArrays(GL_TRIANGLES, 0, 36);
   glBindVertexArray(0);
   glDepthFunc(GL_LESS); // set depth function back to default

}

void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    /*for(int i=0; i<NLP; i++) {
        glm::mat4 M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.1));
        drawObjectMat(sphere, mluz, P, V, M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.025));
        drawObjectMat(sphere, mluz, P, V, M);
    }*/

}

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0) shaders.setBool("uWithNormals",true);
    else                   shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);

}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) {
        /*case GLFW_KEY_UP:    rotX -= 5.0f;   break;
        case GLFW_KEY_DOWN:  rotX += 5.0f;   break;
        case GLFW_KEY_LEFT:  rotY -= 5.0f;   break;
        case GLFW_KEY_RIGHT: rotY += 5.0f;   break;
        case GLFW_KEY_Z:
            if(mods==GLFW_MOD_SHIFT) desZ -= desZ > -24.0f ? 0.1f : 0.0f;
            else                     desZ += desZ <   5.0f ? 0.1f : 0.0f;
            break;
        default:
            rotX = 0.0f;
            rotY = 0.0f;
            break;*/
         case GLFW_KEY_W:  camera.ProcessKeyboard(FORWARD, milisecond); break;
         case GLFW_KEY_S:  camera.ProcessKeyboard(BACKWARD, milisecond); break;
         case GLFW_KEY_A:  camera.ProcessKeyboard(LEFT, milisecond); break;
         case GLFW_KEY_D:  camera.ProcessKeyboard(RIGHT, milisecond); break;
         case GLFW_KEY_Q:  camera.ProcessKeyboard(UP, milisecond); break;
         case GLFW_KEY_E:  camera.ProcessKeyboard(DOWN, milisecond); break;
    }

}

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {

    if(yoffset>0) fovy -= fovy>10.0f ? 5.0f : 0.0f;
    if(yoffset<0) fovy += fovy<90.0f ? 5.0f : 0.0f;
   //camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void funCursorPos(GLFWwindow* window, double xposIn, double yposIn) {

   if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) {
      firstMouse = true;
      return;
   }

    /*float limY = 89.0;
    alphaX = 90.0*(2.0*xpos/(float)w - 1.0);
    alphaY = 90.0*(1.0 - 2.0*ypos/(float)h);
    if(alphaY<-limY) alphaY = -limY;
    if(alphaY> limY) alphaY =  limY;*/

   float xpos = static_cast<float>(xposIn);
   float ypos = static_cast<float>(yposIn);
   if (firstMouse)
   {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
      return;
   }

   float xoffset = xpos - lastX;
   float yoffset = lastY - ypos;

   lastX = xpos;
   lastY = ypos;

   camera.ProcessMouseMovement(xoffset, yoffset);
}

void flotarYGirar(float times){
   if (glfwGetTime() > times)
   {
      if(subir){
         if(girar<=10)
            girar += 1;
         else
            subir = false;
      }
      else{
         if (girar >= 0)
            girar -= 1;
         else
            subir = true;
      }
      if(palante){
         if(flotar<=1.5)
            flotar += 0.1;
         else
            palante = false;
      }else{
         if(flotar>=1)
            flotar -= 0.1;
         else
            palante = true;
      }
      glfwSetTime(0.0);
   }
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
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
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
