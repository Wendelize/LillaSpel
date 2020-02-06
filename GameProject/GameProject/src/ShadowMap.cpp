#include "Header Files/ShadowMap.h"

ShadowMap::ShadowMap()
{
    m_FBO = 0;
    m_shadowMap = 0;
    Init();
    m_shadowShader = new Shader("src/Shaders/ShadowVS.glsl", "src/Shaders/ShadowFS.glsl");
}

ShadowMap::~ShadowMap()
{
}

bool ShadowMap::Init(unsigned int windowWidth, unsigned int windowHeight)
{
    glGenFramebuffers(1, &m_FBO);
    //glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glGenTextures(1, &m_shadowMap);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);


    glReadBuffer(GL_NONE);
    glReadBuffer(GL_NONE);


    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void ShadowMap::BindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

void ShadowMap::BindForReading(GLenum texture)
{
    glActiveTexture(texture);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
}

void ShadowMap::CalcLightSpaceMatrix(vector<Light> light)
{
    for (uint i = 0; i < light.size(); i++)
    {
        if (light.at(i).GetType() == 0) {
            mat4 _lightProj, _lightView, _lightModel;
            _lightProj = ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 10.0f);
            _lightView = lookAt(light.at(i).GetPos(), vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
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
