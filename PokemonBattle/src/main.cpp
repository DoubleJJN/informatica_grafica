#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera/camera.h"

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
void lanzar();

unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);

void drawMimikyu(glm::mat4 P, glm::mat4 V, glm::mat4 S, glm::mat4 T, glm::mat4 R);
void dibujarShadowBall(glm::mat4 P, glm::mat4 V);

// Shaders
Shaders shaders;
Shaders skyboxShaders;

// Modelos
Model sphere;
Model grass;
Model Voltorb;
Model Mimikyu;
Model Pokeball, p;
Model cone;

// Imagenes (texturas)
Texture imgNoEmissive;
Texture imgGold;
Texture imgGrassDiffuse;
Texture imgVoltorbDiffuse, imgVoltorbNormal, imgVoltorbSpecular;
Texture imgMimikyu;
Texture imgPokeball,imgwhite;
Texture imgBrazo;
Texture imgShadowBallExt, imgShadowBallInt, imgShadowBall;

// Luces y materiales
#define   NLD 1
#define   NLP 2
#define   NLF 4
Light     lightG;
Light     lightD[NLD];
Light     lightP[NLP];
Light     lightF[NLF];
Material  mluz;
Textures  texGrass;
Textures texVoltorb;
Textures texMimikyu;
Textures texPokeball, pw;
Textures texBrazo;
Textures texShadowBallExt, texShadowBallInt, texShadowBall;

// Viewport
int w = 700;
int h = 700;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float fovy   = 60.0;
float lastX = (float)w / 2.0;
float lastY = (float)h / 2.0;
bool firstMouse = true;

// Movimiento de Voltorb
float flotar = 1.0;
float girar = 0.0;
float girar2 = 0.0f;
float subirDz = 0.0f;
float subirDy = 0.0f;
bool derecha = true;
bool subir = true;
bool palante = true;

// Tiempo
float milisecond = 0.07;

// Skybox
unsigned int skyboxVAO, skyboxVBO;
unsigned int cubeTexture, cubemapTexture;

// Movimiento de focales
float lMovex=0.0;
float lMovey = 5.0;
float lMovez = 0.0;

// Bola sombra
float x1 = -0.1;
float x2 = -0.7;
float x3 = -1.1;
float x4 = -0.7;
float ballZ = 0.0;
bool permitirLanzar = false;
bool brazosSubidos = false;

// Vertices cubemap
float cubeVertices[] = {
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
   
// Vertices skybox
float skyboxVertices[] = {         
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
      // Movimiento de Voltorb
      flotarYGirar(milisecond);

      // Lanzamiento bola sombra
      if (permitirLanzar){
         lanzar();
      }
      if(x1>=4.8f){
         permitirLanzar = false;
         brazosSubidos = false;
         x1 = -0.1;
         x2 = -0.7;
         x3 = -1.1;
         x4 = -0.7;
         ballZ = 0.0;
      }
   }

   glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
   glfwDestroyWindow(window);
   glDeleteVertexArrays(1, &skyboxVAO);
   glDeleteBuffers(1, &skyboxVBO);
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
   grass.initModel("resources/models/grass.obj");
   Voltorb.initModel("resources/models/voltorb.obj");
   Mimikyu.initModel("resources/models/mimikyu.obj");
   Pokeball.initModel("resources/models/Pokeball_Obj.obj");
   p.initModel("resources/models/Pokeball_Obj.obj");
   cone.initModel("resources/models/cone.obj");

   // Imagenes (texturas)
   imgNoEmissive.initTexture("resources/textures/imgNoEmissive.png");
   imgGold.initTexture("resources/textures/imgGold.png");
   imgGrassDiffuse.initTexture("resources/textures/grass_difuse.jpg");
   imgVoltorbDiffuse.initTexture("resources/textures/voltorb_diffuse.png");
   imgVoltorbNormal.initTexture("resources/textures/voltorb_normal.png");
   imgVoltorbSpecular.initTexture("resources/textures/voltorb_specular.png");
   imgMimikyu.initTexture("resources/textures/mimikyu.png");
   imgBrazo.initTexture("resources/textures/brazo.png");
   imgShadowBallExt.initTexture("resources/textures/shadowBallExt.png");
   imgShadowBallInt.initTexture("resources/textures/shadowBallInt.png");
   imgShadowBall.initTexture("resources/textures/shadowBall.png");
   imgPokeball.initTexture("resources/textures/p.png");
   imgwhite.initTexture("resources/textures/GTex.png");

   // Skybox
   glGenVertexArrays(1, &skyboxVAO);
   glGenBuffers(1, &skyboxVBO);
   glBindVertexArray(skyboxVAO);
   glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

   // Texturas cubemap
   std::vector<std::string> faces{
       "resources/textures/centroPokemon/px.png",
       "resources/textures/centroPokemon/nx.png",
       "resources/textures/centroPokemon/py.png",
       "resources/textures/centroPokemon/ny.png",
       "resources/textures/centroPokemon/pz.png",
       "resources/textures/centroPokemon/nz.png"};
   cubemapTexture = loadCubemap(faces);

   // configuración shader del skybox
   skyboxShaders.useShaders();
   skyboxShaders.setFloat("skybox", 0.0);

   // Luz ambiental global
   lightG.ambient = glm::vec3(0.3, 0.3, 0.3);

   // Luces direccionales
   lightD[0].direction = glm::vec3(-1.0, 0.0, 0.0);
   lightD[0].ambient = glm::vec3(0.1, 0.1, 0.1); //0.1
   lightD[0].diffuse = glm::vec3(0.7, 0.7, 0.7); //0.7
   lightD[0].specular = glm::vec3(0.7, 0.7, 0.7); //0.7

   // Luces posicionales
   lightP[0].position = glm::vec3(4.0, 5.0, -3.0);
   lightP[0].ambient = glm::vec3(0.0, 0.0, 0.0); //0.2
   lightP[0].diffuse = glm::vec3(0.0, 0.0, 0.0); //0.9
   lightP[0].specular = glm::vec3(0, 0, 0); //0.9
   lightP[0].c0 = 1.00;
   lightP[0].c1 = 0.22;
   lightP[0].c2 = 0.20;
   
   // Luces focales
   lightF[0].position = glm::vec3(lMovex, lMovey, lMovez);
   lightF[0].direction = glm::vec3(lMovex, -lMovey, lMovez);
   lightF[0].ambient = glm::vec3(0.2, 0.2, 0.2);
   lightF[0].diffuse = glm::vec3(0.9, 0.9, 0.9);
   lightF[0].specular = glm::vec3(1.0, 1.0, 1.0);

   lightF[0].innerCutOff = 50.0;
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
   lightF[2].ambient = glm::vec3(0.0, 0.0, 0.0);
   lightF[2].diffuse = glm::vec3(0.0, 0.0, 0.0);
   lightF[2].specular = glm::vec3(0.0, 0.0, 0.0);
   lightF[2].innerCutOff = 50.0;
   lightF[2].outerCutOff = lightF[2].innerCutOff + 5.0;
   lightF[2].c0 = 1.000;
   lightF[2].c1 = 0.090;
   lightF[2].c2 = 0.032;

   lightF[3].position = glm::vec3(4.0, 0.0, -3.0);
   lightF[3].direction = glm::vec3(2.0, -2.0, -5.0);
   lightF[3].ambient = glm::vec3(0.0, 0.0, 0.0);
   lightF[3].diffuse = glm::vec3(0.0, 0.0, 0.0);
   lightF[3].specular = glm::vec3(0.0, 0.0, 0.0);
   lightF[3].innerCutOff = 50.0;
   lightF[3].outerCutOff = lightF[3].innerCutOff + 5.0;
   lightF[3].c0 = 1.000;
   lightF[3].c1 = 0.090;
   lightF[3].c2 = 0.032;

   // Materiales
   mluz.ambient = glm::vec4(0.0, 0.0, 0.0, 1.0);
   mluz.diffuse = glm::vec4(0.0, 0.0, 0.0, 1.0);
   mluz.specular = glm::vec4(0.0, 0.0, 0.0, 1.0);
   mluz.emissive = glm::vec4(1.0, 1.0, 1.0, 1.0);
   mluz.shininess = 1.0;

   texGrass.diffuse   = imgGrassDiffuse.getTexture();
   texGrass.normal    = 0;
   texGrass.shininess = 10.0;

   texVoltorb.diffuse = imgVoltorbDiffuse.getTexture();
   texVoltorb.specular = imgVoltorbSpecular.getTexture();
   //texVoltorb.normal = imgVoltorbNormal.getTexture();
   texVoltorb.emissive = 0;
   texVoltorb.shininess = 50.0;

   texMimikyu.diffuse = imgMimikyu.getTexture();
   texMimikyu.specular = 0;
   texMimikyu.normal = 0;
   texMimikyu.emissive = 0;
   texMimikyu.shininess = 50.0;

   texPokeball.diffuse = imgPokeball.getTexture();
   texPokeball.specular = imgwhite.getTexture();
   texPokeball.emissive = 0;
   texPokeball.normal = imgPokeball.getTexture();
   texPokeball.shininess = 50.0;

   texBrazo.diffuse = imgBrazo.getTexture();
   texBrazo.specular = imgBrazo.getTexture();
   texBrazo.emissive = 0;
   texBrazo.shininess = 50.0;

   camera.Position = glm::vec3(2.0f, 2.5f, 11.0f);
   camera.Front = glm::vec3(-0.2f, -0.1f, -1.0f);  

   texShadowBallExt.diffuse = imgShadowBallExt.getTexture();
   texShadowBallExt.emissive = 0;
   texShadowBallExt.shininess = 50.0;

   texShadowBallInt.diffuse = imgShadowBallInt.getTexture();
   texShadowBallInt.emissive = 0;
   texShadowBallInt.shininess = 50.0;

   texShadowBall.diffuse = imgShadowBall.getTexture();
   texShadowBall.emissive = 0;
   texShadowBall.shininess = 50.0;
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
   glm::mat4 V = camera.GetViewMatrix();
   camera.Zoom = fovy;
   shaders.setMat4("uV",V);

   // Fijamos las luces
   setLights(P,V);

   // Bola sombra
   if (brazosSubidos){
      dibujarShadowBall(P, V);
   }
      
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
   glm::mat4 RVy = glm::rotate(I, glm::radians(-40.0f), glm::vec3(0, 1, 0));
   glm::mat4 RVx = glm::rotate(I, glm::radians(girar), glm::vec3(1, 0, 0));
   drawObjectTex(Voltorb, texVoltorb, P, V, RVx * TV * SV * RVy);

   // Mimikyu
   drawMimikyu(P, V, glm::scale(I, glm::vec3(1.5, 1.5, 1.5)), glm::translate(I, glm::vec3(-2.0, 0.5, 3.0)), glm::rotate(I, glm::radians(40.0f), glm::vec3(0, 1, 0)));
   
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

   // Renderizar skybox
   glDepthFunc(GL_LEQUAL);
   skyboxShaders.useShaders();
   V = glm::mat4(glm::mat3(camera.GetViewMatrix()));
   skyboxShaders.setMat4("view", V);
   skyboxShaders.setMat4("projection", P);
   glBindVertexArray(skyboxVAO);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
   glDrawArrays(GL_TRIANGLES, 0, 36);
   glBindVertexArray(0);
   glDepthFunc(GL_LESS);
}

// Dibuja la bola sombra
void dibujarShadowBall(glm::mat4 P, glm::mat4 V)
{
   glm::mat4 T1 = glm::translate(I, glm::vec3(x1, 2.1, 1.2 + ballZ));
   glm::mat4 S1 = glm::scale(I, glm::vec3(0.05, 0.05, 0.05));
   drawObjectTex(sphere, texShadowBall, P, V, T1 * S1);
   glm::mat4 T2 = glm::translate(I, glm::vec3(x2, 2.6, 0.6 + ballZ));
   drawObjectTex(sphere, texShadowBall, P, V, T2 * S1);
   glm::mat4 T3 = glm::translate(I, glm::vec3(x3, 3.3, 1.9 + ballZ));
   drawObjectTex(sphere, texShadowBall, P, V, T3 * S1);
   S1 = glm::scale(I, glm::vec3(0.1, 0.1, 0.1));
   glm::mat4 T4 = glm::translate(I, glm::vec3(x4, 2.7, 1.4 + ballZ));
   drawObjectTex(sphere, texShadowBall, P, V, T4 * S1);
   S1 = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   drawObjectTex(sphere, texShadowBallExt, P, V, T4 * S1);
   S1 = glm::scale(I, glm::vec3(0.4, 0.4, 0.4));
   drawObjectTex(sphere, texShadowBallInt, P, V, T4 * S1);
   glDisable(GL_BLEND);
}

void setLights(glm::mat4 P, glm::mat4 V) {

   shaders.setLight("ulightG", lightG);
   for (int i = 0; i < NLD; i++)
      shaders.setLight("ulightD[" + toString(i) + "]", lightD[i]);
   for (int i = 0; i < NLP; i++)
      shaders.setLight("ulightP[" + toString(i) + "]", lightP[i]);
   for (int i = 0; i < NLF; i++)
      shaders.setLight("ulightF[" + toString(i) + "]", lightF[i]);

   lightF[0].position = glm::vec3(lMovex, lMovey, lMovez);
   glm::mat4 M = glm::translate(I, lightF[0].position) * glm::scale(I, glm::vec3(0.1));
   drawObjectMat(sphere, mluz, P, V, M);
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

void drawMimikyu(glm::mat4 P, glm::mat4 V, glm::mat4 S, glm::mat4 T, glm::mat4 R) {
   drawObjectTex(Mimikyu, texMimikyu, P, V, T * S * R);
   //Brazo derecho
   glm::mat4 TD = glm::translate(I, glm::vec3(-1.53, 0.95 + subirDy, 3.18));
   glm::mat4 SB = glm::scale(I, glm::vec3(0.07, 0.3, 0.07));
   glm::mat4 RD = glm::rotate(I, glm::radians(-100.0f + subirDz), glm::vec3(0, 0, 1));
   glm::mat4 RD2 = glm::rotate(I, glm::radians(-27.0f), glm::vec3(0, 1, 0));
   drawObjectTex(cone, texBrazo, P, V, TD * RD2 * RD * SB);
   //Brazo izquierdo
   glm::mat4 TD2 = glm::translate(I, glm::vec3(-1.53, 0.95 + subirDy, 3.18));
   glm::mat4 Sx2 = glm::scale(I, glm::vec3(0.07, 0.3, 0.07));
   glm::mat4 RD12 = glm::rotate(I, glm::radians(-100.0f + subirDz), glm::vec3(1, 0, 0));
   glm::mat4 RD22 = glm::rotate(I, glm::radians(-27.0f), glm::vec3(0, 1, 0));
   glm::mat4 TD23 = glm::translate(I, glm::vec3(-0.35, 0, -0.69));
   drawObjectTex(cone, texBrazo, P, V, TD23 *TD2 * RD22 * RD12 * Sx2);
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
         //Movimiento de la luz
         case GLFW_KEY_LEFT: if (lMovex > -15.0) lMovex -= 0.1; break;
         case GLFW_KEY_RIGHT: if (lMovex < 15.0) lMovex += 0.1; break;
         case GLFW_KEY_UP: if (lMovez < 15.0) lMovez -= 0.1; break;
         case GLFW_KEY_DOWN: if (lMovez > -15.0) lMovez += 0.1; break;
         //Movimiento de la camara
         case GLFW_KEY_W:  camera.ProcessKeyboard(FORWARD, milisecond); break;
         case GLFW_KEY_S:  camera.ProcessKeyboard(BACKWARD, milisecond); break;
         case GLFW_KEY_A:  camera.ProcessKeyboard(LEFT, milisecond); break;
         case GLFW_KEY_D:  camera.ProcessKeyboard(RIGHT, milisecond); break;
         case GLFW_KEY_Q:  camera.ProcessKeyboard(UP, milisecond); break;
         case GLFW_KEY_E:  camera.ProcessKeyboard(DOWN, milisecond); break;
         // Apagar o encender luz pokeball
         case GLFW_KEY_O:  texPokeball.emissive = 0;
                           lightF[2].ambient = glm::vec3(0.0, 0.0, 0.0);
                           lightF[2].diffuse = glm::vec3(0.0, 0.0, 0.0);
                           lightF[2].specular = glm::vec3(0.0, 0.0, 0.0);
                           lightF[3].ambient = glm::vec3(0.0, 0.0, 0.0);
                           lightF[3].diffuse = glm::vec3(0.0, 0.0, 0.0);
                           lightF[3].specular = glm::vec3(0.0, 0.0, 0.0);
                           break;
         case GLFW_KEY_I:  texPokeball.emissive = imgGold.getTexture();
            lightF[2].ambient = glm::vec3(0.2, 0.2, 0.2);
            lightF[2].diffuse = glm::vec3(0.9, 0.9, 0.9);
            lightF[2].specular = glm::vec3(1.0, 1.0, 1.0);
            lightF[3].ambient = glm::vec3(0.2, 0.2, 0.2);
            lightF[3].diffuse = glm::vec3(0.9, 0.9, 0.9);
            lightF[3].specular = glm::vec3(1.0, 1.0, 1.0);
            break;
         // Subir y bajar brazos
         case GLFW_KEY_M: if (subirDz < 25.0){
               subirDz += 0.3; 
               subirDy += 0.003; 
               }
               else
                  brazosSubidos = true;
               break;
         case GLFW_KEY_N: if(subirDz >=0){
               brazosSubidos = false;
               subirDz -= 0.3;
               subirDy -= 0.003;
            }
               break;
         // Lanzar bola sombra
         case GLFW_KEY_SPACE:
         if(action==GLFW_PRESS) {
            if (brazosSubidos)
            {
               permitirLanzar = true;
               while (subirDz >= 0)
               {
                  subirDz -= 0.3;
                  subirDy -= 0.003;
               }
               std::cout << "* MIMIKYU HA USADO BOLA SOMBRA *" << std::endl;
            }
         }
         break;
   }
}

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {
    if(yoffset>0) fovy -= fovy>10.0f ? 5.0f :
            0.0f;
            if (yoffset < 0)
               fovy += fovy < 90.0f ? 5.0f : 0.0f;
}

void funCursorPos(GLFWwindow* window, double xposIn, double yposIn) {
   if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) {
      firstMouse = true;
      return;
   }

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

// Funcion que lanza la bola sombra
void lanzar(){
   if(x1<=4.8f){
      x1 += 0.1;
      x2 += 0.1;
      x3 += 0.1;
      x4 += 0.1;
      ballZ -= 0.1;
   }
}

// Animacion del voltorb
void flotarYGirar(float times)
   {
      if (glfwGetTime() > times)
      {
         if (subir)
         {
            if (girar <= 10)
               girar += 1;
            else
               subir = false;
         }
         else
         {
            if (girar >= 0)
               girar -= 1;
            else
               subir = true;
         }
         if (palante)
         {
            if (flotar <= 1.5)
               flotar += 0.1;
            else
               palante = false;
         }
         else
         {
            if (flotar >= 1)
               flotar -= 0.1;
            else
               palante = true;
         }

         if (derecha)
            if (girar2 <= 0.0f)
               girar2 += 0.5;
            else
               girar2 -= 0.5;
         else if (girar2 >= 3.0f)
            girar2 -= 0.5;
         else
            girar2 += 0.5;
         glfwSetTime(0.0);
      }
}

unsigned int loadTexture(char const *path) {
   unsigned int textureID;
   glGenTextures(1, &textureID);

   int width, height, nrComponents;
   unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
   if (data) {
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
   } else {
      std::cout << "Texture failed to load at path: " << path << std::endl;
      stbi_image_free(data);
   }

   return textureID;
}

// Funcion que carga el cubemap
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
