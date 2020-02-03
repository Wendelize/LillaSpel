#include "Header Files/Skybox.h"
#include "Header Files/stb_image.h"

Skybox::Skybox()
{
    m_verticesSkybox = NULL;
    m_indicesSkybox = NULL;
    CreateSkybox();
    InitSkybox();
}

Skybox::~Skybox()
{

    delete[] m_verticesSkybox;

    delete[] m_indicesSkybox;

}

void Skybox::CreateSkybox()
{
    //CREATE VERTICES DATA
    skyboxData vertices[] =
    {

         { {-1,  -1,  1 } },
         { {-1,   1,  1 } },
         { { 1,   1,  1 } },
         { { 1,  -1,  1 } },

         { { 1,  -1, -1 } },
         { { 1,   1, -1 } },
         { {-1,   1, -1 } },
         { {-1,  -1, -1 } },

         { {-1,   1,  1 } },
         { {-1,   1, -1 } },
         { { 1,   1, -1 } },
         { { 1,   1,  1 } },

         { {-1,  -1, -1 } },
         { {-1,  -1,  1 } },
         { { 1,  -1,  1 } },
         { { 1,  -1, -1 } },

         { { 1,  -1,  1 } },
         { { 1,   1,  1 } },
         { { 1,   1, -1 } },
         { { 1,  -1, -1 } },

         { {-1,  -1, -1 } },
         { {-1,   1, -1 } },
         { {-1,   1,  1 } },
         { {-1,  -1,  1 } }
    };

    m_nrOfVSkybox = sizeof(vertices) / sizeof(*vertices);
    m_verticesSkybox = new skyboxData[m_nrOfVSkybox];
    memcpy(m_verticesSkybox, vertices, sizeof(vertices));

    //CREATE INDECES DATA
    unsigned int indexData[] = { 0,  2,   1,   0,   3,   2,
                                  4,  6,   5,   4,   7,   6,
                                  8,  10,  9,   8,   11,  10,
                                 12,  14,  13,  12,  15,  14,
                                 16,  18,  17,  16,  19,  18,
                                 20,  22,  21,  20,  23,  22 };
    m_nrOfISkybox = sizeof(indexData) / sizeof(*indexData);
    m_indicesSkybox = new unsigned int[m_nrOfISkybox];
    memcpy(m_indicesSkybox, indexData, sizeof(indexData));

    LoadSkyboxTexture();
}

void Skybox::InitSkybox()
{
    //Bind VertexArray
    glGenVertexArrays(1, &m_vertexArraySkybox);
    glBindVertexArray(m_vertexArraySkybox);

    //Bind VertexBuffer & IndexBuffer
    glGenBuffers(1, &m_vertexBufferSkybox);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferSkybox);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSizeSkybox(), m_verticesSkybox, GL_STATIC_DRAW);

    glGenBuffers(1, &m_indicesBufferSkybox);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferSkybox);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSizeSkybox(), m_indicesSkybox, GL_STATIC_DRAW);

    //Write vertex data to memory
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(skyboxData), 0);
}

void Skybox::DrawSkybox(Shader* shaderProgram)
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    glBindVertexArray(m_vertexArraySkybox);
    Bind3DTexture();

    glDrawElements(GL_TRIANGLES, m_nrOfISkybox, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Skybox::LoadSkyboxTexture()
{
    // stbi_set_flip_vertically_on_load(1);
    vector<string> faces{
        "src/Textures/right.tga", // RIGHT TEXTURE
        "src/Textures/left.tga", // LEFT TEXTURE
        "src/Textures/top.tga", // UP TEXTURE
        "src/Textures/bottom.tga", // DOWN TEXTURE
        "src/Textures/back.tga", // BACK TEXTURE
        "src/Textures/front.tga", // FRONT TEXTURE
    };

    vector<string> faces2{
        "src/Textures/moondust_rt.tga", // RIGHT TEXTURE
        "src/Textures/moondust_lf.tga", // LEFT TEXTURE
        "src/Textures/moondust_up.tga", // UP TEXTURE
        "src/Textures/moondust_dn.tga", // DOWN TEXTURE
        "src/Textures/moondust_bk.tga", // BACK TEXTURE
        "src/Textures/moondust_ft.tga", // FRONT TEXTURE
    };

    vector<string> faces3{
        "src/Textures/posx.jpg", // RIGHT TEXTURE
        "src/Textures/negx.jpg", // LEFT TEXTURE
        "src/Textures/posy.jpg", // UP TEXTURE
        "src/Textures/negy.jpg", // DOWN TEXTURE
        "src/Textures/posz.jpg", // BACK TEXTURE
        "src/Textures/negz.jpg", // FRONT TEXTURE
    };

    vector<string> faces4{
        "src/Textures/n_right.jpg", // RIGHT TEXTURE
        "src/Textures/n_left.jpg", // LEFT TEXTURE
        "src/Textures/n_top.jpg", // UP TEXTURE
        "src/Textures/n_bottom.jpg", // DOWN TEXTURE
        "src/Textures/n_back.jpg", // BACK TEXTURE
        "src/Textures/n_front.jpg", // FRONT TEXTURE
    };

    glGenTextures(1, &m_textureSkybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureSkybox);

    for (unsigned int i = 0; i < faces4.size(); i++)
    {
        unsigned char* data = stbi_load(faces4[i].c_str(), &m_width, &m_height, &m_nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
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
}

void Skybox::Bind3DTexture()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureSkybox);
}
