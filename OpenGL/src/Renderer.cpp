#include "Renderer.h"
#include "GLCall.h"

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    // Bind our shader
    shader.Bind();

    // Bind our vertex array
    va.Bind();

    // Bind our index buffer
    ib.Bind();


    // Draw triangles
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

/*id Renderer::Draw(const Mesh& mesh, const Shader& shader) const
{
    mesh.Bind();
    shader.Bind();

    //Call(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}*/


