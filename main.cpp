
//#pragma warning (disable: 4700)
//#pragma warning (disable: 4996)

#include "tatmodel.h"
#include "pedestrian.h"
#include "time.h"

int windowWidth = 2000;
int windowHeight = 1200;
float offsetX = 50.0f;
float offsetY = 25.0f;
float camRotX = 70.0f;
float camRotZ = 50.0f;
float camTransZ = 30.0f;
int mouseButton = -1;	// -1: No Button | 0: Left | 1: Middle | 2: Right
int prevX = 0;
int prevY = 0;

TATShader shader;
unsigned int vertexArrayID = 0;
TATHomoMatrix camMatrix;
TATHomoMatrix viewMatrix;
TATHomoMatrix modelMatrix;

TATRectangle ground;
TATSphere sphere;
TATSTLMesh tree1;
TATSTLMesh tree2;
TATSTLMesh tree3;
TATSTLMesh tree4;
TATSTLMesh pedesModel;
list<TATPedesCollection*> pedesCollections;
list<TATPedesCollection*>::iterator itePedes;

TATPolyRectangle wall0;
TATPolyRectangle wall1;
TATPolyRectangle wall2;
TATPolyRectangle wall3;
TATPolyRectangle wall4;
TATPolyRectangle wall5;
TATPolyRectangle wall6;
TATPolyRectangle wall7;
TATPolyRectangle wall8;
//TATPolyRectangle wall9;
//TATPolyRectangle wall10;

TATPolyRectangle wallsky;

//TATCube cube0;
//TATCube cube1;

TATVector groundColor = TATVector(0.9f, 0.9f, 0.9f);
TATVector defaultColor = TATVector(1.0f, 0.0f, 0.0f);
TATVector densityColor;
TATTexture groundTexture;
TATTexture wallTexture;
TATTexture skyTexture;

int interval = 100;	// ms
bool isAnimate = false;
bool isDensityColoring = true;
bool isUse3dModel = false;

void DisplayHelp()
{
	printf("==== Keys ====\n");
	printf("  W/S/A/D:\tRotate camera\n");
	printf("  Z/X:\tTranslate camera\n");
	printf("  T:\tToogle animation\n");
	printf("  R:\tReset animation\n");
	printf("  C:\tToogle using density as color\n");
	printf("  M:\tToogle using 3d model for pedestrian\n");
	printf("==============================================\n");
}

void initGLContext()
{
	// Shader
	shader.LoadShaders("VertexShader.txt", "FragmentShader.txt");
	shader.Use();
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	glEnableVertexAttribArray(POSITION_ATTRIBUTE);	// Enable for vertex ( = 0 )
	glEnableVertexAttribArray(NORMAL_ATTRIBUTE);	// Enable for normal ( = 1 )
	glEnableVertexAttribArray(TEXTURE_ATTRIBUTE);	// Enable for texCoord ( = 2 )

	// OpenGL Enviroment
	glClearColor(0.06f, 0.09f, 0.12f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Projection matrix
	glViewport(0, 0, windowWidth, windowHeight);
	float projectMatrix[16];
	TAT::Perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 500.0f, projectMatrix);
	//float sw2 = (float)windowWidth / 2.0f;
	//float sh2 = (float)windowHeight / 2.0f;
	//TAT::Ortho(-sw2, sw2, -sh2, sh2, 0.0f, 1000.0f, projectMatrix);
	shader.SetUniformMatrix4("ProjectMatrix", projectMatrix);

	// Camera matrix
	/*TAT::RotateXO(viewMatrix, -40.0f);
	TAT::RotateYO(viewMatrix, 40.0f);
	TAT::TranslateO(viewMatrix, 0.0f, 0.0f, 100.0f);
	viewMatrix.Invert();
	shader.SetUniformMatrix4("ViewMatrix", viewMatrix);*/

	// Model matrix
	//TAT::TranslateO(modelMatrix, 0.0f, -80.0f, 0.0f);
	//TAT::RotateXO(ModelMatrix, -90.0f);
	//shader.SetUniformMatrix4("ModelMatrix", modelMatrix);

	shader.SetUniformValue("RenderMode", 0);
	shader.SetUniformValue("EnableLight", 1);
	shader.SetUniformValue("EnableTexture", 0);

	// Material
	TATVector color(0.90f, 0.90f, 0.90f);
	TATVector ambientColor(0.0f, 0.0f, 0.0f);
	TATVector diffuseColor(0.90f, 0.0f, 0.0f);
	TATVector specularColor(0.70f, 0.70f, 0.70f);
	float shininess = 128.0;

	shader.SetUniformValue("Alpha", 1.0f);
	shader.SetUniformVector3("Color", color);

	shader.SetUniformVector3("MaterialAmbientColor", ambientColor);
	shader.SetUniformVector3("MaterialDiffuseColor", diffuseColor);
	shader.SetUniformVector3("MaterialSpecularColor", specularColor);
	shader.SetUniformValue("MaterialShininess", shininess);

	// Lighting
	TATVector lightPosition(100.0f, 25.0f, 25.0f);
	TATVector lightAmbientColor(0.0f, 0.0f, 0.0f);
	TATVector lightDiffuseColor(0.70f, 0.70f, 0.70f);
	TATVector lightSpecularColor(0.60f, 0.60f, 0.60f);
	float constantAttentuation = 1.0f;
	float linearAttentuation = 0.0f;
	float quadraticAttentuation = 0.0f;

	shader.SetUniformValue("LightEnable", 1.0f);
	shader.SetUniformVector3("LightPosition_WS[0]", lightPosition);
	shader.SetUniformVector3("LightAmbientColor[0]", lightAmbientColor);
	shader.SetUniformVector3("LightDiffuseColor[0]", lightDiffuseColor);
	shader.SetUniformVector3("LightSpecularColor[0]", lightSpecularColor);
	shader.SetUniformValue("LightConstantAttentuation[0]", constantAttentuation);
	shader.SetUniformValue("LightlinearAttentuation[0]", linearAttentuation);
	shader.SetUniformValue("LightQuadraticAttentuation[0]", quadraticAttentuation);

	// Texture
	glActiveTexture(GL_TEXTURE0);
	shader.SetSampler("SamplerTexture", 0);
}

void initModels() {
	ground.SetSize(80.0f, 140.0f);
	ground.InitBuffers(true, 1.0f, 1.0f);

	sphere.SetRadius(0.4f);
	sphere.SetResolution(10);
	sphere.InitVertices();
	sphere.InitIndices();
	sphere.InitBuffers();

	/*cube0.SetSize(14.0f, 6.0f, 6.0f);
	cube0.InitBuffers();

	cube1.SetSize(3.0f, 1.0f, 2.50f);
	cube1.InitBuffers();*/

	TATVector scale(1.0f, 1.0f, 1.0f);
	/*tree1.LoadBin("models/tree1.stl", 0.02f);
	tree2.LoadBin("models/tree1.stl", 0.025f);
	tree3.LoadBin("models/tree1.stl", 0.03f);
	tree4.LoadBin("models/tree1.stl", 0.035f);*/
	tree1.LoadBin("models/tree.stl", 0.50f);
	tree2.LoadBin("models/tree.stl", 0.6f);
	tree3.LoadBin("models/tree.stl", 0.8f);
	tree4.LoadBin("models/tree.stl", 0.9f);

	pedesModel.LoadBin("models/man.stl", 0.08f);
}

void initWalls()
{
	float width = 0.60f;
	float height = 4.0f;

	wall0.Width = width;
	wall0.Height = height;
	wall0.AddPoint(0.0f, 0.0f, 0.0f);
	wall0.AddPoint(100.0f, 0.0f, 0.0f);
	wall0.InitVertices();
	wall0.InitUVs(6.0f, 1.0f);
	wall0.InitIndices();
	wall0.InitBuffers();

	wall1.Width = width;
	wall1.Height = height;
	wall1.AddPoint(100.0f, 0.0f, 0.0f);
	wall1.AddPoint(100.0f, 6.10f, 0.0f);
	wall1.InitVertices();
	wall1.InitUVs(1.0f, 1.0f);
	wall1.InitIndices();
	wall1.InitBuffers();

	wall2.Width = width;
	wall2.Height = height;
	wall2.AddPoint(100.0f, 20.0f, 0.0f);
	wall2.AddPoint(100.0f, 29.0f, 0.0f);
	wall2.InitVertices();
	wall2.InitUVs(1.0f, 1.0f);
	wall2.InitIndices();
	wall2.InitBuffers();

	wall3.Width = width;
	wall3.Height = height;
	wall3.AddPoint(100.0f, 43.90f, 0.0f);
	wall3.AddPoint(100.0f, 50.0f, 0.0f);
	wall3.InitVertices();
	wall3.InitUVs(1.0f, 1.0f);
	wall3.InitIndices();
	wall3.InitBuffers();

	wall4.Width = width;
	wall4.Height = height;
	wall4.AddPoint(100.0f, 50.0f, 0.0f);
	wall4.AddPoint(0.0f, 50.0f, 0.0f);
	wall4.InitVertices();
	wall4.InitUVs(6.0f, 1.0f);
	wall4.InitIndices();
	wall4.InitBuffers();

	wall5.Width = width;
	wall5.Height = height;
	wall5.AddPoint(40.0f, 10.0f, 0.0f);
	wall5.AddPoint(60.0f, 10.0f, 0.0f);
	wall5.InitVertices();
	wall5.InitUVs(1.0f, 1.0f);
	wall5.InitIndices();
	wall5.InitBuffers();

	wall6.Width = width;
	wall6.Height = height;
	wall6.AddPoint(60.0f, 10.0f, 0.0f);
	wall6.AddPoint(60.0f, 30.0f, 0.0f);
	wall6.InitVertices();
	wall6.InitUVs(1.0f, 1.0f);
	wall6.InitIndices();
	wall6.InitBuffers();

	wall7.Width = width;
	wall7.Height = height;
	wall7.AddPoint(60.0f, 30.0f, 0.0f);
	wall7.AddPoint(40.0f, 30.0f, 0.0f);
	wall7.InitVertices();
	wall7.InitUVs(1.0f, 13.0f);
	wall7.InitIndices();
	wall7.InitBuffers();

	wall8.Width = width;
	wall8.Height = height;
	wall8.AddPoint(40.0f, 30.0f, 0.0f);
	wall8.AddPoint(40.0f, 10.0f, 0.0f);
	wall8.InitVertices();
	wall8.InitUVs(1.0f, 1.0f);
	wall8.InitIndices();
	wall8.InitBuffers();

	/*wall9.Width = width;
	wall9.Height = height;
	wall9.AddPoint(60.0f, 10.0f, 0.0f);
	wall9.AddPoint(82.0f, 18.0f, 0.0f);
	wall9.AddPoint(94.0f, 20.0f, 0.0f);
	wall9.InitVertices();
	wall9.InitUVs(1.0f, 1.0f);
	wall9.InitIndices();
	wall9.InitBuffers();

	wall10.Width = width;
	wall10.Height = height;
	wall10.AddPoint(60.0f, 30.0f, 0.0f);
	wall10.AddPoint(75.0f, 27.0f, 0.0f);
	wall10.AddPoint(85.0f, 27.0f, 0.0f);
	wall10.AddPoint(100.0f, 30.0f, 0.0f);
	wall10.InitVertices();
	wall10.InitUVs(1.0f, 1.0f);
	wall10.InitIndices();
	wall10.InitBuffers();*/

	wallsky.Width = 80.0f;
	wallsky.Height = 140.0f;
	wallsky.AddPoint(0.0f, 0.0f, 0.0f);
	wallsky.AddPoint(120.0f, 0.0f, 0.0f);
	wallsky.InitVertices();
	wallsky.InitUVs(1.0f, 1.0f);
	wallsky.InitIndices();
	wallsky.InitBuffers();
}

void initPedestrian() {
	float x, y, vx, vy, density, median;
	char filePath[128];
	int pointCount = 767;	// maximum number of files
	for (int i = 0; i < pointCount; i++) {
		sprintf(filePath, "data/pedestrian%d", i);
		FILE* file = fopen(filePath, "r");
		if (file == NULL)
			continue;

		TATPedesCollection* pedes = new TATPedesCollection();
		pedesCollections.push_back(pedes);
		do {
			// Read line: x y vx vy density median
			fscanf(file, "%f %f %f %f %f %f", &x, &y, &vx, &vy, &density, &median);
			// Create new point
			TATPedesPoint* point = new TATPedesPoint();	
			point->Position.Set(x, y);
			point->Velocity.Set(vx, vy);
			point->Velocity.ToUnit();
			point->Density = density;
			pedes->AddPoint(point);
		} while (!feof(file));
		
		fclose(file);
	}
	// Init position of pedestrian point to the first
	itePedes = pedesCollections.begin();
}

void initTextures()
{
	groundTexture.LoadTexture("textures/ground.bmp");
	wallTexture.LoadTexture("textures/wall.bmp");
	skyTexture.LoadTexture("textures/sky.bmp");
}

void resetPedestrians() {
	itePedes = pedesCollections.begin();
}

void DeleteAllPedestrians()
{
	list<TATPedesCollection*>::iterator ite = pedesCollections.begin();
	while (ite != pedesCollections.end()) {
		TATPedesCollection* pedes = *ite;
		delete pedes;
		ite++;
	}
	pedesCollections.clear();
}

void resize(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
	float projectMatrix[16];
	TAT::Perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f, projectMatrix);
	//float sw2 = (float)windowWidth / 2.0f;
	//float sh2 = (float)windowHeight / 2.0f;
	//TAT::Ortho(-sw2, sw2, -sh2, sh2, 0.0f, 1000.0f, projectMatrix);
	shader.SetUniformMatrix4("ProjectMatrix", projectMatrix);

}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Camera matrix
	camMatrix.ToIdentity();
	TAT::TranslateO(camMatrix, offsetX + 25.0f, offsetY, 0.0f);
	TAT::RotateZO(camMatrix, camRotZ);
	TAT::RotateXO(camMatrix, camRotX);	
	TAT::TranslateO(camMatrix, 0.0f, 0.0f, camTransZ);
	viewMatrix = camMatrix;
	viewMatrix.Invert();
	shader.SetUniformMatrix4("ViewMatrix", viewMatrix);

	// Draw with texture
	shader.SetUniformValue("EnableLight", 0);
	shader.SetUniformValue("EnableTexture", 1);
	// Draw ground
	glBindTexture(GL_TEXTURE_2D, groundTexture.GetTextureIndex());
	modelMatrix.ToIdentity();
	TAT::TranslateO(modelMatrix, offsetX, offsetY, -0.0f);
	TAT::RotateZO(modelMatrix, 90.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	ground.DrawTexture();

	// Draw walls
	glBindTexture(GL_TEXTURE_2D, wallTexture.GetTextureIndex());
	modelMatrix.ToIdentity();
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	wall0.DrawTexture();
	wall1.DrawTexture();
	wall2.DrawTexture();
	wall3.DrawTexture();
	wall4.DrawTexture();
	wall5.DrawTexture();
	wall6.DrawTexture();
	wall7.DrawTexture();
	wall8.DrawTexture();

	glBindTexture(GL_TEXTURE_2D, skyTexture.GetTextureIndex());
	modelMatrix.ToIdentity();
	TAT::RotateYO(modelMatrix, -90.0f);
	TAT::RotateZO(modelMatrix, 180.0f);
	TAT::TranslateO(modelMatrix, -120.0f, -25.0f, -120.0f);
	
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	wallsky.DrawTexture();

	// Draw with material
	shader.SetUniformValue("EnableLight", 1);
	shader.SetUniformValue("EnableTexture", 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Draw trees
	TATVector treeColor(0.0f, 1.0f, 0.0f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 50, 25.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree1.Draw();

	treeColor.Set(0.05f, 1.0f, 0.1f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 55.0f, 16.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree2.Draw();

	treeColor.Set(0.0f, 0.9f, 0.1f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 10.0f, 0.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree1.Draw();

	treeColor.Set(0.37f, 1.0f, 0.19f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 30.0f, 0.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree2.Draw();

	treeColor.Set(0.1f, 0.80f, 0.3f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 60.0f, 0.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree3.Draw();

	treeColor.Set(0.07f, 0.60f, 0.39f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 80.0f, 0.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree4.Draw();

	treeColor.Set(0.27f, 0.0f, 0.9f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 20.0f, 60.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree1.Draw();

	treeColor.Set(0.07f, 0.70f, 0.19f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 40.0f, 60.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree2.Draw();

	treeColor.Set(0.7f, 0.30f, 0.8f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 70.0f, 60.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree3.Draw();

	treeColor.Set(0.97f, 0.50f, 0.19f);
	shader.SetUniformVector3("MaterialDiffuseColor", treeColor);
	modelMatrix.ToIdentity();
	//TAT::RotateXO(modelMatrix, 90.0f);
	TAT::TranslateO(modelMatrix, 90.0f, 60.0f, 1.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	tree4.Draw();

	// Draw cubes
	/*defaultColor.Set(0.7f, 0.30f, 0.90f);
	shader.SetUniformVector3("MaterialDiffuseColor", defaultColor);
	modelMatrix.ToIdentity();
	TAT::TranslateO(modelMatrix, 30.0f, 20.0f, 0.0f);
	shader.SetUniformMatrix4("ModelMatrix", modelMatrix);
	cube0.Draw();*/

	// Draw pedestrians
	if (itePedes != pedesCollections.end()) {
		TATPedesCollection* pedes = *itePedes;	// Current pedestrian collection (each file)
		list<TATPedesPoint*>::iterator itePoint = pedes->GetFirstPointPos();
		while (itePoint != pedes->GetLastPointPos()) {
			TATPedesPoint* point = *itePoint;	// current point (x,y,vx,vx,density,median)
			point->GetPlace(modelMatrix);	// Calculate place matrix of point
			shader.SetUniformMatrix4("ModelMatrix", modelMatrix);

			// Get color of pedestrians: dxensity color or one color
			if (isDensityColoring) {
				point->GetDensityColor(densityColor);
				shader.SetUniformVector3("MaterialDiffuseColor", densityColor);
			}
			else {
				//shader.SetUniformVector3("MaterialDiffuseColor", defaultColor);
                shader.SetUniformVector3("MaterialDiffuseColor", point->Color);
			}

			// Draw pedestrians: stl model or sphere
			if (isUse3dModel)
				pedesModel.Draw();
			else
				sphere.Draw();

			itePoint++;
		}
	}
	

	glutSwapBuffers();
	glutPostRedisplay();
}

void onTimer(int value) {
	if (isAnimate) {
		if(itePedes != pedesCollections.end())
			itePedes++;
	}
	glutTimerFunc(interval, onTimer, 0);
}

void onKeyPress(unsigned char key, int x, int y) {

	unsigned char a = key;
	switch (key) {
	case 'w':
		if (camRotX > 1.5f)
			camRotX -= 0.5f;
		break;
	case 's':
		if (camRotX <= 89.5f)
			camRotX += 0.5f;
		break;
	case 'a':
		camRotZ -= 0.5f;
		break;
	case 'd':
		camRotZ += 1.0f;
		break;
	case 'z':
		camTransZ -= 0.5f;
		break;
	case 'x':
		camTransZ += 0.5f;
		break;
	case 't':
		isAnimate = !isAnimate;
		break;
	case 'r':
		resetPedestrians();
		break;
	case 'c':
		isDensityColoring = !isDensityColoring;
		break;
	case 'm':
		isUse3dModel = !isUse3dModel;
		break;
	default: break;
	}
}

void onMouseEvent(int button, int state, int x, int y)
{
	if (state == GLUT_UP) {
		mouseButton = -1;
	}
	else {
		mouseButton = button;
		prevX = x;
		prevY = y;
	}
}

void onMouseMove(int x, int y)
{
	// Process
	int dx = x - prevX;
	int dy = y - prevY;

	// Rotate X
	camRotX -= 0.2f * dy;
	if (camRotX < 1.5f)
		camRotX = 1.5f;
	if (camRotX >= 89.5f)
		camRotX = 89.5f;
	// Rotate Z
	camRotZ -= 0.2f * dx;

	// Update new (x,y)
	prevX = x;
	prevY = y;
}

void onExit() {
	DeleteAllPedestrians();
}

// Menu items
enum MENU_TYPE
{
    Toogle_Animation,
    Reset_Animation,
    Three_D_Model,
    Density_Color,
    ZoomIN,
    ZoomOUT,
    //RotateCamera,
    MENU_BACK_FRONT,
  
};

// Assign a default value
MENU_TYPE show = MENU_BACK_FRONT;
void menu(int);

// Menu handling function declaration

void menu(int item)
{
    switch (item)
    {
        case Toogle_Animation:
            isAnimate = !isAnimate;
            break;
        case Reset_Animation:
            resetPedestrians();
            break;
        case Three_D_Model:
            isUse3dModel = !isUse3dModel;
            break;
        case Density_Color:
            isDensityColoring = !isDensityColoring;
            break;
        case ZoomIN:
            camTransZ -= 0.5f;
            break;
        case ZoomOUT:
           camTransZ += 0.5f;
            break;
        case MENU_BACK_FRONT:
        {
            show = (MENU_TYPE) item;
        }
            break;
        default:
        {       /* Nothing */       }
            break;
    }
    
    glutPostRedisplay();
    
    return;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Pedestrian Visualization");

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	glewExperimental = GL_TRUE;
	glewInit();

	DisplayHelp();
	initGLContext();
	initTextures();
	initModels();
	initWalls();
	initPedestrian();
	glutReshapeFunc(resize);
	glutDisplayFunc(drawScene);
	glutTimerFunc(interval, onTimer, 0);
	glutKeyboardFunc(onKeyPress);
	glutMouseFunc(onMouseEvent);
	glutMotionFunc(onMouseMove);
   
    // Create a menu
    glutCreateMenu(menu);
    
    // Add menu items
    glutAddMenuEntry("Zoom In", ZoomIN);
    glutAddMenuEntry("Zoom Out", ZoomOUT);
    //glutAddMenuEntry("Rotate Camera: W/S/A/D",RotateCamera);
    glutAddMenuEntry("Run/Pause", Toogle_Animation);
    glutAddMenuEntry("Reset Animation", Reset_Animation);
    glutAddMenuEntry("3D Model", Three_D_Model);
    glutAddMenuEntry("Density as Color", Density_Color);
   
    
    // Associate a mouse button with menu
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    // exit menu

	atexit(onExit);
	glutMainLoop();

	

	return 0;
}
