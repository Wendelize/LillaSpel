#include "Header Files/ShadowMap.h"

ShadowMap::ShadowMap(int width, int height)
{
    m_FBO = 0;
    m_shadowMap = 0;
    Init(width, height);
    m_shadowShader = new Shader("src/Shaders/ShadowVS.glsl", "src/Shaders/ShadowFS.glsl");
}

ShadowMap::~ShadowMap()
{
    delete m_shadowShader;
}

bool ShadowMap::Init(unsigned int windowWidth, unsigned int windowHeight)
{
    // Create depth-texture 
    glGenTextures(1, &m_shadowMap);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Create Framebuffer
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    // Check if Framebuffer is "open" to store data
    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void ShadowMap::CalcLightSpaceMatrix(vector<Light*> light)
{
    for (uint i = 0; i < light.size(); i++)
    {
        if (light[i]->GetType() == 0) {
            mat4 _lightProj, _lightView, _lightModel;
            _lightProj = ortho(-40.0f, 40.0f, -40.0f, 40.0f, 0.1f, 60.0f);
            _lightView = lookAt(light[i]->GetPos(), vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
            //_lightModel = mat4(1.0);
            m_lightSpaceMatrix = _lightProj * _lightView;
            m_shadowShader->UseShader();
            m_shadowShader->SetUniform("u_LSP", m_lightSpaceMatrix);

        }
    }
}

mat4 ShadowMap::GetLSP()
{
    return m_lightSpaceMatrix;
}

Shader* ShadowMap::GetShader()
{
    return m_shadowShader;
}

GLuint ShadowMap::GetTexture()
{
    return m_shadowMap;
}

GLuint ShadowMap::GetFBO()
{
    return m_FBO;
}
