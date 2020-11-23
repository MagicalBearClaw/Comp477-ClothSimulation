#include "stdafx.h"
#include "ClothSimulationApplication.h"

ClothSimulationApplication::ClothSimulationApplication(const std::string& windowTitle, int windowWith, int windowHeight)
    : Application(windowTitle, windowWith, windowHeight)
{
}

bool ClothSimulationApplication::Initialize()
{
    if (!Application::Initialize())
    {
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    _imguiIO = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForD3D(_window);
    ImGui_ImplDX11_Init(_renderContext->GetDevice(), _renderContext->GetDeviceContext());

    _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    _camera = std::make_unique<Camera>();
    if (!_camera)
    {
        return false;
    }

    // Set the initial position of the camera.
    _camera->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));
    _camera->UpdateBaseViewMatrix();

    _cubeModel = std::make_unique<Model>(_renderContext);
    if (!_cubeModel)
    {
        return false;
    }

    bool result = _cubeModel->Initialize("./Assets/Meshes/cube.txt", "./Assets/Textures/wall01.png");
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the cube model object.", L"Error", MB_OK);
        return false;
    }

    // Set the position for the cube model.
    _cubeModel->SetPosition(DirectX::XMFLOAT3(-2.0f, 2.0f, 0.0f));

    // Create the sphere model object.
    _sphereModel = std::make_unique<Model>(_renderContext);
    if (!_sphereModel)
    {
        return false;
    }

    // Initialize the sphere model object.
    result = _sphereModel->Initialize("./Assets/Meshes/sphere.txt", "./Assets/Textures/ice.png");
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the sphere model object.", L"Error", MB_OK);
        return false;
    }

    // Set the position for the sphere model.
    _sphereModel->SetPosition(DirectX::XMFLOAT3(2.0f, 2.0f, 0.0f));

    // Create the ground model object.
    _floorModel = std::make_unique<Model>(_renderContext);
    if (!_floorModel)
    {
        return false;
    }

    // Initialize the ground model object.
    result = _floorModel->Initialize("./Assets/Meshes/plane.txt", "./Assets/Textures/metal001.png");
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the ground model object.", L"Error", MB_OK);
        return false;
    }

    // Set the position for the ground model.
    _floorModel->SetPosition(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));

    _light = std::make_unique<Light>(_renderContext);
    if (!_light)
    {
        return false;
    }


    // Initialize the light object.
    _light->SetAmbientColor(DirectX::XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f));
    _light->SetDiffuseColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    _light->SetLookAt(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    _light->CreateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

    // Create the render to texture object.
    _renderTexture = std::make_unique<RenderTexture>(_renderContext);
    if (!_renderTexture)
    {
        return false;
    }

    // Initialize the render to texture object.
    result = _renderTexture->Create(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the render to texture object.", L"Error", MB_OK);
        return false;
    }

    // Create the depth shader object.
    _depthShader = std::make_unique<DepthShader>(_renderContext);
    if (!_depthShader)
    {
        return false;
    }

    // Initialize the depth shader object.
    result = _depthShader->Initialize();
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the depth shader object.", L"Error", MB_OK);
        return false;
    }

    // Create the black and white render to texture object.
    _blackWhiteRenderTexture = std::make_unique<RenderTexture>(_renderContext);
    if (!_blackWhiteRenderTexture)
    {
        return false;
    }

    // Initialize the black and white render to texture object.
    result = _blackWhiteRenderTexture->Create(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the black and white render to texture object.", L"Error", MB_OK);
        return false;
    }

    // Create the shadow shader object.
    _shadowShader = std::make_unique<ShadowShader>(_renderContext);
    if (!_shadowShader)
    {
        return false;
    }

    // Initialize the shadow shader object.
    result = _shadowShader->Initialize();
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the shadow shader object.", L"Error", MB_OK);
        return false;
    }

    // Set the size to sample down to.
    int downSampleWidth = SHADOWMAP_WIDTH / 2;
    int downSampleHeight = SHADOWMAP_HEIGHT / 2;

    // Create the down sample render to texture object.
    _downSampleTexture = std::make_unique<RenderTexture>(_renderContext);
    if (!_downSampleTexture)
    {
        return false;
    }

    // Initialize the down sample render to texture object.
    result = _downSampleTexture->Create(downSampleWidth, downSampleHeight, 100.0f, 1.0f);
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the down sample render to texture object.", L"Error", MB_OK);
        return false;
    }

    // Create the small ortho window object.
    _orthographicWindow = std::make_unique<OrthographicWindow>(_renderContext);
    if (!_orthographicWindow)
    {
        return false;
    }

    // Initialize the small ortho window object.
    result = _orthographicWindow->Initialize(downSampleWidth, downSampleHeight);
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the small ortho window object.", L"Error", MB_OK);
        return false;
    }

    // Create the texture shader object.
    _textureShader = std::make_unique<TextureShader>(_renderContext);
    if (!_textureShader)
    {
        return false;
    }

    // Initialize the texture shader object.
    result = _textureShader->Initialize();
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the texture shader object.", L"Error", MB_OK);
        return false;
    }

    // Create the horizontal blur render to texture object.
    _horizontalBlurTexture = std::make_unique<RenderTexture>(_renderContext);
    if (!_horizontalBlurTexture)
    {
        return false;
    }

    // Initialize the horizontal blur render to texture object.
    result = _horizontalBlurTexture->Create(downSampleWidth, downSampleHeight, SCREEN_DEPTH, 0.1f);
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
        return false;
    }

    // Create the horizontal blur shader object.
    _horizontalBlurShader = std::make_unique<HorizontalBlurShader>(_renderContext);
    if (!_horizontalBlurShader)
    {
        return false;
    }

    // Initialize the horizontal blur shader object.
    result = _horizontalBlurShader->Initialize();
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
        return false;
    }

    // Create the vertical blur render to texture object.
    _verticalBlurTexture = std::make_unique<RenderTexture>(_renderContext);
    if (!_verticalBlurTexture)
    {
        return false;
    }

    // Initialize the vertical blur render to texture object.
    result = _verticalBlurTexture->Create(downSampleWidth, downSampleHeight, SCREEN_DEPTH, 0.1f);
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the vertical blur render to texture object.", L"Error", MB_OK);
        return false;
    }

    // Create the vertical blur shader object.
    _verticalBlurShader = std::make_unique<VerticalBlurShader>(_renderContext);;
    if (!_verticalBlurShader)
    {
        return false;
    }

    // Initialize the vertical blur shader object.
    result = _verticalBlurShader->Initialize();
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the vertical blur shader object.", L"Error", MB_OK);
        return false;
    }

    // Create the up sample render to texture object.
    _upSampleTexure = std::make_unique<RenderTexture>(_renderContext);
    if (!_upSampleTexure)
    {
        return false;
    }

    // Initialize the up sample render to texture object.
    result = _upSampleTexure->Create(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, 0.1f);
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the up sample render to texture object.", L"Error", MB_OK);
        return false;
    }

    // Create the full screen ortho window object.
    _fullScreenOrthographicWindow = std::make_unique<OrthographicWindow>(_renderContext);
    if (!_fullScreenOrthographicWindow)
    {
        return false;
    }

    // Initialize the full screen ortho window object.
    result = _fullScreenOrthographicWindow->Initialize(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
        return false;
    }

    // Create the soft shadow shader object.
    _softShadowShader = std::make_unique<SoftShadowShader>(_renderContext);
    if (!_softShadowShader)
    {
        return false;
    }

    // Initialize the soft shadow shader object.
    result = _softShadowShader->Initialize();
    if (!result)
    {
        MessageBoxW(_windowHandle, L"Could not initialize the soft shadow shader object.", L"Error", MB_OK);
        return false;
    }


    return true;
}

bool ClothSimulationApplication::HandleWindowEvent(SDL_Event& event)
{

    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
        return true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(_window))
        return true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(_window))
    {
        // Release all outstanding references to the swap chain's buffers before resizing.
        _renderContext->CleanupRenderTarget();
        _renderContext->GetSwapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
        _renderContext->CreateRenderTarget();
    }

    return false;
}

void ClothSimulationApplication::Draw(float deltaTime)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);
    ImGui::NewFrame();

    // Rendering
    ImGui::Render();

    RenderSceneToTexture();
    RenderBlackAndWhiteShadows();
    DownSampleTexture();
    RenderHorizontalBlurToTexture();
    RenderVerticalBlurToTexture();
    UpSampleTexture();

    ID3D11RenderTargetView* mainView = _renderContext->GetMainRenderTargetView();
    _renderContext->GetDeviceContext()->OMSetRenderTargets(1, &mainView, nullptr);
    _renderContext->GetDeviceContext()->ClearRenderTargetView(mainView, (float*)&_clearColor);

    Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    _renderContext->Present();
}

void ClothSimulationApplication::Update(float deltaTime)
{
    static float lightPositionX = -5.0f;

    // Update the position of the light each frame.
    lightPositionX += 0.05f;
    if (lightPositionX > 5.0f)
    {
        lightPositionX = -5.0f;
    }

    // Update the position of the light.
    _light->SetPosition(DirectX::XMFLOAT3(lightPositionX, 8.0f, -5.0f));

}

void ClothSimulationApplication::FixedUpdate(float deltaTime)
{
}

void ClothSimulationApplication::RenderSceneToTexture()
{
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4X4 lightViewMatrix;
    DirectX::XMFLOAT4X4 lightProjectionMatrix;
    DirectX::XMFLOAT4X4 translateMatrix;
    DirectX::XMFLOAT3 position;

    // Set the render target to be the render to texture.
    //_renderTexture->SetRenderTarget();

    // Clear the render to texture.
    _renderTexture->ClearRenderTarget(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    // Generate the light view matrix based on the light's position.
    _light->CreateViewMatrix();

    // Get the world matrix from the d3d object.
    worldMatrix = _renderContext->GetWorldMatrix();

    // Get the view and orthographic matrices from the light object.
    lightViewMatrix = _light->GetViewMatrix();
    lightProjectionMatrix = _light->GetProjectionMatrix();

    // Setup the translation matrix for the cube model.
    DirectX::XMFLOAT3 cubePosition = _cubeModel->GetPosition();
    DirectX::XMMATRIX worldMatrixGen = DirectX::XMMatrixTranslation(cubePosition.x, cubePosition.y, cubePosition.z);
    DirectX::XMStoreFloat4x4(&worldMatrix, worldMatrixGen);
    // Render the cube model with the depth shader.
    _cubeModel->Render();
    _depthShader->Render(_cubeModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);

    // Reset the world matrix.
    worldMatrix = _renderContext->GetWorldMatrix();

    // Setup the translation matrix for the sphere model.
    DirectX::XMFLOAT3 spherePosition = _sphereModel->GetPosition();
    worldMatrixGen = DirectX::XMMatrixTranslation(spherePosition.x, spherePosition.y, spherePosition.z);
    DirectX::XMStoreFloat4x4(&worldMatrix, worldMatrixGen);

    // Render the sphere model with the depth shader.
    _sphereModel->Render();
    _depthShader->Render(_sphereModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);

    // Reset the world matrix.
    worldMatrix = _renderContext->GetWorldMatrix();

    // Setup the translation matrix for the ground model.
    DirectX::XMFLOAT3 floorPosition = _floorModel->GetPosition();
    worldMatrixGen = DirectX::XMMatrixTranslation(floorPosition.x, floorPosition.y, floorPosition.z);
    DirectX::XMStoreFloat4x4(&worldMatrix, worldMatrixGen);

    // Render the ground model with the depth shader.
    _floorModel->Render();
    _depthShader->Render(_floorModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);

    // Reset the render target back to the original back buffer and not the render to texture anymore.
    _renderContext->SetBackBufferRenderTarget();

    // Reset the viewport back to the original.
    _renderContext->ResetViewport();
}

void ClothSimulationApplication::RenderBlackAndWhiteShadows()
{
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projectionMatrix;
    DirectX::XMFLOAT4X4 translateMatrix;
    DirectX::XMFLOAT4X4 lightViewMatrix;
    DirectX::XMFLOAT4X4 lightProjectionMatrix;

    // Set the render target to be the render to texture.
    _blackWhiteRenderTexture->Bind();

    // Clear the render to texture.
    _blackWhiteRenderTexture->ClearRenderTarget(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    // Generate the view matrix based on the camera's position.
    _camera->Update();

    // Generate the light view matrix based on the light's position.
    _light->CreateViewMatrix();

    // Get the world, view, and projection matrices from the camera and d3d objects.
    viewMatrix = _camera->GetViewMatrix();
    worldMatrix = _renderContext->GetWorldMatrix();
    projectionMatrix = _renderContext->GetProjectionMatrix();

    // Get the light's view and projection matrices from the light object.
    lightViewMatrix = _light->GetViewMatrix();
    lightProjectionMatrix = _light->GetProjectionMatrix();

    // Setup the translation matrix for the cube model.
    DirectX::XMFLOAT3 cubePosition = _cubeModel->GetPosition();
    DirectX::XMMATRIX worldMatrixGen = DirectX::XMMatrixTranslation(cubePosition.x, cubePosition.y, cubePosition.z);
    DirectX::XMStoreFloat4x4(&worldMatrix, worldMatrixGen);
    // Render the cube model using the shadow shader.
    _cubeModel->Render();
    _shadowShader->Render(_cubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
        lightProjectionMatrix, _renderTexture->GetShaderResourceView().Get(), _light->GetPosition());

    // Reset the world matrix.
    worldMatrix = _renderContext->GetWorldMatrix();

    DirectX::XMFLOAT3 spherePosition = _sphereModel->GetPosition();
    worldMatrixGen = DirectX::XMMatrixTranslation(spherePosition.x, spherePosition.y, spherePosition.z);
    DirectX::XMStoreFloat4x4(&worldMatrix, worldMatrixGen);


    // Render the sphere model using the shadow shader.
    _sphereModel->Render();
    _shadowShader->Render(_sphereModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
        lightProjectionMatrix, _renderTexture->GetShaderResourceView().Get(), _light->GetPosition());

    // Reset the world matrix.
    worldMatrix = _renderContext->GetWorldMatrix();

    // Setup the translation matrix for the ground model.
    DirectX::XMFLOAT3 floorPosition = _floorModel->GetPosition();
    worldMatrixGen = DirectX::XMMatrixTranslation(floorPosition.x, floorPosition.y, floorPosition.z);
    DirectX::XMStoreFloat4x4(&worldMatrix, worldMatrixGen);

    // Render the ground model using the shadow shader.
    _floorModel->Render();
    _shadowShader->Render(_floorModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
        lightProjectionMatrix, _renderTexture->GetShaderResourceView().Get(), _light->GetPosition());


    // Reset the render target back to the original back buffer and not the render to texture anymore.
    _renderContext->SetBackBufferRenderTarget();

    // Reset the viewport back to the original.
    _renderContext->ResetViewport();
}


void ClothSimulationApplication::DownSampleTexture()
{
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4X4 baseViewMatrix;
    DirectX::XMFLOAT4X4 orthographicMatrix;


    // Set the render target to be the render to texture.
    _downSampleTexture->Bind();

    // Clear the render to texture.
    _downSampleTexture->ClearRenderTarget(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    // Generate the view matrix based on the camera's position.
    _camera->Update();

    // Get the world and view matrices from the camera and d3d objects.
    worldMatrix = _renderContext->GetWorldMatrix();
    baseViewMatrix = _camera->GetBaseViewMatrix();

    // Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
    orthographicMatrix = _downSampleTexture->GetOrthoMatrix();

    // Turn off the Z buffer to begin all 2D rendering.
    _renderContext->EnableZbuffer(false);

    // Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
    _orthographicWindow->Render();

    // Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource.
    _textureShader->Render(_orthographicWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthographicMatrix,
        _blackWhiteRenderTexture->GetShaderResourceView().Get());

    // Turn the Z buffer back on now that all 2D rendering has completed.
    _renderContext->EnableZbuffer(true);

    // Reset the render target back to the original back buffer and not the render to texture anymore.
    _renderContext->SetBackBufferRenderTarget();

    // Reset the viewport back to the original.
    _renderContext->ResetViewport();
}

void ClothSimulationApplication::RenderHorizontalBlurToTexture()
{
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4X4 baseViewMatrix;
    DirectX::XMFLOAT4X4 orthographicMatrix;

    // Store the screen width in a float that will be used in the horizontal blur shader.
    float screenSizeX = (float)(SHADOWMAP_WIDTH / 2);

    // Set the render target to be the render to texture.
    _horizontalBlurTexture->Bind();

    // Clear the render to texture.
    _horizontalBlurTexture->ClearRenderTarget(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    // Generate the view matrix based on the camera's position.
    _camera->Update();

    // Get the world and view matrices from the camera and d3d objects.
    baseViewMatrix = _camera->GetBaseViewMatrix();
    worldMatrix = _renderContext->GetWorldMatrix();

    // Get the ortho matrix from the render to texture since texture has different dimensions.
    orthographicMatrix = _horizontalBlurTexture->GetOrthoMatrix();

    // Turn off the Z buffer to begin all 2D rendering.
    _renderContext->EnableZbuffer(false);

    // Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
    _orthographicWindow->Render();

    // Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
    _horizontalBlurShader->Render(_orthographicWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthographicMatrix,
        _downSampleTexture->GetShaderResourceView().Get(), screenSizeX);

    // Turn the Z buffer back on now that all 2D rendering has completed.
    _renderContext->EnableZbuffer(true);

    // Reset the render target back to the original back buffer and not the render to texture anymore.
    _renderContext->SetBackBufferRenderTarget();

    // Reset the viewport back to the original.
    _renderContext->ResetViewport();
}


void ClothSimulationApplication::RenderVerticalBlurToTexture()
{
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4X4 baseViewMatrix;
    DirectX::XMFLOAT4X4 orthographicMatrix;
    
    // Store the screen height in a float that will be used in the vertical blur shader.
    float screenSizeY = (float)(SHADOWMAP_HEIGHT / 2);

    // Set the render target to be the render to texture.
    _verticalBlurTexture->Bind();

    // Clear the render to texture.
    _verticalBlurTexture->ClearRenderTarget(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    // Generate the view matrix based on the camera's position.
    _camera->Update();

    // Get the world and view matrices from the camera and d3d objects.
    baseViewMatrix = _camera->GetBaseViewMatrix();
    worldMatrix = _renderContext->GetWorldMatrix();

    // Get the ortho matrix from the render to texture since texture has different dimensions.
    orthographicMatrix = _verticalBlurTexture->GetOrthoMatrix();

    // Turn off the Z buffer to begin all 2D rendering.
    _renderContext->EnableZbuffer(false);

    // Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
    _orthographicWindow->Render();

    // Render the small ortho window using the vertical blur shader and the horizontal blurred render to texture resource.
    _verticalBlurShader->Render(_orthographicWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthographicMatrix,
        _horizontalBlurTexture->GetShaderResourceView().Get(), screenSizeY);

    // Turn the Z buffer back on now that all 2D rendering has completed.
    _renderContext->EnableZbuffer(true);

    // Reset the render target back to the original back buffer and not the render to texture anymore.
    _renderContext->SetBackBufferRenderTarget();

    // Reset the viewport back to the original.
    _renderContext->ResetViewport();
}

void ClothSimulationApplication::UpSampleTexture()
{
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4X4 baseViewMatrix;
    DirectX::XMFLOAT4X4 orthographicMatrix;

    // Set the render target to be the render to texture.
    _upSampleTexure->Bind();

    // Clear the render to texture.
    _upSampleTexure->ClearRenderTarget(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    // Generate the view matrix based on the camera's position.
    _camera->Update();

    // Get the world and view matrices from the camera and d3d objects.
    baseViewMatrix = _camera->GetBaseViewMatrix();
    worldMatrix = _renderContext->GetWorldMatrix();

    // Get the ortho matrix from the render to texture since texture has different dimensions.
    orthographicMatrix = _upSampleTexure->GetOrthoMatrix();

    // Turn off the Z buffer to begin all 2D rendering.
    _renderContext->EnableZbuffer(false);

    // Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
    _fullScreenOrthographicWindow->Render();

    // Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
    _textureShader->Render(_fullScreenOrthographicWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthographicMatrix,
        _verticalBlurTexture->GetShaderResourceView().Get());

    // Turn the Z buffer back on now that all 2D rendering has completed.
    _renderContext->EnableZbuffer(true);

    // Reset the render target back to the original back buffer and not the render to texture anymore.
    _renderContext->SetBackBufferRenderTarget();

    // Reset the viewport back to the original.
    _renderContext->ResetViewport();
}

void ClothSimulationApplication::Render()
{
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projectionMatrix;
    DirectX::XMFLOAT4X4 translateMatrix;
    DirectX::XMFLOAT3 position;

    // Generate the view matrix based on the camera's position.
    _camera->Update();

    // Get the world, view, and projection matrices from the camera and d3d objects.
    viewMatrix = _camera->GetViewMatrix();
    worldMatrix = _renderContext->GetWorldMatrix();
    projectionMatrix = _renderContext->GetProjectionMatrix();

    // Setup the translation matrix for the cube model.
    DirectX::XMFLOAT3 cubePosition = _cubeModel->GetPosition();
    DirectX::XMMATRIX worldMatrixGen = DirectX::XMMatrixTranslation(cubePosition.x, cubePosition.y, cubePosition.z);
    DirectX::XMStoreFloat4x4(&worldMatrix, worldMatrixGen);

    // Render the cube model using the soft shadow shader.
    _cubeModel->Render();
    _softShadowShader->Render(_cubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
        _cubeModel->GetTexture(), _upSampleTexure->GetShaderResourceView().Get(), _light->GetPosition(),
        _light->GetAmbientColor(), _light->GetDiffuseColor());

    // Reset the world matrix.
    worldMatrix = _renderContext->GetWorldMatrix();

    // Setup the translation matrix for the sphere model.
    DirectX::XMFLOAT3 spherePosition = _sphereModel->GetPosition();
    worldMatrixGen = DirectX::XMMatrixTranslation(spherePosition.x, spherePosition.y, spherePosition.z);
    DirectX::XMStoreFloat4x4(&worldMatrix, worldMatrixGen);

    // Render the sphere model using the soft shadow shader.
    _sphereModel->Render();
    _softShadowShader->Render(_sphereModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
        _sphereModel->GetTexture(), _upSampleTexure->GetShaderResourceView().Get(), _light->GetPosition(),
        _light->GetAmbientColor(), _light->GetDiffuseColor());

    // Reset the world matrix.
    worldMatrix = _renderContext->GetWorldMatrix();

    // Setup the translation matrix for the ground model.
    DirectX::XMFLOAT3 floorPosition = _floorModel->GetPosition();
    worldMatrixGen = DirectX::XMMatrixTranslation(floorPosition.x, floorPosition.y, floorPosition.z);
    DirectX::XMStoreFloat4x4(&worldMatrix, worldMatrixGen);


    // Render the ground model using the soft shadow shader.
    _floorModel->Render();
    _softShadowShader->Render(_floorModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
        _floorModel->GetTexture(), _upSampleTexure->GetShaderResourceView().Get(), _light->GetPosition(),
        _light->GetAmbientColor(), _light->GetDiffuseColor());
}
