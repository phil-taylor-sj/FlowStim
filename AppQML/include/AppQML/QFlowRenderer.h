#include <QQuickFramebufferObject>

#include <AppQML/QBuffers.h>

class QFlowRenderer : public QQucikFramebufferObject::Renderer
{
public:
    QFlowRenderer();
    ~QFlowRenderer();

    void render() override;

private:
    QBuffers m_cellBuffers{};
    QBuffers m_gridBuffers{}:

    std::mutex m_mutex;

    void m_createShader();

    void m_createCellMesh(std::shared_ptr<MeshData>& data);
    void m_createGridMesh(std::shared_ptr<MeshData>& data);

    void m_deleteBuffers();
}