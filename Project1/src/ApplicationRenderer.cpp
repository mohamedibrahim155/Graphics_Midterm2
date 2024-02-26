#include"ApplicationRenderer.h"


ApplicationRenderer::ApplicationRenderer()
{
    sceneViewcamera = new Camera();
    sceneViewcamera->name = "Sceneview Camera";

    gameScenecamera = new Camera();
    gameScenecamera->name = "GameScene Camera";

    renderTextureCamera1 = new Camera();
    renderTextureCamera1->name = "RenderTexture1 Camera";

    renderTextureCamera2 = new Camera();
    renderTextureCamera2->name = "RenderTexture2 Camera";

    renderTextureCamera3 = new Camera();
    renderTextureCamera3->name = "RenderTexture3 Camera";

    renderTextureCamera4 = new Camera();
    renderTextureCamera4->name = "RenderTexture4 Camera";

    renderTextureCamera5 = new Camera();
    renderTextureCamera5->name = "RenderTexture5 Camera";

    monitorRenderTexureCamera = new Camera();
    monitorRenderTexureCamera->name = "monitorRenderTexure Camera";
    
    monitorRenderTexureCamera2 = new Camera();
    monitorRenderTexureCamera2->name = "monitorRenderTexure Camera2";
}

ApplicationRenderer::~ApplicationRenderer()
{
}



void ApplicationRenderer::WindowInitialize(int width, int height,  std::string windowName)
{
    windowWidth = width;
    WindowHeight = height;
    lastX = windowWidth / 2.0f;
    lastY= WindowHeight / 2.0f;

    glfwInit();



    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int x, int y)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(w))->SetViewPort(w, x, y);
        });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(window))->KeyCallBack(window, key, scancode, action, mods);
        });


    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xposIn, double yposIn)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(window))->MouseCallBack(window, xposIn, yposIn);
        });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(window))->MouseScroll(window, xoffset, yoffset);
        });
   
   

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.FontGlobalScale = 2.0f;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
   // ImGui_ImplOpenGL3_Init("#version 130");

    //Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Query and print OpenGL version
    const GLubyte* version = glGetString(GL_VERSION);
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    }
    else 
    {
        std::cerr << "Failed to retrieve OpenGL version\n";
     
    }


    FrameBufferSpecification specification;

    specification.width = windowWidth;
    specification.height = WindowHeight;
    specification.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH };
    

    sceneViewframeBuffer = new FrameBuffer(specification);

    gameframeBuffer = new FrameBuffer(specification);

    EditorLayout::GetInstance().applicationRenderer = this;
  

    InitializeShaders();
   
    GraphicsRender::GetInstance().InitializeGraphics();

    DebugModels::GetInstance().defaultCube = new Model("Models/DefaultCube/DefaultCube.fbx", false, true);
    DebugModels::GetInstance().defaultSphere = new Model("Models/DefaultSphere/DefaultSphere.fbx", false, true);
    DebugModels::GetInstance().defaultQuad = new Model("Models/DefaultQuad/DefaultQuad.fbx", false, true);

    InitializeSkybox();

    GraphicsRender::GetInstance().SetCamera(sceneViewcamera);

    sceneViewcamera->InitializeCamera(CameraType::PERSPECTIVE, 45.0f, 0.1f, 1000.0f);
    //sceneViewcamera->transform.SetPosition(  glm::vec3(-2.06f, 2.50, 16.24f));
    sceneViewcamera->transform.SetPosition(  glm::vec3(1.92f, 2.79f, 8.77f));
    
    sceneViewcamera->transform.SetRotation(glm::vec3(-3.40f, 33.10f,0));
   // sceneViewcamera->transform.SetRotation(glm::vec3(0));

    gameScenecamera->InitializeCamera(CameraType::PERSPECTIVE, 45.0f, 0.1f, 1000.0f);
    //gameScenecamera->transform.SetPosition(glm::vec3(-2.06f, 2.50, 16.24f));
    //gameScenecamera->transform.SetRotation(glm::vec3(0));
    gameScenecamera->transform.SetPosition(glm::vec3(1.92f, 2.79f, 8.77f));
    gameScenecamera->transform.SetRotation(glm::vec3(-3.40f, 33.10f, 0));

    renderTextureCamera1->InitializeCamera(CameraType::PERSPECTIVE, 45.0f, 0.1f, 1000.0f);
    renderTextureCamera1->transform.position = glm::vec3(-51, 3.50f,50);
    renderTextureCamera1->transform.SetRotation(glm::vec3(0, 270, 0));

    renderTextureCamera1->IntializeRenderTexture(specification);


    renderTextureCamera2->InitializeCamera(CameraType::PERSPECTIVE, 45.0f, 0.1f, 1000.0f);
    renderTextureCamera2->transform.position = glm::vec3(0, -7.80f, 12.62f);
    renderTextureCamera2->transform.SetRotation(glm::vec3(35,175, 0));
    renderTextureCamera2->IntializeRenderTexture(specification);

    renderTextureCamera3->InitializeCamera(CameraType::PERSPECTIVE, 45.0f, 0.1f, 1000.0f);
    renderTextureCamera3->transform.position = glm::vec3(29.71f, -8.40f, 43.35f);
    renderTextureCamera3->transform.SetRotation(glm::vec3(43.10f, 90, 0));
    renderTextureCamera3->IntializeRenderTexture(specification);

    renderTextureCamera4->InitializeCamera(CameraType::PERSPECTIVE, 45.0f, 0.1f, 1000.0f);
    renderTextureCamera4->transform.position = glm::vec3(-18.97f, -25.55f, 29.54f);
    renderTextureCamera4->transform.SetRotation(glm::vec3(50.00f, 145.00f,0));
    renderTextureCamera4->IntializeRenderTexture(specification);

    renderTextureCamera5->InitializeCamera(CameraType::PERSPECTIVE, 45.0f, 0.1f, 1000.0f);
    renderTextureCamera5->transform.position = glm::vec3(-25.66f, 9.41f, 35.00f);
    renderTextureCamera5->transform.SetRotation(glm::vec3(-16.30f, 128.40f, 0.00));
    renderTextureCamera5->IntializeRenderTexture(specification);

    monitorRenderTexureCamera->InitializeCamera(CameraType::PERSPECTIVE, 45.0f, 0.1f, 1000.0f);
    monitorRenderTexureCamera->transform.position = glm::vec3(10.00f,0, 1.3f);
    monitorRenderTexureCamera->transform.SetRotation(glm::vec3(0));
    monitorRenderTexureCamera->IntializeRenderTexture(specification);

    monitorRenderTexureCamera2->InitializeCamera(CameraType::PERSPECTIVE, 45.0f, 0.1f, 1000.0f);
    monitorRenderTexureCamera2->transform.position = glm::vec3(20.0f, 0, 1.3f);
    monitorRenderTexureCamera2->transform.SetRotation(glm::vec3(0));
    monitorRenderTexureCamera2->IntializeRenderTexture(specification);
   // renderTextureCamera->IntializeRenderTexture(new RenderTexture());
  
    isImguiPanelsEnable = true;
}

void ApplicationRenderer::InitializeShaders()
{
    defaultShader = new Shader("Shaders/DefaultShader_Vertex.vert", "Shaders/DefaultShader_Fragment.frag");


    solidColorShader = new Shader("Shaders/SolidColor_Vertex.vert", "Shaders/SolidColor_Fragment.frag", SOLID);
    stencilShader = new Shader("Shaders/StencilOutline.vert", "Shaders/StencilOutline.frag", OPAQUE);
    //ScrollShader = new Shader("Shaders/ScrollTexture.vert", "Shaders/ScrollTexture.frag");

    alphaBlendShader = new Shader("Shaders/DefaultShader_Vertex.vert", "Shaders/DefaultShader_Fragment.frag", ALPHA_BLEND);
    alphaBlendShader->blendMode = ALPHA_BLEND;

    alphaCutoutShader = new Shader("Shaders/DefaultShader_Vertex.vert", "Shaders/DefaultShader_Fragment.frag", ALPHA_CUTOUT);
    alphaCutoutShader->blendMode = ALPHA_CUTOUT;

    alphaCombinedShader = new Shader("Shaders/AlphaBlendCombined.vert", "Shaders/AlphaBlendCombined.frag", ALPHA_BLEND);
    alphaCombinedShader->blendMode = ALPHA_BLEND;

    skyboxShader = new Shader("Shaders/SkyboxShader.vert", "Shaders/SkyboxShader.frag");
    skyboxShader->modelUniform = false;

    renderTextureShader = new Shader("Shaders/RenderTexture.vert", "Shaders/RenderTexture.frag");

    GraphicsRender::GetInstance().defaultShader = defaultShader;
    GraphicsRender::GetInstance().solidColorShader = solidColorShader;
    GraphicsRender::GetInstance().stencilShader = stencilShader; 
    GraphicsRender::GetInstance().renderTextureShader = renderTextureShader;

   

}

void ApplicationRenderer::InitializeSkybox()
{
    skyBoxModel = new Model("Models/DefaultCube/DefaultCube.fbx", false, true);
    skyBoxModel->meshes[0]->meshMaterial = new SkyboxMaterial();

    skyBoxMaterial = skyBoxModel->meshes[0]->meshMaterial->skyboxMaterial();

    std::vector<std::string> faces
    {
       ("Textures/skybox/right.jpg"),
       ("Textures/skybox/left.jpg"),
       ("Textures/skybox/top.jpg"),
       ("Textures/skybox/bottom.jpg"),
       ("Textures/skybox/front.jpg"),
       ("Textures/skybox/back.jpg")
    };

    skyBoxMaterial->skyBoxTexture->LoadTexture(faces);

    GraphicsRender::GetInstance().SkyBoxModel = skyBoxModel;
}



void ApplicationRenderer::Start()
{


    sceneViewcamera->postprocessing->InitializePostProcessing();

    gameScenecamera->postprocessing->InitializePostProcessing();

    monitorRenderTexureCamera->postprocessing->InitializePostProcessing();
    monitorRenderTexureCamera->isPostprocessing = true;
    monitorRenderTexureCamera->postprocessing->pixelization->isEnabled = true;
    monitorRenderTexureCamera->postprocessing->chromatic->isEnabled = false;
    monitorRenderTexureCamera->postprocessing->flicker->isEnabled = true;
    monitorRenderTexureCamera->postprocessing->staticEffect->isEnabled = true;

    monitorRenderTexureCamera2->postprocessing->InitializePostProcessing();

    monitorRenderTexureCamera2->isPostprocessing = true;
    monitorRenderTexureCamera2->postprocessing->pixelization->isEnabled = true;
    monitorRenderTexureCamera2->postprocessing->chromatic->isEnabled = false;
    monitorRenderTexureCamera2->postprocessing->flicker->isEnabled = true;
    monitorRenderTexureCamera2->postprocessing->staticEffect->isEnabled = true;
  

     Model* directionLightModel = new Model("Models/DefaultSphere/Sphere_1_unit_Radius.ply",false, true);
     directionLightModel->transform.SetScale(glm::vec3(0.5f));
    // Model* spotlight = new Model(*Sphere);
     //spotlight->transform.SetPosition(glm::vec3(-2.0f, 0.0f, -3.0f));

     Light* directionLight = new Light();
     directionLight->Initialize(LightType::DIRECTION_LIGHT, 1);
     directionLight->SetAmbientColor(glm::vec4(1, 1, 1, 1.0f));
     //directionLight->SetAmbientColor(glm::vec4(15, 15, 15, 1.0f));
   //  directionLight->SetAmbientColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
     directionLight->SetIntensity(0.75f);
     directionLight->SetColor(glm::vec4(1, 1, 1, 1.0f));
     directionLight->SetAttenuation(1, 1, 0.01f);
     directionLight->SetInnerAndOuterCutoffAngle(20, 25);

     directionLight->transform.SetPosition(glm::vec3(-2.30f, 2.20f, 3));
 


     Light* pointLight = new Light();
     pointLight->Initialize(LightType::POINT_LIGHT, 1);
     pointLight->SetAmbientColor(glm::vec4(1.5f, 1.5f, 1.5f, 1.0f));
     //directionLight->SetAmbientColor(glm::vec4(15, 15, 15, 1.0f));
   //  directionLight->SetAmbientColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
     pointLight->SetIntensity(0.75f);
     pointLight->SetColor(glm::vec4(1, 1, 1, 1.0f));
     pointLight->SetAttenuation(1, 1, 0.01f);
     pointLight->SetInnerAndOuterCutoffAngle(20, 25);

     pointLight->transform.SetPosition(glm::vec3(0, 0.60f, 28.90f));


     Light* pointLight2 = new Light();
     pointLight2->Initialize(LightType::POINT_LIGHT, 1);
     pointLight2->SetAmbientColor(glm::vec4(1, 1, 1, 1.0f));
     //directionLight->SetAmbientColor(glm::vec4(15, 15, 15, 1.0f));
   //  directionLight->SetAmbientColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
     pointLight2->SetIntensity(0.75f);
     pointLight2->SetColor(glm::vec4(1, 1, 1, 1.0f));
     pointLight2->SetAttenuation(1, 1, 0.01f);
     pointLight2->SetInnerAndOuterCutoffAngle(20, 25);

     pointLight2->transform.SetPosition(glm::vec3(21.20f, 3.00f, 37.90f));


     Light* pointLight3 = new Light();
     pointLight3->Initialize(LightType::POINT_LIGHT, 1);
     pointLight3->SetAmbientColor(glm::vec4(1.5f, 1.5f, 1.5f, 1.0f));     //directionLight->SetAmbientColor(glm::vec4(15, 15, 15, 1.0f));
   //  directionLight->SetAmbientColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
     pointLight3->SetIntensity(0.75f);
     pointLight3->SetColor(glm::vec4(1, 1, 1, 1.0f));
     pointLight3->SetAttenuation(1, 1, 0.01f);
     pointLight3->SetInnerAndOuterCutoffAngle(20, 25);
               
     pointLight3->transform.SetPosition(glm::vec3(-21.0f, 2.10f, 44.90f));

     directionLight->isVisible = false;
     pointLight->isVisible = false;
     pointLight2->isVisible = false;
     pointLight3->isVisible = false;


     GameScene();
     AsteroidScene();

}

void ApplicationRenderer::GameScene()
{


    std::string scifiTexturePath = "Models/Spacestation/SciFi.jpg";
    std::string scifi2TexturePath = "Models/Spacestation/SciFi2.jpg";
    std::string scifi3TexturePath = "Models/Spacestation/SciFi3.jpg";
    std::string scifi4TexturePath = "Models/Spacestation/SciFi4.jpg";

    std::string codeTexurePath = "Models/Spacestation/ComputerCode.jpg";
    std::string codeTexurePath2 = "Models/Spacestation/ComputerCode2.jpg";
    std::string codeTexurePath3 = "Models/Spacestation/ComputerCode3.jpg";
    std::string codeTexurePath4 = "Models/Spacestation/ComputerCode4.jpg";

    Texture* sciFiTexture = new Texture(scifiTexturePath);
    Texture* sciFiTexture2 = new Texture(scifi2TexturePath);
    Texture* sciFiTexture3 = new Texture(scifi3TexturePath);
    Texture* sciFiTexture4 = new Texture(scifi4TexturePath);

    Texture* codeTexture = new Texture(codeTexurePath);
    Texture* codeTexture2 = new Texture(codeTexurePath2);
    Texture* codeTexture3 = new Texture(codeTexurePath3);
    Texture* codeTexture4 = new Texture(codeTexurePath4);


    RenderQuad* quadWithTexture = new RenderQuad();
    //Model* quadWithTexture = new Model("Models/DefaultQuad/DefaultQuad.fbx");
    quadWithTexture->transform.SetPosition(glm::vec3(10, 0, 0));
    quadWithTexture->SetTexture(sciFiTexture);
    quadWithTexture->SetChangeTextures({ sciFiTexture,sciFiTexture2, sciFiTexture3,sciFiTexture4 });


    //GraphicsRender::GetInstance().AddModelAndShader(quadWithTexture, defaultShader);

    RenderQuad* quadWithTexture2 = new RenderQuad();
    // Model* quadWithTexture2 = new Model("Models/DefaultQuad/DefaultQuad.fbx");
    quadWithTexture2->transform.SetPosition(glm::vec3(20, 0, 0));
    quadWithTexture2->SetTexture(codeTexture);
    quadWithTexture2->SetChangeTextures({ codeTexture, codeTexture2,codeTexture3,codeTexture4 });

    //  GraphicsRender::GetInstance().AddModelAndShader(quadWithTexture2, defaultShader);

    std::string diffuseTextureFile = "Models/Spacestation/SpaceInteriors_Texture.png";

    std::string UVMapperTextureFile = "Models/Spacestation/uv_mapper.png";
    std::string fingerprintTextureFile = "Models/Spacestation/Fingerprint_3.png";
    std::string fingerprint2TextureFile = "Models/Spacestation/Fingerprint_4.png";
    std::string scratchTextureFile = "Models/Spacestation/Scratch.png";

    std::string scratch2TexxtureFile = "Models/Spacestation/ScratchesGlass.png";

    Texture* interiorTexture = new Texture(diffuseTextureFile);
    Texture* UVMapperTexture = new Texture(UVMapperTextureFile);
    Texture* fingerprintTexture = new Texture(fingerprintTextureFile);
    Texture* fingerprint2Texture = new Texture(fingerprint2TextureFile);
    Texture* fogTexture = new Texture(scratch2TexxtureFile);
    Texture* scratchTexture = new Texture(scratchTextureFile);

    Model* console1 = new Model("Models/Spacestation/SM_Env_Consoles_01_xyz_n_rgba_uv.ply");
    console1->meshes[0]->meshMaterial->material()->diffuseTexture = interiorTexture;
   
    GraphicsRender::GetInstance().AddModelAndShader(console1, defaultShader);

    Model* cornerConsole1 = new Model("Models/Spacestation/SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply");
    cornerConsole1->name = "Console 1";
    cornerConsole1->meshes[0]->meshMaterial->material()->diffuseTexture = interiorTexture;
    cornerConsole1->transform.SetPosition(glm::vec3(-10, 0, 5));
    GraphicsRender::GetInstance().AddModelAndShader(cornerConsole1, defaultShader);

    Model* cornerConsole2 = new Model("Models/Spacestation/SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply");
    cornerConsole2->name = "Console 2";
    cornerConsole2->meshes[0]->meshMaterial->material()->diffuseTexture = interiorTexture;
    cornerConsole2->transform.SetPosition(glm::vec3(5, 0, 5));
    cornerConsole2->transform.SetScale(glm::vec3(-1, 1, 1));
    GraphicsRender::GetInstance().AddModelAndShader(cornerConsole2, defaultShader);



    Model* consoleScreen1 = new Model("Models/Spacestation/SM_Env_Consoles_01_screen_1_xyz_n_rgba_uv.ply");
    consoleScreen1->name = "Console  Screen 1";
    consoleScreen1->meshes[0]->meshMaterial->material()->diffuseTexture = monitorRenderTexureCamera->renderTexture;
  //  corner1Screen1->transform.SetPosition(glm::vec3(5, 0, 5));
  //  corner1Screen1->transform.SetScale(glm::vec3(-1, 1, 1));
    GraphicsRender::GetInstance().AddModelAndShader(consoleScreen1, renderTextureShader);



    Model* consoleScreen2 = new Model("Models/Spacestation/SM_Env_Consoles_01_screen_2_xyz_n_rgba_uv.ply");
    consoleScreen2->name = "Console  Screen 2";
    consoleScreen2->meshes[0]->meshMaterial->material()->alphaTexture = fingerprintTexture;    //UVMAP
    consoleScreen2->meshes[0]->meshMaterial->material()->combineTexure = fogTexture;    //UVMAP
    consoleScreen2->meshes[0]->meshMaterial->material()->useMaskTexture = true;
    GraphicsRender::GetInstance().AddModelAndShader(consoleScreen2, alphaCombinedShader);



    Model* consoleScreen3 = new Model("Models/Spacestation/SM_Env_Consoles_01_screen_3_xyz_n_rgba_uv.ply");
    consoleScreen3->name = "Console  Screen 3";
    consoleScreen3->meshes[0]->meshMaterial->material()->diffuseTexture = monitorRenderTexureCamera2->renderTexture;
    GraphicsRender::GetInstance().AddModelAndShader(consoleScreen3, renderTextureShader);
    

    ComputerScreen* corner1Screen1 = new ComputerScreen(0);
    corner1Screen1->name = "Corner_Console_1_Screen_1";
    corner1Screen1->SetRenderTextures({ renderTextureCamera1->renderTexture ,renderTextureCamera2->renderTexture,
        renderTextureCamera3->renderTexture,renderTextureCamera4->renderTexture,renderTextureCamera5->renderTexture });

    corner1Screen1->transform.SetPosition(glm::vec3(2, 0, 7));
    corner1Screen1->transform.SetRotation(glm::vec3(0, -90, 0));
  

    ComputerScreen* corner2Screen1 = new ComputerScreen(0);
    corner2Screen1->name = "Corner_Console_2_Screen_1";

    corner2Screen1->SetRenderTextures({ renderTextureCamera1->renderTexture ,renderTextureCamera2->renderTexture,
    renderTextureCamera3->renderTexture,renderTextureCamera4->renderTexture,renderTextureCamera5->renderTexture });

    corner2Screen1->transform.SetPosition(glm::vec3(-7.00f, 0, -2.00f));
    corner2Screen1->transform.SetRotation(glm::vec3(0, 90, 0));
  


    Model* corner1Screen2 = new Model("Models/Spacestation/SM_Env_Consoles_Corner_01_screen_2_xyz_n_rgba_uv.ply");
    corner1Screen2->name = "Corner_Console_1_Screen_2";
    corner1Screen2->meshes[0]->meshMaterial->material()->alphaTexture = fingerprint2Texture;
    corner1Screen2->meshes[0]->meshMaterial->material()->combineTexure = fogTexture;
    corner1Screen2->meshes[0]->meshMaterial->material()->useMaskTexture = true;  //UVMAP
    corner1Screen2->transform.SetPosition(glm::vec3(-10.0f, 0, 5));
    GraphicsRender::GetInstance().AddModelAndShader(corner1Screen2, alphaCombinedShader);

    Model* corner2Screen2 = new Model("Models/Spacestation/SM_Env_Consoles_Corner_01_screen_2_xyz_n_rgba_uv.ply");
    corner2Screen2->name = "Corner_Console_2_Screen_2";
    
   // corner2Screen2->meshes[0]->meshMaterial->material()->diffuseTexture = scratchTexture; //UVMAP
    corner2Screen2->meshes[0]->meshMaterial->material()->alphaTexture = fingerprintTexture; //UVMAP
    corner2Screen2->meshes[0]->meshMaterial->material()->combineTexure = scratchTexture; //UVMAP
    corner2Screen2->meshes[0]->meshMaterial->material()->useMaskTexture = true; 
    corner2Screen2->transform.SetPosition(glm::vec3(5.0f, 0, 5));
    corner2Screen2->transform.SetScale(glm::vec3(-1, 1, 1));
    GraphicsRender::GetInstance().AddModelAndShader(corner2Screen2, alphaCombinedShader);



    Model* floor1 = new Model("Models/Spacestation/Ply (converted)/SM_Env_Floor_01_xyz_n_rgba_uv.ply");
    floor1->name = "Floor1";
    floor1->meshes[0]->meshMaterial->material()->diffuseTexture = interiorTexture;
    floor1->transform.SetPosition(glm::vec3(2.5f, 0, 0));
    floor1->transform.SetScale(glm::vec3(1, 1, 1));
    GraphicsRender::GetInstance().AddModelAndShader(floor1, defaultShader);

    Model* floor2 = new Model("Models/Spacestation/Ply (converted)/SM_Env_Floor_01_xyz_n_rgba_uv.ply");
    floor2->name = "Floor2";
    floor2->meshes[0]->meshMaterial->material()->diffuseTexture = interiorTexture; 
    floor2->transform.SetPosition(glm::vec3(-2.5f, 0, 0));
    floor2->transform.SetScale(glm::vec3(1, 1, 1));
    GraphicsRender::GetInstance().AddModelAndShader(floor2, defaultShader);
    

    Model* floor3 = new Model("Models/Spacestation/Ply (converted)/SM_Env_Floor_01_xyz_n_rgba_uv.ply");
    floor3->name = "Floor3";
    floor3->meshes[0]->meshMaterial->material()->diffuseTexture = interiorTexture;
    floor3->transform.SetPosition(glm::vec3(2.5f, 0, 5));
    floor3->transform.SetScale(glm::vec3(1, 1, 1));
    GraphicsRender::GetInstance().AddModelAndShader(floor3, defaultShader);


    Model* floor4 = new Model("Models/Spacestation/Ply (converted)/SM_Env_Floor_01_xyz_n_rgba_uv.ply");
    floor4->name = "Floor4";
    floor4->meshes[0]->meshMaterial->material()->diffuseTexture = interiorTexture; 
    floor4->transform.SetPosition(glm::vec3(-2.5f, 0, 5));
    floor4->transform.SetScale(glm::vec3(1, 1, 1));
    GraphicsRender::GetInstance().AddModelAndShader(floor4, defaultShader);



    Model* Wall1 = new Model("Models/Spacestation/Ply (converted)/SM_Env_Wall_Curved_01_xyz_n_rgba_uv.ply");
    Wall1->name = "Wall1";
    Wall1->meshes[0]->meshMaterial->material()->diffuseTexture = interiorTexture;
    Wall1->transform.SetPosition(glm::vec3(3.5f, 0, 10));
    Wall1->transform.SetRotation(glm::vec3(0, -90, 0));
    Wall1->transform.SetScale(glm::vec3(1, 1, 1));
    GraphicsRender::GetInstance().AddModelAndShader(Wall1, defaultShader);

    Model* Wall2 = new Model("Models/Spacestation/Ply (converted)/SM_Env_Wall_Curved_01_xyz_n_rgba_uv.ply");
    Wall2->name = "Wall2";
    Wall2->meshes[0]->meshMaterial->material()->diffuseTexture = interiorTexture;
    Wall2->transform.SetPosition(glm::vec3(-8.70f, 0, 5.0f));
    Wall2->transform.SetRotation(glm::vec3(0, 90, 0));
    Wall2->transform.SetScale(glm::vec3(1, 1, 1));
    GraphicsRender::GetInstance().AddModelAndShader(Wall2, defaultShader);


   

}

void ApplicationRenderer::AsteroidScene()
{


    Asteroid* asteroidMain = new Asteroid(1);
    asteroidMain->name = "AsteroidMain";
    asteroidMain->transform.SetPosition(glm::vec3(0, 0, 20));
    asteroidMain->transform.SetRotation(glm::vec3(0, 180, 0));

    Asteroid* asteroid2 = new Asteroid(2);
    asteroid2->name = "asteroid2";
    asteroid2->transform.SetPosition(glm::vec3(0, 0, 20));
    asteroid2->transform.SetRotation(glm::vec3(0, 180, 0));

    Asteroid* asteroid3 = new Asteroid(3);
    asteroid3->name = "asteroid3";
    asteroid3->transform.SetPosition(glm::vec3(0, 0, 20));
    asteroid3->transform.SetRotation(glm::vec3(0, 180, 0));
 
    Asteroid* asteroid4 = new Asteroid(0);
    asteroid4->name = "asteroid4";
    asteroid4->transform.SetPosition(glm::vec3(0, 0, 20));
    asteroid4->transform.SetRotation(glm::vec3(0, 180, 0));
 

 
}

void ApplicationRenderer::PreRender()
{
    projection = sceneViewcamera->GetProjectionMatrix();

    view = sceneViewcamera->GetViewMatrix();

    skyBoxView = glm::mat4(glm::mat3(sceneViewcamera->GetViewMatrix()));
  

    defaultShader->Bind();
    LightManager::GetInstance().UpdateUniformValuesToShader(defaultShader);

    defaultShader->setMat4("projection", projection);
    defaultShader->setMat4("view", view);
    defaultShader->setVec3("viewPos", sceneViewcamera->transform.position.x, sceneViewcamera->transform.position.y, sceneViewcamera->transform.position.z);
    defaultShader->setFloat("time", scrollTime);
    defaultShader->setBool("isDepthBuffer", false);

    alphaBlendShader->Bind();
    LightManager::GetInstance().UpdateUniformValuesToShader(alphaBlendShader);
    alphaBlendShader->setMat4("projection", projection);
    alphaBlendShader->setMat4("view", view);
    alphaBlendShader->setVec3("viewPos", sceneViewcamera->transform.position.x, sceneViewcamera->transform.position.y, sceneViewcamera->transform.position.z);
    alphaBlendShader->setFloat("time", scrollTime);
    alphaBlendShader->setBool("isDepthBuffer", false);

    alphaCutoutShader->Bind();
    LightManager::GetInstance().UpdateUniformValuesToShader(alphaCutoutShader);
    alphaCutoutShader->setMat4("projection", projection);
    alphaCutoutShader->setMat4("view", view);
    alphaCutoutShader->setVec3("viewPos", sceneViewcamera->transform.position.x, sceneViewcamera->transform.position.y, sceneViewcamera->transform.position.z);
    alphaCutoutShader->setFloat("time", scrollTime);
    alphaCutoutShader->setBool("isDepthBuffer", false);

    solidColorShader->Bind();
    solidColorShader->setMat4("projection", projection);
    solidColorShader->setMat4("view", view);

    stencilShader->Bind();
    stencilShader->setMat4("projection", projection);
    stencilShader->setMat4("view", view);

    glDepthFunc(GL_LEQUAL);
    skyboxShader->Bind();
    skyboxShader->setMat4("projection", projection);
    skyboxShader->setMat4("view", skyBoxView);

    GraphicsRender::GetInstance().SkyBoxModel->Draw(*skyboxShader);
    glDepthFunc(GL_LESS);


    /* ScrollShader->Bind();
       ScrollShader->setMat4("ProjectionMatrix", _projection);*/

}

void ApplicationRenderer::Render()
{
   
    Start();
  
    EditorLayout::GetInstance().InitializeEditors();

    Time::GetInstance().lastFrame = glfwGetTime();
   // glEnable(GL_BLEND);
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
    while (!glfwWindowShouldClose(window))
    {

        Time::GetInstance().SetCurrentTime(glfwGetTime());
       
      
       // scrollTime += Time::GetInstance().deltaTime;

        EngineGameLoop();

        EngineGraphicsRender();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void ApplicationRenderer::EngineGraphicsRender()
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    if (isImguiPanelsEnable)
    {
        PanelManager::GetInstance().Update((float)windowWidth, (float)WindowHeight);
    }

   


    /*sceneViewframeBuffer->Bind();

    GraphicsRender::GetInstance().Clear();
    PreRender();
    GraphicsRender::GetInstance().Draw();

    sceneViewframeBuffer->Unbind();*/
    RenderForCamera(sceneViewcamera, sceneViewframeBuffer);


  /*  RenderForCamera(gameScenecamera, gameframeBuffer);

    RenderForCamera(renderTextureCamera, renderTextureCamera->renderTexture->framebuffer);*/


    for (Camera* camera :  CameraManager::GetInstance().GetCameras())
    {
        if (camera->renderTexture == nullptr)
        {
            RenderForCamera(camera, gameframeBuffer);                  // GAME SCENE CAMERA

           
        }
        else
        {
            RenderForCamera(camera, camera->renderTexture->framebuffer); 
        }
       
    }

    //gameframeBuffer->Bind();
    //GraphicsRender::GetInstance().Clear();
    //PreRender();
    //GraphicsRender::GetInstance().Draw();

    //gameframeBuffer->Unbind();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void ApplicationRenderer::EngineGameLoop()
{
    ProcessInput(window);

    if (isPlayMode)
    {
        EntityManager::GetInstance().Update(Time::GetInstance().deltaTime);
    }

    PostRender();
}
void ApplicationRenderer::RenderForCamera(Camera* camera, FrameBuffer* framebuffer)
{

  /*  sceneViewframeBuffer->Bind();

    GraphicsRender::GetInstance().Clear();
    PreRender();
    GraphicsRender::GetInstance().Draw();*/


    framebuffer->Bind();

    GraphicsRender::GetInstance().Clear();

    projection = camera->GetProjectionMatrix();

    view = camera->GetViewMatrix();

    skyBoxView = glm::mat4(glm::mat3(camera->GetViewMatrix()));


    renderTextureShader->Bind();
    LightManager::GetInstance().UpdateUniformValuesToShader(renderTextureShader);

    renderTextureShader->setMat4("projection", projection);
    renderTextureShader->setMat4("view", view);
    renderTextureShader->setVec3("viewPos", camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);
    renderTextureShader->setFloat("time", scrollTime);
    renderTextureShader->setBool("isDepthBuffer", false);

    defaultShader->Bind();
    LightManager::GetInstance().UpdateUniformValuesToShader(defaultShader);

    defaultShader->setMat4("projection", projection);
    defaultShader->setMat4("view", view);
    defaultShader->setVec3("viewPos", camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);
    defaultShader->setFloat("time", scrollTime);
    defaultShader->setBool("isDepthBuffer", false);

    


    alphaBlendShader->Bind();
    LightManager::GetInstance().UpdateUniformValuesToShader(alphaBlendShader);
    alphaBlendShader->setMat4("projection", projection);
    alphaBlendShader->setMat4("view", view);
    alphaBlendShader->setVec3("viewPos", camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);
    alphaBlendShader->setFloat("time", scrollTime);
    alphaBlendShader->setBool("isDepthBuffer", false);

    alphaCombinedShader->Bind();
    LightManager::GetInstance().UpdateUniformValuesToShader(alphaCombinedShader);
    alphaCombinedShader->setMat4("projection", projection);
    alphaCombinedShader->setMat4("view", view);
    alphaCombinedShader->setVec3("viewPos", camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);
    alphaCombinedShader->setFloat("time", scrollTime);
    alphaCombinedShader->setBool("isDepthBuffer", false);

    alphaCutoutShader->Bind();
    LightManager::GetInstance().UpdateUniformValuesToShader(alphaCutoutShader);
    alphaCutoutShader->setMat4("projection", projection);
    alphaCutoutShader->setMat4("view", view);
    alphaCutoutShader->setVec3("viewPos", camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);
    alphaCutoutShader->setFloat("time", scrollTime);
    alphaCutoutShader->setBool("isDepthBuffer", false);

    solidColorShader->Bind();
    solidColorShader->setMat4("projection", projection);
    solidColorShader->setMat4("view", view);

    stencilShader->Bind();
    stencilShader->setMat4("projection", projection);
    stencilShader->setMat4("view", view);

    glDepthFunc(GL_LEQUAL);
    skyboxShader->Bind();
    skyboxShader->setMat4("projection", projection);
    skyboxShader->setMat4("view", skyBoxView);

    GraphicsRender::GetInstance().SkyBoxModel->Draw(*skyboxShader);
    glDepthFunc(GL_LESS);

    
    /* ScrollShader->Bind();
       ScrollShader->setMat4("ProjectionMatrix", _projection);*/

    GraphicsRender::GetInstance().Draw();

    framebuffer->Unbind();

    if (camera->isPostprocessing)
    {
       // if (camera->postprocessing.isPostProccesingEnabled)
        {
            camera->postprocessing->ApplyPostprocessing(framebuffer);
        }
    }



}
void ApplicationRenderer::PostRender()
{
   // glDisable(GL_BLEND);

}

void ApplicationRenderer::Clear()
{
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 0.1f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
   //glStencilMask(0x00);
}

void ApplicationRenderer::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed=25;

    if (EditorLayout::GetInstance().IsViewportHovered())
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            sceneViewcamera->ProcessKeyboard(FORWARD, Time::GetInstance().deltaTime * cameraSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            sceneViewcamera->ProcessKeyboard(BACKWARD, Time::GetInstance().deltaTime * cameraSpeed);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            sceneViewcamera->ProcessKeyboard(LEFT, Time::GetInstance().deltaTime * cameraSpeed);

        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            sceneViewcamera->ProcessKeyboard(RIGHT, Time::GetInstance().deltaTime * cameraSpeed);

        }
    }

}


 void ApplicationRenderer::SetViewPort(GLFWwindow* window, int width, int height)
{
 //   glViewport(0, 0, width, height);
}

 void ApplicationRenderer::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
 {  
         if (key == GLFW_KEY_V && action == GLFW_PRESS)
         {

            
             std::cout << "V pressed" << std::endl;

             std::vector<Model*> listOfModels = GraphicsRender::GetInstance().GetModelList();
            
          

             selectedModelCount++;

             if (selectedModelCount > listOfModels.size()-1)
             {
                 selectedModelCount = 0;
             }

            
             GraphicsRender::GetInstance().selectedModel = listOfModels[selectedModelCount];


         }

         if (!isPlayMode) return;

         if (action == GLFW_PRESS)
         {
             InputManager::GetInstance().OnKeyPressed(key);
         }
         else if(action == GLFW_RELEASE)
         {
             InputManager::GetInstance().OnKeyReleased(key);
         }
         else if (action == GLFW_REPEAT)
         {
             InputManager::GetInstance().OnkeyHold(key);
         }
     
 }

 void ApplicationRenderer::MouseCallBack(GLFWwindow* window, double xposIn, double yposIn)
 {

    float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);
     
        if (firstMouse)
        {

        }

         if (firstMouse)
         {
             lastX = xpos;
             lastY = ypos;
             firstMouse = false;
         }
     
         float xoffset = xpos - lastX;
         float yoffset = lastY - ypos;
     
         lastX = xpos;
         lastY = ypos;
     
         if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && EditorLayout::GetInstance().IsViewportHovered())
         {
             sceneViewcamera->ProcessMouseMovement(xoffset, yoffset);
         }
 }

 void ApplicationRenderer::MouseScroll(GLFWwindow* window, double xoffset, double yoffset)
 {
     sceneViewcamera->ProcessMouseScroll(static_cast<float>(yoffset));
 }
