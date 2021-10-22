#include "mesh.h"

Mesh::Mesh(vector<Vertex>& vertices, vector<GLuint>& indices, vector<Texture>& textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO EBO(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}


void Mesh::Draw(Shader& shader, Camera& camera) {
    // Bind shader to be able to access uniforms
    shader.Activate();
    VAO.Bind();

    // Keep track of how many of each type of textures we have
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string num;
        std::string type = textures[i].type;
        if (type == "diffuse")
        {
                num = std::to_string(numDiffuse++);
        }
        else if (type == "specular")
        {
                num = std::to_string(numSpecular++);
        }
        textures[i].SetUniformUnit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }

    // Take care of the camera Matrix
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.m_position.x, camera.m_position.y, camera.m_position.z);
    camera.SetMatrix(shader, "camMatrix");

    // Draw the actual mesh
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
