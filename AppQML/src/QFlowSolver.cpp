#include <AppQML/QFlowSolver.h>

void QFlowSolver::run()
{
    clock_t minLoopTime = 32;
    clock_t loopTime = minLoopTime;
    while (true)
    {
        clock_t startTime = clock();
        if (!this->m_pause)
        {
            this->m_compute();
        }
        loopTime = clock() - startTime;
        if (loopTime < minLoopTime)
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(minLoopTime - loopTime)
                );
            loopTime = minLoopTime; 
        }
    }
}

void QFlowSolver::start()
{
    this->m_pause = true;
}

void QFlowSolver::stop()
{
    this->m_pause = false;
}

void QFlowSolver::generate()
{
    
    this->stop();
    std::lock_guard<std::mutex> guard(this->m_mutex);
    
    fstim::MeshFactory2dStructured factory = fstim::MeshFactory2dStructured();
    std::unique_ptr<fstim::Mesh2d> mesh = factory(this->m_size, this->m_length);
    fstim::FaceSetFactory::fourWalls(*(mesh.get()));
    
    int nCells = mesh->nCells;

    this->m_solver->setMesh(std::move(mesh));

    this->m_solver->initialiseViscosity(0.001);

    std::unique_ptr<fstim::VectorFieldEqu> velocity = std::make_unique<fstim::VectorFieldEqu>(nCells);
    velocity->addBc(fstim::BcType::ZEROGRADIENT, vecp::Vec2d(0.0, 0.0));
    velocity->addBc(fstim::BcType::ZEROGRADIENT, vecp::Vec2d(0.0, 0.0));
    velocity->addBc(fstim::BcType::FIXEDVALUE, vecp::Vec2d(1.0, 1.0));
    velocity->addBc(fstim::BcType::FIXEDVALUE, vecp::Vec2d(0.0, 0.0));

    this->m_solver->setVelocity(std::move(velocity));

    this->m_updateMeshData();
}

QFlowSolver::QFlowSolver(QObject *parent) : QObject(parent), m_timer(new QTimer(this))
{
    
    this->m_solver = std::make_unique<fstim::BurgersSolver>();
}

void QFlowSolver::m_updateMeshData()
{
    std::shared_ptr<MeshData> data = std::make_shared<MeshData>();
    const fstim::Mesh2d* mesh = this->m_solver->getMesh();
    data->length = mesh->length.toFloat();
    data->nCells = mesh->nCells;

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
    }

    emit this->sendMesh(data);
}

void QFlowSolver::m_updateVelocityData()
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

QFlowSolver::~QFlowSolver2D()
{
    delete this->m_timer;
}

void QFlowSolver::m_compute()
{
        std::lock_guard<std::mutex> guard(this->m_mutex);
        if (this->m_solver->getMesh() == nullptr)
        {
            return;
        }
        this->m_solver->compute(0.032f);
        this->m_updateVelocityData();
}

