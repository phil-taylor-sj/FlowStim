#include <Simulation2D.h>

#include <Core/Case/Case2dCouetteFlowFactory.h>
#include <Core/Case/Case2dCavityFlowFactory.h>
#include <Core/Domain/Compass.h>

#include <stdexcept>
#include <mutex>

void Simulation2D::start()
{
    std::lock_guard<std::mutex> guard(this->m_timerMutex);
    this->m_timer->start(32);
}

void Simulation2D::stop()
{
    std::lock_guard<std::mutex> guard(this->m_timerMutex);
    this->m_timer->stop();
}

void Simulation2D::generate()
{
    this->stop();
    std::lock_guard<std::mutex> guard(this->m_mutex);

    // Try block for primarily error logging
    // If thrown the programme will crash for a segmentation fault.
    try
    {
        fstim::Case2dCavityFlowFactory caseFactory{};
        caseFactory.setDomainSize(vecp::Vec2i(20, 20));
        caseFactory.setDomainLength(vecp::Vec2d(1., 1.));
        caseFactory.setReferenceVelocity(vecp::Vec2d(1.0, 0.0));
        caseFactory.setReferenceDirection(fstim::Compass::NORTH);
        
        this->m_solver = caseFactory.buildCase(); 
    }
    catch (std::out_of_range& err)
    {
        std::cerr << "[Out of Range] error thrown during Solver construction.\n"
            << err.what() << std::endl;
    }
    catch (std::exception& err)
    {
        std::cerr << "An unknown error has occurred during Solver construction.\n"
            << err.what() << std::endl;
    }

    //int nCells = m_solver->getMesh()->nCells;

    /**
    fstim::Mesh2dStructuredFactory factory = fstim::Mesh2dStructuredFactory();
    std::unique_ptr<fstim::Mesh2d> mesh = factory(this->m_size, this->m_length);
    fstim::FaceSetFactory::fourWalls(*(mesh.get()));
    int nCells = mesh->nCells;
    this->m_solver->setMesh(std::move(mesh));
    std::unique_ptr<fstim::VectorFieldEqu> velocity = std::make_unique<fstim::VectorFieldEqu>(nCells);
    velocity->addBc(fstim::BcType::ZEROGRADIENT, vecp::Vec2d(0.0, 0.0));
    velocity->addBc(fstim::BcType::ZEROGRADIENT, vecp::Vec2d(0.0, 0.0));
    velocity->addBc(fstim::BcType::FIXEDVALUE, vecp::Vec2d(1.0, 1.0));
    velocity->addBc(fstim::BcType::FIXEDVALUE, vecp::Vec2d(0.0, 0.0));
    this->m_solver->setVelocity(std::move(velocity));
    */
    this->m_solver->initialiseViscosity(0.001);

    this->m_updateMeshData();
}

Simulation2D::Simulation2D(QObject *parent) : QObject(parent), m_timer(new QTimer(this))
{
    
    //this->m_solver = std::make_unique<fstim::BurgersSolver>();

    connect(m_timer, &QTimer::timeout, this, &Simulation2D::m_compute);
}

void Simulation2D::m_updateMeshData()
{
    std::shared_ptr<MeshData> data = std::make_shared<MeshData>();
    const fstim::Mesh2d* mesh = this->m_solver->getMesh();
    data->length = mesh->length.toFloat();
    data->nCells = mesh->nCells;
    //data->vertices.resize(2 * 4 * mesh->nCells);
    data->vertices.resize(mesh->nVertices);
    data->cellElements.resize(mesh->nCells);
    
    vecp::Vec2f halfLength = data->length * 0.5f;
    float maxLength = data->length.max();
    
    for (size_t id = 0; id < mesh->nVertices; id++)
    {
        vecp::Vec2f position =  mesh->vertices[id].position.toFloat();
        data->vertices[id] = (position - halfLength) / (0.55f * maxLength);
    }

    const fstim::Cell2d* begin = mesh->cells.get(); 
    const fstim::Cell2d* end = begin + mesh->nCells;

    for (const fstim::Cell2d* cell = begin; cell != end; cell++)
    {
        data->cellElements[cell->id] = cell->vertexId;
        //for (vecp::Vec2f vertex : cell->vertices)
        //{

            //data->vertices[index++] = (vertex.x - halfLength.x) / (0.55f * maxLength);
            //data->vertices[index++] = (vertex.y - halfLength.y) / (0.55f * maxLength);
        //}
    }

    emit this->sendMesh(data);
}

void Simulation2D::m_updateVelocityData()
{
    const fstim::VectorField* field = this->m_solver->getVelocity();
    const fstim::Mesh2d* mesh = this->m_solver->getMesh();
    const vecp::Vec2d* velocity = field->readValues();
    
    auto data = std::make_shared<std::vector<vecp::Vec2f>>();
    data->resize(mesh->nVertices);
    
    for (int id = 0; id < mesh->nVertices; id++)
    {
        const fstim::Vertex2d& vertex = mesh->vertices[id];
        vecp::Vec2d vertexValue = vecp::Vec2d();
        for (int index = 0; index < vertex.cellId.size(); index++)
        {
            vertexValue += velocity[vertex.cellId[index]]; //* vertex.cellWeight[index];
        }
        (*data)[id] = vertexValue.toFloat();
    }

    emit this->sendVelocity(data);
}

Simulation2D::~Simulation2D()
{
    delete this->m_timer;
}

void Simulation2D::m_compute()
{
    std::lock_guard<std::mutex> guard(this->m_mutex);
    if (this->m_solver->getMesh() == nullptr)
    {
        return;
    }
    this->m_solver->compute(0.005f);
    this->m_updateVelocityData();
}