/*
*
* Proyecto final - Casa sustentable
*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <animatedmodel.h>
#include <material.h>
#include <light.h>
#include <cubemap.h>

#include <irrKlang.h>
using namespace irrklang;

// Functions
bool Start();
bool Update();
void changeCubeMapFaces(vector<std::string> faces);

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
//void dynamicLightDraw(Model, glm::mat4, glm::mat4, glm::vec3, glm::vec3, float, glm::vec3, int);
void dynamicLightDraw(Model* model, glm::mat4 projection, glm::mat4 view, glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f), \
	glm::vec3 rotate = glm::vec3(1.0f, 0.0f, 0.0f), float degree = 180.0f, glm::vec3 scale = glm::vec3(1.0f), int t = 0, glm::vec3 reflex = glm::vec3(1.0f,1.0f,1.0f));
void outlineDraw(Model* model, glm::mat4 projection, glm::mat4 view, glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f), \
	glm::vec3 rotate = glm::vec3(1.0f, 0.0f, 0.0f), float degree = 180.0f, glm::vec3 scale = glm::vec3(1.0f), float outline = 1.08);
void textDraw(Model * texto, Camera camera, glm::mat4 projection, glm::mat4 view, glm::vec3 scale = glm::vec3(1.0f), int ID_text = 0.0f);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de espejos
glm::vec3 mirror1_position(-14.85f, 2.24f, -0.022f);
Camera mirror1_camera(mirror1_position, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);  //Ve a X positivo


// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;
float t = 0.0f;
float day_duration_sec = 120.0f; //Los segundos aproximados que dura un día
bool time_flow = true;
int toggle = 0;
int HouseFrame = 0;

//Variables de control de texto
GLint valorLeido;
bool textEnable = false;

//Control de entrada
int pastPRESS = 0;
float timeRELEASE = 0;


glm::vec3 position(0.0f, 0.0f, 0.0f);
glm::vec3 forwardView(0.0f, 0.0f, 1.0f);
float     trdpersonOffset = 1.5f;
float     frspersonOffset = 0.4f;
float     scaleV = 0.025f;
float     rotateCharacter = 0.0f;
float	  door_offset = 0.0f;
float	  door_rotation = 0.0f;

float	  posicion_mono_x = 1.0f;
float	  posicion_mono_y = 1.0f;


// Shaders
Shader* dynamicLightsShader;
Shader* proceduralShader;
Shader* wavesShader;
Shader* solidColorShader;
Shader* cubemapShader;
Shader* dynamicShader;
Shader* sunShader; //Auxiliar para ver la ubicación de luces
Shader* dynamicSky; //Para el cambio del color del cielo
Shader* mirrorStencilShader; //"Renderiza" el plano del espejo
Shader* crystalShader; //Para todos los cristales de la escena
Shader* outlineShader; //Para bordes
Shader* textShader;
// Carga la información del modelo
Model* house;
Model* doorframe;
Model* door;
Model* moon;
Model* cultivos;
Model* cultivos_outline;
Model* gridMesh;
Model* SunModel; //Rombo para ver las luces
Model* mirror; //Un plano que sirve para recortar con el stencil shader
Model* crystals;
Model* terreno;
Model* tinaco;
Model* texto;



// Modelos animados
//AnimatedModel* character01, * character02, * character03;

float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

// Cubemap
CubeMap* mainCubeMap;

// Light gLight;
std::vector<Light> gLights;

// Materiales
Material material01;

float proceduralTime = 0.0f;
float wavesTime = 0.0f;

// Audio
ISoundEngine *SoundEngine = createIrrKlangDevice();

// selección de cámara
bool    activeCamera = 1; // activamos la primera cámara

// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;

}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Casa sustentable",NULL, NULL); //Tercer parametro  glfwGetPrimaryMonitor() para fullscreen
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST); //Habilita el stencil buffer
	
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  //Remplazará por el último objeto renderizado
	//Esto implica si el último objeto está cubierto por un objeto que se renderizó primero, no 
	//se encimará la ref del objeto tracero, para obtener el del más cercano usar keep,keep,replace

	//KEEP,KEEP,REPLACE implica que

	// Compilación y enlace de shaders
	dynamicLightsShader = new Shader("shaders/11_PhongCasa.vs", "shaders/11_PhongCasa.fs");
	proceduralShader = new Shader("shaders/12_ProceduralAnimation.vs", "shaders/12_ProceduralAnimation.fs");
	wavesShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/13_wavesAnimation.fs");
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	dynamicShader = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");
	sunShader = new Shader("shaders/10_vertex_simple.vs", "shaders/Dynamic_sun.fs");
	dynamicSky = new Shader("shaders/10_vertex_cubemap.vs", "shaders/Dynamic_sky.fs");
	mirrorStencilShader = new Shader("shaders/24_mirror.vs", "shaders/24_mirror.fs");
	crystalShader = new Shader("shaders/11_PhongCrystal.vs", "shaders/11_PhongCrystal.fs");
	outlineShader = new Shader("shaders/14_outline.vs", "shaders/14_outline.fs");
	textShader = new Shader("shaders/16_text.vs", "shaders/16_text.fs");
	//solidColorShader = new Shader("shaders/01-simple.vs", "shaders/01-simple.fs");
	
	// Máximo número de huesos: 100
	dynamicShader->setBonesIDs(MAX_RIGGING_BONES);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	//cout << "Casa" << endl;

	//house = new Model("models/CasaAnim.fbx");
	//cout << "Puerta" << endl;
	door = new Model("models/door.fbx");
	moon = new Model("models/IllumModels/moon.fbx");
	//gridMesh = new Model("models/IllumModels/grid.fbx");
	cultivos = new Model("models/CultivosHidro.fbx");
	cultivos_outline = new Model("models/Cultivos_outline.fbx");
	mirror = new Model("models/mirror.fbx");
	//crystals = new Model("models/Cristales_tex.fbx");
	//doorframe = new Model("models/Doorframe.fbx");
	//terreno = new Model("models/Terreno.fbx");
	//tinaco = new Model("models/Tinaco.fbx");
	//character01 = new AnimatedModel("models/personaje2.fbx");
	texto = new Model("models/Texto.fbx");
	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/03/posx.png",
		"textures/cubemap/03/negx.png",
		"textures/cubemap/03/posy.png",
		"textures/cubemap/03/negy.png",
		"textures/cubemap/03/posz.png",
		"textures/cubemap/03/negz.png"
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);

	camera3rd.Position = position;
	camera3rd.Position.y += 1.7f;
	camera3rd.Position -= trdpersonOffset * forwardView;
	camera3rd.Front = forwardView;

	// Lights configuration

	Light sun;
	sun.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	sun.Color = glm::vec4(0.2f, 0.1f, 0.0f, 1.0f);
	sun.Power = glm::vec4(100.0f, 100.0f, 100.0f, 1.0f);
	gLights.push_back(sun);

	SunModel = new Model("models/IllumModels/Sol.fbx");

	// SoundEngine->play2D("sound/EternalGarden.mp3", true);

	return true;
}


void SetLightUniformInt(Shader* shader, const char* propertyName, size_t lightIndex, int value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setInt(uniformName.c_str(), value);
}
void SetLightUniformFloat(Shader* shader, const char* propertyName, size_t lightIndex, float value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setFloat(uniformName.c_str(), value);
}
void SetLightUniformVec4(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec4 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec4(uniformName.c_str(), value);
}
void SetLightUniformVec3(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec3 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec3(uniformName.c_str(), value);
}


bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	if (time_flow)
		t += deltaTime;

	if (t >= day_duration_sec) {
		t = 0;
	}
	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 mirror1_projection;
	glm::mat4 mirror1_view;

	if (activeCamera) {
		// Cámara en primera persona
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera.GetViewMatrix();
		//Camara de espejo1
		mirror1_camera.Position.x = mirror1_position.x - (camera.Position.x - mirror1_position.x); //Se invierte en x porque este espejo ve a ese eje
		mirror1_camera.Position.y = camera.Position.y; //Siempre se conserva el movimiento en Y
		mirror1_camera.Position.z = -camera.Position.z; //Se conserva el movimiento porque no ve a z
		mirror1_camera.Front = glm::vec3(-camera.Front.x, camera.Front.y, -camera.Front.z); //Solo se conserva y

		mirror1_projection = glm::perspective(glm::radians(mirror1_camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, glm::distance(mirror1_position, mirror1_camera.Position) + 0.3f, 10000.0f);
		mirror1_view = mirror1_camera.GetViewMatrix();
	}
	else {
		// cámara en tercera persona
		projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera3rd.GetViewMatrix();

		//Camara de espejo1
		mirror1_camera.Position.x = -camera3rd.Position.x; //Se invierte en x porque este espejo ve a ese eje
		mirror1_camera.Position.y = camera3rd.Position.y; //Siempre se conserva el movimiento en Y
		mirror1_camera.Position.z = camera3rd.Position.z; //Se conserva el movimiento porque no ve a z
		mirror1_camera.Front = glm::vec3(-camera3rd.Front.x, camera3rd.Front.y, -camera3rd.Front.z); //Solo se conserva y

		mirror1_projection = glm::perspective(glm::radians(mirror1_camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		mirror1_view = mirror1_camera.GetViewMatrix();
	}


	
	//glClear(GL_COLOR_BUFFER_BIT); //Se elimina el color el plano una vez se ha creado la máscara

	//
	//glStencilMask(0x00);//El cube map no afecta al buffer
	//{
	//	//glStencilFunc(GL_ALWAYS, 0, 0xFF);//Asigna 0 al cubemap
	//	mainCubeMap->drawCubeMap(*dynamicSky, projection, view, gLights.at(0).Color); //Stencil index = 0
	//}
	//glStencilMask(0xFF);//El cube map no afecta al buffer

	//glStencilFunc(GL_ALWAYS, 5, 0xFF);
	//dynamicLightDraw(terreno, projection, view);

	//glStencilFunc(GL_ALWAYS, 10, 0xFF);
	//dynamicLightDraw(house, projection, view, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), -90.0f,glm::vec3(1.0f),HouseFrame);
	

	//El espejo debe hacerse después de la casa por un Z-index mayor

	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //Solo remplazará si no hay ningun que obstruya
	//glStencilFunc(GL_ALWAYS, 1, 0xFF); //Todo fragmento dibujado pasará la prueba
	//{
	//	mirrorStencilShader->use(); //Inicializa el dibujado del espejo
	//	// Activamos para objetos transparentes
	//	//glEnable(GL_BLEND);
	//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	// Aplica la camara para que la mascara existe en la perspectiva del usuario
	//	mirrorStencilShader->setMat4("projection", projection);
	//	mirrorStencilShader->setMat4("view", view);

	//	// Aplicamos transformaciones del modelo
	//	glm::mat4 model = glm::mat4(1.0f);
	//	model = glm::translate(model, mirror1_position); // translate it down so it's at the center of the scene
	//	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//	model = glm::scale(model, glm::vec3(0.312f, 0.321f, 0.2f));	// it's a bit too big for our scene, so scale it down
	//	mirrorStencilShader->setMat4("model", model);


	//	mirrorStencilShader->setFloat("show", 1.0f); //Muestra el espejo, nada mas para depura



	//	mirror->Draw(*mirrorStencilShader);
	//	glUseProgram(0);
	//}

	////Ahora solo va a dibujar en donde se cumpla la máscara
	//glStencilFunc(GL_EQUAL, 1, 0xFF);
	////Bloquea la máscara para que no se modifique
	//glStencilMask(0x00);

	//Reflejo de la casa
	/*dynamicLightDraw(house, mirror1_projection, mirror1_view, glm::vec3(0.0f), glm::vec3(1.0f,0.0f,0.0f),\
		-90.0f, glm::vec3(1.0f, 1.0f, 1.0f),HouseFrame,glm::vec3(1.0f,1.0f,-1.0f));
*/


	// Reflejo del personaje
	//{
	//	character01->UpdateAnimation(deltaTime);

	//	// Activación del shader del personaje
	//	dynamicShader->use();

	//	// Aplicamos transformaciones de proyección y cámara (si las hubiera)
	//	dynamicShader->setMat4("projection", mirror1_projection);
	//	dynamicShader->setMat4("view", mirror1_view);

	//	// Aplicamos transformaciones del modelo
	//	glm::mat4 model = glm::mat4(1.0f);

	//	if (camera.Front.z == 0.0f)
	//		camera.Front.z = 0.0001f;
	//	float rotation = glm::atan(camera.Front.x / -camera.Front.z);
	//	if (camera.Front.z > 0)
	//		rotation = rotation + 3.1415;


	//	model = glm::translate(model, glm::vec3(camera.Position.x, 0.24f, -camera.Position.z));
	//	model = glm::rotate(model, rotation, glm::vec3(0.0, 1.0f, 0.0f));
	//	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));

	//	dynamicShader->setMat4("model", model);

	//	dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, character01->gBones);

	//	// Dibujamos el modelo
	//	character01->Draw(*dynamicShader);
	//}
	//glUseProgram(0);

	
	//{ //Dibujo del sol
	//	sunShader->use();

	//	sunShader->setMat4("projection", projection);
	//	sunShader->setMat4("view", view);

	//	glm::mat4 model;

	//	for (size_t i = 0; i < gLights.size(); ++i) {
	//		model = glm::mat4(1.0f);
	//		model = glm::translate(model, gLights[i].Position);
	//		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//		sunShader->setMat4("model", model);

	//		SunModel->Draw(*sunShader);
	//	}

	//}
	//glUseProgram(0);

	//{//Animación del sol
	//	sunShader->use();

	//	sunShader->setMat4("projection", projection);
	//	sunShader->setMat4("view", view);

	//	glm::mat4 model;

	//	//Movimiento del sol
	//	gLights.at(0).Position.x = 400 * cos(glm::radians(360 / day_duration_sec * t));
	//	gLights.at(0).Position.y = 400 * sin(glm::radians(360 / day_duration_sec * t));
	//	gLights.at(0).Power = glm::vec4(100.0f, 100.0f, 100.0f, 1.0f);

	//	if (t > day_duration_sec / 2) { //apaga el sol en la noche
	//		HouseFrame = 1;
	//		gLights.at(0).Position = glm::vec4(0.0f, 300.0f, 0.0f, 1.0f);
	//		model = glm::mat4(1.0f);
	//		model = glm::translate(model, gLights[0].Position);
	//		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
	//		sunShader->setMat4("model", model);

	//		if (toggle != 2) {
	//			vector<std::string> faces = {
	//				"textures/cubemap/04/posx.png",
	//				"textures/cubemap/04/negx.png",
	//				"textures/cubemap/04/posy.png",
	//				"textures/cubemap/04/negy.png",
	//				"textures/cubemap/04/posz.png",
	//				"textures/cubemap/04/negz.png"
	//			};
	//			changeCubeMapFaces(faces);
	//			toggle = 2;
	//		}

	//	}
	//	else {
	//		HouseFrame = 0;
	//		model = glm::mat4(1.0f);
	//		model = glm::translate(model, gLights[0].Position);
	//		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//		sunShader->setMat4("model", model);
	//		if (toggle != 0) {
	//			vector<std::string> faces = {
	//				"textures/cubemap/03/posx.png",
	//				"textures/cubemap/03/negx.png",
	//				"textures/cubemap/03/posy.png",
	//				"textures/cubemap/03/negy.png",
	//				"textures/cubemap/03/posz.png",
	//				"textures/cubemap/03/negz.png"
	//			};
	//			changeCubeMapFaces(faces);
	//			toggle = 0;
	//		}
	//	}

	//	//Atardeceres
	//	if (t > day_duration_sec / 2) { //Anochecer
	//		gLights.at(0).Color = glm::vec4(0.08f, 0.08f, 0.2f, 1.0f);

	//	}
	//	else { //Amanecer, día y atardecer
	//		gLights.at(0).Color.x = 0.2f; //Rojo activo todo el día
	//		gLights.at(0).Color.y = 0.2f * pow(sin(glm::radians(360 / day_duration_sec * t)), 0.3);
	//		gLights.at(0).Color.z = 0.2f * pow(sin(glm::radians(360 / day_duration_sec * t)), 0.4);
	//	}
	//	sunShader->setVec4("LightColor", gLights.at(0).Color);
	//	SunModel->Draw(*sunShader);
	//}
	//glUseProgram(0);

	//{ //Marco Puerta
	//
	//	dynamicLightsShader->use();

	//	// Activamos para objetos transparentes
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	dynamicLightsShader->setMat4("projection", projection);
	//	dynamicLightsShader->setMat4("view", view);

	//	// Aplicamos transformaciones del modelo
	//	glm::mat4 model = glm::mat4(1.0f);
	//	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	//	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//	model = glm::scale(model, glm::vec3(1.0f, 1.00f, 1.00f));
	//	dynamicLightsShader->setMat4("model", model);

	//	glm::mat4 reflex = glm::mat4(1.0f);

	//	// Configuramos propiedades de fuentes de luz
	//	dynamicLightsShader->setInt("numLights", (int)gLights.size());
	//	for (size_t i = 0; i < gLights.size(); ++i) {
	//		SetLightUniformVec3(dynamicLightsShader, "Position", i, gLights[i].Position);
	//		SetLightUniformVec3(dynamicLightsShader, "Direction", i, gLights[i].Direction);
	//		SetLightUniformVec4(dynamicLightsShader, "Color", i, gLights[i].Color);
	//		SetLightUniformVec4(dynamicLightsShader, "Power", i, gLights[i].Power);
	//		SetLightUniformInt(dynamicLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
	//		SetLightUniformFloat(dynamicLightsShader, "distance", i, gLights[i].distance);
	//	}

	//	dynamicLightsShader->setVec3("eye", camera.Position);

	//	// Aplicamos propiedades materiales
	//	dynamicLightsShader->setVec4("MaterialAmbientColor", material01.ambient);
	//	dynamicLightsShader->setVec4("MaterialDiffuseColor", material01.diffuse);
	//	dynamicLightsShader->setVec4("MaterialSpecularColor", material01.specular);
	//	dynamicLightsShader->setFloat("transparency", material01.transparency);
	//	dynamicLightsShader->setMat4("reflex", reflex);

	//	////Carga de animación
	//	dynamicLightsShader->setInt("frame", 0);

	//	doorframe->Draw(*dynamicLightsShader);
	//	glEnable(GL_DEPTH_TEST);
	//	glUseProgram(0);
	//}

	glStencilFunc(GL_ALWAYS, 100, 0xFF);
	glStencilMask(0xFF);
	dynamicLightDraw(cultivos, projection, view, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
	
	
	
	//{
	//	//Puerta
	//	dynamicLightsShader->use();

	//	// Activamos para objetos transparentes
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	dynamicLightsShader->setMat4("projection", projection);
	//	dynamicLightsShader->setMat4("view", view);

	//	// Aplicamos transformaciones del modelo
	//	glm::mat4 model = glm::mat4(1.0f);
	//	model = glm::translate(model, glm::vec3(4.01f, 1.30f, -1.69f)); // translate it down so it's at the center of the scene
	//	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//	model = glm::rotate(model, glm::radians(door_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	//	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	//	dynamicLightsShader->setMat4("model", model);

	//	glm::mat4 reflex = glm::mat4(1.0f);

	//	// Configuramos propiedades de fuentes de luz
	//	dynamicLightsShader->setInt("numLights", (int)gLights.size());
	//	for (size_t i = 0; i < gLights.size(); ++i) {
	//		SetLightUniformVec3(dynamicLightsShader, "Position", i, gLights[i].Position);
	//		SetLightUniformVec3(dynamicLightsShader, "Direction", i, gLights[i].Direction);
	//		SetLightUniformVec4(dynamicLightsShader, "Color", i, gLights[i].Color);
	//		SetLightUniformVec4(dynamicLightsShader, "Power", i, gLights[i].Power);
	//		SetLightUniformInt(dynamicLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
	//		SetLightUniformFloat(dynamicLightsShader, "distance", i, gLights[i].distance);
	//	}

	//	dynamicLightsShader->setVec3("eye", camera.Position);

	//	// Aplicamos propiedades materiales
	//	dynamicLightsShader->setVec4("MaterialAmbientColor", material01.ambient);
	//	dynamicLightsShader->setVec4("MaterialDiffuseColor", material01.diffuse);
	//	dynamicLightsShader->setVec4("MaterialSpecularColor", material01.specular);
	//	dynamicLightsShader->setFloat("transparency", material01.transparency);
	//	dynamicLightsShader->setMat4("reflex", reflex);

	//	////Carga de animación
	//	dynamicLightsShader->setInt("frame", 0);

	//	door->Draw(*dynamicLightsShader);
	//	glEnable(GL_DEPTH_TEST);
	//	glUseProgram(0);
	//}
		
	


	//{ //Elemtos cristalinos
	//	crystalShader->use();

	//	// Activamos para objetos transparentes
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//	// Aplicamos transformaciones de proyección y cámara (si las hubiera)
	//	crystalShader->setMat4("projection", projection);
	//	crystalShader->setMat4("view", view);

	//	// Aplicamos transformaciones del modelo
	//	glm::mat4 model = glm::mat4(1.0f);
	//	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	//	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	//	crystalShader->setMat4("model", model);

	//	glm::mat4 reflex = glm::mat4(1.0f);
	//	crystalShader->setMat4("reflex", reflex);

	//	// Configuramos propiedades de fuentes de luz
	//	crystalShader->setInt("numLights", (int)gLights.size());
	//	for (size_t i = 0; i < gLights.size(); ++i) {
	//		SetLightUniformVec3(crystalShader, "Position", i, gLights[i].Position);
	//		SetLightUniformVec3(crystalShader, "Direction", i, gLights[i].Direction);
	//		SetLightUniformVec4(crystalShader, "Color", i, gLights[i].Color);
	//		SetLightUniformVec4(crystalShader, "Power", i, gLights[i].Power);
	//		SetLightUniformInt(crystalShader, "alphaIndex", i, gLights[i].alphaIndex);
	//		SetLightUniformFloat(crystalShader, "distance", i, gLights[i].distance);
	//	}

	//	crystalShader->setVec3("eye", camera.Position);

	//	// Aplicamos propiedades materiales
	//	crystalShader->setVec4("MaterialAmbientColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//	crystalShader->setVec4("MaterialDiffuseColor", glm::vec4(0.55f, 0.55f, 0.55f, 1.0f));
	//	crystalShader->setVec4("MaterialSpecularColor", glm::vec4(0.70f, 0.7f, 0.7f, 1.0f));
	//	crystalShader->setFloat("transparency", 0.2f);



	//	crystals->Draw(*crystalShader);
	//	glEnable(GL_DEPTH_TEST);
	//	glUseProgram(0);
	//}






	// Objeto animado 2
	/*
	{
		character02->UpdateAnimation(deltaTime);

		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(6.0f, 1.0f, 4.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, character02->gBones);

		// Dibujamos el modelo
		character02->Draw(*dynamicShader);
	}

	glUseProgram(0);*/


	// Objeto animado 3
/*
	{
		character03->UpdateAnimation(deltaTime);

		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, character03->gBones);

		// Dibujamos el modelo
		character03->Draw(*dynamicShader);
	}

	glUseProgram(0);*/

	/*GLint pantalla[4];
	GLint valorLeido;*/
	/*glGetIntegerv(GL_VIEWPORT,pantalla);
	cout << "Pantalla: " << pantalla[0] << " " << pantalla[1] << " " << pantalla[2] << " " << pantalla[3] << endl;
	width*/
	//Se obtiene el ángulo de vista de la cámara

	/*projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);*/

	//outlineDraw(moon, projection, view, glm::vec3(3.0f*sin(theta)*cos(phi) + camera.Position.x, 3.0f*sin(phi), -3.0f * cos(theta) * cos(phi) + camera.Position.z), glm::vec3(1.0f), 0.0f, glm::vec3(0.4));
	

	//outlineDraw(texto, projection,view, glm::vec3(5.0f *camera.Front.x+camera.Position.x, 5.0f*camera.Front.y+camera.Position.y,5.0f*camera.Front.z+camera.Position.z), glm::vec3(1.0f,0.0f,0.0f), -90.0f, glm::vec3(0.4));
	//outlineDraw(texto, projection, view);
	
		//{
	//	outlin->use(); //Inicializa el dibujado del espejo
	//	// Activamos para objetos transparentes
	//	//glEnable(GL_BLEND);
	//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

	//	//Se obtiene el ángulo de vista de la cámara
	//	if (camera.Front.z == 0.0f)
	//		camera.Front.z = 0.0001f;
	//	float rotation = glm::atan(camera.Front.x / -camera.Front.z);
	//	if (camera.Front.z > 0)
	//		rotation = rotation + 3.1415;



	//	// Aplica la camara para que la mascara existe en la perspectiva del usuario
	//	mirrorStencilShader->setMat4("projection", projection);
	//	mirrorStencilShader->setMat4("view", view);

	//	cout << "x:" << cos(rotation) + camera.Position.x << endl;

	//	cout << "z" << sin(rotation) + camera.Position.z << endl;
	//	cout << endl;
	//	// Aplicamos transformaciones del modelo
	//	glm::mat4 model = glm::mat4(1.0f);
	//	model = glm::translate(model, glm::vec3(0.0f, 1.3f, 0.0f)); // translate it down so it's at the center of the scene
	//	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//	model = glm::scale(model, glm::vec3(0.312f, 0.321f, 0.6f));	// it's a bit too big for our scene, so scale it down
	//	mirrorStencilShader->setMat4("model", model);


	//	mirrorStencilShader->setFloat("show", 1.0f); //Muestra el espejo, nada mas para depura


	//	moon->Draw(*mirrorStencilShader);
	//	glUseProgram(0);
	//}


	GLint *valor;
	valor = &valorLeido;
	GLint center_x = (GLint)(SCR_WIDTH / 2.0);
	GLint center_y = (GLint)(SCR_HEIGHT / 2.0);
	GLsizei GLwidth = (GLsizei)(SCR_WIDTH);
	GLsizei GLheight = (GLsizei)(SCR_HEIGHT);
	glReadPixels(center_x, center_y, 1, 1, GL_STENCIL_INDEX, GL_INT, valor);
	
	glStencilFunc(GL_NOTEQUAL, 100, 0xFF);
	glStencilMask(0x00);
	if (valorLeido == 100) {
		outlineDraw(cultivos_outline, projection, view, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
		if (textEnable) {
			glStencilMask(0xFF); //Habilita la edición del buffer
			glStencilFunc(GL_ALWAYS, 0, 0xFF); //Vuelve el mappeo de 0 a todo
			glClear(GL_DEPTH_BUFFER_BIT);
			textDraw(texto, camera, projection, view);
		}
		//if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS||textEnable==true) {

		//}

		//cout << "Valor escaneado: " << valorLeido << endl;
	}
	else {
		textEnable = false;
	}

	//Dibujar cursos
	//Reseteo del stencil mask
	glStencilMask(0xFF); //Habilita la edición del buffer
	glStencilFunc(GL_ALWAYS, 0, 0xFF); //Vuelve el mappeo de 0 a todo
	glEnable(GL_DEPTH_TEST);

	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}



void changeCubeMapFaces(vector<std::string> faces) {
	//mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		time_flow = not time_flow;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		if (door_rotation <= 90) door_rotation += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		if (door_rotation >= 0) door_rotation -= 1.0f;
//	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		//cout << valorLeido << endl;
		if (valorLeido >= 100) { //Se abre el mensaje
			if (pastPRESS == GLFW_RELEASE) { //No había mensaje
				textEnable = not textEnable;
				pastPRESS = 1;
			}
		}
	}else {
		if (timeRELEASE < 0.4) { //400 ms de cooldawn
			timeRELEASE += deltaTime;
		}
		else { //Una vez termina el tiempo, permite un nuevo press
			pastPRESS = 0;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		mirror1_position.x += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		mirror1_position.x -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		mirror1_position.z -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		mirror1_position.z += 0.01f;

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

		position = position + scaleV * forwardView;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(FORWARD, deltaTime);
		camera3rd.Position = position;
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= trdpersonOffset * forwardView;

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position = position - scaleV * forwardView;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(BACKWARD, deltaTime);
		camera3rd.Position = position;
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotateCharacter += 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		camera3rd.Position = position;
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotateCharacter -= 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		camera3rd.Position = position;
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		activeCamera = 0;
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		activeCamera = 1;

}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

void outlineDraw(Model * model, glm::mat4 projection, glm::mat4 view, glm::vec3 translate, \
	glm::vec3 rotate, float degree, glm::vec3 scale,float outline){
	outlineShader->use();
	outlineShader->setMat4("projection", projection);
	outlineShader->setMat4("view", view);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, translate);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(degree), rotate);
	modelMatrix = glm::scale(modelMatrix, scale);
	outlineShader->setMat4("model", modelMatrix);
	outlineShader->setFloat("outlining", outline);
	model->Draw(*outlineShader);
	
	glUseProgram(0);
}

void textDraw(Model * texto, Camera camera, glm::mat4 projection, glm::mat4 view, glm::vec3 scale, int ID_text){
	textShader->use();
	textShader->setMat4("projection",projection);
	textShader->setMat4("view", view);

	//float theta = atan(camera.Front.z / camera.Front.x);
	if (camera.Front.x == 0.0f)
		camera.Front.x = 0.0001f;
	float theta = glm::atan(abs(camera.Front.z / camera.Front.x));
	if (camera.Front.x > 0)
		theta = theta + 3.1415;

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	
	modelMatrix = glm::translate(modelMatrix, glm::vec3(5.0f * camera.Front.x + camera.Position.x, 5.0f * camera.Front.y + camera.Position.y, 5.0f * camera.Front.z + camera.Position.z))*glm::rotate(modelMatrix, -glm::radians(camera.Yaw), glm::vec3(0.0f,1.0f,0.0f))*glm::rotate(modelMatrix, glm::radians(camera.Pitch), glm::vec3(0.0f, 0.0f, 1.0f))* glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//*

	
	//modelMatrix = glm::rotate(modelMatrix,theta, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7));
	textShader->setMat4("model", modelMatrix);
	texto->Draw(*textShader);
	
	glUseProgram(0);

}

void dynamicLightDraw(Model *model, glm::mat4 projection, glm::mat4 view, glm::vec3 translate , \
	glm::vec3 rotate , float degree , glm::vec3 scale , int t, glm::vec3 reflex) {


	dynamicLightsShader->use();
	dynamicLightsShader->setMat4("projection", projection);
	dynamicLightsShader->setMat4("view", view);

	// Aplicamos transformaciones del modelo
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, translate); // translate it down so it's at the center of the scene
	modelMatrix = glm::rotate(modelMatrix, glm::radians(degree), rotate);
	modelMatrix = glm::scale(modelMatrix, scale);
	dynamicLightsShader->setMat4("model", modelMatrix);

	//Afectado únicamente por la luz solar
	// Configuramos propiedades de fuentes de luz
	dynamicLightsShader->setInt("numLights", (int)gLights.size());
	for (size_t i = 0; i < gLights.size(); ++i) {
		SetLightUniformVec3(dynamicLightsShader, "Position", i, gLights[i].Position);
		SetLightUniformVec3(dynamicLightsShader, "Direction", i, gLights[i].Direction);
		SetLightUniformVec4(dynamicLightsShader, "Color", i, gLights[i].Color);
		SetLightUniformVec4(dynamicLightsShader, "Power", i, gLights[i].Power);
		SetLightUniformInt(dynamicLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
		SetLightUniformFloat(dynamicLightsShader, "distance", i, gLights[i].distance);
	}

	dynamicLightsShader->setVec3("eye", camera.Position);

	// Aplicamos propiedades materiales
	dynamicLightsShader->setVec4("MaterialAmbientColor", material01.ambient);
	dynamicLightsShader->setVec4("MaterialDiffuseColor", material01.diffuse);
	dynamicLightsShader->setVec4("MaterialSpecularColor", material01.specular);
	dynamicLightsShader->setFloat("transparency", material01.transparency);

	
	glm::mat4 reflexMatrix = glm::mat4(glm::vec4(reflex.x, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f,reflex.y,\
		0.0f, 0.0f), glm::vec4(0.0f, 0.0f, reflex.z, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	dynamicLightsShader->setMat4("reflex", reflexMatrix);

	////Carga de animación
	dynamicLightsShader->setInt("frame", t);

	model->Draw(*dynamicLightsShader);
	glUseProgram(0);
}

